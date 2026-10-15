#include "opensles_shim.h"

/* Let FMOD's normal dlopen("libOpenSLES.so") path find the SDL-backed bridge. */
SLresult slCreateEngine(void **engine, SLuint32 options_count,
                        const void *options, SLuint32 interfaces_count,
                        const SLInterfaceID *interfaces,
                        const SLBoolean *required) {
  return slCreateEngine_shim(engine, options_count, options,
                             interfaces_count, interfaces, required);
}
