#define _GNU_SOURCE
/* Android strips MenuNavigation.Update, although gameplay has native Gamepad
 * actions. Navigate the original live Menu/SimpleButton objects by reflection;
 * invoke their original OnPress/Done methods instead of screen coordinates. */
#include "threes_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static void *(*domain_get)(void);
static void **(*assemblies_get)(void *,size_t *);
static void *(*image_get)(void *),*(*object_class)(void *),*(*parent_get)(void *);
static void *(*class_get)(void *,const char *,const char *),*(*method_get)(void *,const char *,int);
static void *(*class_type)(void *),*(*type_object)(void *);
static void *(*invoke)(void *,void *,void **,void **);
static void *menu_class,*find,*get_name,*get_go,*is_active,*set_active,*get_scale,*set_scale;
static void *game_camera_class,*game_camera_getter;
static void *camera_main,*world_to_viewport,*get_position;
static void *focus,*current_menu;static float focus_scale[3];
static uint32_t previous;static int old_direction,focus_index;
static void *call(void *method,void *object,void **args){
  if(!method)return NULL;void *error=NULL,*result=invoke(method,object,args,&error);
  if(error){fprintf(stderr,"[THREES-UI] managed call failed\n");return NULL;}return result;
}
static void utf8(void *string,char *out,size_t size){
  if(!size)return;out[0]=0;if(!string)return;
  int length=*(int *)((char *)string+0x10);unsigned short *chars=(void *)((char *)string+0x14);
  size_t n=0;for(int i=0;i<length&&n+1<size;i++)out[n++]=chars[i]<128?(char)chars[i]:'?';out[n]=0;
}
static void name(void *object,char *out,size_t size){utf8(call(get_name,object,NULL),out,size);}
static int active(void *component){void *go=call(get_go,component,NULL);void *value=go?call(is_active,go,NULL):NULL;return value&&*(unsigned char *)((char *)value+16);}
static int visible(void *button){
  if(!active(button))return 0;
  void *transform=*(void **)((char *)button+0x68),*camera=call(camera_main,NULL,NULL);
  void *position=transform?call(get_position,transform,NULL):NULL;
  if(!camera||!position)return 0;
  void *args[]={(char *)position+16};void *viewport=call(world_to_viewport,camera,args);
  if(!viewport)return 0;float *p=(void *)((char *)viewport+16);
  return p[0]>=0&&p[0]<=1&&p[1]>=0&&p[1]<=1&&p[2]>0;
}
static void unfocus(void){
  if(focus){void *text=*(void **)((char *)focus+0x68);if(text){void *args[]={focus_scale};call(set_scale,text,args);}}
  focus=NULL;
}
static void select_button(void *button){
  if(focus==button)return;unfocus();focus=button;
  void *text=*(void **)((char *)button+0x68);if(text){void *scale=call(get_scale,text,NULL);
    if(scale){memcpy(focus_scale,(char *)scale+16,sizeof(focus_scale));float enlarged[3];
      for(int i=0;i<3;i++)enlarged[i]=focus_scale[i]*1.15f;void *args[]={enlarged};call(set_scale,text,args);}}
  char label[128];name(button,label,sizeof(label));fprintf(stderr,"[THREES-UI] focus=%s\n",label);
}
void threes_ui_init(so_module *module){
  so_module *saved=so_save();so_use(module);
#define API(target,symbol) target=(void *)so_find_addr_safe(symbol)
  API(domain_get,"il2cpp_domain_get");API(assemblies_get,"il2cpp_domain_get_assemblies");
  API(image_get,"il2cpp_assembly_get_image");API(class_get,"il2cpp_class_from_name");
  API(object_class,"il2cpp_object_get_class");API(parent_get,"il2cpp_class_get_parent");
  API(method_get,"il2cpp_class_get_method_from_name");API(class_type,"il2cpp_class_get_type");
  API(type_object,"il2cpp_type_get_object");API(invoke,"il2cpp_runtime_invoke");
#undef API
  so_use(saved);free(saved);
}
static int prepare(void){
  if(find)return 1;void *domain=domain_get();if(!domain)return 0;size_t count=0;
  void **assemblies=assemblies_get(domain,&count);void *obj=NULL,*component=NULL,*go=NULL,*transform=NULL,*camera=NULL;
  for(size_t i=0;i<count;i++){void *image=image_get(assemblies[i]);
    if(!menu_class)menu_class=class_get(image,"","Menu");
    if(!game_camera_class)game_camera_class=class_get(image,"","GameCamera");
    if(!obj)obj=class_get(image,"UnityEngine","Object");
    if(!component)component=class_get(image,"UnityEngine","Component");
    if(!go)go=class_get(image,"UnityEngine","GameObject");
    if(!transform)transform=class_get(image,"UnityEngine","Transform");
    if(!camera)camera=class_get(image,"UnityEngine","Camera");}
  if(!menu_class||!obj||!component||!go||!transform||!camera){menu_class=NULL;return 0;}
  find=method_get(obj,"FindObjectsOfType",2);get_name=method_get(obj,"get_name",0);
  get_go=method_get(component,"get_gameObject",0);is_active=method_get(go,"get_activeInHierarchy",0);
  camera_main=method_get(camera,"get_main",0);world_to_viewport=method_get(camera,"WorldToViewportPoint",1);
  get_position=method_get(transform,"get_position",0);
  set_active=method_get(go,"SetActive",1);
  get_scale=method_get(transform,"get_localScale",0);set_scale=method_get(transform,"set_localScale",1);
  if(game_camera_class)game_camera_getter=method_get(parent_get(game_camera_class),"get_I",0);
  return find&&get_name&&get_go&&is_active;
}
int threes_ui_gameplay(void){
  if(!prepare()||!game_camera_getter)return 0;
  void *camera=call(game_camera_getter,NULL,NULL);if(!camera)return 0;
  void *menu=*(void **)((char *)camera+0x50);if(!menu)return 0;
  char label[128];name(menu,label,sizeof(label));return !strcmp(label,"MidGame");
}
void threes_ui_step(uint32_t buttons,float x,float y){
  int direction=fabsf(y)>.5f?(y>0?-1:1):fabsf(x)>.5f?(x>0?1:-1):0;
  uint32_t pressed=buttons&~previous;previous=buttons;
  int move=direction&&direction!=old_direction?direction:0;old_direction=direction;
  if(!pressed&&!move)return;if(!prepare())return;
  unsigned char include_inactive=0;
  void *type=type_object(class_type(menu_class)),*args[]={type,&include_inactive},*menus=call(find,NULL,args);
  if(!menus)return;size_t length=*(size_t *)((char *)menus+0x18);void **items=(void *)((char *)menus+0x20);
  void *menu=NULL;char menu_name[128];int most_visible=0;
  for(size_t i=0;i<length;i++)if(items[i]&&*(unsigned char *)((char *)items[i]+0x34)&&active(items[i])){
    name(items[i],menu_name,sizeof(menu_name));
    if(strstr(menu_name,"Name")){menu=items[i];break;}
    void *array=*(void **)((char *)items[i]+0x20);if(!array)continue;
    size_t n=*(size_t *)((char *)array+0x18);void **all=(void *)((char *)array+0x20);int total=0;
    for(size_t j=0;j<n;j++)if(all[j]&&visible(all[j]))total++;
    if(total>most_visible){menu=items[i];most_visible=total;}
  }
  if(menu!=current_menu){unfocus();current_menu=menu;focus_index=0;if(menu){name(menu,menu_name,sizeof(menu_name));fprintf(stderr,"[THREES-UI] menu=%s\n",menu_name);}}
  if(!menu)return;
  name(menu,menu_name,sizeof(menu_name));
  if(strstr(menu_name,"Name")){
    if(pressed&((1U<<6)|(1U<<5)|(1U<<12))){
      call(method_get(object_class(menu),"Done",0),menu,NULL);
      fprintf(stderr,"[THREES-UI] accepted score signature\n");}
    return;
  }
  /* The original swipe carousel owns its page activation and headers. */
  int page=(pressed&(1U<<11))?1:(pressed&(1U<<10))?-1:0;
  if(move&&fabsf(x)>.5f&&fabsf(x)>fabsf(y))page=x>0?1:-1;
  if(page&&game_camera_getter){
    void *camera=call(game_camera_getter,NULL,NULL);
    void *master=camera?*(void **)((char *)camera+0x50):NULL;
    void *set_index=master?method_get(object_class(master),"SetIndex",1):NULL;
    if(set_index){
      int old=*(int *)((char *)master+0x128),minimum=*(int *)((char *)master+0x120);
      int maximum=*(int *)((char *)master+0x118)-1,next=old+page;
      if(next<minimum)next=minimum;if(next>maximum)next=maximum;
      if(next!=old){unfocus();focus_index=0;void *args[]={&next};call(set_index,master,args);
        fprintf(stderr,"[THREES-PAGE] %d -> %d bounds=%d..%d\n",old,next,minimum,maximum);}
      return;
    }
  }
  /* Gameplay directions and confirm belong to the board, not the HUD. */
  if(!strcmp(menu_name,"MidGame")){
    const char *wanted=(pressed&(1U<<12))?"Pause":(pressed&(1U<<13))?"Stats":NULL;
    if(!wanted){unfocus();return;}
    void *array=*(void **)((char *)menu+0x20);if(!array)return;
    size_t n=*(size_t *)((char *)array+0x18);void **all=(void *)((char *)array+0x20);
    for(size_t i=0;i<n;i++)if(all[i]&&visible(all[i])){char label[128];name(all[i],label,sizeof(label));
      if(!strcmp(label,wanted)){void *text=call(get_name,all[i],NULL),*args[]={text};call(method_get(object_class(menu),"OnPress",1),menu,args);break;}}
    return;
  }
  if(*(unsigned char *)((char *)menu+0x41))return;
  /* A visible page and its parent toolbar are separate Menu objects. Keep
   * each button's owner so Back/Close remains reachable across that boundary. */
  void *buttons_live[128],*owners[128];int count=0;
  for(size_t m=0;m<length;m++){
    void *owner=items[m];if(!owner||!active(owner)||*(unsigned char *)((char *)owner+0x41))continue;
    void *array=*(void **)((char *)owner+0x20);if(!array)continue;
    size_t n=*(size_t *)((char *)array+0x18);void **all=(void *)((char *)array+0x20);
    for(size_t i=0;i<n&&count<128;i++)if(all[i]&&visible(all[i])){
      char label[128];name(all[i],label,sizeof(label));
      if(!strcmp(label,"Tweet")||!strcmp(label,"GC")||!strcmp(label,"Share")){
        unsigned char enabled=0;void *hide_args[]={&enabled};call(set_active,call(get_go,all[i],NULL),hide_args);continue;
      }
      if(!*(unsigned char *)((char *)all[i]+0x40)||*(unsigned char *)((char *)all[i]+0x55))continue;
      int duplicate=0;for(int j=0;j<count;j++)if(buttons_live[j]==all[i])duplicate=1;
      if(!duplicate){buttons_live[count]=all[i];owners[count++]=owner;}
    }
  }
  if(!count)return;if(focus_index>=count)focus_index=0;
  if(move)focus_index=(focus_index+move+count)%count;
  select_button(buttons_live[focus_index]);
  int back=pressed&((1U<<5)|(1U<<12));
  int back_found=0;
  if(back){for(int i=0;i<count;i++){char label[128];name(buttons_live[i],label,sizeof(label));
      if(strcasestr(label,"back")||strcasestr(label,"cancel")||strcasestr(label,"close")){focus_index=i;back_found=1;break;}}}
  if((pressed&(1U<<6))||(back&&back_found)){
    void *target=buttons_live[focus_index],*label=call(get_name,target,NULL);char text[128];utf8(label,text,sizeof(text));
    unfocus();void *press_args[]={label};call(method_get(object_class(owners[focus_index]),"OnPress",1),owners[focus_index],press_args);
    fprintf(stderr,"[THREES-UI] press=%s\n",text);
  }
}
