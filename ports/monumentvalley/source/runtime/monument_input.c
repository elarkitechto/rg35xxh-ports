#include "monument_input.h"
#include "monument_display.h"
#include "egl_shim.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
static void *(*domain_get)(void),**(*assemblies_get)(void*,size_t*),*(*image_get)(void*),*(*class_get)(void*,const char*,const char*),*(*method_get)(void*,const char*,int),*(*string_new)(const char*),*(*invoke)(void*,void*,void**,void**);
static void (*class_init)(void*),(*format_exception)(void*,char*,int);
static void *queue,*device,*prefs_save,*common_class,*common_instance,*common_get;
static void *presentation_update,*screen_resize,*screen_width,*screen_height,*presentation_force;
static const void *(*class_type)(void*);
static void *(*type_object)(const void*);
static void *ui_camera_type,*ui_camera_resize,*find_objects;
static int device_id,chord_latched,previous_mode=-1,activated;
static double (*input_time)(void*);
static int (*gc_incremental)(void);
static int64_t (*gc_slice_get)(void);
static void (*gc_slice_set)(int64_t);
#pragma pack(push,4)
struct gamepad_state {uint32_t buttons;float lx,ly,rx,ry,lt,rt;};
struct state_event {uint32_t type;uint16_t size,device;double time;uint32_t event_id,format;struct gamepad_state state;};
#pragma pack(pop)
_Static_assert(sizeof(struct state_event)==52,"Unity Gamepad event ABI");
static void report(const char *what,void *error){char text[2048]={0};if(error&&format_exception)format_exception(error,text,sizeof(text));fprintf(stderr,"[MONUMENT-INPUT] %s: %s\n",what,text);}
void monument_input_init(so_module *module){
 so_module *saved=so_save();so_use(module);
#define API(var,name) var=(void*)so_find_addr_safe(name)
 API(gc_incremental,"il2cpp_gc_is_incremental");API(gc_slice_get,"il2cpp_gc_get_max_time_slice_ns");API(gc_slice_set,"il2cpp_gc_set_max_time_slice_ns");
 API(class_type,"il2cpp_class_get_type");API(type_object,"il2cpp_type_get_object");
 API(domain_get,"il2cpp_domain_get");API(assemblies_get,"il2cpp_domain_get_assemblies");API(image_get,"il2cpp_assembly_get_image");API(class_get,"il2cpp_class_from_name");API(method_get,"il2cpp_class_get_method_from_name");API(string_new,"il2cpp_string_new");API(invoke,"il2cpp_runtime_invoke");API(class_init,"il2cpp_runtime_class_init");API(format_exception,"il2cpp_format_exception");
#undef API
 /* NativeInputSystem.get_currentTime from this pinned IL2CPP image. */
 input_time=(void*)(0x500000000UL+0x269a238);
 so_use(saved);free(saved);
}
static int attach(void){
 void *domain=domain_get();if(!domain)return 0;size_t count=0;void **assemblies=assemblies_get(domain,&count),*input=NULL;
 for(size_t i=0;i<count;i++){
  void *image=image_get(assemblies[i]);if(!input)input=class_get(image,"UnityEngine.InputSystem","InputSystem");
  void *prefs=class_get(image,"UnityEngine","PlayerPrefs");if(prefs)prefs_save=method_get(prefs,"Save",0);
  void *presentation=class_get(image,"MVCommon","GamePresentation");
  if(presentation)presentation_update=method_get(presentation,"UpdateInputModeAndMostRecentGamepadType",3);
  if(!common_class)common_class=class_get(image,"MVCommon.InputService","CommonInputService");
 }
 if(!input)return 0;class_init(input);void *add=method_get(input,"AddDevice",3);queue=method_get(input,"QueueEvent",1);if(!add||!queue)return 0;
 void *args[]={string_new("XInputController"),string_new("RG35XX-H"),string_new("")},*error=NULL;
 device=invoke(add,NULL,args,&error);if(error||!device){report("AddDevice",error);return 0;}
 /* InputDevice.m_DeviceId offset confirmed from the matching metadata dump. */
 uint32_t state_format=*(uint32_t*)((char*)device+0x10);
 if(state_format!=0x47504144){fprintf(stderr,"[MONUMENT-INPUT] unsupported controller state format=%08x\n",state_format);exit(24);}
 fprintf(stderr,"[MONUMENT-INPUT] XInputController state format=GPAD verified\n");
 device_id=*(int*)((char*)device+0xe0);if(device_id<=0||device_id>65535)exit(24);
 if(common_class)common_get=method_get(common_class,"get_Instance",0);
 fprintf(stderr,"[MONUMENT-INPUT] Unity Gamepad attached id=%d\n",device_id);return 1;
}
static int button(SDL_GameController *p,SDL_GameControllerButton b){return p&&SDL_GameControllerGetButton(p,b);}
static float axis(SDL_GameController *p,SDL_GameControllerAxis a){if(!p)return 0;float v=SDL_GameControllerGetAxis(p,a)/32768.f;/* The original GamepadMove action applies StickDeadzone. Preserve raw
     stick values here instead of adding a discontinuous axial deadzone. */
 if(a<=SDL_CONTROLLER_AXIS_RIGHTY)return v;return fabsf(v)<.18f?0:v;}
