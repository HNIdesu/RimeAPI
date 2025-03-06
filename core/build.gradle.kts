import com.android.build.gradle.internal.tasks.factory.dependsOn

plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
}
val abiList = arrayOf("arm64-v8a","x86_64","x86","armeabi-v7a")
val scriptDirectory =
tasks.register("cloneLibrime") {
    doLast {
        println("cloneLibrime")
        ProcessBuilder("git","clone","https://github.com/HNIdesu/librime-android.git","--recursive")
            .directory(File("../native")).start().waitFor()
    }
}

tasks.register("buildYamlCpp") {
    doLast {
        for (abi in abiList) {
            ProcessBuilder("cmake",).start().waitFor()
        }
    }
    dependsOn("cloneLibrime")
}

tasks.register("buildLeveldb") {
    dependsOn("cloneLibrime")
}

tasks.register("buildMarisaTrie") {
    dependsOn("cloneLibrime")
}

tasks.register("buildOpencc") {
    dependsOn("cloneLibrime")
}

tasks.register("buildRime") {
    dependsOn("buildYamlCpp","buildLeveldb","buildMarisaTrie","buildOpencc")
    doLast {

    }
}

tasks.named("build").dependsOn("buildRime")

android {
    namespace = "com.hnidesu.rime"
    compileSdk = 34

    defaultConfig {
        minSdk = 23
        lint.targetSdk = 34
        ndk {
            abiFilters.addAll(abiList)
            version = "27.0.12077973"
        }
    }

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
}