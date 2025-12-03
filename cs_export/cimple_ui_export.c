#include "cimple_ui_export.h"
#include "../lib/Cimple_ui/Cimple_ui.h"
#include <stdlib.h>
#include <string.h>


/* Windows functions */
EXPORT WindowUI_handle CimpleUI_InitWindow(Arena_handle arena, const char* title, uint32_t width, uint32_t height, bool vsync, bool fullscreen)
{
    Arena* arena_ptr = (Arena*)arena;
    return (WindowUI_handle)create_arena_window_ui(title, width, height, arena_ptr, vsync, fullscreen);
}

EXPORT void CimpleUI_DestroyWindow(WindowUI_handle window)
{
    if (window)
    {
        destroy_SDL2_ui((WindowUI*)window);
    }
}


EXPORT void CimpleUI_ClearScreen(WindowUI_handle window, ColorRGBA color)
{
    WindowUI* w = (WindowUI*)window;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    clear_screen_with_color(w->renderer, sdlColor);
}

EXPORT void CimpleUI_Present(WindowUI_handle window)
{
    WindowUI* w = (WindowUI*)window;
    SDL_RenderPresent(w->renderer);
}

EXPORT uint32_t CimpleUI_GetWindowWidth(WindowUI_handle window)
{
    return ((WindowUI*)window)->width;
}

EXPORT uint32_t CimpleUI_GetWindowHeight(WindowUI_handle window)
{
    return ((WindowUI*)window)->height;
}


/* Arena Functions */

EXPORT Arena_handle CimpleUI_CreateArena()
{
    return (Arena_handle)arena_init(1024 * 1024, 8, true);
}

EXPORT void CimpleUI_DestroyArena(Arena_handle arena)
{
    if (arena)
    {
        arena_destroy((Arena*)arena);
    }
}


/* String memory functions */
EXPORT StringMemory_handle CimpleUI_CreateStringMemory(Arena_handle arena, uint16_t maxStrings)
{
    return (StringMemory_handle)string_memory_init((Arena*)arena);
}


// Font holder
EXPORT FontHolder_handle CimpleUI_CreateFontHolder(Arena_handle arena, uint8_t maxFonts)
{
    return (FontHolder_handle)font_holder_init((Arena*)arena, maxFonts);
}

EXPORT void CimpleUI_LoadFont(FontHolder_handle fh, const char* fileName, uint8_t fontSize)
{
    load_fonts((FontHolder*)fh, fileName, fontSize);
}

EXPORT void CimpleUI_DestroyFonts(FontHolder_handle fh)
{
    destroy_fonts((FontHolder*)fh);
}


/* UI Controller */
EXPORT UIController_handle CimpleUI_CreateUIController(Arena_handle arena, uint16_t totalElements)
{
    return (UIController_handle)ui_controller_init((Arena*)arena, totalElements);
}

EXPORT void CimpleUI_UpdateUI(UIController_handle uiC, float deltaTime)
{
    ui_update((UIController*)uiC, deltaTime);
}

EXPORT void CimpleUI_RenderUI(WindowUI_handle window, UIController_handle uiC)
{
    WindowUI* w = (WindowUI*)window;
    ui_render(w->renderer, (UIController*)uiC);
}

EXPORT void CimpleUI_DestroyUIController(UIController_handle uiC)
{
    ui_controller_destroy((UIController*)uiC);
}

/* TextBox */
EXPORT TextBox_handle CimpleUI_CreateTextBox(
    Arena_handle arena,
    UIController_handle uiController,
    StringMemory_handle sm,
    uint8_t fontIndex,
    FontHolder_handle fh,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height)
{
    FontHolder* fontHolder = (FontHolder*)fh;
    StringMemory* stringMemory = (StringMemory*)sm;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    return (TextBox_handle)textbox_init(
               (Arena*)arena,
               (UIController*)uiController,
               stringMemory,
               fontHolder->fonts[fontIndex],
               fontSize,
               sdlColor,
               x, y, width, height
           );
}

EXPORT void CimpleUI_TextBoxAppendText(Arena_handle arena, StringMemory_handle sm, TextBox_handle textbox, const char* text)
{
    textbox_append_text((Arena*)arena, (StringMemory*)sm, (TextBox*)textbox, text);
}

EXPORT const char* CimpleUI_TextBoxGetText(TextBox_handle textbox)
{
    char *dest;
    TextBox* tb = (TextBox*)textbox;
    c_string_sendback(tb->string, dest);
    return dest;
}


/* Button functions */
EXPORT BasicButton_handle CimpleUI_CreateButton(
    Arena_handle arena,
    UIController_handle uiController,
    WindowUI_handle window,
    FontHolder_handle fh,
    uint8_t fontIndex,
    int x, int y, int width, int height,
    const char* text,
    ColorRGBA color)
{
    WindowUI* w = (WindowUI*)window;
    FontHolder* fontHolder = (FontHolder*)fh;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    return (BasicButton_handle)button_basic_init(
               (Arena*)arena,
               (UIController*)uiController,
               x, y, width, height,
               text,
               fontHolder->fonts[fontIndex],
               sdlColor,
               w->renderer
           );
}

// Wrapper for button callback
static void button_callback_wrapper(const Event* ev, void* user_data)
{
    ButtonClickCallback callback = (ButtonClickCallback)user_data;
    if (callback)
    {
        callback(ev->sourceObj);
    }
}

EXPORT void CimpleUI_ButtonAddClickListener(Arena_handle arena, BasicButton_handle button, ButtonClickCallback callback, void* userData)
{
    // Store the callback in a way that can be called from C#
    button_basic_add_listener((Arena*)arena, (BasicButton*)button, button_callback_wrapper, (void*)callback);
}

EXPORT int CimpleUI_ButtonGetState(BasicButton_handle button)
{
    return ((BasicButton*)button)->state;
}

/* UI-update function */
EXPORT bool ui_window_event_check(WindowUI_handle window, Arena_handle arena, StringMemory_handle sm, UIController_handle uiC)
{
    SDL_Event e;
    bool change = false;

    while(SDL_PollEvent(&e) != 0)
    {
        ui_event_check((Arena*)arena, (StringMemory*)sm, (UIController*)uiC, &e);

        if (windowUI_update((WindowUI*)window, &e) || windowUI_fullscreen((WindowUI*)window, &e))
            change = true;
    }
    return change;
}
