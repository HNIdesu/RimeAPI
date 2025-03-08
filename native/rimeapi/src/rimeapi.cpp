#include <rime_api.h>
#include <iostream>
#include <jni.h>
#include <android/log.h>
#include <vector>
#include "RimeConverter.h"

#define val auto const
#define var auto

using namespace std;
static const char Tag[] = "rime.hnidesu";

const char *clone_jstring(JNIEnv *env, jstring s) {
    val chars = env->GetStringUTFChars(s, NULL);
    val len = strlen(chars);
    val buffer = new char[len + 1];
    memcpy(buffer, chars, len);
    buffer[len] = '\0';
    env->ReleaseStringUTFChars(s, chars);
    return buffer;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_selectSchema(JNIEnv *env, jobject thisObject, jlong sessionId,
                                           jstring schema) {
    val api = rime_get_api();
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val result = api->select_schema(sessionId, c_schema);
    env->ReleaseStringUTFChars(schema, c_schema);
    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hnidesu_rime_RimeAPI_finalize(JNIEnv *env, jobject thisObj) {
    rime_get_api()->finalize();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_destroySession(JNIEnv *env, jobject thisObj, jlong sessionId) {
    return rime_get_api()->destroy_session(sessionId);
}

class GC {
private:
    vector<const char *> *ptr_list;
public:
    GC(int init_size) {
        this->ptr_list = new vector<const char *>(init_size);
    };

    ~GC() {
        for (int i = 0; i < ptr_list->size(); i++)
            delete[] ptr_list->at(i);
        delete ptr_list;
    }

    const char *add(const char *ptr) {
        ptr_list->push_back(ptr);
        return ptr;
    }
};

extern "C"
JNIEXPORT void JNICALL
Java_com_hnidesu_rime_RimeAPI_setOption(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jstring option,
        jboolean value) {
    val api = rime_get_api();
    val c_option = env->GetStringUTFChars(option, NULL);
    api->set_option(sessionId, c_option, value);
    env->ReleaseStringUTFChars(option, c_option);
    return;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_getOption(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jstring option) {
    val api = rime_get_api();
    val c_option = env->GetStringUTFChars(option, NULL);
    val result = api->get_option(sessionId, c_option);
    env->ReleaseStringUTFChars(option, c_option);
    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hnidesu_rime_RimeAPI_initialize(
        JNIEnv *env,
        jobject thisObj,
        jstring prebuiltDataDirectory,
        jstring stagingDirectory,
        jstring logDirectory,
        jstring shareDataDirectory,
        jstring userDataDirectory) {
    GC gc(1);
    val api = rime_get_api();
    RIME_STRUCT(RimeTraits, traits);
    traits.prebuilt_data_dir = gc.add(env->GetStringUTFChars(prebuiltDataDirectory, NULL));
    traits.app_name = Tag;
    if (logDirectory != NULL)
        traits.log_dir = gc.add(clone_jstring(env, logDirectory));
    if (stagingDirectory != NULL)
        traits.staging_dir = gc.add(clone_jstring(env, stagingDirectory));
    if (shareDataDirectory != NULL)
        traits.shared_data_dir = gc.add(clone_jstring(env, shareDataDirectory));
    if (userDataDirectory != NULL)
        traits.user_data_dir = gc.add(clone_jstring(env, userDataDirectory));
    api->setup(&traits);
    api->deployer_initialize(&traits);
    api->initialize(nullptr);
    return;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_hnidesu_rime_RimeAPI_createSession(JNIEnv *env, jobject thisObj) {
    return rime_get_api()->create_session();
}

extern "C"
JNIEXPORT JNICALL jboolean
Java_com_hnidesu_rime_RimeAPI_processKey(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jint keyCode,
        jint mask) {
    val api = rime_get_api();
    return api->process_key(sessionId, keyCode, mask);
}

extern "C"
JNIEXPORT JNICALL jobjectArray
Java_com_hnidesu_rime_RimeAPI_getCandidates(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId) {
    val api = rime_get_api();
    RIME_STRUCT(RimeContext, context);
    api->get_context(sessionId, &context);
    val num_candidates = context.menu.num_candidates;
    val stringArray = env->NewObjectArray(num_candidates, env->FindClass("java/lang/String"), NULL);
    val candidates = context.menu.candidates;
    for (int i = 0; i < num_candidates; i++) {
        val candidate = candidates[i];
        env->SetObjectArrayElement(stringArray, i, env->NewStringUTF(candidate.text));
    }
    api->free_context(&context);
    return stringArray;
}

extern "C"
JNIEXPORT JNICALL jboolean
Java_com_hnidesu_rime_RimeAPI_simulateKeySequence(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jstring input) {
    val api = rime_get_api();
    val input_data = env->GetStringUTFChars(input, NULL);
    val result = api->simulate_key_sequence(sessionId, input_data);
    env->ReleaseStringUTFChars(input, input_data);
    return result;
}

extern "C"
JNIEXPORT JNICALL jstring
Java_com_hnidesu_rime_RimeAPI_getCommit(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId) {
    val api = rime_get_api();
    RIME_STRUCT(RimeCommit, commit);
    api->get_commit(sessionId, &commit);
    val result = env->NewStringUTF(commit.text);
    api->free_commit(&commit);
    return result;
}

extern "C"
JNIEXPORT JNICALL jobject
Java_com_hnidesu_rime_RimeAPI_getComposition(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId) {
    val api = rime_get_api();
    RIME_STRUCT(RimeContext, context);
    api->get_context(sessionId, &context);
    val result = convertRimeComposition(env, context.composition);
    api->free_context(&context);
    return result;
}

extern "C"
JNIEXPORT JNICALL void
Java_com_hnidesu_rime_RimeAPI_clearComposition(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId) {
    rime_get_api()->clear_composition(sessionId);
}

extern "C"
JNIEXPORT JNICALL jboolean
Java_com_hnidesu_rime_RimeAPI_selectCandidate(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jint index) {
    return rime_get_api()->select_candidate_on_current_page(sessionId, index);
}

extern "C"
JNIEXPORT JNICALL jstring
Java_com_hnidesu_rime_RimeAPI_getInput(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId) {
    val input = rime_get_api()->get_input(sessionId);
    return env->NewStringUTF(input);
}

extern "C"
JNIEXPORT JNICALL void
Java_com_hnidesu_rime_RimeAPI_cleanupAllSessions(
        JNIEnv *env,
        jobject thisObj) {
    rime_get_api()->cleanup_all_sessions();
    return;
}

extern "C"
JNIEXPORT JNICALL jobject
Java_com_hnidesu_rime_RimeAPI_getSchemaList(
        JNIEnv *env,
        jobject thisObj) {
    val api = rime_get_api();
    RimeSchemaList schema_list;
    api->get_schema_list(&schema_list);
    val hashMapClass = env->FindClass("java/util/HashMap");
    val schemaList = env->NewObject(
            hashMapClass,
            env->GetMethodID(hashMapClass, "<init>", "()V"));
    val putMethod = env->GetMethodID(
            hashMapClass,
            "put",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (int i = 0; i < schema_list.size; i++) {
        val item = schema_list.list[i];
        env->CallObjectMethod(schemaList, putMethod, env->NewStringUTF(item.schema_id),
                              env->NewStringUTF(item.name));
    }
    api->free_schema_list(&schema_list);
    return schemaList;
}

extern "C"
JNIEXPORT JNICALL void
Java_com_hnidesu_rime_RimeAPI_schemaSetInt(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key,
        jint value) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    api->config_set_int(&config, c_key, value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return;
}

extern "C"
JNIEXPORT JNICALL jint
Java_com_hnidesu_rime_RimeAPI_schemaGetInt(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    int value;
    api->config_get_int(&config, c_key, &value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return value;
}

extern "C"
JNIEXPORT JNICALL void
Java_com_hnidesu_rime_RimeAPI_schemaSetString(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key,
        jstring value) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    val c_value = env->GetStringUTFChars(value, NULL);
    api->config_set_string(&config, c_key, c_value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    env->ReleaseStringUTFChars(value, c_value);
    return;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_hnidesu_rime_RimeAPI_schemaGetString(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    char buffer[128];
    api->config_get_string(&config, c_key, buffer, 128);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return env->NewStringUTF(buffer);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hnidesu_rime_RimeAPI_schemaSetBool(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key,
        jboolean value) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    api->config_set_bool(&config, c_key, value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_schemaGetBool(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    int value;
    api->config_get_bool(&config, c_key, &value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return value;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hnidesu_rime_RimeAPI_schemaSetDouble(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key,
        jdouble value) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    api->config_set_double(&config, c_key, value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return;
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_hnidesu_rime_RimeAPI_schemaGetDouble(
        JNIEnv *env,
        jobject thisObj,
        jstring schema,
        jstring key) {
    val api = rime_get_api();
    RimeConfig config;
    val c_schema = env->GetStringUTFChars(schema, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    api->schema_open(c_schema, &config);
    double value;
    api->config_get_double(&config, c_key, &value);
    api->config_close(&config);
    env->ReleaseStringUTFChars(schema, c_schema);
    env->ReleaseStringUTFChars(key, c_key);
    return value;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_deleteCandidateOnCurrentPage(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jint index) {
    return rime_get_api()->delete_candidate_on_current_page(sessionId, index);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_deleteCandidate(
        JNIEnv *env,
        jobject thisObj,
        jlong sessionId,
        jint index) {
    return rime_get_api()->delete_candidate(sessionId, index);
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_hnidesu_rime_RimeAPI_getUserDictEntries(
        JNIEnv *env,
        jobject thisObj,
        jstring dict_name) {
    val api = rime_get_api();
    val c_dict_name = env->GetStringUTFChars(dict_name, NULL);
    UserDictData data;
    api->get_user_dict_entries(c_dict_name, &data);
    env->ReleaseStringUTFChars(dict_name, c_dict_name);
    val hashMapClass = env->FindClass("java/util/HashMap");
    val entry_list = env->NewObject(
            hashMapClass,
            env->GetMethodID(hashMapClass, "<init>", "()V"));
    val put_method = env->GetMethodID(
            hashMapClass,
            "put",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (int i = 0, count = data.count; i < count; i++) {
        val item = data.entries[i];
        env->CallObjectMethod(entry_list, put_method, env->NewStringUTF(item.key),
                              env->NewStringUTF(item.value));
    }
    api->release_user_dict_entries(&data);
    return entry_list;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hnidesu_rime_RimeAPI_deleteUserDictEntry(
        JNIEnv *env,
        jobject thiz,
        jstring dict_name,
        jstring key) {
    val c_dict_name = env->GetStringUTFChars(dict_name, NULL);
    val c_key = env->GetStringUTFChars(key, NULL);
    val result = rime_get_api()->delete_user_dict_entry(c_dict_name, c_key);
    env->ReleaseStringUTFChars(dict_name, c_dict_name);
    env->ReleaseStringUTFChars(key, c_key);
    return result;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_hnidesu_rime_RimeAPI_getVersion(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(rime_get_api()->get_version());
}
extern "C"
JNIEXPORT int JNICALL
Java_com_hnidesu_rime_RimeAPI_deploy(JNIEnv *env, jobject thiz) {
    return rime_get_api()->deploy();
}
extern "C"
JNIEXPORT int JNICALL
Java_com_hnidesu_rime_RimeAPI_prebuild(JNIEnv *env, jobject thiz) {
    return rime_get_api()->prebuild();
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_hnidesu_rime_RimeAPI_getStatus(
        JNIEnv *env, jobject thiz, jlong session_id) {
    RIME_STRUCT(RimeStatus, status)
    rime_get_api()->get_status(session_id, &status);
    val result = convertRimeStatus(env, status);
    rime_get_api()->free_status(&status);
    return result;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_hnidesu_rime_RimeAPI_getContext(JNIEnv *env, jobject thiz, jlong session_id) {
    val rimeContextClass = env->FindClass("com/hnidesu/rime/RimeContext");
    RIME_STRUCT(RimeContext, context)
    rime_get_api()->get_context(session_id, &context);
    val result = convertRimeContext(env, context);
    rime_get_api()->free_context(&context);
    return result;
}