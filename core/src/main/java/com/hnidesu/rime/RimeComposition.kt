package com.hnidesu.rime

data class RimeComposition(
    val length: Int,
    val cursorPos: Int,
    val selStart: Int,
    val selEnd: Int,
    val preedit: String?
)