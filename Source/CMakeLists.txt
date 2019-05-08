# Adds convenience methods, see CMake/cleanCppExtensions.CMake
include(cleanCppExtensions)

# Creates options to turn on sanitizers, see CMake/sanitizers.CMake
include(sanitizers)

# Enable helpfull warnings and C++14 for all files
if(MSVC)
    add_compile_options(/std:c++14 /W3 /WX )
else()
    add_compile_options(-std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
endif()

# configure version.cpp.in with selected version
configure_file (
  "Version.cpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
  )
# If you want to have date and time of the build, your targets has to depend on this.
# This will force recompilation of version.o and thus forcing gcc to update __DATE__ macro.
add_custom_target(versionFileTouchForRebuild
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")

#------------------------------------------------------------------------------
# Configure QT

# Homebrew installs Qt5 (up to at least 5.9.1) in
# /usr/local/qt5, ensure it can be found by CMake since
# it is not in the default /usr/local prefix.
if(APPLE AND EXISTS /usr/local/opt/qt5)
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${Qt5Widgets_INCLUDES} ${Qt5OpenGL_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

#------------------------------------------------------------------------------
# Libraries dependencies
find_package (Threads REQUIRED) #threading

find_package(OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

#------------------------------------------------------------------------------
# By using macro to add common dependencies you can avoid repetition when you have
# multiple binaries.
# Use with `addCommonDependencies(AlphaEngine)`
# macro(addCommonDependencies TARGET)
#     add_dependencies(${TARGET} versionFileTouchForRebuild) # We want precise time of build in version
#     target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#     target_link_libraries (${TARGET} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
#     target_compile_definitions(${TARGET} PRIVATE -DSOME_DEFINE)
# endmacro()

#-------------------------------------------------------------------------------
# AlphaEngine executable setup

set(ICON_RESOURCE "")
if(WIN32) #windows icon resource file, provided from /Packaging directory
    set(ICON_RESOURCE "${PROJECT_BINARY_DIR}/AlphaEngine.icon.rc")
elseif (APPLE) #osx icon ressource file, provided from /Packaging directory
    set(ICON_RESOURCE "${PROJECT_SOURCE_DIR}/Packaging/AlphaEngine.icns")
endif()

FILE(GLOB_RECURSE UIS ${CMAKE_SOURCE_DIR}/Sources/Qt/*.ui)

add_executable(AlphaEngine MACOSX_BUNDLE
    ${ICON_RESOURCE}
    ${UIS}
    ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
        Application.cpp
        Version.cpp
        Qt/Window.cpp
        Qt/GLWidget.cpp)
add_dependencies(AlphaEngine versionFileTouchForRebuild) # We want precise time of build in version
target_include_directories(AlphaEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
    set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "AlphaEngine")
    set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.AlphaEngine.AlphaEngine")
    set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_ICON_FILE AlphaEngine.icns)
    set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/Packaging/MacOSXBundleInfo.plist.in)
    set_source_files_properties(${PROJECT_SOURCE_DIR}/Packaging/AlphaEngine.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

install(TARGETS AlphaEngine
    BUNDLE DESTINATION "."
    RUNTIME DESTINATION bin)

# Using macro from CMake/cleanCpppExtensions.cpp included on the top
# Provides AlphaEngine-run and AlphaEngine-dbg
addRunAndDebugTargets(AlphaEngine)

# short convenience target from CMake/cleanCpppExtensions.cpp
add_custom_target(run DEPENDS AlphaEngine-run)

# Manually specified convenience target for running the binary, we use macro
# add_custom_target(AlphaEngine-run
#     WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
#     USES_TERMINAL
#     COMMAND ./AlphaEngine
#     DEPENDS Install_)

# And this would add -DBAR_ENABLE_FEATURE or equivalent to build command
# target_compile_definitions(AlphaEngine PRIVATE BAR_ENABLE_FEATURE)

# Add compile time definition based on option
# option(OPTION_NAME "description for end user" OFF)
# if(OPTION_NAME)
#     # you can do anything here, eg. setup new optional executable
#     target_compile_definitions(AlphaEngine PRIVATE DEFINE_NAME)
# endif()

# Add building new shared library
# add_library(exampleLibrary SHARED ${LIBRARY_SOURCES})
# target_link_libraries (exampleLibrary ${CMAKE_THREAD_LIBS_INIT})
# set_property(TARGET exampleLibrary PROPERTY CXX_STANDARD 14) # we want C++14

# How to include and link OpenGL and GLUT libraries
# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
# target_include_directories(AlphaEngine SYSTEM ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )
# target_link_libraries(AlphaEngine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# How to use some other standard packages
# find_package(ZLIB REQUIRED) 
# target_link_libraries (AlphaEngine PRIVATE ZLIB::ZLIB)
# 
# find_package(OpenSSL REQUIRED)
# target_link_libraries (AlphaEngine PRIVATE OpenSSL::SSL OpenSSL::Crypto)
#
# find_package(CURL REQUIRED)
# target_link_libraries (AlphaEngine PRIVATE CURL::libcurl)

qt5_use_modules(AlphaEngine Widgets OpenGL)

target_link_libraries(AlphaEngine Threads::Threads)
target_link_libraries(AlphaEngine spdlog)
target_link_libraries(AlphaEngine ${OPENGL_gl_LIBRARY})
target_link_libraries(AlphaEngine ${OPENGL_glu_LIBRARY})
target_link_libraries(AlphaEngine ${QT_LIBRARIES})
target_link_libraries(AlphaEngine ${OPENGL_LIBRARIES})

#-------------------------------------------------------------------------------
# Copy MINGW needed libraries for building on windows

if(MINGW)
    message(STATUS "MinGW detected")
    get_filename_component(GCC_PATH ${CMAKE_C_COMPILER} PATH)
    if(${GCC_PATH} MATCHES "mingw64/bin")
        set(libgcc "libgcc_s_seh-1.dll") #64bit
        message(STATUS "  64bit dlls is building")
    else()
        set(libgcc "libgcc_s_dw2-1.dll") #32bit
        message(STATUS "  32bit dlls is building")
    endif()

    install(FILES ${GCC_PATH}/${libgcc}
        ${GCC_PATH}/libwinpthread-1.dll
        ${GCC_PATH}/libstdc++-6.dll
        DESTINATION ./bin/
    )
endif(MINGW)


#------------------------------------------------------------------------------
# Other MISC targets

# Adds misc targets: format, cppcheck, tidy, see CMake/cleanCppExtensions.CMake
addMiscTargets()


