/* Per-port output calibration, applied before the soft limiter. */
#ifndef PORT_AUDIO_GAIN_H
#define PORT_AUDIO_GAIN_H
#include <math.h>
#ifndef PORT_AUDIO_GAIN_DB
#define PORT_AUDIO_GAIN_DB 9.0f
#endif
static inline float port_audio_master_gain(void) {
  return 0.30f * powf(10.0f, PORT_AUDIO_GAIN_DB / 20.0f);
}
#endif
