set(HEADER_FILES
        ${INCLUDE_DIR}/karin/system/application.h
        system/application_impl.h
)

set(SOURCE_FILES
        system/application.cpp
)

if (WIN32)
    set(SOURCE_FILES ${SOURCE_FILES}
            system/windows/win_application_impl.cpp
    )
    set(HEADER_FILES ${HEADER_FILES}
            system/windows/win_application_impl.h
    )
endif ()

add_library(karin_system STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(karin_system PUBLIC ${INCLUDE_DIR})

if (WIN32)
    target_compile_definitions(karin_system PUBLIC KARIN_PLATFORM_WINDOWS)
    target_compile_definitions(karin_system PUBLIC UNICODE)
endif ()