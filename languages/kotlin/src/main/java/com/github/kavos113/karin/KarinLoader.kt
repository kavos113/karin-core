package com.github.kavos113.karin

import java.io.File
import java.nio.file.Files
import java.nio.file.StandardCopyOption

object KarinLoader {
    const val LIB_NAME = "karin_languages_java"

    private var loaded = false

    @Synchronized
    fun load() {
        if (loaded) {
            return
        }

        val os = System.getProperty("os.name").lowercase()
        val (dir, file) = when {
            os.contains("win") -> "windows" to "$LIB_NAME.dll"
            os.contains("mac") -> "macos" to "lib$LIB_NAME.dylib"
            os.contains("linux") -> "linux" to "lib$LIB_NAME.so"
            else -> throw UnsupportedOperationException("Unsupported operating system: $os")
        }

        val resourcePath = "/$dir/$file"

        val inputStream = KarinLoader::class.java.getResourceAsStream(resourcePath)
            ?: throw RuntimeException("Failed to load native library from resource: $resourcePath")

        val tmpFile = File.createTempFile("$LIB_NAME-", file)
        tmpFile.deleteOnExit()

        inputStream.use { input ->
            Files.copy(input, tmpFile.toPath(), StandardCopyOption.REPLACE_EXISTING)
        }

        System.load(tmpFile.absolutePath)
        loaded = true
    }
}