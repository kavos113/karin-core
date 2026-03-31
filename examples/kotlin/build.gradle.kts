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