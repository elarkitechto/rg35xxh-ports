#define _GNU_SOURCE
#include "imports.h"
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
/* Same offline boundary as Alto; local IPC, including PulseAudio, stays available. */
static int local_socket(int family,int type,int protocol) {
  if(family==AF_INET||family==AF_INET6){errno=ENETUNREACH;return -1;}
  return socket(family,type,protocol);
}
static int local_dns(const char *node,const char *service,const struct addrinfo *hints,struct addrinfo **out) {
  if(!node||!strcmp(node,"localhost")||!strcmp(node,"127.0.0.1")||!strcmp(node,"::1"))return getaddrinfo(node,service,hints,out);
  if(out)*out=NULL;return EAI_NONAME;
}
void threes_offline_wire(void) {
  for(size_t i=0;i<dynlib_numfunctions;i++) {
    if(!strcmp(dynlib_functions[i].symbol,"socket"))dynlib_functions[i].func=(uintptr_t)local_socket;
    if(!strcmp(dynlib_functions[i].symbol,"getaddrinfo"))dynlib_functions[i].func=(uintptr_t)local_dns;
  }
}
