qt_add_library(TheoIDEModels)

qt_add_qml_module(
    TheoIDEModels
    VERSION 1.0
    URI TheoIDE.Models
)

target_sources(
    TheoIDEModels
    PRIVATE filesystemservice.cpp
    PUBLIC include/filesystemservice.hpp
)

target_include_directories(
    TheoIDEModels
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/
)

target_link_libraries(TheoIDEModels PRIVATE Qt::Core Qt::Quick Qt::Concurrent)
