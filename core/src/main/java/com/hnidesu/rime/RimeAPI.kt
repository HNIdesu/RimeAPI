package com.hnidesu.rime

import androidx.annotation.Keep

object RimeAPI {
    init {
        System.loadLibrary("rimeapi")
    }

    @Keep
    external fun finalize()

    @Keep
    external fun initialize(
        prebuiltDataDirectory: String,
        stagingDirectory: String?,
        logDirectory: String?,
        shareDataDirectory: String?,
        userDataDirectory: String?
    )

    @Keep
    external fun deploy(): Int

    @Keep
    external fun prebuild(): Int

    @Keep
    external fun selectSchema(sessionId: Long, schema: String): Boolean

    @Keep
    external fun processKey(sessionId: Long, keyCode: Int, mask: Int): Boolean

    @Keep
    external fun getCandidates(sessionId: Long): Array<String>

    @Keep
    external fun getCommit(sessionId: Long): String?

    @Keep
    external fun simulateKeySequence(sessionId: Long, input: String): Boolean

    @Keep
    external fun clearComposition(sessionId: Long)

    @Keep
    external fun getInput(sessionId: Long): String

    @Keep
    external fun getComposition(sessionId: Long): RimeComposition

    @Keep
    external fun selectCandidate(sessionId: Long, index: Int): Boolean

    @Keep
    external fun getOption(sessionId: Long, option: String): Boolean

    @Keep
    external fun setOption(sessionId: Long, option: String, value: Boolean)

    @Keep
    external fun createSession(): Long

    @Keep
    external fun destroySession(sessionId: Long): Boolean

    @Keep
    external fun cleanupAllSessions()

    @Keep
    external fun schemaSetInt(schema: String, key: String, value: Int)

    @Keep
    external fun schemaSetBool(schema: String, key: String, value: Boolean)

    @Keep
    external fun schemaSetString(schema: String, key: String, value: String)

    @Keep
    external fun schemaSetDouble(schema: String, key: String, value: Double)

    @Keep
    external fun schemaGetInt(schema: String, key: String): Int

    @Keep
    external fun schemaGetBool(schema: String, key: String): Boolean

    @Keep
    external fun schemaGetString(schema: String, key: String): String

    @Keep
    external fun schemaGetDouble(schema: String, key: String): Double

    @Keep
    external fun deleteCandidate(sessionId: Long, index: Int): Boolean

    @Keep
    external fun deleteCandidateOnCurrentPage(sessionId: Long, index: Int): Boolean

    @Keep
    external fun getUserDictEntries(dictName: String): HashMap<String, String>

    @Keep
    external fun deleteUserDictEntry(dictName: String, key: String): Boolean

    @Keep
    external fun getVersion(): String

    @Keep
    external fun getStatus(sessionId: Long): RimeStatus

    @Keep
    external fun getContext(sessionId: Long): RimeContext

}