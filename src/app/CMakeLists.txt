qt_add_executable(TheoIDEApp
    main.cpp
    models/sqlcontactmodel.cpp
    models/sqlcontactmodel.hpp
    models/sqlconversationmodel.cpp
    models/sqlconversationmodel.hpp
)

set(QML_COMPONENTS components/ContactPage.qml components/ConversationPage.qml)

set(IMAGE_ASSETS
    assets/images/Albert_Einstein.png
    assets/images/Ernest_Hemingway.png
    assets/images/Hans_Gude.png
)

qt_add_qml_module(TheoIDEApp
    URI QtTest
    VERSION 1.0
    QML_FILES
    main.qml
    ${QML_COMPONENTS}
    RESOURCES
    ${IMAGE_ASSETS}
)

set_target_properties(
    TheoIDEApp
    PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER io.github.theo-ide.theo-ide
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING
            ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        CXX_STANDARD 20
)

target_link_libraries(
    TheoIDEApp
    PRIVATE TheoVM TheoC Qt6::Quick Qt6::Core Qt6::Sql
)

install(
    TARGETS TheoIDEApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
