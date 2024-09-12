# Theo-IDE

IDE for concept languages such as LOOP, WHILE and GOTO

## Building

The TheoIDEApp can be build on the platforms Linux, Windows and MacOS and for the platforms Android and IOS. Building on all platforms is reviewed and tested with CMake in combination with the Ninja generator.

To build the application from source the following tools are needed:
- Qt6.7 or newer
- CMake 3.25 or newer
- FLEX 2.6.4 or newer
- C++20 compiler (Tested and reviewed: gcc on Linux, MSVC on Windows, Appleclang on MacOS)
- OpenGL

The basic build and install commands for any platform are:

```
cmake -S <project-directory> -B <build-directory> -DCMAKE_PREFIX_PATH=<path-to-Qt>/<target_platform>/lib/cmake -DCMAKE_BUILD_TYPE=Release
cmake --build <build-directory> 
cmake --install <build-directory>
```

### Debian/Ubuntu

To run the installed application on Debian/Ubuntu the library "libxcb-cursor0" needs to be installed.

```
sudo apt-get libxcb-cursor0
```

### Windows

To build the application with Visual Studio on Windows the following line needs to be added to CMakeLists.txt:

```
set(CMAKE_PREFIX_PATH <path-to-Qt>/<Qt-Version>/msvc2019_64/lib/cmake)
```

After setting the Qt path you can open the project in Visual Studio as administrator, choose "TheoIDE.exe(install)" from the drop-down menu and start the building- and installationprocess. By default the application will be installed at:"c:/Program Files/TheoIDE". You can change the installation path in the CMakeLists.txt by adding:

```
set(CMAKE_INSTALL_PREFIX <\desired_path>)
```


### MacOS

If you want to build the application for Xcode you first need to create an Xcode project with CMake gui or on the commandline as follows:

```
cmake -S <project-directory> -B <build-directory> -GXcode -DCMAKE_PREFIX_PATH=<path-to-Qt>/<Qt-Version>/macos/lib/cmake

# or add: "set(CMAKE_PREFIX_PATH <path-to-Qt>/<Qt-Version>/macos/lib/cmake)" to the CMakeLists.txt file
```

After generating the Xcode project you can head to the build directory, open the Xcode project file and build and run the application within Xcode.
