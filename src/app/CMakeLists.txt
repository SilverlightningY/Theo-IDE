qt_add_executable(TheoIDEApp main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(TheoIDEApp PRIVATE QT_QML_DEBUG)
endif()

qt_add_qml_module(TheoIDEApp
    VERSION 1.0
    URI app
    QML_FILES
        main.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        Example.Chat.Components
)

set_target_properties(
    TheoIDEApp
    PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER io.github.Theo_IDE.Theo_IDE
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING
            ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        CXX_STANDARD 20
)

add_subdirectory(Example/Chat/Components)

target_link_libraries(
    TheoIDEApp
    PRIVATE TheoVM TheoC ExampleChatComponents Qt6::Quick Qt6::Core Qt6::Sql
)

install(
    TARGETS TheoIDEApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
