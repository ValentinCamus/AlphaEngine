09/15/19
* Add GL_CHECK to handle OpenGL error.
* Add Static Mesh -> Mesh + Material
* Add Static Mesh Model -> Collection of Static Mesh
* Add Static Mesh Entity -> Instance of Static Mesh Model

09/13/19
* Add Shader abstract class + OpenGL implementation.
* Add Material class from v2.
* Add an image loader.
* Add Static Mesh abstract class + OpenGL implementation.

09/12/19
* Add stb library.
* Add Texture2D OpenGL implementation.
* Minor fixes.
* MacOS compilation.

09/11/19:
* Add BuildInWidgets based on ImGui
    > Docker
    > Viewport:
        Resizable widget based on a framebuffer.
* Refactoring classes/functions/names.
* Add Framebuffer + OpenGL implementation.
* Add Texture2D abstract class.
* Add Light (spot, directional, point) from v-2.
* Add Collection system.


09/10/19:
* Add Changelog.txt
* AlphaEngine Version 3
* Abstract renderer: The renderer can now support different rendering API.
    note: Only OpenGL the moment.
* New application system with the notion of layer.
* The engine uses now a system of layer:
    Each layers receives now all the events by the function OnEvent and the tick event by OnUpdate.
    This allows to render multiple scene in the same application.
        > Layer 00 -> Scene 0
        > Layer 01 -> Scene 1
        > ...
        > Layer N  -> Gui 0
* New event system:
    The engine uses now a event system based on an argument:
        EventA(EventType e): where <e> defines the type of the event.
        Example:
           OnWindowClosed(WindowCloseEvent e) -> called when a window is closed.
           OnKeyPressed(KeyPressEvent e) -> called when a key is pressed.
        The argument can also stores variables: uint32 key = e.GetKeyCode()
* Input system:
    The engine allows now to the user to check the state (IsPressed, IsReleased, ...etc)
    of an input (i.e mouse button, key, ...).
* New Window system:
    Abstract window system: The window system can now support different API.
    For the moment the engine is based on the cross-platform and w/ no dependency GLFW library.
* Gui: New GUI system based on ImGui and the layer system.
    This feature implements Docking!
    ImGui is now based on GLFW and OpenGL3 (this can depends on the renderer API).