int monument_input_step(long frame){
 if(frame==120 && gc_incremental && gc_slice_get && gc_slice_set){
  int enabled=gc_incremental();int64_t slice=gc_slice_get();
  if(enabled && slice>2000000)gc_slice_set(2000000);
  fprintf(stderr,"[MONUMENT-GC] incremental=%d slice_ns=%lld\n",enabled,(long long)gc_slice_get());
 }
 SDL_Event e;while(SDL_PollEvent(&e))if(e.type==SDL_QUIT)return 1;
 SDL_GameController *p=egl_shim_controller();
 if(button(p,SDL_CONTROLLER_BUTTON_START)&&(button(p,SDL_CONTROLLER_BUTTON_GUIDE)||button(p,SDL_CONTROLLER_BUTTON_BACK)))return 1;
 float lt=axis(p,SDL_CONTROLLER_AXIS_TRIGGERLEFT),rt=axis(p,SDL_CONTROLLER_AXIS_TRIGGERRIGHT);int chord=lt>.65f&&rt>.65f;
 if(chord&&!chord_latched)monument_display_toggle();chord_latched=chord;
 if(!device&&frame>=60&&frame%60==0)attach();if(!device)return 0;
 struct state_event event={.type=0x53544154,.size=sizeof(event),.device=device_id,.format=0x47504144,.time=input_time(NULL)};
 struct gamepad_state *s=&event.state;
 s->lx=axis(p,SDL_CONTROLLER_AXIS_LEFTX);s->ly=-axis(p,SDL_CONTROLLER_AXIS_LEFTY);s->rx=axis(p,SDL_CONTROLLER_AXIS_RIGHTX);s->ry=-axis(p,SDL_CONTROLLER_AXIS_RIGHTY);s->lt=chord?0:lt;s->rt=chord?0:rt;
 if(monument_display_tate()){
  /* GamepadMove is bound to leftStick. In TATE the right stick is the
     comfortable primary control; retain the left stick as a fallback. */
  float x=s->lx,y=s->ly;
  if(s->rx*s->rx+s->ry*s->ry>x*x+y*y){x=s->rx;y=s->ry;}
  s->lx=y;s->ly=-x;x=s->rx;s->rx=s->ry;s->ry=-x;
 }
 /* SDL names describe Xbox positions. RG35XX-H letters are A=east,
    B=south, X=north, Y=west; send the matching Unity/XInput letter. */
 int mapping[][2]={{SDL_CONTROLLER_BUTTON_DPAD_UP,0},{SDL_CONTROLLER_BUTTON_DPAD_DOWN,1},{SDL_CONTROLLER_BUTTON_DPAD_LEFT,2},{SDL_CONTROLLER_BUTTON_DPAD_RIGHT,3},{SDL_CONTROLLER_BUTTON_X,4},{SDL_CONTROLLER_BUTTON_A,5},{SDL_CONTROLLER_BUTTON_B,6},{SDL_CONTROLLER_BUTTON_Y,7},{SDL_CONTROLLER_BUTTON_LEFTSTICK,8},{SDL_CONTROLLER_BUTTON_RIGHTSTICK,9},{SDL_CONTROLLER_BUTTON_LEFTSHOULDER,10},{SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,11},{SDL_CONTROLLER_BUTTON_START,12},{SDL_CONTROLLER_BUTTON_BACK,13}};
 for(size_t i=0;i<sizeof(mapping)/sizeof(*mapping);i++)if(button(p,mapping[i][0]))s->buttons|=1U<<mapping[i][1];
 void *ptr=&event,*args[]={&ptr},*error=NULL;invoke(queue,NULL,args,&error);if(error){report("QueueEvent",error);return 1;}
 if(common_get){
  common_instance=invoke(common_get,NULL,NULL,&error);
  if(common_instance&&!error && !activated && frame>=120){
   int desired=2,type=1;void *params[]={&desired},*supported=method_get(common_class,"IsInputModePossiblySupported",1);
   void *result=supported?invoke(supported,common_instance,params,&error):NULL;
   void *pad_input=*(void**)((char*)common_instance+0x28);
   fprintf(stderr,"[MONUMENT-INPUT] stock gamepad support=%d controller=%p orb=%p\n",result?*(unsigned char*)((char*)result+16):-1,pad_input,pad_input?*(void**)((char*)pad_input+0x20):NULL);
   if(pad_input){void *refresh=method_get(common_class,"RefreshInputControllers",2),*refresh_args[]={&desired,&type};if(refresh){invoke(refresh,common_instance,refresh_args,&error);if(error)report("Enable native Gamepad",error);else {
    if(getenv("MONUMENT_FIXED_PAD") && presentation_update){struct {int has_value,value;} nullable_type={1,1};int changed=1;void *presentation_args[]={&desired,&nullable_type,&changed};invoke(presentation_update,NULL,presentation_args,&error);if(error)report("Xbox presentation",error);}
    activated=1;
   }}}
  }
  if(common_instance&&!error){void *controller=*(void**)((char*)common_instance+0x10);int mode=controller&&controller==*(void**)((char*)common_instance+0x28)?2:controller==*(void**)((char*)common_instance+0x18)?1:0;if(mode!=previous_mode){fprintf(stderr,"[MONUMENT-INPUT] controller mode=%d (touch0 mouse1 gamepad2)\n",mode);previous_mode=mode;}}
 }
 return 0;
}
void monument_input_save(void){if(prefs_save){void *error=NULL;invoke(prefs_save,NULL,NULL,&error);if(error)report("PlayerPrefs.Save",error);}}

