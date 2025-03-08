#ifndef RIMEAPI_RIMECONVERTER_H
#define RIMEAPI_RIMECONVERTER_H

#include <rime_api.h>
#include <jni.h>

jobject convertRimeStatus(JNIEnv *env, const RimeStatus &status);

jobject convertRimeMenu(JNIEnv *env, const RimeMenu &menu);

jobject convertRimeCandidate(JNIEnv *env, const RimeCandidate &candidate);

jobject convertRimeContext(JNIEnv *env, const RimeContext &context);

jobject convertRimeComposition(JNIEnv *env, const RimeComposition &composition);

jobject convertRimeConfig(JNIEnv *env, const RimeConfig &config);

#endif
