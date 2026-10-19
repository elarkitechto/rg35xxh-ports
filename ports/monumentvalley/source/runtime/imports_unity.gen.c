// Import table derived from Threes!; extended against pinned Monument Valley 3.10.114.
#include "so_util.h"
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

long stub_AHardwareBuffer_acquire(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AHardwareBuffer_acquire\n"); return 0; }
long stub_AHardwareBuffer_describe(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AHardwareBuffer_describe\n"); return 0; }
long stub_AHardwareBuffer_release(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AHardwareBuffer_release\n"); return 0; }
long stub_AImageReader_acquireLatestImage(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImageReader_acquireLatestImage\n"); return 0; }
long stub_AImageReader_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImageReader_delete\n"); return 0; }
long stub_AImageReader_getWindow(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImageReader_getWindow\n"); return 0; }
long stub_AImageReader_newWithUsage(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImageReader_newWithUsage\n"); return 0; }
long stub_AImageReader_setBufferRemovedListener(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImageReader_setBufferRemovedListener\n"); return 0; }
long stub_AImageReader_setImageListener(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImageReader_setImageListener\n"); return 0; }
long stub_AImage_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImage_delete\n"); return 0; }
long stub_AImage_deleteAsync(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImage_deleteAsync\n"); return 0; }
long stub_AImage_getHardwareBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImage_getHardwareBuffer\n"); return 0; }
long stub_AImage_getTimestamp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AImage_getTimestamp\n"); return 0; }
long stub_ALooper_acquire(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ALooper_acquire\n"); return 0; }
long stub_ALooper_forThread(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ALooper_forThread\n"); return 0; }
long stub_ALooper_pollAll(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ALooper_pollAll\n"); return 0; }
long stub_ALooper_prepare(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ALooper_prepare\n"); return 0; }
long stub_ALooper_release(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ALooper_release\n"); return 0; }
long stub_ALooper_wake(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ALooper_wake\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_CHANNEL_COUNT(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_CHANNEL_COUNT\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_COLOR_FORMAT(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_COLOR_FORMAT\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_COLOR_RANGE(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_COLOR_RANGE\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_COLOR_STANDARD(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_COLOR_STANDARD\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_DURATION(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_DURATION\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_ENCODER_DELAY(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_ENCODER_DELAY\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_FRAME_RATE(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_FRAME_RATE\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_HEIGHT(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_HEIGHT\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_LANGUAGE(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_LANGUAGE\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_MIME(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_MIME\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_ROTATION(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_ROTATION\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_SAMPLE_RATE(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_SAMPLE_RATE\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_SLICE_HEIGHT(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_SLICE_HEIGHT\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_STRIDE(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_STRIDE\n"); return 0; }
long stub_AMEDIAFORMAT_KEY_WIDTH(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMEDIAFORMAT_KEY_WIDTH\n"); return 0; }
long stub_AMediaCodec_configure(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_configure\n"); return 0; }
long stub_AMediaCodec_createDecoderByType(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_createDecoderByType\n"); return 0; }
long stub_AMediaCodec_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_delete\n"); return 0; }
long stub_AMediaCodec_dequeueInputBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_dequeueInputBuffer\n"); return 0; }
long stub_AMediaCodec_dequeueOutputBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_dequeueOutputBuffer\n"); return 0; }
long stub_AMediaCodec_flush(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_flush\n"); return 0; }
long stub_AMediaCodec_getInputBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_getInputBuffer\n"); return 0; }
long stub_AMediaCodec_getOutputBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_getOutputBuffer\n"); return 0; }
long stub_AMediaCodec_getOutputFormat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_getOutputFormat\n"); return 0; }
long stub_AMediaCodec_queueInputBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_queueInputBuffer\n"); return 0; }
long stub_AMediaCodec_releaseOutputBuffer(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_releaseOutputBuffer\n"); return 0; }
long stub_AMediaCodec_setOutputSurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_setOutputSurface\n"); return 0; }
long stub_AMediaCodec_start(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_start\n"); return 0; }
long stub_AMediaCodec_stop(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaCodec_stop\n"); return 0; }
long stub_AMediaDataSource_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaDataSource_delete\n"); return 0; }
long stub_AMediaDataSource_new(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaDataSource_new\n"); return 0; }
long stub_AMediaDataSource_setClose(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaDataSource_setClose\n"); return 0; }
long stub_AMediaDataSource_setGetSize(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaDataSource_setGetSize\n"); return 0; }
long stub_AMediaDataSource_setReadAt(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaDataSource_setReadAt\n"); return 0; }
long stub_AMediaDataSource_setUserdata(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaDataSource_setUserdata\n"); return 0; }
long stub_AMediaExtractor_advance(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_advance\n"); return 0; }
long stub_AMediaExtractor_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_delete\n"); return 0; }
long stub_AMediaExtractor_getSampleTime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_getSampleTime\n"); return 0; }
long stub_AMediaExtractor_getSampleTrackIndex(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_getSampleTrackIndex\n"); return 0; }
long stub_AMediaExtractor_getTrackCount(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_getTrackCount\n"); return 0; }
long stub_AMediaExtractor_getTrackFormat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_getTrackFormat\n"); return 0; }
long stub_AMediaExtractor_new(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_new\n"); return 0; }
long stub_AMediaExtractor_readSampleData(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_readSampleData\n"); return 0; }
long stub_AMediaExtractor_seekTo(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_seekTo\n"); return 0; }
long stub_AMediaExtractor_selectTrack(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_selectTrack\n"); return 0; }
long stub_AMediaExtractor_setDataSource(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_setDataSource\n"); return 0; }
long stub_AMediaExtractor_setDataSourceCustom(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_setDataSourceCustom\n"); return 0; }
long stub_AMediaExtractor_setDataSourceFd(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaExtractor_setDataSourceFd\n"); return 0; }
long stub_AMediaFormat_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaFormat_delete\n"); return 0; }
long stub_AMediaFormat_getFloat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaFormat_getFloat\n"); return 0; }
long stub_AMediaFormat_getInt32(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaFormat_getInt32\n"); return 0; }
long stub_AMediaFormat_getInt64(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaFormat_getInt64\n"); return 0; }
long stub_AMediaFormat_getString(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaFormat_getString\n"); return 0; }
long stub_AMediaFormat_setInt32(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] AMediaFormat_setInt32\n"); return 0; }
long stub_ANativeWindow_acquire(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_acquire\n"); return 0; }
long stub_ANativeWindow_fromSurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_fromSurface\n"); return 0; }
long stub_ANativeWindow_getHeight(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_getHeight\n"); return 0; }
long stub_ANativeWindow_getWidth(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_getWidth\n"); return 0; }
long stub_ANativeWindow_release(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_release\n"); return 0; }
long stub_ANativeWindow_setBuffersGeometry(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_setBuffersGeometry\n"); return 0; }
long stub_ANativeWindow_toSurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ANativeWindow_toSurface\n"); return 0; }
long stub_ASensorEventQueue_disableSensor(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorEventQueue_disableSensor\n"); return 0; }
long stub_ASensorEventQueue_enableSensor(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorEventQueue_enableSensor\n"); return 0; }
long stub_ASensorEventQueue_getEvents(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorEventQueue_getEvents\n"); return 0; }
long stub_ASensorEventQueue_hasEvents(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorEventQueue_hasEvents\n"); return 0; }
long stub_ASensorEventQueue_setEventRate(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorEventQueue_setEventRate\n"); return 0; }
long stub_ASensorManager_createEventQueue(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorManager_createEventQueue\n"); return 0; }
long stub_ASensorManager_destroyEventQueue(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorManager_destroyEventQueue\n"); return 0; }
long stub_ASensorManager_getDefaultSensor(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorManager_getDefaultSensor\n"); return 0; }
long stub_ASensorManager_getInstance(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorManager_getInstance\n"); return 0; }
long stub_ASensorManager_getSensorList(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensorManager_getSensorList\n"); return 0; }
long stub_ASensor_getMinDelay(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensor_getMinDelay\n"); return 0; }
long stub_ASensor_getName(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensor_getName\n"); return 0; }
long stub_ASensor_getResolution(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensor_getResolution\n"); return 0; }
long stub_ASensor_getType(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensor_getType\n"); return 0; }
long stub_ASensor_getVendor(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ASensor_getVendor\n"); return 0; }
long stub__ZTH15gDeferredAction(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] _ZTH15gDeferredAction\n"); return 0; }
long stub___FD_ISSET_chk(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __FD_ISSET_chk\n"); return 0; }
long stub___FD_SET_chk(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __FD_SET_chk\n"); return 0; }
long stub___android_log_print(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __android_log_print\n"); return 0; }
long stub___android_log_vprint(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __android_log_vprint\n"); return 0; }
long stub___android_log_write(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __android_log_write\n"); return 0; }
long stub___ctype_get_mb_cur_max(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __ctype_get_mb_cur_max\n"); return 0; }
long stub___cxa_atexit(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __cxa_atexit\n"); return 0; }
long stub___cxa_finalize(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __cxa_finalize\n"); return 0; }
long stub___errno(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __errno\n"); return 0; }
long stub___memmove_chk(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __memmove_chk\n"); return 0; }
long stub___sF(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __sF\n"); return 0; }
long stub___stack_chk_fail(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __stack_chk_fail\n"); return 0; }
long stub___strlen_chk(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __strlen_chk\n"); return 0; }
long stub___system_property_find(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __system_property_find\n"); return 0; }
long stub___system_property_get(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __system_property_get\n"); return 0; }
long stub___system_property_read(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __system_property_read\n"); return 0; }
long stub___vsnprintf_chk(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] __vsnprintf_chk\n"); return 0; }
long stub__ctype_(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] _ctype_\n"); return 0; }
long stub_abort(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] abort\n"); return 0; }
long stub_accept(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] accept\n"); return 0; }
long stub_access(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] access\n"); return 0; }
long stub_acos(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] acos\n"); return 0; }
long stub_acosf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] acosf\n"); return 0; }
long stub_android_set_abort_message(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] android_set_abort_message\n"); return 0; }
long stub_asinf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] asinf\n"); return 0; }
long stub_atan(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] atan\n"); return 0; }
long stub_atan2(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] atan2\n"); return 0; }
long stub_atan2f(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] atan2f\n"); return 0; }
long stub_atanf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] atanf\n"); return 0; }
long stub_atoi(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] atoi\n"); return 0; }
long stub_atol(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] atol\n"); return 0; }
long stub_basename(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] basename\n"); return 0; }
long stub_bind(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] bind\n"); return 0; }
long stub_bsearch(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] bsearch\n"); return 0; }
long stub_btowc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] btowc\n"); return 0; }
long stub_calloc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] calloc\n"); return 0; }
long stub_cbrtf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] cbrtf\n"); return 0; }
long stub_chmod(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] chmod\n"); return 0; }
long stub_clearerr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] clearerr\n"); return 0; }
long stub_clock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] clock\n"); return 0; }
long stub_clock_getres(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] clock_getres\n"); return 0; }
long stub_clock_gettime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] clock_gettime\n"); return 0; }
long stub_close(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] close\n"); return 0; }
long stub_closedir(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] closedir\n"); return 0; }
long stub_closelog(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] closelog\n"); return 0; }
long stub_connect(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] connect\n"); return 0; }
long stub_cos(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] cos\n"); return 0; }
long stub_cosf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] cosf\n"); return 0; }
long stub_difftime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] difftime\n"); return 0; }
long stub_div(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] div\n"); return 0; }
long stub_dl_iterate_phdr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dl_iterate_phdr\n"); return 0; }
long stub_dladdr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dladdr\n"); return 0; }
long stub_dlclose(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dlclose\n"); return 0; }
long stub_dlerror(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dlerror\n"); return 0; }
long stub_dlopen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dlopen\n"); return 0; }
long stub_dlsym(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dlsym\n"); return 0; }
long stub_dup(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] dup\n"); return 0; }
long stub_eglChooseConfig(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglChooseConfig\n"); return 0; }
long stub_eglCreateContext(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglCreateContext\n"); return 0; }
long stub_eglCreatePbufferSurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglCreatePbufferSurface\n"); return 0; }
long stub_eglCreateWindowSurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglCreateWindowSurface\n"); return 0; }
long stub_eglDestroyContext(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglDestroyContext\n"); return 0; }
long stub_eglDestroySurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglDestroySurface\n"); return 0; }
long stub_eglGetConfigAttrib(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglGetConfigAttrib\n"); return 0; }
long stub_eglGetCurrentContext(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglGetCurrentContext\n"); return 0; }
long stub_eglGetCurrentSurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglGetCurrentSurface\n"); return 0; }
long stub_eglGetDisplay(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglGetDisplay\n"); return 0; }
long stub_eglGetError(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglGetError\n"); return 0; }
long stub_eglGetProcAddress(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglGetProcAddress\n"); return 0; }
long stub_eglInitialize(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglInitialize\n"); return 0; }
long stub_eglMakeCurrent(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglMakeCurrent\n"); return 0; }
long stub_eglQueryString(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglQueryString\n"); return 0; }
long stub_eglQuerySurface(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglQuerySurface\n"); return 0; }
long stub_eglSurfaceAttrib(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglSurfaceAttrib\n"); return 0; }
long stub_eglSwapBuffers(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglSwapBuffers\n"); return 0; }
long stub_eglSwapInterval(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglSwapInterval\n"); return 0; }
long stub_eglTerminate(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] eglTerminate\n"); return 0; }
long stub_exit(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] exit\n"); return 0; }
long stub_exp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] exp\n"); return 0; }
long stub_exp2f(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] exp2f\n"); return 0; }
long stub_expf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] expf\n"); return 0; }
long stub_fchmod(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fchmod\n"); return 0; }
long stub_fclose(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fclose\n"); return 0; }
long stub_fcntl(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fcntl\n"); return 0; }
long stub_fdopen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fdopen\n"); return 0; }
long stub_feof(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] feof\n"); return 0; }
long stub_ferror(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ferror\n"); return 0; }
long stub_fflush(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fflush\n"); return 0; }
long stub_fgets(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fgets\n"); return 0; }
long stub_fileno(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fileno\n"); return 0; }
long stub_flock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] flock\n"); return 0; }
long stub_fmod(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fmod\n"); return 0; }
long stub_fmodf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fmodf\n"); return 0; }
long stub_fnmatch(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fnmatch\n"); return 0; }
long stub_fopen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fopen\n"); return 0; }
long stub_fprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fprintf\n"); return 0; }
long stub_fputc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fputc\n"); return 0; }
long stub_fputs(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fputs\n"); return 0; }
long stub_fread(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fread\n"); return 0; }
long stub_free(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] free\n"); return 0; }
long stub_freeaddrinfo(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] freeaddrinfo\n"); return 0; }
long stub_freelocale(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] freelocale\n"); return 0; }
long stub_fscanf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fscanf\n"); return 0; }
long stub_fseek(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fseek\n"); return 0; }
long stub_fseeko(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fseeko\n"); return 0; }
long stub_fstat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fstat\n"); return 0; }
long stub_ftell(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ftell\n"); return 0; }
long stub_ftello(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ftello\n"); return 0; }
long stub_ftruncate(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ftruncate\n"); return 0; }
long stub_futimens(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] futimens\n"); return 0; }
long stub_fwrite(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] fwrite\n"); return 0; }
long stub_getaddrinfo(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getaddrinfo\n"); return 0; }
long stub_getcwd(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getcwd\n"); return 0; }
long stub_getegid(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getegid\n"); return 0; }
long stub_getenv(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getenv\n"); return 0; }
long stub_geteuid(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] geteuid\n"); return 0; }
long stub_gethostbyaddr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] gethostbyaddr\n"); return 0; }
long stub_gethostbyname(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] gethostbyname\n"); return 0; }
long stub_getpagesize(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getpagesize\n"); return 0; }
long stub_getpeername(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getpeername\n"); return 0; }
long stub_getpid(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getpid\n"); return 0; }
long stub_getpriority(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getpriority\n"); return 0; }
long stub_getpwuid(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getpwuid\n"); return 0; }
long stub_getpwuid_r(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getpwuid_r\n"); return 0; }
long stub_getsockname(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getsockname\n"); return 0; }
long stub_getsockopt(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getsockopt\n"); return 0; }
long stub_gettid(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] gettid\n"); return 0; }
long stub_gettimeofday(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] gettimeofday\n"); return 0; }
long stub_getuid(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] getuid\n"); return 0; }
long stub_gmtime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] gmtime\n"); return 0; }
long stub_gmtime_r(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] gmtime_r\n"); return 0; }
long stub_hypot(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] hypot\n"); return 0; }
long stub_if_nametoindex(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] if_nametoindex\n"); return 0; }
long stub_inet_addr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] inet_addr\n"); return 0; }
long stub_inet_ntop(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] inet_ntop\n"); return 0; }
long stub_inet_pton(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] inet_pton\n"); return 0; }
long stub_inflate(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] inflate\n"); return 0; }
long stub_inflateEnd(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] inflateEnd\n"); return 0; }
long stub_inflateInit2_(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] inflateInit2_\n"); return 0; }
long stub_ioctl(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ioctl\n"); return 0; }
long stub_isatty(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] isatty\n"); return 0; }
long stub_isdigit_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] isdigit_l\n"); return 0; }
long stub_islower_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] islower_l\n"); return 0; }
long stub_isupper_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] isupper_l\n"); return 0; }
long stub_iswalpha_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswalpha_l\n"); return 0; }
long stub_iswblank_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswblank_l\n"); return 0; }
long stub_iswcntrl_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswcntrl_l\n"); return 0; }
long stub_iswdigit_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswdigit_l\n"); return 0; }
long stub_iswlower_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswlower_l\n"); return 0; }
long stub_iswprint_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswprint_l\n"); return 0; }
long stub_iswpunct_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswpunct_l\n"); return 0; }
long stub_iswspace_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswspace_l\n"); return 0; }
long stub_iswupper_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswupper_l\n"); return 0; }
long stub_iswxdigit_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] iswxdigit_l\n"); return 0; }
long stub_isxdigit_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] isxdigit_l\n"); return 0; }
long stub_ldexp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ldexp\n"); return 0; }
long stub_ldexpf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ldexpf\n"); return 0; }
long stub_link(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] link\n"); return 0; }
long stub_listen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] listen\n"); return 0; }
long stub_lldiv(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] lldiv\n"); return 0; }
long stub_localeconv(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] localeconv\n"); return 0; }
long stub_localtime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] localtime\n"); return 0; }
long stub_localtime_r(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] localtime_r\n"); return 0; }
long stub_log(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] log\n"); return 0; }
long stub_log10(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] log10\n"); return 0; }
long stub_log10f(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] log10f\n"); return 0; }
long stub_log2(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] log2\n"); return 0; }
long stub_log2f(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] log2f\n"); return 0; }
long stub_logb(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] logb\n"); return 0; }
long stub_logf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] logf\n"); return 0; }
long stub_longjmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] longjmp\n"); return 0; }
long stub_lrand48(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] lrand48\n"); return 0; }
long stub_lseek(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] lseek\n"); return 0; }
long stub_lseek64(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] lseek64\n"); return 0; }
long stub_lstat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] lstat\n"); return 0; }
long stub_madvise(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] madvise\n"); return 0; }
long stub_malloc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] malloc\n"); return 0; }
long stub_mbrlen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mbrlen\n"); return 0; }
long stub_mbrtowc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mbrtowc\n"); return 0; }
long stub_mbsnrtowcs(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mbsnrtowcs\n"); return 0; }
long stub_mbsrtowcs(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mbsrtowcs\n"); return 0; }
long stub_mbtowc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mbtowc\n"); return 0; }
long stub_memalign(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memalign\n"); return 0; }
long stub_memchr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memchr\n"); return 0; }
long stub_memcmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memcmp\n"); return 0; }
long stub_memcpy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memcpy\n"); return 0; }
long stub_memmove(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memmove\n"); return 0; }
long stub_memrchr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memrchr\n"); return 0; }
long stub_memset(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] memset\n"); return 0; }
long stub_mkdir(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mkdir\n"); return 0; }
long stub_mktime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mktime\n"); return 0; }
long stub_mmap(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mmap\n"); return 0; }
long stub_modf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] modf\n"); return 0; }
long stub_mprotect(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] mprotect\n"); return 0; }
long stub_munmap(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] munmap\n"); return 0; }
long stub_nanosleep(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] nanosleep\n"); return 0; }
long stub_newlocale(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] newlocale\n"); return 0; }
long stub_open(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] open\n"); return 0; }
long stub_opendir(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] opendir\n"); return 0; }
long stub_openlog(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] openlog\n"); return 0; }
long stub_pipe(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pipe\n"); return 0; }
long stub_poll(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] poll\n"); return 0; }
long stub_posix_memalign(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] posix_memalign\n"); return 0; }
long stub_pow(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pow\n"); return 0; }
long stub_powf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] powf\n"); return 0; }
long stub_prctl(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] prctl\n"); return 0; }
long stub_printf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] printf\n"); return 0; }
long stub_pthread_atfork(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_atfork\n"); return 0; }
long stub_pthread_attr_destroy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_attr_destroy\n"); return 0; }
long stub_pthread_attr_getstack(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_attr_getstack\n"); return 0; }
long stub_pthread_attr_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_attr_init\n"); return 0; }
long stub_pthread_attr_setdetachstate(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_attr_setdetachstate\n"); return 0; }
long stub_pthread_attr_setstacksize(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_attr_setstacksize\n"); return 0; }
long stub_pthread_cond_broadcast(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_cond_broadcast\n"); return 0; }
long stub_pthread_cond_destroy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_cond_destroy\n"); return 0; }
long stub_pthread_cond_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_cond_init\n"); return 0; }
long stub_pthread_cond_signal(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_cond_signal\n"); return 0; }
long stub_pthread_cond_timedwait(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_cond_timedwait\n"); return 0; }
long stub_pthread_cond_wait(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_cond_wait\n"); return 0; }
long stub_pthread_condattr_destroy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_condattr_destroy\n"); return 0; }
long stub_pthread_condattr_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_condattr_init\n"); return 0; }
long stub_pthread_condattr_setclock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_condattr_setclock\n"); return 0; }
long stub_pthread_create(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_create\n"); return 0; }
long stub_pthread_detach(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_detach\n"); return 0; }
long stub_pthread_equal(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_equal\n"); return 0; }
long stub_pthread_exit(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_exit\n"); return 0; }
long stub_pthread_getattr_np(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_getattr_np\n"); return 0; }
long stub_pthread_getspecific(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_getspecific\n"); return 0; }
long stub_pthread_join(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_join\n"); return 0; }
long stub_pthread_key_create(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_key_create\n"); return 0; }
long stub_pthread_key_delete(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_key_delete\n"); return 0; }
long stub_pthread_kill(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_kill\n"); return 0; }
long stub_pthread_mutex_destroy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutex_destroy\n"); return 0; }
long stub_pthread_mutex_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutex_init\n"); return 0; }
long stub_pthread_mutex_lock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutex_lock\n"); return 0; }
long stub_pthread_mutex_trylock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutex_trylock\n"); return 0; }
long stub_pthread_mutex_unlock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutex_unlock\n"); return 0; }
long stub_pthread_mutexattr_destroy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutexattr_destroy\n"); return 0; }
long stub_pthread_mutexattr_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutexattr_init\n"); return 0; }
long stub_pthread_mutexattr_settype(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_mutexattr_settype\n"); return 0; }
long stub_pthread_once(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_once\n"); return 0; }
long stub_pthread_rwlock_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_rwlock_init\n"); return 0; }
long stub_pthread_rwlock_rdlock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_rwlock_rdlock\n"); return 0; }
long stub_pthread_rwlock_unlock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_rwlock_unlock\n"); return 0; }
long stub_pthread_rwlock_wrlock(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_rwlock_wrlock\n"); return 0; }
long stub_pthread_self(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_self\n"); return 0; }
long stub_pthread_setname_np(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_setname_np\n"); return 0; }
long stub_pthread_setspecific(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_setspecific\n"); return 0; }
long stub_pthread_sigmask(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] pthread_sigmask\n"); return 0; }
long stub_ptrace(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] ptrace\n"); return 0; }
long stub_puts(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] puts\n"); return 0; }
long stub_qsort(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] qsort\n"); return 0; }
long stub_raise(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] raise\n"); return 0; }
long stub_read(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] read\n"); return 0; }
long stub_readdir(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] readdir\n"); return 0; }
long stub_readlink(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] readlink\n"); return 0; }
long stub_realloc(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] realloc\n"); return 0; }
long stub_realpath(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] realpath\n"); return 0; }
long stub_recv(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] recv\n"); return 0; }
long stub_remove(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] remove\n"); return 0; }
long stub_rename(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] rename\n"); return 0; }
long stub_rmdir(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] rmdir\n"); return 0; }
long stub_scalbn(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] scalbn\n"); return 0; }
long stub_sched_getaffinity(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sched_getaffinity\n"); return 0; }
long stub_sched_setaffinity(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sched_setaffinity\n"); return 0; }
long stub_sched_yield(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sched_yield\n"); return 0; }
long stub_select(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] select\n"); return 0; }
long stub_sem_destroy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sem_destroy\n"); return 0; }
long stub_sem_getvalue(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sem_getvalue\n"); return 0; }
long stub_sem_init(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sem_init\n"); return 0; }
long stub_sem_post(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sem_post\n"); return 0; }
long stub_sem_timedwait(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sem_timedwait\n"); return 0; }
long stub_sem_wait(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sem_wait\n"); return 0; }
long stub_send(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] send\n"); return 0; }
long stub_sendfile(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sendfile\n"); return 0; }
long stub_setenv(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] setenv\n"); return 0; }
long stub_setjmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] setjmp\n"); return 0; }
long stub_setlocale(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] setlocale\n"); return 0; }
long stub_setpriority(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] setpriority\n"); return 0; }
long stub_setsockopt(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] setsockopt\n"); return 0; }
long stub_setvbuf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] setvbuf\n"); return 0; }
long stub_shutdown(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] shutdown\n"); return 0; }
long stub_sigaction(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigaction\n"); return 0; }
long stub_sigaddset(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigaddset\n"); return 0; }
long stub_sigaltstack(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigaltstack\n"); return 0; }
long stub_sigdelset(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigdelset\n"); return 0; }
long stub_sigemptyset(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigemptyset\n"); return 0; }
long stub_sigfillset(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigfillset\n"); return 0; }
long stub_signal(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] signal\n"); return 0; }
long stub_sigsuspend(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sigsuspend\n"); return 0; }
long stub_sin(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sin\n"); return 0; }
long stub_sincosf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sincosf\n"); return 0; }
long stub_sinf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sinf\n"); return 0; }
long stub_snprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] snprintf\n"); return 0; }
long stub_socket(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] socket\n"); return 0; }
long stub_sprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sprintf\n"); return 0; }
long stub_sqrtf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sqrtf\n"); return 0; }
long stub_srand48(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] srand48\n"); return 0; }
long stub_sscanf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] sscanf\n"); return 0; }
long stub_stat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] stat\n"); return 0; }
long stub_statfs(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] statfs\n"); return 0; }
long stub_strcasecmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcasecmp\n"); return 0; }
long stub_strcasestr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcasestr\n"); return 0; }
long stub_strcat(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcat\n"); return 0; }
long stub_strchr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strchr\n"); return 0; }
long stub_strcmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcmp\n"); return 0; }
long stub_strcoll_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcoll_l\n"); return 0; }
long stub_strcpy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcpy\n"); return 0; }
long stub_strcspn(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strcspn\n"); return 0; }
long stub_strdup(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strdup\n"); return 0; }
long stub_strerror(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strerror\n"); return 0; }
long stub_strerror_r(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strerror_r\n"); return 0; }
long stub_strftime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strftime\n"); return 0; }
long stub_strftime_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strftime_l\n"); return 0; }
long stub_strlcpy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strlcpy\n"); return 0; }
long stub_strlen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strlen\n"); return 0; }
long stub_strncmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strncmp\n"); return 0; }
long stub_strncpy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strncpy\n"); return 0; }
long stub_strnlen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strnlen\n"); return 0; }
long stub_strrchr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strrchr\n"); return 0; }
long stub_strspn(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strspn\n"); return 0; }
long stub_strstr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strstr\n"); return 0; }
long stub_strtod(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtod\n"); return 0; }
long stub_strtof(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtof\n"); return 0; }
long stub_strtok_r(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtok_r\n"); return 0; }
long stub_strtol(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtol\n"); return 0; }
long stub_strtold(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtold\n"); return 0; }
long stub_strtold_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtold_l\n"); return 0; }
long stub_strtoll(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtoll\n"); return 0; }
long stub_strtoll_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtoll_l\n"); return 0; }
long stub_strtoul(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtoul\n"); return 0; }
long stub_strtoull(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtoull\n"); return 0; }
long stub_strtoull_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strtoull_l\n"); return 0; }
long stub_strxfrm_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] strxfrm_l\n"); return 0; }
long stub_swprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] swprintf\n"); return 0; }
long stub_symlink(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] symlink\n"); return 0; }
long stub_syscall(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] syscall\n"); return 0; }
/* Bionic query IDs differ from glibc; AOSP bits/sysconf.h. */
long stub_sysconf(int query) {
 int host;
 switch(query) {
 case 0x00: host=_SC_ARG_MAX; break;
 case 0x05: host=_SC_CHILD_MAX; break;
 case 0x06: host=_SC_CLK_TCK; break;
 case 0x0b: host=_SC_OPEN_MAX; break;
 case 0x27: case 0x28: host=_SC_PAGESIZE; break;
 case 0x4c: host=_SC_THREAD_STACK_MIN; break;
 case 0x60: host=_SC_NPROCESSORS_CONF; break;
 case 0x61: host=_SC_NPROCESSORS_ONLN; break;
 case 0x62: host=_SC_PHYS_PAGES; break;
 case 0x63: host=_SC_AVPHYS_PAGES; break;
 default: fprintf(stderr,"[MONUMENT-SYSCONF] unmapped bionic query=%x\n",query); errno=EINVAL; return -1;
 }
 long result=sysconf(host);
 fprintf(stderr,"[MONUMENT-SYSCONF] bionic=%x host=%d value=%ld\n",query,host,result);
 return result;
}
long stub_syslog(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] syslog\n"); return 0; }
long stub_tan(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] tan\n"); return 0; }
long stub_tanf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] tanf\n"); return 0; }
long stub_time(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] time\n"); return 0; }
long stub_tolower_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] tolower_l\n"); return 0; }
long stub_toupper_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] toupper_l\n"); return 0; }
long stub_towlower(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] towlower\n"); return 0; }
long stub_towlower_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] towlower_l\n"); return 0; }
long stub_towupper_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] towupper_l\n"); return 0; }
long stub_truncate(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] truncate\n"); return 0; }
long stub_uname(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] uname\n"); return 0; }
long stub_unlink(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] unlink\n"); return 0; }
long stub_unsetenv(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] unsetenv\n"); return 0; }
long stub_uselocale(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] uselocale\n"); return 0; }
long stub_usleep(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] usleep\n"); return 0; }
long stub_utime(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] utime\n"); return 0; }
long stub_utimes(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] utimes\n"); return 0; }
long stub_vasprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] vasprintf\n"); return 0; }
long stub_vfprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] vfprintf\n"); return 0; }
long stub_vprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] vprintf\n"); return 0; }
long stub_vsnprintf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] vsnprintf\n"); return 0; }
long stub_vsscanf(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] vsscanf\n"); return 0; }
long stub_wcrtomb(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcrtomb\n"); return 0; }
long stub_wcscoll_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcscoll_l\n"); return 0; }
long stub_wcslen(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcslen\n"); return 0; }
long stub_wcsnrtombs(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcsnrtombs\n"); return 0; }
long stub_wcstod(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstod\n"); return 0; }
long stub_wcstof(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstof\n"); return 0; }
long stub_wcstol(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstol\n"); return 0; }
long stub_wcstold(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstold\n"); return 0; }
long stub_wcstoll(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstoll\n"); return 0; }
long stub_wcstoul(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstoul\n"); return 0; }
long stub_wcstoull(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcstoull\n"); return 0; }
long stub_wcsxfrm_l(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wcsxfrm_l\n"); return 0; }
long stub_wctob(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wctob\n"); return 0; }
long stub_wmemchr(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wmemchr\n"); return 0; }
long stub_wmemcmp(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wmemcmp\n"); return 0; }
long stub_wmemcpy(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wmemcpy\n"); return 0; }
long stub_wmemmove(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wmemmove\n"); return 0; }
long stub_wmemset(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] wmemset\n"); return 0; }
long stub_write(void) { static unsigned n; if (n++ < 4) fprintf(stderr, "[MONUMENT-STUB] write\n"); return 0; }


/* Additional imports measured in pinned Monument Valley 3.10.114. */
static long extra_AImage_getWidth(void) { fprintf(stderr,"[MONUMENT-IMPORT] unsupported AImage_getWidth\n"); return -1; }
static long extra_ALooper_pollOnce(void) { fprintf(stderr,"[MONUMENT-IMPORT] unsupported ALooper_pollOnce\n"); return -1; }
static long extra_asin(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: asin\n"); __builtin_trap(); }
static long extra_dup2(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: dup2\n"); __builtin_trap(); }
static long extra_environ(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: environ\n"); __builtin_trap(); }
static long extra_fstat64(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: fstat64\n"); __builtin_trap(); }
static long extra_getauxval(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: getauxval\n"); __builtin_trap(); }
static long extra_gethostname(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: gethostname\n"); __builtin_trap(); }
static long extra_getnameinfo(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: getnameinfo\n"); __builtin_trap(); }
static long extra_mmap64(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: mmap64\n"); __builtin_trap(); }
static long extra_modff(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: modff\n"); __builtin_trap(); }
static long extra_recvfrom(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: recvfrom\n"); __builtin_trap(); }
static long extra_recvmsg(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: recvmsg\n"); __builtin_trap(); }
static long extra_sendmsg(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: sendmsg\n"); __builtin_trap(); }
static long extra_setbuf(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: setbuf\n"); __builtin_trap(); }
static long extra_stat64(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: stat64\n"); __builtin_trap(); }
static long extra_statfs64(void) { fprintf(stderr,"[MONUMENT-IMPORT] host symbol unavailable: statfs64\n"); __builtin_trap(); }
DynLibFunction dynlib_functions[] = {
  {"AHardwareBuffer_acquire", (uintptr_t)&stub_AHardwareBuffer_acquire},
  {"AHardwareBuffer_describe", (uintptr_t)&stub_AHardwareBuffer_describe},
  {"AHardwareBuffer_release", (uintptr_t)&stub_AHardwareBuffer_release},
  {"AImageReader_acquireLatestImage", (uintptr_t)&stub_AImageReader_acquireLatestImage},
  {"AImageReader_delete", (uintptr_t)&stub_AImageReader_delete},
  {"AImageReader_getWindow", (uintptr_t)&stub_AImageReader_getWindow},
  {"AImageReader_newWithUsage", (uintptr_t)&stub_AImageReader_newWithUsage},
  {"AImageReader_setBufferRemovedListener", (uintptr_t)&stub_AImageReader_setBufferRemovedListener},
  {"AImageReader_setImageListener", (uintptr_t)&stub_AImageReader_setImageListener},
  {"AImage_delete", (uintptr_t)&stub_AImage_delete},
  {"AImage_deleteAsync", (uintptr_t)&stub_AImage_deleteAsync},
  {"AImage_getHardwareBuffer", (uintptr_t)&stub_AImage_getHardwareBuffer},
  {"AImage_getTimestamp", (uintptr_t)&stub_AImage_getTimestamp},
  {"ALooper_acquire", (uintptr_t)&stub_ALooper_acquire},
  {"ALooper_forThread", (uintptr_t)&stub_ALooper_forThread},
  {"ALooper_pollAll", (uintptr_t)&stub_ALooper_pollAll},
  {"ALooper_prepare", (uintptr_t)&stub_ALooper_prepare},
  {"ALooper_release", (uintptr_t)&stub_ALooper_release},
  {"ALooper_wake", (uintptr_t)&stub_ALooper_wake},
  {"AMEDIAFORMAT_KEY_CHANNEL_COUNT", (uintptr_t)&stub_AMEDIAFORMAT_KEY_CHANNEL_COUNT},
  {"AMEDIAFORMAT_KEY_COLOR_FORMAT", (uintptr_t)&stub_AMEDIAFORMAT_KEY_COLOR_FORMAT},
  {"AMEDIAFORMAT_KEY_COLOR_RANGE", (uintptr_t)&stub_AMEDIAFORMAT_KEY_COLOR_RANGE},
  {"AMEDIAFORMAT_KEY_COLOR_STANDARD", (uintptr_t)&stub_AMEDIAFORMAT_KEY_COLOR_STANDARD},
  {"AMEDIAFORMAT_KEY_DURATION", (uintptr_t)&stub_AMEDIAFORMAT_KEY_DURATION},
  {"AMEDIAFORMAT_KEY_ENCODER_DELAY", (uintptr_t)&stub_AMEDIAFORMAT_KEY_ENCODER_DELAY},
  {"AMEDIAFORMAT_KEY_FRAME_RATE", (uintptr_t)&stub_AMEDIAFORMAT_KEY_FRAME_RATE},
  {"AMEDIAFORMAT_KEY_HEIGHT", (uintptr_t)&stub_AMEDIAFORMAT_KEY_HEIGHT},
  {"AMEDIAFORMAT_KEY_LANGUAGE", (uintptr_t)&stub_AMEDIAFORMAT_KEY_LANGUAGE},
  {"AMEDIAFORMAT_KEY_MIME", (uintptr_t)&stub_AMEDIAFORMAT_KEY_MIME},
  {"AMEDIAFORMAT_KEY_ROTATION", (uintptr_t)&stub_AMEDIAFORMAT_KEY_ROTATION},
  {"AMEDIAFORMAT_KEY_SAMPLE_RATE", (uintptr_t)&stub_AMEDIAFORMAT_KEY_SAMPLE_RATE},
  {"AMEDIAFORMAT_KEY_SLICE_HEIGHT", (uintptr_t)&stub_AMEDIAFORMAT_KEY_SLICE_HEIGHT},
  {"AMEDIAFORMAT_KEY_STRIDE", (uintptr_t)&stub_AMEDIAFORMAT_KEY_STRIDE},
  {"AMEDIAFORMAT_KEY_WIDTH", (uintptr_t)&stub_AMEDIAFORMAT_KEY_WIDTH},
  {"AMediaCodec_configure", (uintptr_t)&stub_AMediaCodec_configure},
  {"AMediaCodec_createDecoderByType", (uintptr_t)&stub_AMediaCodec_createDecoderByType},
  {"AMediaCodec_delete", (uintptr_t)&stub_AMediaCodec_delete},
  {"AMediaCodec_dequeueInputBuffer", (uintptr_t)&stub_AMediaCodec_dequeueInputBuffer},
  {"AMediaCodec_dequeueOutputBuffer", (uintptr_t)&stub_AMediaCodec_dequeueOutputBuffer},
  {"AMediaCodec_flush", (uintptr_t)&stub_AMediaCodec_flush},
  {"AMediaCodec_getInputBuffer", (uintptr_t)&stub_AMediaCodec_getInputBuffer},
  {"AMediaCodec_getOutputBuffer", (uintptr_t)&stub_AMediaCodec_getOutputBuffer},
  {"AMediaCodec_getOutputFormat", (uintptr_t)&stub_AMediaCodec_getOutputFormat},
  {"AMediaCodec_queueInputBuffer", (uintptr_t)&stub_AMediaCodec_queueInputBuffer},
  {"AMediaCodec_releaseOutputBuffer", (uintptr_t)&stub_AMediaCodec_releaseOutputBuffer},
  {"AMediaCodec_setOutputSurface", (uintptr_t)&stub_AMediaCodec_setOutputSurface},
  {"AMediaCodec_start", (uintptr_t)&stub_AMediaCodec_start},
  {"AMediaCodec_stop", (uintptr_t)&stub_AMediaCodec_stop},
  {"AMediaDataSource_delete", (uintptr_t)&stub_AMediaDataSource_delete},
  {"AMediaDataSource_new", (uintptr_t)&stub_AMediaDataSource_new},
  {"AMediaDataSource_setClose", (uintptr_t)&stub_AMediaDataSource_setClose},
  {"AMediaDataSource_setGetSize", (uintptr_t)&stub_AMediaDataSource_setGetSize},
  {"AMediaDataSource_setReadAt", (uintptr_t)&stub_AMediaDataSource_setReadAt},
  {"AMediaDataSource_setUserdata", (uintptr_t)&stub_AMediaDataSource_setUserdata},
  {"AMediaExtractor_advance", (uintptr_t)&stub_AMediaExtractor_advance},
  {"AMediaExtractor_delete", (uintptr_t)&stub_AMediaExtractor_delete},
  {"AMediaExtractor_getSampleTime", (uintptr_t)&stub_AMediaExtractor_getSampleTime},
  {"AMediaExtractor_getSampleTrackIndex", (uintptr_t)&stub_AMediaExtractor_getSampleTrackIndex},
  {"AMediaExtractor_getTrackCount", (uintptr_t)&stub_AMediaExtractor_getTrackCount},
  {"AMediaExtractor_getTrackFormat", (uintptr_t)&stub_AMediaExtractor_getTrackFormat},
  {"AMediaExtractor_new", (uintptr_t)&stub_AMediaExtractor_new},
  {"AMediaExtractor_readSampleData", (uintptr_t)&stub_AMediaExtractor_readSampleData},
  {"AMediaExtractor_seekTo", (uintptr_t)&stub_AMediaExtractor_seekTo},
  {"AMediaExtractor_selectTrack", (uintptr_t)&stub_AMediaExtractor_selectTrack},
  {"AMediaExtractor_setDataSource", (uintptr_t)&stub_AMediaExtractor_setDataSource},
  {"AMediaExtractor_setDataSourceCustom", (uintptr_t)&stub_AMediaExtractor_setDataSourceCustom},
  {"AMediaExtractor_setDataSourceFd", (uintptr_t)&stub_AMediaExtractor_setDataSourceFd},
  {"AMediaFormat_delete", (uintptr_t)&stub_AMediaFormat_delete},
  {"AMediaFormat_getFloat", (uintptr_t)&stub_AMediaFormat_getFloat},
  {"AMediaFormat_getInt32", (uintptr_t)&stub_AMediaFormat_getInt32},
  {"AMediaFormat_getInt64", (uintptr_t)&stub_AMediaFormat_getInt64},
  {"AMediaFormat_getString", (uintptr_t)&stub_AMediaFormat_getString},
  {"AMediaFormat_setInt32", (uintptr_t)&stub_AMediaFormat_setInt32},
  {"ANativeWindow_acquire", (uintptr_t)&stub_ANativeWindow_acquire},
  {"ANativeWindow_fromSurface", (uintptr_t)&stub_ANativeWindow_fromSurface},
  {"ANativeWindow_getHeight", (uintptr_t)&stub_ANativeWindow_getHeight},
  {"ANativeWindow_getWidth", (uintptr_t)&stub_ANativeWindow_getWidth},
  {"ANativeWindow_release", (uintptr_t)&stub_ANativeWindow_release},
  {"ANativeWindow_setBuffersGeometry", (uintptr_t)&stub_ANativeWindow_setBuffersGeometry},
  {"ANativeWindow_toSurface", (uintptr_t)&stub_ANativeWindow_toSurface},
  {"ASensorEventQueue_disableSensor", (uintptr_t)&stub_ASensorEventQueue_disableSensor},
  {"ASensorEventQueue_enableSensor", (uintptr_t)&stub_ASensorEventQueue_enableSensor},
  {"ASensorEventQueue_getEvents", (uintptr_t)&stub_ASensorEventQueue_getEvents},
  {"ASensorEventQueue_hasEvents", (uintptr_t)&stub_ASensorEventQueue_hasEvents},
  {"ASensorEventQueue_setEventRate", (uintptr_t)&stub_ASensorEventQueue_setEventRate},
  {"ASensorManager_createEventQueue", (uintptr_t)&stub_ASensorManager_createEventQueue},
  {"ASensorManager_destroyEventQueue", (uintptr_t)&stub_ASensorManager_destroyEventQueue},
  {"ASensorManager_getDefaultSensor", (uintptr_t)&stub_ASensorManager_getDefaultSensor},
  {"ASensorManager_getInstance", (uintptr_t)&stub_ASensorManager_getInstance},
  {"ASensorManager_getSensorList", (uintptr_t)&stub_ASensorManager_getSensorList},
  {"ASensor_getMinDelay", (uintptr_t)&stub_ASensor_getMinDelay},
  {"ASensor_getName", (uintptr_t)&stub_ASensor_getName},
  {"ASensor_getResolution", (uintptr_t)&stub_ASensor_getResolution},
  {"ASensor_getType", (uintptr_t)&stub_ASensor_getType},
  {"ASensor_getVendor", (uintptr_t)&stub_ASensor_getVendor},
  {"_ZTH15gDeferredAction", (uintptr_t)&stub__ZTH15gDeferredAction},
  {"__FD_ISSET_chk", (uintptr_t)&stub___FD_ISSET_chk},
  {"__FD_SET_chk", (uintptr_t)&stub___FD_SET_chk},
  {"__android_log_print", (uintptr_t)&stub___android_log_print},
  {"__android_log_vprint", (uintptr_t)&stub___android_log_vprint},
  {"__android_log_write", (uintptr_t)&stub___android_log_write},
  {"__ctype_get_mb_cur_max", (uintptr_t)&stub___ctype_get_mb_cur_max},
  {"__cxa_atexit", (uintptr_t)&stub___cxa_atexit},
  {"__cxa_finalize", (uintptr_t)&stub___cxa_finalize},
  {"__errno", (uintptr_t)&stub___errno},
  {"__memmove_chk", (uintptr_t)&stub___memmove_chk},
  {"__sF", (uintptr_t)&stub___sF},
  {"__stack_chk_fail", (uintptr_t)&stub___stack_chk_fail},
  {"__strlen_chk", (uintptr_t)&stub___strlen_chk},
  {"__system_property_find", (uintptr_t)&stub___system_property_find},
  {"__system_property_get", (uintptr_t)&stub___system_property_get},
  {"__system_property_read", (uintptr_t)&stub___system_property_read},
  {"__vsnprintf_chk", (uintptr_t)&stub___vsnprintf_chk},
  {"_ctype_", (uintptr_t)&stub__ctype_},
  {"abort", (uintptr_t)&stub_abort},
  {"accept", (uintptr_t)&stub_accept},
  {"access", (uintptr_t)&stub_access},
  {"acos", (uintptr_t)&stub_acos},
  {"acosf", (uintptr_t)&stub_acosf},
  {"android_set_abort_message", (uintptr_t)&stub_android_set_abort_message},
  {"asinf", (uintptr_t)&stub_asinf},
  {"atan", (uintptr_t)&stub_atan},
  {"atan2", (uintptr_t)&stub_atan2},
  {"atan2f", (uintptr_t)&stub_atan2f},
  {"atanf", (uintptr_t)&stub_atanf},
  {"atoi", (uintptr_t)&stub_atoi},
  {"atol", (uintptr_t)&stub_atol},
  {"basename", (uintptr_t)&stub_basename},
  {"bind", (uintptr_t)&stub_bind},
  {"bsearch", (uintptr_t)&stub_bsearch},
  {"btowc", (uintptr_t)&stub_btowc},
  {"calloc", (uintptr_t)&stub_calloc},
  {"cbrtf", (uintptr_t)&stub_cbrtf},
  {"chmod", (uintptr_t)&stub_chmod},
  {"clearerr", (uintptr_t)&stub_clearerr},
  {"clock", (uintptr_t)&stub_clock},
  {"clock_getres", (uintptr_t)&stub_clock_getres},
  {"clock_gettime", (uintptr_t)&stub_clock_gettime},
  {"close", (uintptr_t)&stub_close},
  {"closedir", (uintptr_t)&stub_closedir},
  {"closelog", (uintptr_t)&stub_closelog},
  {"connect", (uintptr_t)&stub_connect},
  {"cos", (uintptr_t)&stub_cos},
  {"cosf", (uintptr_t)&stub_cosf},
  {"difftime", (uintptr_t)&stub_difftime},
  {"div", (uintptr_t)&stub_div},
  {"dl_iterate_phdr", (uintptr_t)&stub_dl_iterate_phdr},
  {"dladdr", (uintptr_t)&stub_dladdr},
  {"dlclose", (uintptr_t)&stub_dlclose},
  {"dlerror", (uintptr_t)&stub_dlerror},
  {"dlopen", (uintptr_t)&stub_dlopen},
  {"dlsym", (uintptr_t)&stub_dlsym},
  {"dup", (uintptr_t)&stub_dup},
  {"eglChooseConfig", (uintptr_t)&stub_eglChooseConfig},
  {"eglCreateContext", (uintptr_t)&stub_eglCreateContext},
  {"eglCreatePbufferSurface", (uintptr_t)&stub_eglCreatePbufferSurface},
  {"eglCreateWindowSurface", (uintptr_t)&stub_eglCreateWindowSurface},
  {"eglDestroyContext", (uintptr_t)&stub_eglDestroyContext},
  {"eglDestroySurface", (uintptr_t)&stub_eglDestroySurface},
  {"eglGetConfigAttrib", (uintptr_t)&stub_eglGetConfigAttrib},
  {"eglGetCurrentContext", (uintptr_t)&stub_eglGetCurrentContext},
  {"eglGetCurrentSurface", (uintptr_t)&stub_eglGetCurrentSurface},
  {"eglGetDisplay", (uintptr_t)&stub_eglGetDisplay},
  {"eglGetError", (uintptr_t)&stub_eglGetError},
  {"eglGetProcAddress", (uintptr_t)&stub_eglGetProcAddress},
  {"eglInitialize", (uintptr_t)&stub_eglInitialize},
  {"eglMakeCurrent", (uintptr_t)&stub_eglMakeCurrent},
  {"eglQueryString", (uintptr_t)&stub_eglQueryString},
  {"eglQuerySurface", (uintptr_t)&stub_eglQuerySurface},
  {"eglSurfaceAttrib", (uintptr_t)&stub_eglSurfaceAttrib},
  {"eglSwapBuffers", (uintptr_t)&stub_eglSwapBuffers},
  {"eglSwapInterval", (uintptr_t)&stub_eglSwapInterval},
  {"eglTerminate", (uintptr_t)&stub_eglTerminate},
  {"exit", (uintptr_t)&stub_exit},
  {"exp", (uintptr_t)&stub_exp},
  {"exp2f", (uintptr_t)&stub_exp2f},
  {"expf", (uintptr_t)&stub_expf},
  {"fchmod", (uintptr_t)&stub_fchmod},
  {"fclose", (uintptr_t)&stub_fclose},
  {"fcntl", (uintptr_t)&stub_fcntl},
  {"fdopen", (uintptr_t)&stub_fdopen},
  {"feof", (uintptr_t)&stub_feof},
  {"ferror", (uintptr_t)&stub_ferror},
  {"fflush", (uintptr_t)&stub_fflush},
  {"fgets", (uintptr_t)&stub_fgets},
  {"fileno", (uintptr_t)&stub_fileno},
  {"flock", (uintptr_t)&stub_flock},
  {"fmod", (uintptr_t)&stub_fmod},
  {"fmodf", (uintptr_t)&stub_fmodf},
  {"fnmatch", (uintptr_t)&stub_fnmatch},
  {"fopen", (uintptr_t)&stub_fopen},
  {"fprintf", (uintptr_t)&stub_fprintf},
  {"fputc", (uintptr_t)&stub_fputc},
  {"fputs", (uintptr_t)&stub_fputs},
  {"fread", (uintptr_t)&stub_fread},
  {"free", (uintptr_t)&stub_free},
  {"freeaddrinfo", (uintptr_t)&stub_freeaddrinfo},
  {"freelocale", (uintptr_t)&stub_freelocale},
  {"fscanf", (uintptr_t)&stub_fscanf},
  {"fseek", (uintptr_t)&stub_fseek},
  {"fseeko", (uintptr_t)&stub_fseeko},
  {"fstat", (uintptr_t)&stub_fstat},
  {"ftell", (uintptr_t)&stub_ftell},
  {"ftello", (uintptr_t)&stub_ftello},
  {"ftruncate", (uintptr_t)&stub_ftruncate},
  {"futimens", (uintptr_t)&stub_futimens},
  {"fwrite", (uintptr_t)&stub_fwrite},
  {"getaddrinfo", (uintptr_t)&stub_getaddrinfo},
  {"getcwd", (uintptr_t)&stub_getcwd},
  {"getegid", (uintptr_t)&stub_getegid},
  {"getenv", (uintptr_t)&stub_getenv},
  {"geteuid", (uintptr_t)&stub_geteuid},
  {"gethostbyaddr", (uintptr_t)&stub_gethostbyaddr},
  {"gethostbyname", (uintptr_t)&stub_gethostbyname},
  {"getpagesize", (uintptr_t)&stub_getpagesize},
  {"getpeername", (uintptr_t)&stub_getpeername},
  {"getpid", (uintptr_t)&stub_getpid},
  {"getpriority", (uintptr_t)&stub_getpriority},
  {"getpwuid", (uintptr_t)&stub_getpwuid},
  {"getpwuid_r", (uintptr_t)&stub_getpwuid_r},
  {"getsockname", (uintptr_t)&stub_getsockname},
  {"getsockopt", (uintptr_t)&stub_getsockopt},
  {"gettid", (uintptr_t)&stub_gettid},
  {"gettimeofday", (uintptr_t)&stub_gettimeofday},
  {"getuid", (uintptr_t)&stub_getuid},
  {"gmtime", (uintptr_t)&stub_gmtime},
  {"gmtime_r", (uintptr_t)&stub_gmtime_r},
  {"hypot", (uintptr_t)&stub_hypot},
  {"if_nametoindex", (uintptr_t)&stub_if_nametoindex},
  {"inet_addr", (uintptr_t)&stub_inet_addr},
  {"inet_ntop", (uintptr_t)&stub_inet_ntop},
  {"inet_pton", (uintptr_t)&stub_inet_pton},
  {"inflate", (uintptr_t)&stub_inflate},
  {"inflateEnd", (uintptr_t)&stub_inflateEnd},
  {"inflateInit2_", (uintptr_t)&stub_inflateInit2_},
  {"ioctl", (uintptr_t)&stub_ioctl},
  {"isatty", (uintptr_t)&stub_isatty},
  {"isdigit_l", (uintptr_t)&stub_isdigit_l},
  {"islower_l", (uintptr_t)&stub_islower_l},
  {"isupper_l", (uintptr_t)&stub_isupper_l},
  {"iswalpha_l", (uintptr_t)&stub_iswalpha_l},
  {"iswblank_l", (uintptr_t)&stub_iswblank_l},
  {"iswcntrl_l", (uintptr_t)&stub_iswcntrl_l},
  {"iswdigit_l", (uintptr_t)&stub_iswdigit_l},
  {"iswlower_l", (uintptr_t)&stub_iswlower_l},
  {"iswprint_l", (uintptr_t)&stub_iswprint_l},
  {"iswpunct_l", (uintptr_t)&stub_iswpunct_l},
  {"iswspace_l", (uintptr_t)&stub_iswspace_l},
  {"iswupper_l", (uintptr_t)&stub_iswupper_l},
  {"iswxdigit_l", (uintptr_t)&stub_iswxdigit_l},
  {"isxdigit_l", (uintptr_t)&stub_isxdigit_l},
  {"ldexp", (uintptr_t)&stub_ldexp},
  {"ldexpf", (uintptr_t)&stub_ldexpf},
  {"link", (uintptr_t)&stub_link},
  {"listen", (uintptr_t)&stub_listen},
  {"lldiv", (uintptr_t)&stub_lldiv},
  {"localeconv", (uintptr_t)&stub_localeconv},
  {"localtime", (uintptr_t)&stub_localtime},
  {"localtime_r", (uintptr_t)&stub_localtime_r},
  {"log", (uintptr_t)&stub_log},
  {"log10", (uintptr_t)&stub_log10},
  {"log10f", (uintptr_t)&stub_log10f},
  {"log2", (uintptr_t)&stub_log2},
  {"log2f", (uintptr_t)&stub_log2f},
  {"logb", (uintptr_t)&stub_logb},
  {"logf", (uintptr_t)&stub_logf},
  {"longjmp", (uintptr_t)&stub_longjmp},
  {"lrand48", (uintptr_t)&stub_lrand48},
  {"lseek", (uintptr_t)&stub_lseek},
  {"lseek64", (uintptr_t)&stub_lseek64},
  {"lstat", (uintptr_t)&stub_lstat},
  {"madvise", (uintptr_t)&stub_madvise},
  {"malloc", (uintptr_t)&stub_malloc},
  {"mbrlen", (uintptr_t)&stub_mbrlen},
  {"mbrtowc", (uintptr_t)&stub_mbrtowc},
  {"mbsnrtowcs", (uintptr_t)&stub_mbsnrtowcs},
  {"mbsrtowcs", (uintptr_t)&stub_mbsrtowcs},
  {"mbtowc", (uintptr_t)&stub_mbtowc},
  {"memalign", (uintptr_t)&stub_memalign},
  {"memchr", (uintptr_t)&stub_memchr},
  {"memcmp", (uintptr_t)&stub_memcmp},
  {"memcpy", (uintptr_t)&stub_memcpy},
  {"memmove", (uintptr_t)&stub_memmove},
  {"memrchr", (uintptr_t)&stub_memrchr},
  {"memset", (uintptr_t)&stub_memset},
  {"mkdir", (uintptr_t)&stub_mkdir},
  {"mktime", (uintptr_t)&stub_mktime},
  {"mmap", (uintptr_t)&stub_mmap},
  {"modf", (uintptr_t)&stub_modf},
  {"mprotect", (uintptr_t)&stub_mprotect},
  {"munmap", (uintptr_t)&stub_munmap},
  {"nanosleep", (uintptr_t)&stub_nanosleep},
  {"newlocale", (uintptr_t)&stub_newlocale},
  {"open", (uintptr_t)&stub_open},
  {"opendir", (uintptr_t)&stub_opendir},
  {"openlog", (uintptr_t)&stub_openlog},
  {"pipe", (uintptr_t)&stub_pipe},
  {"poll", (uintptr_t)&stub_poll},
  {"posix_memalign", (uintptr_t)&stub_posix_memalign},
  {"pow", (uintptr_t)&stub_pow},
  {"powf", (uintptr_t)&stub_powf},
  {"prctl", (uintptr_t)&stub_prctl},
  {"printf", (uintptr_t)&stub_printf},
  {"pthread_atfork", (uintptr_t)&stub_pthread_atfork},
  {"pthread_attr_destroy", (uintptr_t)&stub_pthread_attr_destroy},
  {"pthread_attr_getstack", (uintptr_t)&stub_pthread_attr_getstack},
  {"pthread_attr_init", (uintptr_t)&stub_pthread_attr_init},
  {"pthread_attr_setdetachstate", (uintptr_t)&stub_pthread_attr_setdetachstate},
  {"pthread_attr_setstacksize", (uintptr_t)&stub_pthread_attr_setstacksize},
  {"pthread_cond_broadcast", (uintptr_t)&stub_pthread_cond_broadcast},
  {"pthread_cond_destroy", (uintptr_t)&stub_pthread_cond_destroy},
  {"pthread_cond_init", (uintptr_t)&stub_pthread_cond_init},
  {"pthread_cond_signal", (uintptr_t)&stub_pthread_cond_signal},
  {"pthread_cond_timedwait", (uintptr_t)&stub_pthread_cond_timedwait},
  {"pthread_cond_wait", (uintptr_t)&stub_pthread_cond_wait},
  {"pthread_condattr_destroy", (uintptr_t)&stub_pthread_condattr_destroy},
  {"pthread_condattr_init", (uintptr_t)&stub_pthread_condattr_init},
  {"pthread_condattr_setclock", (uintptr_t)&stub_pthread_condattr_setclock},
  {"pthread_create", (uintptr_t)&stub_pthread_create},
  {"pthread_detach", (uintptr_t)&stub_pthread_detach},
  {"pthread_equal", (uintptr_t)&stub_pthread_equal},
  {"pthread_exit", (uintptr_t)&stub_pthread_exit},
  {"pthread_getattr_np", (uintptr_t)&stub_pthread_getattr_np},
  {"pthread_getspecific", (uintptr_t)&stub_pthread_getspecific},
  {"pthread_join", (uintptr_t)&stub_pthread_join},
  {"pthread_key_create", (uintptr_t)&stub_pthread_key_create},
  {"pthread_key_delete", (uintptr_t)&stub_pthread_key_delete},
  {"pthread_kill", (uintptr_t)&stub_pthread_kill},
  {"pthread_mutex_destroy", (uintptr_t)&stub_pthread_mutex_destroy},
  {"pthread_mutex_init", (uintptr_t)&stub_pthread_mutex_init},
  {"pthread_mutex_lock", (uintptr_t)&stub_pthread_mutex_lock},
  {"pthread_mutex_trylock", (uintptr_t)&stub_pthread_mutex_trylock},
  {"pthread_mutex_unlock", (uintptr_t)&stub_pthread_mutex_unlock},
  {"pthread_mutexattr_destroy", (uintptr_t)&stub_pthread_mutexattr_destroy},
  {"pthread_mutexattr_init", (uintptr_t)&stub_pthread_mutexattr_init},
  {"pthread_mutexattr_settype", (uintptr_t)&stub_pthread_mutexattr_settype},
  {"pthread_once", (uintptr_t)&stub_pthread_once},
  {"pthread_rwlock_init", (uintptr_t)&stub_pthread_rwlock_init},
  {"pthread_rwlock_rdlock", (uintptr_t)&stub_pthread_rwlock_rdlock},
  {"pthread_rwlock_unlock", (uintptr_t)&stub_pthread_rwlock_unlock},
  {"pthread_rwlock_wrlock", (uintptr_t)&stub_pthread_rwlock_wrlock},
  {"pthread_self", (uintptr_t)&stub_pthread_self},
  {"pthread_setname_np", (uintptr_t)&stub_pthread_setname_np},
  {"pthread_setspecific", (uintptr_t)&stub_pthread_setspecific},
  {"pthread_sigmask", (uintptr_t)&stub_pthread_sigmask},
  {"ptrace", (uintptr_t)&stub_ptrace},
  {"puts", (uintptr_t)&stub_puts},
  {"qsort", (uintptr_t)&stub_qsort},
  {"raise", (uintptr_t)&stub_raise},
  {"read", (uintptr_t)&stub_read},
  {"readdir", (uintptr_t)&stub_readdir},
  {"readlink", (uintptr_t)&stub_readlink},
  {"realloc", (uintptr_t)&stub_realloc},
  {"realpath", (uintptr_t)&stub_realpath},
  {"recv", (uintptr_t)&stub_recv},
  {"remove", (uintptr_t)&stub_remove},
  {"rename", (uintptr_t)&stub_rename},
  {"rmdir", (uintptr_t)&stub_rmdir},
  {"scalbn", (uintptr_t)&stub_scalbn},
  {"sched_getaffinity", (uintptr_t)&stub_sched_getaffinity},
  {"sched_setaffinity", (uintptr_t)&stub_sched_setaffinity},
  {"sched_yield", (uintptr_t)&stub_sched_yield},
  {"select", (uintptr_t)&stub_select},
  {"sem_destroy", (uintptr_t)&stub_sem_destroy},
  {"sem_getvalue", (uintptr_t)&stub_sem_getvalue},
  {"sem_init", (uintptr_t)&stub_sem_init},
  {"sem_post", (uintptr_t)&stub_sem_post},
  {"sem_timedwait", (uintptr_t)&stub_sem_timedwait},
  {"sem_wait", (uintptr_t)&stub_sem_wait},
  {"send", (uintptr_t)&stub_send},
  {"sendfile", (uintptr_t)&stub_sendfile},
  {"setenv", (uintptr_t)&stub_setenv},
  {"setjmp", (uintptr_t)&stub_setjmp},
  {"setlocale", (uintptr_t)&stub_setlocale},
  {"setpriority", (uintptr_t)&stub_setpriority},
  {"setsockopt", (uintptr_t)&stub_setsockopt},
  {"setvbuf", (uintptr_t)&stub_setvbuf},
  {"shutdown", (uintptr_t)&stub_shutdown},
  {"sigaction", (uintptr_t)&stub_sigaction},
  {"sigaddset", (uintptr_t)&stub_sigaddset},
  {"sigaltstack", (uintptr_t)&stub_sigaltstack},
  {"sigdelset", (uintptr_t)&stub_sigdelset},
  {"sigemptyset", (uintptr_t)&stub_sigemptyset},
  {"sigfillset", (uintptr_t)&stub_sigfillset},
  {"signal", (uintptr_t)&stub_signal},
  {"sigsuspend", (uintptr_t)&stub_sigsuspend},
  {"sin", (uintptr_t)&stub_sin},
  {"sincosf", (uintptr_t)&stub_sincosf},
  {"sinf", (uintptr_t)&stub_sinf},
  {"snprintf", (uintptr_t)&stub_snprintf},
  {"socket", (uintptr_t)&stub_socket},
  {"sprintf", (uintptr_t)&stub_sprintf},
  {"sqrtf", (uintptr_t)&stub_sqrtf},
  {"srand48", (uintptr_t)&stub_srand48},
  {"sscanf", (uintptr_t)&stub_sscanf},
  {"stat", (uintptr_t)&stub_stat},
  {"statfs", (uintptr_t)&stub_statfs},
  {"strcasecmp", (uintptr_t)&stub_strcasecmp},
  {"strcasestr", (uintptr_t)&stub_strcasestr},
  {"strcat", (uintptr_t)&stub_strcat},
  {"strchr", (uintptr_t)&stub_strchr},
  {"strcmp", (uintptr_t)&stub_strcmp},
  {"strcoll_l", (uintptr_t)&stub_strcoll_l},
  {"strcpy", (uintptr_t)&stub_strcpy},
  {"strcspn", (uintptr_t)&stub_strcspn},
  {"strdup", (uintptr_t)&stub_strdup},
  {"strerror", (uintptr_t)&stub_strerror},
  {"strerror_r", (uintptr_t)&stub_strerror_r},
  {"strftime", (uintptr_t)&stub_strftime},
  {"strftime_l", (uintptr_t)&stub_strftime_l},
  {"strlcpy", (uintptr_t)&stub_strlcpy},
  {"strlen", (uintptr_t)&stub_strlen},
  {"strncmp", (uintptr_t)&stub_strncmp},
  {"strncpy", (uintptr_t)&stub_strncpy},
  {"strnlen", (uintptr_t)&stub_strnlen},
  {"strrchr", (uintptr_t)&stub_strrchr},
  {"strspn", (uintptr_t)&stub_strspn},
  {"strstr", (uintptr_t)&stub_strstr},
  {"strtod", (uintptr_t)&stub_strtod},
  {"strtof", (uintptr_t)&stub_strtof},
  {"strtok_r", (uintptr_t)&stub_strtok_r},
  {"strtol", (uintptr_t)&stub_strtol},
  {"strtold", (uintptr_t)&stub_strtold},
  {"strtold_l", (uintptr_t)&stub_strtold_l},
  {"strtoll", (uintptr_t)&stub_strtoll},
  {"strtoll_l", (uintptr_t)&stub_strtoll_l},
  {"strtoul", (uintptr_t)&stub_strtoul},
  {"strtoull", (uintptr_t)&stub_strtoull},
  {"strtoull_l", (uintptr_t)&stub_strtoull_l},
  {"strxfrm_l", (uintptr_t)&stub_strxfrm_l},
  {"swprintf", (uintptr_t)&stub_swprintf},
  {"symlink", (uintptr_t)&stub_symlink},
  {"syscall", (uintptr_t)&stub_syscall},
  {"sysconf", (uintptr_t)&stub_sysconf},
  {"syslog", (uintptr_t)&stub_syslog},
  {"tan", (uintptr_t)&stub_tan},
  {"tanf", (uintptr_t)&stub_tanf},
  {"time", (uintptr_t)&stub_time},
  {"tolower_l", (uintptr_t)&stub_tolower_l},
  {"toupper_l", (uintptr_t)&stub_toupper_l},
  {"towlower", (uintptr_t)&stub_towlower},
  {"towlower_l", (uintptr_t)&stub_towlower_l},
  {"towupper_l", (uintptr_t)&stub_towupper_l},
  {"truncate", (uintptr_t)&stub_truncate},
  {"uname", (uintptr_t)&stub_uname},
  {"unlink", (uintptr_t)&stub_unlink},
  {"unsetenv", (uintptr_t)&stub_unsetenv},
  {"uselocale", (uintptr_t)&stub_uselocale},
  {"usleep", (uintptr_t)&stub_usleep},
  {"utime", (uintptr_t)&stub_utime},
  {"utimes", (uintptr_t)&stub_utimes},
  {"vasprintf", (uintptr_t)&stub_vasprintf},
  {"vfprintf", (uintptr_t)&stub_vfprintf},
  {"vprintf", (uintptr_t)&stub_vprintf},
  {"vsnprintf", (uintptr_t)&stub_vsnprintf},
  {"vsscanf", (uintptr_t)&stub_vsscanf},
  {"wcrtomb", (uintptr_t)&stub_wcrtomb},
  {"wcscoll_l", (uintptr_t)&stub_wcscoll_l},
  {"wcslen", (uintptr_t)&stub_wcslen},
  {"wcsnrtombs", (uintptr_t)&stub_wcsnrtombs},
  {"wcstod", (uintptr_t)&stub_wcstod},
  {"wcstof", (uintptr_t)&stub_wcstof},
  {"wcstol", (uintptr_t)&stub_wcstol},
  {"wcstold", (uintptr_t)&stub_wcstold},
  {"wcstoll", (uintptr_t)&stub_wcstoll},
  {"wcstoul", (uintptr_t)&stub_wcstoul},
  {"wcstoull", (uintptr_t)&stub_wcstoull},
  {"wcsxfrm_l", (uintptr_t)&stub_wcsxfrm_l},
  {"wctob", (uintptr_t)&stub_wctob},
  {"wmemchr", (uintptr_t)&stub_wmemchr},
  {"wmemcmp", (uintptr_t)&stub_wmemcmp},
  {"wmemcpy", (uintptr_t)&stub_wmemcpy},
  {"wmemmove", (uintptr_t)&stub_wmemmove},
  {"wmemset", (uintptr_t)&stub_wmemset},
  {"write", (uintptr_t)&stub_write},
  {"AImage_getWidth", (uintptr_t)&extra_AImage_getWidth},
  {"ALooper_pollOnce", (uintptr_t)&extra_ALooper_pollOnce},
  {"asin", (uintptr_t)&extra_asin},
  {"dup2", (uintptr_t)&extra_dup2},
  {"environ", (uintptr_t)&extra_environ},
  {"fstat64", (uintptr_t)&extra_fstat64},
  {"getauxval", (uintptr_t)&extra_getauxval},
  {"gethostname", (uintptr_t)&extra_gethostname},
  {"getnameinfo", (uintptr_t)&extra_getnameinfo},
  {"mmap64", (uintptr_t)&extra_mmap64},
  {"modff", (uintptr_t)&extra_modff},
  {"recvfrom", (uintptr_t)&extra_recvfrom},
  {"recvmsg", (uintptr_t)&extra_recvmsg},
  {"sendmsg", (uintptr_t)&extra_sendmsg},
  {"setbuf", (uintptr_t)&extra_setbuf},
  {"stat64", (uintptr_t)&extra_stat64},
  {"statfs64", (uintptr_t)&extra_statfs64},
};
size_t dynlib_numfunctions = sizeof(dynlib_functions) / sizeof(dynlib_functions[0]);

static void *resolve_bionic(const char *name) {
  if (!strcmp(name, "setjmp")) return dlsym(RTLD_DEFAULT, "_setjmp");
  if (!strcmp(name, "longjmp")) return dlsym(RTLD_DEFAULT, "_longjmp");
  if (!strcmp(name, "__errno")) return dlsym(RTLD_DEFAULT, "__errno_location");
  return dlsym(RTLD_DEFAULT, name);
}
void recon_fill_passthrough(void) {
  dlopen("libz.so.1", RTLD_NOW | RTLD_GLOBAL);
  dlopen("libm.so.6", RTLD_NOW | RTLD_GLOBAL);
  size_t real = 0;
  for (size_t i = 0; i < dynlib_numfunctions; ++i) {
    void *p = !strcmp(dynlib_functions[i].symbol,"sysconf") ? (void *)stub_sysconf : resolve_bionic(dynlib_functions[i].symbol);
    if (p) { dynlib_functions[i].func = (uintptr_t)p; ++real; }
    else fprintf(stderr, "[MONUMENT-IMPORT] Android shim required: %s\n", dynlib_functions[i].symbol);
  }
  fprintf(stderr, "[MONUMENT-IMPORT] host functions=%zu Android shims=%zu\n", real, dynlib_numfunctions-real);
}
