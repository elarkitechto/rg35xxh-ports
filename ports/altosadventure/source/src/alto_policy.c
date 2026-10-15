#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include "imports.h"
#include "alto_policy.h"
struct policy_patch {const char *name;uintptr_t offset;uint32_t expected[2];int result,is_void;};
#include "alto_policy_patches.h"
static unsigned denied_dns,denied_sockets;
static int offline_socket(int domain,int type,int protocol){
 if(domain==AF_INET || domain==AF_INET6){
  if(denied_sockets++<8)fprintf(stderr,"[ALTO-OFFLINE] deny internet socket family=%d\n",domain);
  errno=ENETUNREACH;return -1;
 }
 return socket(domain,type,protocol);
}
static int offline_dns(const char *node,const char *service,const struct addrinfo *hints,struct addrinfo **out){
 if(!node || !strcmp(node,"localhost") || !strcmp(node,"127.0.0.1") || !strcmp(node,"::1"))return getaddrinfo(node,service,hints,out);
 if(out)*out=NULL;
 if(denied_dns++<8)fprintf(stderr,"[ALTO-OFFLINE] deny DNS %s\n",node);
 return EAI_NONAME;
}
void alto_policy_wire_imports(void){
 for(size_t i=0;i<dynlib_numfunctions;i++){
  if(!strcmp(dynlib_functions[i].symbol,"socket"))dynlib_functions[i].func=(uintptr_t)offline_socket;
  if(!strcmp(dynlib_functions[i].symbol,"getaddrinfo"))dynlib_functions[i].func=(uintptr_t)offline_dns;
 }
}
void alto_policy_apply(void *base){
 /* Validate every site first; a mismatched build must never be partly patched. */
 for(size_t i=0;i<sizeof(policy_patches)/sizeof(*policy_patches);i++){
  const struct policy_patch *p=&policy_patches[i];uint32_t *fn=(void*)((char*)base+p->offset);
  if(fn[0]!=p->expected[0] || (!p->is_void && fn[1]!=p->expected[1])){fprintf(stderr,"[ALTO-POLICY] wrong build: %s\n",p->name);exit(20);}
 }
 for(size_t i=0;i<sizeof(policy_patches)/sizeof(*policy_patches);i++){
  const struct policy_patch *p=&policy_patches[i];uint32_t *fn=(void*)((char*)base+p->offset);
  if(p->is_void)fn[0]=0xd65f03c0; /* ret */
  else {int result=p->result;if(strstr(p->name,"UnityEngine.Analytics.Analytics."))result=2;
   fn[0]=0x52800000U|((unsigned)result<<5);fn[1]=0xd65f03c0;}
  __builtin___clear_cache((char*)fn,(char*)fn+8);
  fprintf(stderr,"[ALTO-POLICY] %s\n",p->name);
 }
 /* GuiGameplay.Update reactivates the touch-only pause icon each frame.
  * Its physical Start route lives in the SDL bridge and remains independent. */
 {uint32_t *site=(void*)((char*)base+0x6f7234);
  if(*site!=0x320003e1){fprintf(stderr,"[ALTO-POLICY] wrong pause HUD opcode\n");exit(25);}
  *site=0x2a1f03e1; /* mov w1,wzr: pauseButton.SetActive(false) */
  __builtin___clear_cache((char*)site,(char*)site+4);
  fprintf(stderr,"[ALTO-POLICY] touch pause HUD hidden; Start retains pause/resume\n");
 }
 /* IAP: keep singleton and purchased cache, omit Google builder. */
 {uint32_t *site=(void*)((char*)base+0x70b524);if(*site!=0xb0006e68)exit(22);*site=0x17fffff8;__builtin___clear_cache((char*)site,(char*)site+4);}
 /* Ads: keep singleton, omit mediators. */
 {uint32_t *site=(void*)((char*)base+0x846e74);if(*site!=0xd0006514)exit(22);*site=0x14000050;__builtin___clear_cache((char*)site,(char*)site+4);}
 /* The native SDL bridge supplies controller events; keep the game out of
  * Siri Remote mode and avoid its second InControl polling path. */
 uint32_t *type=(void*)((char*)base+0x773f58);
 if(*type!=0x2a0103f3){fprintf(stderr,"[ALTO-POLICY] wrong input-type instruction\n");exit(21);}
 *type=0x52800053; /* mov w19,#2 (Controller) */
 __builtin___clear_cache((char*)type,(char*)type+4);
 fprintf(stderr,"[ALTO-POLICY] SDL controller mode; analytics/ads/consent services offline\n");
}
void alto_policy_report(void){fprintf(stderr,"[ALTO-OFFLINE] DNS blocked=%u sockets blocked=%u\n",denied_dns,denied_sockets);}
