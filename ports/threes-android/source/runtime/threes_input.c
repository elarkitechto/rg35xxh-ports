#include "threes_input.h"
#include "threes_ui.h"
#include "threes_display.h"
#include "egl_shim.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void *(*domain_get)(void);
static void **(*assemblies_get)(void *,size_t *);
static void *(*image_get)(void *);
static void *(*class_get)(void *,const char *,const char *);
static void *(*parent_get)(void *);
static void *(*method_get)(void *,const char *,int);
static void (*class_init)(void *);
static void *(*field_get)(void *,const char *);
static void (*static_get)(void *,void *);
static void *state_field,*moves_field,*score_field,*nope_field,*merge_field;
extern void threes_rumble_play(int);
static int playing_context,test_controls;
static uint32_t test_buttons,blocked_faces;
static int previous_context=-1,previous_tate=-1,blocked_motion;
static int board_counts(int counts[32]);
static int read_int(void *field){int value=0;if(field)static_get(field,&value);return value;}
static void feedback_step(void){
  static int ready,last_moves,last_state,last_nope,previous_counts[32];
  int state=read_int(state_field),moves=read_int(moves_field),nope=read_int(nope_field);
  int counts[32]={0},valid=board_counts(counts),merged_value=0;
  if(ready&&valid&&moves>last_moves&&last_state==0){
    /* Compare original Slot values, not a record counter: repeated equal-rank
     * merges must produce feedback even when no record changes. */
    for(int rank=3;rank<30;rank++)
      if(previous_counts[rank]>counts[rank]&&counts[rank+1]>previous_counts[rank+1])
        merged_value=3<<(rank-2);
  }
  if(ready){
    if(last_state!=1&&state==1)threes_rumble_play(7);
    else if(merged_value>=12){
      threes_rumble_play(8);
      fprintf(stderr,"[THREES-MERGE] result=%d pulse=strong\n",merged_value);
    }
    else if(merged_value>0)threes_rumble_play(6);
    else if(moves>last_moves&&last_state==0)threes_rumble_play(4);
    else if(nope&&!last_nope&&state==0)threes_rumble_play(5);
  }
  if(valid&&(!ready||moves!=last_moves||state!=0))memcpy(previous_counts,counts,sizeof(counts));
  ready=valid;last_moves=moves;last_state=state;last_nope=nope;
}
static void *(*invoke)(void *,void *,void **,void **);
static void *(*string_new)(const char *);
static void (*format_exception)(void *,char *,int);
static void *queue_method,*threes_class,*remember_class,*device;
static int device_id,chord_latched;
static double (*input_time)(void *);

#pragma pack(push,4)
struct gamepad_state { uint32_t buttons; float lx,ly,rx,ry,lt,rt; };
struct state_event {
  uint32_t type; uint16_t size,device;
  double time; uint32_t event_id,format;
  struct gamepad_state state;
};
#pragma pack(pop)
_Static_assert(sizeof(struct state_event)==52,"Unity StateEvent layout");

