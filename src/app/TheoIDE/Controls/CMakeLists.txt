qt_add_library(TheoIDEControls)

qt_add_qml_module(
    TheoIDEControls
    VERSION 1.0
    QML_FILES
        ClosableTabButton.qml
        CodeEditor.qml
        ExecutionStatePanel.qml
        LineInfoCell.qml
        TabContent.qml
    URI TheoIDE.Controls
    IMPORTS
        QtQuick
        QtQuick.Controls.Material
        TheoIDE.Persistence
)

target_link_libraries(
    TheoIDEControls
    PRIVATE TheoIDEPersistence Qt::Core Qt::Quick Qt::QuickControls2
)
