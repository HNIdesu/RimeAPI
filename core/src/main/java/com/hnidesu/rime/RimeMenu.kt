package com.hnidesu.rime

data class RimeMenu(
    val pageSize: Int,
    val pageNo: Int,
    val isLastPage: Boolean,
    val highlightedCandidateIndex: Int,
    val candidates: Array<RimeCandidate>,
    val selectKeys: String?
)