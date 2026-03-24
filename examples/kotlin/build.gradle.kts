plugins {
    kotlin("jvm") version "2.3.0"
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("com.github.kavos113:karin-kotlin:1.0-SNAPSHOT")
}

kotlin {
    jvmToolchain(24)
}

tasks.withType<JavaExec> {
    val nativeLibDir = file("$rootDir/../../languages/kotlin/build/native").absolutePath
    systemProperty("java.library.path", nativeLibDir)

    val os = System.getProperty("os.name").lowercase()
    val envVar = when {
        os.contains("win") -> "PATH"
        os.contains("mac") -> "DYLD_LIBRARY_PATH"
        else -> "LD_LIBRARY_PATH"
    }

    val currEnv = System.getenv(envVar) ?: ""
    environment(envVar, "$nativeLibDir${File.pathSeparator}$currEnv")
}