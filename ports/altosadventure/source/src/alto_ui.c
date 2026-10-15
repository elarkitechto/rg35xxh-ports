/* Native SDL -> the game's NGUI buttons. No direct title->gameplay shortcut. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "alto_ui.h"
#include "egl_shim.h"
#include <SDL2/SDL.h>
extern int egl_shim_width(void),egl_shim_height(void);
#define FN(off,type) ((type)(0x500000000UL+(off)))
typedef struct {float x,y,z;} vec3;
typedef void *(*object_fn)(void*,void*);
struct target {void *go,*press,*slider;float x,y;int primary;char name[96];};
static struct target targets[256];static int count,last_state=-1,last_click=-1000,last_refresh=-1000;
static unsigned title_ready_at;
static void *dialog_instance_field,*dialog_instance_getter;
static void *focus,*button_type,*click_type,*widget_type,*gui_class,*title_class,*pause_class,*slider_type,*dialog_class;
static void *(*resolve_api)(const char*);
static void *(*class_type)(void*),*(*type_object)(void*);
static void *find_class(const char *name){
 void *(*domain)(void)=resolve_api("il2cpp_domain_get");
 void **(*assemblies)(void*,size_t*)=resolve_api("il2cpp_domain_get_assemblies");
 void *(*image)(void*)=resolve_api("il2cpp_assembly_get_image");
 void *(*klass)(void*,const char*,const char*)=resolve_api("il2cpp_class_from_name");
 size_t n=0;void **a=assemblies(domain(),&n);
 for(size_t i=0;i<n;i++){void *k=klass(image(a[i]),"",name);if(k)return k;}return NULL;
}
static void *find_object(void *klass){return klass?FN(0xb10e68,object_fn)(type_object(class_type(klass)),NULL):NULL;}
static void object_name(void *go,char *out){
 char *s=FN(0xb10420,object_fn)(go,NULL);out[0]=0;if(!s)return;
 int n=*(int*)(s+0x10);if(n>95)n=95;if(n<0)return;
 for(int i=0;i<n;i++){unsigned short c=*(unsigned short*)(s+0x14+i*2);out[i]=c<128?(char)c:'?';}out[n]=0;
}
void alto_ui_init(void *(*resolver)(const char*)){
 resolve_api=resolver;class_type=resolver("il2cpp_class_get_type");type_object=resolver("il2cpp_type_get_object");
 void *k=find_class("UIButton");button_type=k?type_object(class_type(k)):NULL;
 k=find_class("GUIOnClick");click_type=k?type_object(class_type(k)):NULL;
 k=find_class("UIWidget");widget_type=k?type_object(class_type(k)):NULL;
 k=find_class("UISlider");slider_type=k?type_object(class_type(k)):NULL;pause_class=find_class("GuiPauseMenu");
 dialog_class=find_class("DialogBox");
 if(dialog_class){
  void *(*parent)(void*)=resolver("il2cpp_class_get_parent");
  void *(*field)(void*,const char*)=resolver("il2cpp_class_get_field_from_name");
  dialog_instance_field=field(parent(dialog_class),"_instance");
  void *(*method)(void*,const char*,int)=resolver("il2cpp_class_get_method_from_name");
  dialog_instance_getter=method(parent(dialog_class),"get_Instance",0);
 }
 gui_class=find_class("GUIManager");title_class=find_class("GuiTitleScreen");
 fprintf(stderr,"[ALTO-UI] NGUI button and action types ready\n");
}
static void collect(void *type,int actions){
 if(!type)return;char *array=FN(0xb10d20,object_fn)(type,NULL);if(!array)return;
 size_t n=*(size_t*)(array+0x18);if(n>4096)return;
 for(size_t i=0;i<n && count<256;i++){
  char *component=*(void**)(array+0x20+8*i);if(!component)continue;
  void *go=FN(0xb420ac,object_fn)(component,NULL);
  if(!go || !FN(0xb4aa1c,int(*)(void*,void*))(go,NULL))continue;
  if(!FN(0xb3e994,int(*)(void*,void*))(component,NULL))continue;
  if(actions==1){void *collider=*(void**)(component+0x30);if(collider && !FN(0xf714e4,int(*)(void*,void*))(collider,NULL))continue;}
  else if(actions==0 && !FN(0xab3f44,int(*)(void*,void*))(component,NULL))continue;
  int existing=-1;for(int j=0;j<count;j++)if(targets[j].go==go){existing=j;break;}
  if(existing>=0){if(actions==2)targets[existing].slider=component;if(actions==1 && *(int*)(component+0x18)==1)targets[existing].press=component;continue;}
  void *widget=FN(0xb42170,void*(*)(void*,void*,void*))(go,widget_type,NULL);
  if(!widget)widget=FN(0xb422a4,void*(*)(void*,void*,int,void*))(go,widget_type,0,NULL);
  if(widget && (!FN(0x7910cc,int(*)(void*,void*))(widget,NULL) || FN(0x7910a0,float(*)(void*,void*))(widget,NULL)<0.02f))continue;
  void *transform=FN(0xb4a8bc,object_fn)(go,NULL);if(!transform)continue;
  vec3 world=widget?FN(0x791e48,vec3(*)(void*,void*))(widget,NULL):FN(0xf23930,vec3(*)(void*,void*))(transform,NULL);
  int layer=FN(0xb4a8fc,int(*)(void*,void*))(go,NULL);
  void *camera_component=FN(0xabac60,void*(*)(int,void*))(layer,NULL);if(!camera_component)continue;
  void *camera=FN(0xab92dc,object_fn)(camera_component,NULL);if(!camera)continue;
  vec3 screen=FN(0xb408c4,vec3(*)(void*,vec3,void*))(camera,world,NULL);
  if(!isfinite(screen.x)||!isfinite(screen.y)||screen.z<=0 ||screen.x<0||screen.y<0||screen.x>egl_shim_width()||screen.y>egl_shim_height())continue;
  struct target *t=&targets[count++];t->go=go;t->x=screen.x;t->y=screen.y;t->press=actions==1 && *(int*)(component+0x18)==1?component:NULL;
  t->slider=actions==2?component:NULL;
  t->primary=actions==1 && component[0x28];object_name(go,t->name);
 }
}
static void set_focus(int index){
 if(index<0||index>=count)return;void *go=targets[index].go;
 if(go!=focus){focus=go;
  FN(0xab87b8,void(*)(int,void*))(2,NULL);
  FN(0xaba20c,void(*)(void*,void*))(go,NULL);FN(0xabb574,void(*)(void*,void*))(go,NULL);
  fprintf(stderr,"[ALTO-UI] focus %s (%.0f,%.0f)\n",targets[index].name,targets[index].x,targets[index].y);
 }
}
static int selected(void){for(int i=0;i<count;i++)if(targets[i].go==focus)return i;return -1;}
static int initial(int state){
 int best=0,score=-999;
 void *preferred=NULL;
 if(state==2){char *title=find_object(title_class);if(title)preferred=*(void**)(title+0x38);}
 for(int i=0;i<count;i++){
  char name[96];for(int j=0;j<96;j++){char c=targets[i].name[j];name[j]=c>='A'&&c<='Z'?c+32:c;if(!c)break;}
  int s=targets[i].primary?10:0;
  if(targets[i].go==preferred)s+=100;
  if(strstr(name,"next")||strstr(name,"resume")||strstr(name,"restart")||strstr(name,"retry"))s+=30;
  if(state==2 && strstr(name,"start"))s+=40;
  if(strstr(name,"video")||strstr(name,"premium")||strstr(name,"purchase")||strstr(name,"coins"))s-=100;
  if(s>score){score=s;best=i;}
 }return best;
}
static int ready(void){void *gui=find_object(gui_class);return !gui || !FN(0x6b56ac,int(*)(void*,void*))(gui,NULL);}
static void click(int index,int frame){
 if(index<0||index>=count ||frame-last_click<12 || !ready())return;
 last_click=frame;set_focus(index);fprintf(stderr,"[ALTO-UI] activate %s\n",targets[index].name);
 if(targets[index].press)FN(0x6c7374,void(*)(void*,void*))(targets[index].press,NULL);
 else {void *(*new_string)(const char*)=resolve_api("il2cpp_string_new");FN(0xaba950,void(*)(void*,void*,void*,void*))(targets[index].go,new_string("OnClick"),NULL,NULL);}
 focus=NULL;
}
int alto_ui_tick(int frame,int state,void *manager,struct alto_ui_input in){
 if(!resolve_api)return 0;
 /* RefreshMenu may reactivate these: hide before collecting controller targets. */
 if(state==6 || state==12){
  char *pause=find_object(pause_class);
  if(pause)for(int offset=0x180;offset<=0x188;offset+=8){
   void *go=*(void**)(pause+offset);
   if(go && FN(0xb4aa1c,int(*)(void*,void*))(go,NULL)){
    FN(0xb4a98c,void(*)(void*,int,void*))(go,0,NULL);
    fprintf(stderr,"[ALTO-UI] Google Play entry hidden (%x)\n",offset);
   }
  }
 }
 char *dialog=NULL;
 if(dialog_instance_field){
  void (*get)(void*,void*)=resolve_api("il2cpp_field_static_get_value");
  get(dialog_instance_field,&dialog);
 }
 if(!dialog)dialog=find_object(dialog_class);
 if(!dialog && dialog_instance_getter && getenv("ALTO_DIALOG_SMOKE") && state==2 && frame>=900){
  void *(*invoke)(void*,void*,void**,void**)=resolve_api("il2cpp_runtime_invoke");
  void *exception=NULL;dialog=invoke(dialog_instance_getter,NULL,NULL,&exception);
  if(exception){fprintf(stderr,"[ALTO-DIALOG-TEST] singleton exception\n");dialog=NULL;}
 }
 /* Explicit diagnostic only: reproduce the real one-button DialogBox without
  * spending coins or touching purchased upgrades. Normal launcher unsets it. */
 if(getenv("ALTO_DIALOG_SMOKE") && dialog && state==2 && frame>=900){
  static int phase;static unsigned deadline;
  unsigned now=SDL_GetTicks();
  if((phase==0 || phase==3) && !*(unsigned char*)(dialog+0xd8)){
   void *(*str)(const char*)=resolve_api("il2cpp_string_new");
   FN(0x6be25c,void*(*)(void*,int,void*,void*,void*,void*,int,void*))
    (dialog,1,str("Upgrade complete"),str("Controller dismiss test"),str("Thanks!"),str("Cancel"),0,NULL);
   deadline=now+2500;phase++;
  }else if((phase==1 || phase==4) && now>=deadline){
   in.confirm=phase==1;in.back=phase==4;phase++;deadline=now+1500;
  }else if((phase==2 || phase==5) && now>=deadline){
   int shown=*(unsigned char*)(dialog+0xd8);
   fprintf(stderr,"[ALTO-DIALOG-TEST] %s shown=%d %s\n",phase==2?"confirm":"back",shown,shown?"FAIL":"PASS");
   phase=shown?6:phase+1;
  }
 }
 if(dialog && *(unsigned char*)(dialog+0xd8)){
  /* Dismiss has no caller waiting to hide it after ResponsePositive. */
  char *response=*(void**)(dialog+0xf0);
  int type=response?*(int*)(response+0x14):-1;
  void *dismiss=*(void**)(dialog+0x58);
  int dismiss_visible=dismiss && FN(0xb4aa1c,int(*)(void*,void*))(dismiss,NULL);
  if((type==1 || (type<0 && dismiss_visible)) && (in.confirm || in.back)){
   if(frame-last_click>=12 && !*(void**)(dialog+0xe0)){
    last_click=frame;focus=NULL;last_refresh=-1000;
    fprintf(stderr,"[ALTO-UI] dialog dismiss type=%d\n",type);
    FN(0x6be418,void(*)(void*,void*,void*))(dialog,response,NULL);
   }
  }else if(type==2 || type==3){
   if(in.confirm){fprintf(stderr,"[ALTO-UI] dialog accept type=%d\n",type);FN(0x6be5f4,void(*)(void*,void*))(dialog,NULL);}
   else if(in.back){fprintf(stderr,"[ALTO-UI] dialog cancel type=%d\n",type);FN(0x6be694,void(*)(void*,void*))(dialog,NULL);}
  }else if(type==4 && in.back){
   fprintf(stderr,"[ALTO-UI] dialog verify cancel\n");FN(0x6be694,void(*)(void*,void*))(dialog,NULL);
  }
  /* Wait/transition dialogs do not accept. Never forward modal input to shop. */
  return 1;
 }

 int menu=state!=0 && state!=1 && state!=5 && state!=10 && state!=11;
 if(state!=last_state){fprintf(stderr,"[ALTO-UI] screen state=%d\n",state);last_state=state;focus=NULL;last_refresh=-1000;if(state==2){float seconds=manager?*(float*)((char*)manager+0x4c):3.0f;if(seconds<2)seconds=2;title_ready_at=SDL_GetTicks()+(unsigned)(seconds*1000);}}
 if(state==2 && (SDL_GetTicks()<title_ready_at || !ready()))return 1;
 if(in.pause && ready()){
  if(state==5)FN(0x6b57a0,void(*)(void*,int,float,void*))(manager,6,-1.0f,NULL);
  else if(state==6)FN(0x6b57a0,void(*)(void*,int,float,void*))(manager,5,-1.0f,NULL);
  else if(state==2)in.back=1;
  return 1;
 }
 if(!menu)return 0;
 if((state==6 || state==12) && in.tab && ready()){
  void *pause=find_object(pause_class);if(pause){
   if(in.tab>0)FN(0x6ff4b0,void(*)(void*,void*))(pause,NULL);
   else FN(0x6ff59c,void(*)(void*,void*))(pause,NULL);
   focus=NULL;last_refresh=-1000;fprintf(stderr,"[ALTO-UI] tab %d\n",in.tab);return 1;
  }
 }

 if(frame-last_refresh>=12 || in.confirm||in.back||in.dx||in.dy){count=0;collect(button_type,0);collect(click_type,1);collect(slider_type,2);last_refresh=frame;}
 if(getenv("ALTO_SETTINGS_SMOKE") && frame%120==0){
  for(int s=0;s<count;s++)if(targets[s].slider){
   float v=FN(0x98aec4,float(*)(void*,void*))(targets[s].slider,NULL);
   FN(0x98afd0,void(*)(void*,float,void*))(targets[s].slider,1.0f,NULL);
   fprintf(stderr,"[ALTO-UI] slider test %s %.2f -> %.2f\n",targets[s].name,v,FN(0x98aec4,float(*)(void*,void*))(targets[s].slider,NULL));
  }
 }
 int index=selected();if(index<0 && count)index=initial(state);
 if(index>=0)set_focus(index);
 if(in.dx && index>=0 && targets[index].slider){
  void *slider=targets[index].slider;float value=FN(0x98aec4,float(*)(void*,void*))(slider,NULL);
  value+=in.dx*0.1f;if(value<0)value=0;if(value>1)value=1;
  FN(0x98afd0,void(*)(void*,float,void*))(slider,value,NULL);
  fprintf(stderr,"[ALTO-UI] slider %s=%.2f\n",targets[index].name,value);return 1;
 }
 if(in.dx||in.dy){
  float best=1e30f;int next=-1;
  for(int i=0;i<count;i++){if(i==index)continue;
   float x=targets[i].x-targets[index].x,y=targets[i].y-targets[index].y;
   float forward=x*in.dx+y*in.dy;if(forward<4)continue;
   float perpendicular=fabsf(x*in.dy-y*in.dx);float score=forward+perpendicular*3;
   if(score<best){best=score;next=i;}
  }
  if(next>=0){index=next;set_focus(index);}
 }
 if(in.back && ready()){
  if(state==2){char *title=find_object(title_class);void *go=title?*(void**)(title+0x20):NULL;
   if(go)for(int i=0;i<count;i++)if(targets[i].go==go){click(i,frame);return 1;}
  }else if(state==6){FN(0x6b57a0,void(*)(void*,int,float,void*))(manager,5,-1.0f,NULL);return 1;}
  else {for(int i=0;i<count;i++)if(strstr(targets[i].name,"Back")||strstr(targets[i].name,"Close")||strstr(targets[i].name,"Home")){click(i,frame);return 1;}}
 }
 if(in.confirm && count){if(!targets[index].slider)click(index,frame);return 1;}
 return in.confirm||in.back||in.dx||in.dy;
}
