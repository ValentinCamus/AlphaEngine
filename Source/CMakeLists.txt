# Adds convenience methods, see CMake/cleanCppExtensions.CMake
include(cleanCppExtensions)

# Creates options to turn on sanitizers, see CMake/sanitizers.CMake
include(sanitizers)

# Enable helpfull warnings and C++14 for all files
if (MSVC)
    add_compile_options(/std:c++14 /W3 /WX)
else ()
    add_compile_options(-std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
endif ()

#------------------------------------------------------------------------------
# Libraries dependencies
find_package(OpenGL REQUIRED)

#-------------------------------------------------------------------------------
# AlphaEngine executable setup

set(ICON_RESOURCE "")
if (WIN32) #windows icon resource file, provided from /Packaging directory
    set(ICON_RESOURCE "${PROJECT_BINARY_DIR}/AlphaEngine.icon.rc")
elseif (APPLE) #osx icon ressource file, provided from /Packaging directory
    set(ICON_RESOURCE "${PROJECT_SOURCE_DIR}/Packaging/AlphaEngine.icns")
endif ()

set(IMGUI Alpha/ThirdParty/imgui/imstb_truetype.h
        Alpha/ThirdParty/imgui/imstb_textedit.h
        Alpha/ThirdParty/imgui/imstb_rectpack.h
        Alpha/ThirdParty/imgui/imgui_demo.cpp
        Alpha/ThirdParty/imgui/imgui_widgets.cpp
        Alpha/ThirdParty/imgui/imgui_internal.h
        Alpha/ThirdParty/imgui/imgui_draw.cpp
        Alpha/ThirdParty/imgui/imgui.h
        Alpha/ThirdParty/imgui/imgui.cpp
        Alpha/ThirdParty/imgui/imconfig.h)

set(GLAD Alpha/ThirdParty/glad/glad.cpp
        Alpha/ThirdParty/glad/glad.h
        Alpha/ThirdParty/glad/khrplatform.h)

set(THIRD_PARTY ${GLAD} ${IMGUI})

set(ENGINE_SRCS
        EntryPoint.cpp

        Alpha/Application/Application.cpp

        Alpha/Engine/EngineMinimal.cpp

        Alpha/Window/DefaultWindow/DefaultWindow.cpp
        Alpha/Input/DefaultInput/DefaultInput.cpp

        Alpha/Layer/LayerStack.cpp
        Alpha/Gui/ImGuiLayer.cpp

        Alpha/Gui/ImGuiDetails/ImGuiImplGLFW.cpp
        Alpha/Gui/ImGuiDetails/ImGuiImplOpenGL3.cpp

        Alpha/Engine/Renderer/Renderer.cpp
        Alpha/Engine/Renderer/Framebuffer.cpp
        Alpha/Engine/Renderer/Texture.cpp

        Alpha/Engine/Renderer/OpenGL/OpenGLRenderer.cpp
        Alpha/Engine/Renderer/OpenGL/OpenGLFramebuffer.cpp

        Alpha/Gui/BuildInWidgets/DockerWidget.cpp
        Alpha/Gui/BuildInWidgets/ViewportWidget.cpp)

add_executable(AlphaEngine
        MACOSX_BUNDLE
        ${ICON_RESOURCE}
        ${ENGINE_SRCS}
        ${THIRD_PARTY})


target_include_directories(AlphaEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "AlphaEngine")
set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.AlphaEngine.AlphaEngine")
set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_ICON_FILE AlphaEngine.icns)
set_target_properties(AlphaEngine PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/Packaging/MacOSXBundleInfo.plist.in)
set_source_files_properties(${PROJECT_SOURCE_DIR}/Packaging/AlphaEngine.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

install(TARGETS AlphaEngine BUNDLE DESTINATION "." RUNTIME DESTINATION bin)

# Using macro from CMake/CleanCpppExtensions.cpp included on the top
# Provides AlphaEngine-run and AlphaEngine-dbg
addRunAndDebugTargets(AlphaEngine)

# Short convenience target from CMake/CleanCpppExtensions.cpp
add_custom_target(run DEPENDS AlphaEngine-run)

#-------------------------------------------------------------------------------
# Target link libraries

target_link_libraries(AlphaEngine spdlog)
#target_link_libraries(AlphaEngine glm)
#target_link_libraries(AlphaEngine assimp)
#target_link_libraries(AlphaEngine imgui)
target_link_libraries(AlphaEngine glfw)

target_link_libraries(AlphaEngine ${OPENGL_gl_LIBRARY})
target_link_libraries(AlphaEngine ${OPENGL_glu_LIBRARY})
target_link_libraries(AlphaEngine ${OPENGL_LIBRARIES})

#-------------------------------------------------------------------------------
# Copy MINGW needed libraries for building on windows

if (MINGW)
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