static void exception_report(const char *action,void *exception) {
  char text[2048]={0};if(exception&&format_exception)format_exception(exception,text,sizeof(text));
  fprintf(stderr,"[THREES-INPUT] %s failed: %s\n",action,text);
}
void threes_input_init(so_module *il2cpp) {
  so_module *saved=so_save();so_use(il2cpp);
#define API(target,name) target=(void *)so_find_addr_safe(name)
  API(domain_get,"il2cpp_domain_get");API(assemblies_get,"il2cpp_domain_get_assemblies");
  API(image_get,"il2cpp_assembly_get_image");API(class_get,"il2cpp_class_from_name");
  API(parent_get,"il2cpp_class_get_parent");API(method_get,"il2cpp_class_get_method_from_name");
  API(field_get,"il2cpp_class_get_field_from_name");API(static_get,"il2cpp_field_static_get_value");
  API(class_init,"il2cpp_runtime_class_init");API(invoke,"il2cpp_runtime_invoke");
  API(string_new,"il2cpp_string_new");API(format_exception,"il2cpp_format_exception");
#undef API
  /* NativeInputSystem.currentTime, pinned to this IL2CPP image. */
  input_time=(void *)(0x500000000UL+0xf314d0);
  so_use(saved);free(saved);
  threes_ui_init(il2cpp);
  test_controls=getenv("THREES_TEST_CONTROLS")!=NULL;
}
static int attach_gamepad(void) {
  void *domain=domain_get();if(!domain)return 0;
  size_t count=0;void **assemblies=assemblies_get(domain,&count);void *input_class=NULL;
  for(size_t i=0;i<count;i++) {
    void *image=image_get(assemblies[i]);
    if(!input_class)input_class=class_get(image,"UnityEngine.InputSystem","InputSystem");
    if(!threes_class)threes_class=class_get(image,"","Threes");
    if(!remember_class)remember_class=class_get(image,"","VoRemember");
  }
  if(!input_class)return 0;
  if(threes_class){
    state_field=field_get(threes_class,"state");moves_field=field_get(threes_class,"numMoves");
    score_field=field_get(threes_class,"score");nope_field=field_get(threes_class,"didNope");merge_field=field_get(threes_class,"lastHighestMerge");
  }
  class_init(input_class);
  void *add=method_get(input_class,"AddDevice",3);
  queue_method=method_get(input_class,"QueueEvent",1);
  if(!add||!queue_method)return 0;
  void *layout=string_new("Gamepad"),*name=string_new("RG35XX-H"),*variants=string_new("");
  void *args[]={layout,name,variants},*error=NULL;
  device=invoke(add,NULL,args,&error);
  if(error||!device){exception_report("AddDevice",error);return 0;}
  device_id=*(int *)((char *)device+0xe0);
  if(device_id<=0||device_id>65535){fprintf(stderr,"[THREES-INPUT] invalid device id %d\n",device_id);exit(24);}
  fprintf(stderr,"[THREES-INPUT] SDL controller connected to Unity Gamepad device=%d\n",device_id);
  return 1;
}
static int board_counts(int counts[32]){
  if(!threes_class)return 0;
  void *getter=method_get(parent_get(threes_class),"get_I",0),*error=NULL;
  void *game=getter?invoke(getter,NULL,NULL,&error):NULL;if(!game||error)return 0;
  void *grid=*(void **)((char *)game+0xa0);if(!grid)return 0;
  size_t length=*(size_t *)((char *)grid+0x18);if(length>16)return 0;
  void **slots=(void *)((char *)grid+0x20);
  for(size_t i=0;i<length;i++)if(slots[i]){
    int value=*(int *)((char *)slots[i]+0x10),rank=value;
    if(value>2){rank=3;while(value>3&&rank<31){value/=2;rank++;}}
    if(rank>0&&rank<32)counts[rank]++;
  }
  return 1;
}
static void trace_saved_board(const char *phase){
  if(!getenv("THREES_TRACE_SAVE")||!remember_class)return;
  void *getter=method_get(remember_class,"CryptoInt",1);
  fprintf(stderr,"[THREES-BOARD] %s",phase);
  for(int i=0;i<16;i++){char key[32];snprintf(key,sizeof(key),"Grid%d",i);
    void *name=string_new(key),*args[]={name},*error=NULL;
    void *value=invoke(getter,NULL,args,&error);
    if(error||!value){fprintf(stderr," error");break;}
    fprintf(stderr," %d",*(int *)((char *)value+16));
  }
  fprintf(stderr,"\n");
}
static float axis(SDL_GameController *pad,SDL_GameControllerAxis which) {
  if(test_controls||!pad)return 0;float n=SDL_GameControllerGetAxis(pad,which)/32768.0f;
  return fabsf(n)<.18f?0:n;
}
static int button(SDL_GameController *pad,SDL_GameControllerButton which) {
  if(test_controls)return (test_buttons&(1U<<which))!=0;
  return pad&&SDL_GameControllerGetButton(pad,which);
}
static int face_button(SDL_GameController *pad,SDL_GameControllerButton which){
  return !(blocked_faces&(1U<<which))&&button(pad,which);
}
static void rotate(float *x,float *y) {
  if(threes_display_tate()){float previous=*x;*x=*y;*y=-previous;}
}
int threes_input_step(long frame) {
  test_buttons=0;
  if(test_controls&&frame>=180){
    int phase=(frame-180)%360,step=phase/90;
    if(phase%90<6){
      int key=step==0?SDL_CONTROLLER_BUTTON_START:step==1?SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:step==2?SDL_CONTROLLER_BUTTON_LEFTSHOULDER:SDL_CONTROLLER_BUTTON_B;
      test_buttons=1U<<key;
    }
  }
  SDL_Event e;
  while(SDL_PollEvent(&e))if(e.type==SDL_QUIT)return 1;
  SDL_GameController *pad=egl_shim_controller();
  int start=button(pad,SDL_CONTROLLER_BUTTON_START);
  if(start&&(button(pad,SDL_CONTROLLER_BUTTON_GUIDE)||button(pad,SDL_CONTROLLER_BUTTON_BACK)))return 1;
  float lt=axis(pad,SDL_CONTROLLER_AXIS_TRIGGERLEFT),rt=axis(pad,SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
  int chord=lt>.5f&&rt>.5f;
  if(chord&&!chord_latched){threes_display_toggle();fprintf(stderr,"[THREES-INPUT] orientation=%s\n",threes_display_tate()?"TATE":"landscape");}
  chord_latched=chord;
  if(getenv("THREES_TEST_ROTATE")&&(frame==600||frame==900))threes_display_toggle();
  if(!device&&frame>=60&&frame%60==0)attach_gamepad();
  if(!device)return 0;
  if(frame==120)trace_saved_board("loaded");
  playing_context=read_int(state_field)==0&&threes_ui_gameplay();
  feedback_step();
  uint32_t raw_faces=0;
  for(int i=SDL_CONTROLLER_BUTTON_A;i<=SDL_CONTROLLER_BUTTON_Y;i++)if(button(pad,i))raw_faces|=1U<<i;
  int tate=threes_display_tate();
  if(playing_context!=previous_context||tate!=previous_tate){blocked_faces|=raw_faces;blocked_motion=1;}
  blocked_faces&=raw_faces;previous_context=playing_context;previous_tate=tate;
  struct state_event event={.type=0x53544154,.size=sizeof(event),.device=device_id,
                            .format=0x47504144,.time=input_time(NULL)};
  struct gamepad_state *s=&event.state;
  float x=axis(pad,SDL_CONTROLLER_AXIS_LEFTX),y=-axis(pad,SDL_CONTROLLER_AXIS_LEFTY);
  float rx=axis(pad,SDL_CONTROLLER_AXIS_RIGHTX),ry=-axis(pad,SDL_CONTROLLER_AXIS_RIGHTY);
  const Uint8 *keys=SDL_GetKeyboardState(NULL);
  float dx=button(pad,SDL_CONTROLLER_BUTTON_DPAD_RIGHT)-button(pad,SDL_CONTROLLER_BUTTON_DPAD_LEFT);
  float dy=button(pad,SDL_CONTROLLER_BUTTON_DPAD_UP)-button(pad,SDL_CONTROLLER_BUTTON_DPAD_DOWN);
  dx+=keys[SDL_SCANCODE_RIGHT]-keys[SDL_SCANCODE_LEFT];dy+=keys[SDL_SCANCODE_UP]-keys[SDL_SCANCODE_DOWN];
  if(threes_display_tate()&&playing_context) {
    dx+=face_button(pad,SDL_CONTROLLER_BUTTON_B)-face_button(pad,SDL_CONTROLLER_BUTTON_X);
    dy+=face_button(pad,SDL_CONTROLLER_BUTTON_Y)-face_button(pad,SDL_CONTROLLER_BUTTON_A);
    if(button(pad,SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))s->buttons|=1U<<6;
    if(button(pad,SDL_CONTROLLER_BUTTON_LEFTSHOULDER))s->buttons|=1U<<5;
  } else {
    /* Menu face buttons keep their labelled meaning in either orientation. */
    if(face_button(pad,SDL_CONTROLLER_BUTTON_A))s->buttons|=1U<<6;
    if(face_button(pad,SDL_CONTROLLER_BUTTON_B))s->buttons|=1U<<5;
    if(face_button(pad,SDL_CONTROLLER_BUTTON_X))s->buttons|=1U<<7;
    if(face_button(pad,SDL_CONTROLLER_BUTTON_Y))s->buttons|=1U<<4;
    if(button(pad,SDL_CONTROLLER_BUTTON_LEFTSHOULDER))s->buttons|=1U<<10;
    if(button(pad,SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))s->buttons|=1U<<11;
  }
  if(blocked_motion){
    if(x==0&&y==0&&rx==0&&ry==0&&dx==0&&dy==0)blocked_motion=0;
    else x=y=rx=ry=dx=dy=0;
  }
  rotate(&x,&y);rotate(&rx,&ry);rotate(&dx,&dy);
  if(fabsf(rx)+fabsf(ry)>fabsf(x)+fabsf(y)){x=rx;y=ry;}
  /* The original Move action reads leftStick; mirror digital directions too. */
  if(fabsf(dx)+fabsf(dy)>fabsf(x)+fabsf(y)){x=dx;y=dy;}
  s->lx=x;s->ly=y;s->rx=rx;s->ry=ry;s->lt=chord?0:lt;s->rt=chord?0:rt;
  if(dx>0)s->buttons|=1U<<3;if(dx<0)s->buttons|=1U<<2;
  if(dy>0)s->buttons|=1U<<0;if(dy<0)s->buttons|=1U<<1;
  if(start)s->buttons|=1U<<12;
  if(button(pad,SDL_CONTROLLER_BUTTON_BACK))s->buttons|=1U<<13;
  if(keys[SDL_SCANCODE_RETURN])s->buttons|=1U<<6;
  if(keys[SDL_SCANCODE_ESCAPE])s->buttons|=1U<<5;
  if(test_controls&&test_buttons){
    fprintf(stderr,"[THREES-MAP] frame=%ld context=%s raw=%x buttons=%x left=%.1f,%.1f\n",frame,playing_context?"game":"menu",test_buttons,s->buttons,s->lx,s->ly);
  }
  /* Diagnostic only: repeatable actions through the identical Unity queue. */
  if(getenv("THREES_TEST_INPUT")&&frame>=180) {
    int phase=(frame-180)%30,index=(frame-180)/30;
    uint32_t random=(uint32_t)(index+1)*747796405U+2891336453U;
    random=((random>>((random>>28)+4))^random)*277803737U;
    int step=index%8==0?0:1+(int)(((random>>22)^random)&3);
    if(phase<12){
      if(step==0)s->buttons|=1U<<6;
      else {s->buttons|=1U<<(step==1?3:step==2?1:step==3?2:0);
        s->lx=step==1?1:step==3?-1:0;s->ly=step==2?-1:step==4?1:0;}
    }
  }
  threes_ui_step(s->buttons,s->lx,s->ly);
  void *ptr=&event,*args[]={&ptr},*error=NULL;
  invoke(queue_method,NULL,args,&error);
  if(error){exception_report("QueueEvent",error);return 1;}
  return 0;
}
void threes_input_save(void) {
  if(!device||!threes_class)return;
  void *getter=method_get(parent_get(threes_class),"get_I",0),*error=NULL;
  void *instance=getter?invoke(getter,NULL,NULL,&error):NULL;
  void *save=method_get(threes_class,"SaveGameForReal",0);
  if(!error&&instance&&save){invoke(save,instance,NULL,&error);if(!error)fprintf(stderr,"[THREES-SAVE] original SaveGameForReal completed\n");}
  if(error)exception_report("SaveGameForReal",error);
  else trace_saved_board("saved");
}
