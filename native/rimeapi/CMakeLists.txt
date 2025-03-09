include_directories(include)
link_directories(../../core/src/main/jniLibs/${ANDROID_ABI})
add_library(rimeapi SHARED src/rimeapi.cpp src/RimeConverter.cpp)
target_link_libraries(rimeapi PRIVATE rime -llog)