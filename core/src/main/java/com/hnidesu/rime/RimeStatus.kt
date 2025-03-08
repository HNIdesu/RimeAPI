package com.hnidesu.rime

data class RimeStatus(
    val schemaId: String,
    val schemaName: String,
    val isDisabled: Boolean,
    val isComposing: Boolean,
    val isAsciiMode: Boolean,
    val isFullShape: Boolean,
    val isSimplified: Boolean,
    val isTraditional: Boolean,
    val isAsciiPunct: Boolean
)