# Cimple_ui v.0.1

Cimple_ui is a lightweight and easy-to-use GUI library for C.

Combining my strings, arena memory libraries, and SDL2, Cimple_ui provides a simple way to create graphical user interfaces in C.

The first basic draft has been completed, and the library is now functional. Future updates will focus on adding more features and improving usability. 

## Features 
- Simple and intuitive API for creating GUI elements
- Support for basic GUI components like buttons, labels, and text fields
- Easy integration with existing C projects
- Cross-platform support using SDL2
- Built-in memory management using arena allocation

## Getting Started
To get started with Cimple_ui, yoink the lib directory from the repository and include the necessary header files in your project. Other than SDL2, there are no additional dependencies. 

```c
#include "lib/Cimple_ui/Cimple_ui.h"
```     

Make sure to link against the SDL2 library when compiling your project.

## UI Elements
Cimple_ui currently supports the following UI elements:
- Button
- Label
- Text Field
- Text box
- Tabs  
- Dropdown Menu 
- Pop-up Noftifications

## Memory Management
Cimple_ui uses arena memory allocation for efficient memory management and ease of use.
Most of the destruction of UI elements is handled automatically for the UIController.

#### Example Usage 
Best is to have one arena for the font holder and string memory that last the whole program, and one arena and UIController per window.  

```c
    // main arena for fonts and strings
    Arena* mainArena = arena_init(456, 8, false);
    StringMemory* string_memory = string_memory_init(mainArena);
    FontHolder* fh = font_holder_init(mainArena, 1);
    load_fonts(fh, text, DEFAULT_FONT_SIZE);

```c
    Arena* arena = arena_init(ARENA_BLOCK_SIZE, 8, false);
    WindowUI windowUI;
    if (!init_SDL2_ui(&windowUI, "SDL2 UI Example", 800, 800, true, false))
    UIController* UIController = ui_controller_init(arena, 16);
````    

```c
    // to destroy the window arean, window and all its elements
    destroy_window(&windowUI, window_arena, string_memory, UIController);
    // to destroy the main arena, fonts and strings
    destroy_sdl2_ui_complete(string_memory, fh, mainArena); 
````    

*Note: Arena free list is under development, so parse false in the arena_init function*
-> Thinking it might actually be better without, as we now only have arenas grouped by lifetimes or request size.

Element examples and functional example can be seen in the main file in the repository.    

## Still under Construction

enjoy :)
