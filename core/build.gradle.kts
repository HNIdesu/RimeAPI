plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
    id("maven-publish")
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
            abiFilters.addAll(arrayOf("arm64-v8a", "x86_64", "x86", "armeabi-v7a"))
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

publishing {
    publications {
        create<MavenPublication>("release") {
            groupId = "com.github.hnidesu.rimeapi"
            version = "1.0.0"
            artifactId = "core"
            afterEvaluate {
                from(components["release"])
            }
        }
    }
}


val ndkDirectory = File(System.getenv("ANDROID_HOME"), "ndk/${android.ndkVersion}")
val librimeDir = File(rootDir, "native/librime")
dependencies {
    implementation(libs.androidx.annotation.jvm)
}

tasks.whenTaskAdded {
    val matchResult =
        Regex("^buildCMake(Debug|Release)\\[(.+)\\]$").matchEntire(name)
            ?: Regex("^buildCMakeRelWithDebInfo\\[(.+)\\]$").matchEntire(name)
    if (matchResult != null) {
        val abi = matchResult.groupValues.last()
        if (tasks.findByPath("installLibrime[$abi]") == null)
            tasks.register<Exec>("installLibrime[$abi]") {
                onlyIf { !File(projectDir, "src/main/jniLibs/$abi/librime.so").exists() }
                commandLine("bash","./install-librime.sh")
                workingDir(librimeDir.parentFile)
                environment(
                    "NDK_PATH" to ndkDirectory.absolutePath,
                    "ANDROID_ARCH_ABI" to abi,
                    "ANDROID_PLATFORM" to android.defaultConfig.minSdk
                )
                this@whenTaskAdded.dependsOn(path)
            }
    }
}

