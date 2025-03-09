# RimeAPI

RimeAPI是一个封装了 [librime](https://github.com/rime/librime) 的库，旨在简化在 Android 平台上调用librime输入法框架的过程。

## 特性
- 封装了[librime](https://github.com/rime/librime) ，提供简化的接口，便于 Android 开发者集成。
- 支持通过 JitPack 自动化发布和更新，免去手动编译的烦恼。

## 使用方法

### 1. 配置 JitPack 仓库

在 `settings.gradle.kts` 文件中添加以下配置：

```kotlin
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        mavenCentral()
        maven { url = uri("https://jitpack.io") }
    }
}
```

### 2. 添加依赖

在 `build.gradle.kts` 文件的 `dependencies` 块中添加以下依赖：

```kotlin
dependencies {
    implementation("com.github.HNIdesu:RimeAPI:1.0.0")
}
```