int monument_cursor_position(float *x,float *y,int *down){(void)x;(void)y;(void)down;return 0;}

int monument_input_resize(int notify){
 if(!screen_resize){
  void *domain=domain_get();if(!domain)return 0;size_t count=0;void **assemblies=assemblies_get(domain,&count);
  for(size_t i=0;i<count;i++){
   void *image=image_get(assemblies[i]),*screen=class_get(image,"UnityEngine","Screen"),*presentation=class_get(image,"MVCommon","GamePresentation");
   if(screen){screen_resize=method_get(screen,"SetResolution",3);screen_width=method_get(screen,"get_width",0);screen_height=method_get(screen,"get_height",0);}
   if(presentation)presentation_force=method_get(presentation,"ForceOnPresentationChanged",0);
   void *camera=class_get(image,"","UICamera"),*object=class_get(image,"UnityEngine","Object");
   if(camera && class_type && type_object){ui_camera_type=type_object(class_type(camera));ui_camera_resize=method_get(camera,"OnScreenSizeChangedHandler",0);}
   if(object)find_objects=method_get(object,"FindObjectsOfType",2);
  }
 }
 if(!screen_resize)return 0;
 void *error=NULL;
 if(!notify){int w=monument_display_width(),h=monument_display_height();unsigned char fullscreen=1;void *args[]={&w,&h,&fullscreen};invoke(screen_resize,NULL,args,&error);fprintf(stderr,"[MONUMENT-RESIZE] requested=%dx%d\n",w,h);}
 else {
  /* Reflow actual scene cameras, after presentation listeners have finished. The game's
     own handler selects the screen archetype, invalidates UILayout and lays
     out its children; do not retain positions from the preceding orientation. */
  if(notify==1 && presentation_force)invoke(presentation_force,NULL,NULL,&error);
  if(notify==2 && !error && find_objects && ui_camera_type && ui_camera_resize){
   unsigned char inactive=0;void *args[]={ui_camera_type,&inactive};
   void *array=invoke(find_objects,NULL,args,&error);
   if(array && !error){uintptr_t count=*(uintptr_t*)((char*)array+24);void **items=(void**)((char*)array+32);
    for(uintptr_t i=0;i<count && !error;i++)invoke(ui_camera_resize,items[i],NULL,&error);
    fprintf(stderr,"[MONUMENT-RESIZE] original HUD reflow cameras=%lu\n",(unsigned long)count);
   }
  }
  void *we=NULL,*he=NULL,*wb=screen_width?invoke(screen_width,NULL,NULL,&we):NULL,*hb=screen_height?invoke(screen_height,NULL,NULL,&he):NULL;
  fprintf(stderr,"[MONUMENT-RESIZE] Unity=%dx%d logical=%dx%d\n",wb?*(int*)((char*)wb+16):0,hb?*(int*)((char*)hb+16):0,monument_display_width(),monument_display_height());
 }
 if(error){report("Screen resize",error);return 0;}return 1;
}
