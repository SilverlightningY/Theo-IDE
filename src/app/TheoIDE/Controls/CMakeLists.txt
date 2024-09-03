qt_add_library(TheoIDEControls)

qt_add_qml_module(
    TheoIDEControls
    VERSION 1.0
    QML_FILES
        ClosableTabButton.qml
        CodeEditor.qml
        ExecutionStatePanel.qml
        LineInfoCell.qml
        LineInfoColumn.qml
        PlainTextEditor.qml
        TabContent.qml
    URI TheoIDE.Controls
    IMPORTS
        QtQuick
        QtQuick.Controls.Material
        TheoIDE.Persistence
        TheoIDE.Models
)

target_sources(
    TheoIDEControls
    PRIVATE lineinfocolumnmodel.cpp editormodel.cpp
    PUBLIC include/lineinfocolumnmodel.hpp include/editormodel.hpp
)

target_include_directories(
    TheoIDEControls
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/
)

target_link_libraries(
    TheoIDEControls
    PUBLIC
        TheoIDEModels
        TheoIDEPersistence
        Qt::Core
        Qt::Quick
        Qt::QuickControls2
        Qt::Concurrent
)
