cmake_minimum_required(VERSION 3.25)

project(TheoIDE VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
include(CPack)

find_package(Qt6 6.6 COMPONENTS Core Quick QuickControls2 Sql REQUIRED)

qt_standard_project_setup(REQUIRES 6.6)
qt_policy(SET QTP0003 NEW)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(
                FATAL_ERROR
                "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules"
            )
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/src/libtheo/CMakeLists.txt")
    message(
        FATAL_ERROR
        "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again."
    )
endif()

add_subdirectory(src/libtheo)
add_subdirectory(src/app)

qt_generate_deploy_qml_app_script(TARGET TheoIDEApp
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
