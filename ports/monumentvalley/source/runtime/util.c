/*
 * util.c -- misc utility functions
 *
 * Based on max_arm64 by Jaakko Lukkari / fgsfds / Andy Nguyen
 * Adapted for Syberia ARM64 port
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int debugPrintf(const char *text, ...) {
  static int enabled=-1;
  if(enabled<0)enabled=getenv("MONUMENT_DEBUG")!=NULL;
  if(!enabled)return 0;
  va_list list;

  va_start(list, text);
  vfprintf(stderr, text, list);
  va_end(list);

  return 0;
}

int ret0(void) { return 0; }
int ret1(void) { return 1; }
int retm1(void) { return -1; }
