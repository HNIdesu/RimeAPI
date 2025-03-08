CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(native)
INCLUDE(ExternalProject)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

ADD_SUBDIRECTORY(rimeapi)

INCLUDE(ExternalProject)

ExternalProject_Add(
        yaml-cpp
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/librime
        BINARY_DIR ${CMAKE_SOURCE_DIR}/librime/deps/yaml-cpp/build
        STAMP_DIR ${CMAKE_SOURCE_DIR}/librime/deps/yaml-cpp/stamp
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/librime/deps/yaml-cpp
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/librime
            -DCMAKE_BUILD_TYPE=Release
            -DBUILD_SHARED_LIBS=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DCMAKE_CXX_FLAGS=-Os
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
            -DANDROID_NDK=${ANDROID_NDK}
            -DANDROID_ABI=${ANDROID_ABI}
			-DANDROID_PLATFORM=${ANDROID_PLATFORM}
            -DYAML_CPP_BUILD_CONTRIB=OFF
            -DYAML_CPP_BUILD_TOOLS=OFF
            -DYAML_CPP_FORMAT_SOURCE=OFF
			-DCMAKE_MAKE_PROGRAM=${CMAKE_SOURCE_DIR}/../ninja
)

ExternalProject_Add(
        opencc
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/librime
        BINARY_DIR ${CMAKE_SOURCE_DIR}/librime/deps/opencc/build
        STAMP_DIR ${CMAKE_SOURCE_DIR}/librime/deps/opencc/stamp
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/librime/deps/opencc
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/librime
            -DCMAKE_BUILD_TYPE=Release
            -DBUILD_SHARED_LIBS=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DCMAKE_CXX_FLAGS=-Os
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
            -DANDROID_NDK=${ANDROID_NDK}
            -DANDROID_ABI=${ANDROID_ABI}
			-DANDROID_PLATFORM=${ANDROID_PLATFORM}
            -DBUILD_DOCUMENTATION=OFF
            -DENABLE_GTEST=OFF
            -DENABLE_BENCHMARK=OFF
            -DENABLE_DARTS=OFF
			-DBUILD_DATA=OFF
			-DCMAKE_MAKE_PROGRAM=${CMAKE_SOURCE_DIR}/../ninja
)

ExternalProject_Add(
        marisa-trie
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/librime
        BINARY_DIR ${CMAKE_SOURCE_DIR}/librime/deps/marisa-trie/build
        STAMP_DIR ${CMAKE_SOURCE_DIR}/librime/deps/marisa-trie/stamp
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/librime/deps/marisa-trie
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/librime
            -DCMAKE_BUILD_TYPE=Release
            -DBUILD_SHARED_LIBS=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DCMAKE_CXX_FLAGS=-Os
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
            -DANDROID_NDK=${ANDROID_NDK}
            -DANDROID_ABI=${ANDROID_ABI}
			-DANDROID_PLATFORM=${ANDROID_PLATFORM}
			-DCMAKE_MAKE_PROGRAM=${CMAKE_SOURCE_DIR}/../ninja
)

ExternalProject_Add(
        leveldb
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/librime
        BINARY_DIR ${CMAKE_SOURCE_DIR}/librime/deps/leveldb/build
        STAMP_DIR ${CMAKE_SOURCE_DIR}/librime/deps/leveldb/stamp
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/librime/deps/leveldb
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/librime
            -DCMAKE_BUILD_TYPE=Release
            -DBUILD_SHARED_LIBS=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DCMAKE_CXX_FLAGS=-Os
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
            -DANDROID_NDK=${ANDROID_NDK}
            -DANDROID_ABI=${ANDROID_ABI}
			-DANDROID_PLATFORM=${ANDROID_PLATFORM}
            -DLEVELDB_BUILD_TESTS=OFF
            -DLEVELDB_BUILD_BENCHMARKS=OFF
            -DLEVELDB_INSTALL=ON
			-DCMAKE_MAKE_PROGRAM=${CMAKE_SOURCE_DIR}/../ninja
)

ExternalProject_Add(
        boost
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/librime
        BINARY_DIR ${CMAKE_SOURCE_DIR}/librime/deps/boost/build
        STAMP_DIR ${CMAKE_SOURCE_DIR}/librime/deps/boost/stamp
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/librime/deps/boost
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/librime
            -DCMAKE_BUILD_TYPE=Release
            -DBUILD_SHARED_LIBS=OFF
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DCMAKE_CXX_FLAGS=-Os
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DANDROID_NDK=${ANDROID_NDK}
            -DANDROID_ABI=${ANDROID_ABI}
			-DANDROID_PLATFORM=${ANDROID_PLATFORM}
			-DCMAKE_MAKE_PROGRAM=${CMAKE_SOURCE_DIR}/../ninja
)

ExternalProject_Add(
        librime
        INSTALL_DIR ${CMAKE_SOURCE_DIR}/rimeapi
        BINARY_DIR ${CMAKE_SOURCE_DIR}/librime/build
        STAMP_DIR ${CMAKE_SOURCE_DIR}/librime/stamp
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/librime
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/rimeapi
			-DCMAKE_FIND_ROOT_PATH=${CMAKE_SOURCE_DIR}/librime
            -DCMAKE_BUILD_TYPE=Release
            -DCMAKE_SHARED_LINKER_FLAGS=-s
            -DCMAKE_EXE_LINKER_FLAGS=-s
            -DBUILD_SHARED_LIBS=ON
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DCMAKE_CXX_FLAGS=-Os
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            -DANDROID_NDK=${ANDROID_NDK}
            -DANDROID_ABI=${ANDROID_ABI}
			-DANDROID_PLATFORM=${ANDROID_PLATFORM}
            -DENABLE_LOGGING=OFF
            -DBUILD_MERGED_PLUGINS=ON
            -DENABLE_EXTERNAL_PLUGINS=OFF
            -DBUILD_STATIC=ON
            -DBUILD_DATA=ON
            -DBUILD_TEST=OFF
			-DCMAKE_MAKE_PROGRAM=${CMAKE_SOURCE_DIR}/../ninja
)

ADD_DEPENDENCIES(librime  yaml-cpp leveldb opencc marisa-trie boost)