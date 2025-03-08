package com.hnidesu.rime

data class RimeContext(
    val composition: RimeComposition,
    val menu: RimeMenu,
    val commitTextPreview: String
)