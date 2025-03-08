#include "RimeConverter.h"

#define val auto const
#define var auto

jobject convertRimeStatus(JNIEnv *env, const RimeStatus &status) {
    val rimeStatusClass = env->FindClass("com/hnidesu/rime/RimeStatus");
    val constructor = env->GetMethodID(
            rimeStatusClass,
            "<init>",
            "(Ljava/lang/String;Ljava/lang/String;ZZZZZZZ)V"
    );
    val schemaId = env->NewStringUTF(status.schema_id);
    val schemaName = env->NewStringUTF(status.schema_name);
    return env->NewObject(rimeStatusClass, constructor,
                          schemaId, schemaName,
                          status.is_disabled,
                          status.is_composing,
                          status.is_ascii_mode,
                          status.is_full_shape,
                          status.is_simplified,
                          status.is_traditional,
                          status.is_ascii_punct
    );
}

jobject convertRimeMenu(JNIEnv *env, const RimeMenu &menu) {
    val rimeMenuClass = env->FindClass("com/hnidesu/rime/RimeMenu");
    val rimeCandidateClass = env->FindClass("com/hnidesu/rime/RimeCandidate");
    val constructor = env->GetMethodID(rimeMenuClass,
                                       "<init>",
                                       "(IIZI[Lcom/hnidesu/rime/RimeCandidate;Ljava/lang/String;)V");
    val candidateArray = env->NewObjectArray(menu.num_candidates, rimeCandidateClass,
                                             nullptr);
    for (int i = 0; i < menu.num_candidates; i++) {
        auto const candidate = convertRimeCandidate(env, menu.candidates[i]);
        env->SetObjectArrayElement(candidateArray, i, candidate);
    }
    val selectKeys = env->NewStringUTF(menu.select_keys);
    return env->NewObject(rimeMenuClass, constructor, menu.page_size, menu.page_no,
                          menu.is_last_page, menu.highlighted_candidate_index, candidateArray,
                          selectKeys);
}

jobject convertRimeCandidate(JNIEnv *env, const RimeCandidate &candidate) {
    val rimeCandidateClass = env->FindClass("com/hnidesu/rime/RimeCandidate");
    val constructor = env->GetMethodID(
            rimeCandidateClass,
            "<init>",
            "(Ljava/lang/String;Ljava/lang/String;)V"
    );
    val text = env->NewStringUTF(candidate.text);
    val comment = env->NewStringUTF(candidate.comment);
    return env->NewObject(rimeCandidateClass, constructor,
                          text,
                          comment
    );
}

jobject convertRimeContext(JNIEnv *env, const RimeContext &context) {
    val rimeContextClass = env->FindClass("com/hnidesu/rime/RimeContext");
    val constructor = env->GetMethodID(rimeContextClass, "<init>",
                                       "(Lcom/hnidesu/rime/RimeComposition;Lcom/hnidesu/rime/RimeMenu;Ljava/lang/String;)V");
    val rimeComposition = convertRimeComposition(env, context.composition);
    val rimeMenu = convertRimeMenu(env, context.menu);
    val commitTextPreview = env->NewStringUTF(context.commit_text_preview);
    return env->NewObject(rimeContextClass, constructor, rimeComposition, rimeMenu,
                          commitTextPreview);
}

jobject convertRimeComposition(JNIEnv *env, const RimeComposition &composition) {
    val rimeCompositionClass = env->FindClass("com/hnidesu/rime/RimeComposition");
    val constructor = env->GetMethodID(rimeCompositionClass, "<init>", "(IIIILjava/lang/String;)V");
    val preedit = env->NewStringUTF(composition.preedit);
    return env->NewObject(rimeCompositionClass, constructor, composition.length,
                          composition.cursor_pos, composition.sel_start, composition.sel_end,
                          preedit);
}