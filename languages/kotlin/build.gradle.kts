plugins {
    kotlin("jvm") version "2.3.0"
}

group = "com.github.kavos113"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(kotlin("test"))
}

kotlin {
    jvmToolchain(25)
}

tasks.test {
    useJUnitPlatform()
}

tasks.withType<JavaCompile> {
    val jniHeaderDir = file("src/main/cpp/include")

    doFirst {
        jniHeaderDir.mkdirs()
    }

    options.compilerArgs.addAll(
        listOf(
            "-h", jniHeaderDir.absolutePath
        )
    )
}