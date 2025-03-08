plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
}
val abiList = arrayOf("arm64-v8a","x86_64","x86","armeabi-v7a")
android {
    namespace = "com.hnidesu.rime"
    compileSdk = 34

    defaultConfig {
        minSdk = 23
        externalNativeBuild {
            cmake {
                cppFlags += ""
            }
        }
        ndk {
            abiFilters.addAll(abiList)
        }
    }
    ndkVersion = "27.0.12077973"
    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
    externalNativeBuild {
        cmake {
            path = file("../native/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}



val ndkDirectory = File(System.getenv("ANDROID_HOME"),"ndk/${android.ndkVersion}")
val librimeDir = File(rootDir,"native/librime")
dependencies {
    implementation(libs.androidx.annotation.jvm)
}

tasks.register<Exec>("cloneLibrime") {
    onlyIf { !librimeDir.exists() }
    commandLine("git", "clone", "--recursive", "https://github.com/HNIdesu/librime-android.git", "librime")
    workingDir(librimeDir.parentFile)
}

tasks.register<Exec>("downloadBoost") {
    onlyIf { File(librimeDir, "deps").listFiles()?.any { it.isDirectory && it.name.equals("boost") } != true }
    dependsOn("cloneLibrime")
    if (System.getProperty("os.name").lowercase().contains("win"))
        commandLine("cmd", "/c","install-boost.bat","--download")
    else
        commandLine("bash", "-c", "./install-boost.sh","--download")
    workingDir(librimeDir)
}

fun getCommonBuildRimePluginOptions(abi :String) : Array<String> {
    return arrayOf(
        "-DCMAKE_INSTALL_PREFIX=${librimeDir.absolutePath}",
        "-DANDROID_ABI=$abi",
        "-DCMAKE_BUILD_TYPE=Release",
        "-DBUILD_SHARED_LIBS=OFF",
        "-DCMAKE_POSITION_INDEPENDENT_CODE=ON",
        "-DCMAKE_CXX_FLAGS=-Os",
        "-DCMAKE_TOOLCHAIN_FILE=${ndkDirectory.absolutePath}/build/cmake/android.toolchain.cmake",
        "-DANDROID_USE_LEGACY_TOOLCHAIN_FILE=ON",
        "-DANDROID_NATIVE_API_LEVEL=${android.defaultConfig.minSdk}",
        "-DANDROID_NDK=${ndkDirectory.absolutePath}",
    )
}

abiList.forEach { abi ->
    tasks.register<Exec>("configureLibrime[$abi]") {
        onlyIf { !File(librimeDir,"build/$abi").exists() }
        dependsOn(
            "installBoost[$abi]",
            "installOpencc[$abi]",
            "installLeveldb[$abi]",
            "installMarisaTrie[$abi]",
            "installYamlCpp[$abi]"
        )
        commandLine("cmake",".","-Bbuild/$abi",
            "-DCMAKE_FIND_ROOT_PATH=${librimeDir.absolutePath}",
            "-DCMAKE_BUILD_TYPE=Release",
            "-DCMAKE_SHARED_LINKER_FLAGS=-s",
            "-DCMAKE_EXE_LINKER_FLAGS=-s",
            "-DCMAKE_CXX_FLAGS=-Os",
            "-DBUILD_SHARED_LIBS=ON",
            "-DCMAKE_POSITION_INDEPENDENT_CODE=ON",
            "-DCMAKE_TOOLCHAIN_FILE=${ndkDirectory.absolutePath}/build/cmake/android.toolchain.cmake",
            "-DENABLE_LOGGING=OFF",
            "-DANDROID_NATIVE_API_LEVEL=${android.defaultConfig.minSdk}",
            "-DANDROID_NDK=${ndkDirectory.absolutePath}",
            "-DANDROID_ABI=$abi",
            "-DBUILD_MERGED_PLUGINS=ON",
            "-DENABLE_EXTERNAL_PLUGINS=OFF",
            "-DBUILD_STATIC=ON",
            "-DBUILD_DATA=ON",
            "-DBUILD_TEST=OFF"
        )
        workingDir(librimeDir)
    }
    tasks.register<Exec>("buildLibrime[$abi]") {
        onlyIf { !File(librimeDir,"build/$abi/lib/librime.so").exists() }
        dependsOn("configureLibrime[$abi]")
        commandLine("cmake","--build","build/$abi","--parallel")
        isIgnoreExitValue = true
        workingDir(librimeDir)
    }
    tasks.register<Copy>("installLibrime[$abi]") {
        onlyIf { !File(projectDir,"src/main/jniLibs/$abi/librime.so").exists() }
        dependsOn("buildLibrime[$abi]")
        from(File(librimeDir,"build/$abi/lib/librime.so"))
        into(File(projectDir,"src/main/jniLibs/$abi"))
    }
    tasks.register<Exec>("configureBoost[$abi]"){
        dependsOn("downloadBoost")
        commandLine("cmake",".","-B","build/$abi",
            *getCommonBuildRimePluginOptions(abi)
        )
        workingDir(File(librimeDir,"deps/boost"))
    }
    tasks.register<Exec>("configureOpencc[$abi]") {
        dependsOn("cloneLibrime")
        commandLine("cmake",".","-B","build/$abi",
            *getCommonBuildRimePluginOptions(abi),
            "-DBUILD_DOCUMENTATION=OFF",
            "-DENABLE_GTEST=OFF",
            "-DENABLE_BENCHMARK=OFF",
            "-DENABLE_DARTS=OFF"
        )
        workingDir(File(librimeDir,"deps/opencc"))
    }
    tasks.register<Exec>("configureLeveldb[$abi]") {
        dependsOn("cloneLibrime")
        commandLine("cmake",".","-B","build/$abi",
            *getCommonBuildRimePluginOptions(abi),
            "-DLEVELDB_BUILD_TESTS=OFF",
            "-DLEVELDB_BUILD_BENCHMARKS=OFF",
            "-DLEVELDB_INSTALL=ON"
        )
        workingDir(File(librimeDir,"deps/leveldb"))
    }
    tasks.register<Exec>("configureMarisaTrie[$abi]") {
        dependsOn("cloneLibrime")
        commandLine("cmake",".","-B","build/$abi",
            *getCommonBuildRimePluginOptions(abi)
        )
        workingDir(File(librimeDir,"deps/marisa-trie"))
    }
    tasks.register<Exec>("configureYamlCpp[$abi]") {
        dependsOn("cloneLibrime")
        commandLine("cmake",".","-B","build/$abi",
            *getCommonBuildRimePluginOptions(abi),
            "-DYAML_CPP_BUILD_CONTRIB=OFF",
            "-DYAML_CPP_BUILD_TOOLS=OFF",
            "-DYAML_CPP_FORMAT_SOURCE=OFF"
        )
        workingDir(File(librimeDir,"deps/yaml-cpp"))
    }
    tasks.register<Exec>("buildBoost[$abi]") {
        dependsOn("configureBoost[$abi]")
        commandLine("cmake","--build","build/$abi","--parallel")
        workingDir(File(librimeDir,"deps/boost"))
    }
    tasks.register<Exec>("buildOpencc[$abi]") {
        dependsOn("configureOpencc[$abi]")
        commandLine("cmake","--build","build/$abi","--parallel")
        isIgnoreExitValue = true
        workingDir(File(librimeDir,"deps/opencc"))
        doLast {
            if (!File(librimeDir,"deps/opencc/build/$abi/src/libopencc.a").exists())
                throw GradleException()
        }
    }
    tasks.register<Exec>("buildLeveldb[$abi]") {
        dependsOn("configureLeveldb[$abi]")
        commandLine("cmake","--build","build/$abi","--parallel")
        workingDir(File(librimeDir,"deps/leveldb"))
    }
    tasks.register<Exec>("buildMarisaTrie[$abi]") {
        dependsOn("configureMarisaTrie[$abi]")
        commandLine("cmake","--build","build/$abi","--parallel")
        workingDir(File(librimeDir,"deps/marisa-trie"))
    }
    tasks.register<Exec>("buildYamlCpp[$abi]") {
        dependsOn("configureYamlCpp[$abi]")
        commandLine("cmake","--build","build/$abi","--parallel")
        workingDir(File(librimeDir,"deps/yaml-cpp"))
    }
    tasks.register<Exec>("installBoost[$abi]") {
        dependsOn("buildBoost[$abi]")
        commandLine("cmake","--install","build/$abi")
        isIgnoreExitValue = true
        workingDir(File(librimeDir,"deps/boost"))
    }
    tasks.register<Exec>("installOpencc[$abi]") {
        dependsOn("buildOpencc[$abi]")
        commandLine("cmake","--install","build/$abi")
        isIgnoreExitValue = true
        workingDir(File(librimeDir,"deps/opencc"))
    }
    tasks.register<Exec>("installLeveldb[$abi]") {
        dependsOn("buildLeveldb[$abi]")
        commandLine("cmake","--install","build/$abi")
        isIgnoreExitValue = true
        workingDir(File(librimeDir,"deps/leveldb"))
    }
    tasks.register<Exec>("installMarisaTrie[$abi]") {
        dependsOn("buildMarisaTrie[$abi]")
        commandLine("cmake","--install","build/$abi")
        isIgnoreExitValue = true
        workingDir(File(librimeDir,"deps/marisa-trie"))
    }
    tasks.register<Exec>("installYamlCpp[$abi]") {
        dependsOn("buildYamlCpp[$abi]")
        commandLine("cmake","--install","build/$abi")
        isIgnoreExitValue = true
        workingDir(File(librimeDir,"deps/yaml-cpp"))
    }
}

tasks.whenTaskAdded {
    val matchResult =
        Regex("^buildCMake(Debug|Release)\\[(.+)\\]$").matchEntire(name)?:
        Regex("^buildCMakeRelWithDebInfo\\[(.+)\\]$").matchEntire(name)
    if(matchResult != null)
    {
        val abi = matchResult.groupValues[matchResult.groupValues.size-1]
        dependsOn("installLibrime[$abi]")
    }
}

