set(HEADER_FILES
        system/application.h
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