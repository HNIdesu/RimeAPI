plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
}

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
            abiFilters.addAll(arrayOf("arm64-v8a","x86_64","x86","armeabi-v7a"))
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
    doLast {
        File(librimeDir, "deps").listFiles()?.first { it.isDirectory && it.name.startsWith("boost") }!!.renameTo(
            File(librimeDir, "deps/boost")
        )
    }
}

tasks.register("installNinja") {
    onlyIf { !File(projectDir.parentFile,"ninja").exists() }
    doLast {
        ProcessBuilder("wget","https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-linux.zip")
            .directory(projectDir.parentFile)
            .start().waitFor()
        ProcessBuilder("unzip","ninja-linux.zip").directory(projectDir.parentFile).start().waitFor()
    }
}

tasks.whenTaskAdded {
    if(name.startsWith("configureCMake")){
        dependsOn("installNinja","downloadBoost")
        return@whenTaskAdded
    }
    val matchResult =
        Regex("^buildCMake(Debug|Release)\\[(.+)\\]$").matchEntire(name)?:
        Regex("^buildCMakeRelWithDebInfo\\[(.+)\\]$").matchEntire(name)
    if(matchResult != null)
    {
		val abi = matchResult.groupValues.last()
        if(tasks.findByPath("copyLibrime[$abi]") == null)
            tasks.register<Copy>("copyLibrime[$abi]") {
                from(File(librimeDir,"build/lib/librime.so"))
                into(File(projectDir,"src/main/jniLibs/$abi/librime.so"))
                dependsOn(this@whenTaskAdded.path)
            }
    }
}

