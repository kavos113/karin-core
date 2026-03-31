package com.github.kavos113.karin;

import java.io.File;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

class KarinLoader {
    private static boolean loaded = false;
    private static final String LIB_NAME = "karin_languages_java";

    public static synchronized void load() {
        if (loaded) {
            return;
        }

        String os = System.getProperty("os.name").toLowerCase();
        String resourcePath;

        if (os.contains("win")) {
            resourcePath = "/native/windows/" + LIB_NAME + ".dll";
        } else if (os.contains("mac")) {
            resourcePath = "/native/macos/lib" + LIB_NAME + ".dylib";
        } else if (os.contains("linux")) {
            resourcePath = "/native/linux/lib" + LIB_NAME + ".so";
        } else {
            throw new UnsupportedOperationException("Unsupported operating system: " + os);
        }

        try (InputStream inputStream = KarinLoader.class.getResourceAsStream(resourcePath)) {
            if (inputStream == null) {
                throw new RuntimeException("Failed to load native library from resource: " + resourcePath);
            }

            File tmpFile = File.createTempFile(LIB_NAME + "-", new File(resourcePath).getName());
            tmpFile.deleteOnExit();

            Files.copy(inputStream, tmpFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
            System.load(tmpFile.getAbsolutePath());
        } catch (Exception e) {
            throw new RuntimeException("Failed to load native library: " + e.getMessage(), e);
        } finally {
            loaded = true;
        }
    }
}