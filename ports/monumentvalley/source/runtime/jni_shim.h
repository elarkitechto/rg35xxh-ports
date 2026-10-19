/*
 * jni_shim.h -- fake JNI environment for Syberia
 *
 * Provides a fake JavaVM and JNIEnv with stub vtables so that
 * JNI calls from libsyberia1.so don't crash.
 */

#ifndef __JNI_SHIM_H__
#define __JNI_SHIM_H__

void jni_shim_init(void **out_vm, void **out_env);
struct hk_inject_s { int action, keycode, source, deviceId, metaState, repeat,
                     scancode, flags, unicode; long eventTime, downTime; };
extern struct hk_inject_s g_hk_inject;
void *hk_keyevent_object(void);

// recon: acha o ponteiro de um metodo nativo registrado via RegisterNatives
void *jni_find_native(const char *name);

// Set package name and OBB version (call before jni_shim_init)
void jni_shim_set_package(const char *package_name, int obb_version);

void monument_prefs_shutdown(void);

#endif
