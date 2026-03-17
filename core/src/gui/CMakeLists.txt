set(GUI_SOURCES
        view_node.cpp
        window.cpp
        container_node.cpp
        rectangle_node.cpp
        leaf_node.cpp
        application.cpp
        application_context.cpp
        text_node.cpp
)

add_library(karin_gui STATIC ${GUI_SOURCES})
target_include_directories(karin_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(karin_gui PRIVATE ${COMMON_DIR})
target_include_directories(karin_gui PUBLIC ${INCLUDE_DIR})
target_include_directories(karin_gui PRIVATE ${THIRD_PARTY_DIR})

find_package(yoga CONFIG REQUIRED)
target_link_libraries(karin_gui PRIVATE yoga::yogacore)