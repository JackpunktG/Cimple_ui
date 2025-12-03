#ifndef CIMPLE_UI_EXPORT_H
#define CIMPLE_UI_EXPORT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif


// Opaque pointers for C#
typedef struct Arena_Opaque* Arena_handle;

typedef struct FontHolder_Opaque* FontHolder_handle;
typedef struct StringMemory_Opaque* StringMemory_handle;

typedef struct WindowUI_Opaque* WindowUI_handle;
typedef struct UIController_Opaque* UIController_handle;
typedef struct TextBox_Opaque* TextBox_handle;
typedef struct BasicButton_Opaque* BasicButton_handle;

// Color struct
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} ColorRGBA;

// Callback type for button events
typedef void (*ButtonClickCallback)(void* userData);

// Window functions
EXPORT WindowUI_handle CimpleUI_InitWindow(Arena_handle arena, const char* title, uint32_t width, uint32_t height, bool vsync, bool fullscreen);
EXPORT void CimpleUI_Destroy_Window(WindowUI_handle window);
EXPORT void CimpleUI_Clear_Screen(WindowUI_handle window, ColorRGBA color);
EXPORT void CimpleUI_Present_Render(WindowUI_handle window);
EXPORT uint32_t CimpleUI_GetWindowWidth(WindowUI_handle window);
EXPORT uint32_t CimpleUI_GetWindowHeight(WindowUI_handle window);

// Arena_handle memory
EXPORT Arena_handle CimpleUI_CreateArena();
EXPORT void CimpleUI_DestroyArena(Arena_handle arena);

// String memory
EXPORT StringMemory_handle CimpleUI_CreateStringMemory(Arena_handle arena, uint16_t maxStrings);

// Font holder
EXPORT FontHolder_handle CimpleUI_CreateFontHolder(Arena_handle arena, uint8_t maxFonts);
EXPORT void CimpleUI_LoadFont(FontHolder_handle fh, const char* fileName, uint8_t fontSize);
EXPORT void CimpleUI_DestroyFonts(FontHolder_handle fh);

// UI Controller
EXPORT UIController_handle CimpleUI_CreateUIController(Arena_handle arena, uint16_t totalElements);
EXPORT void CimpleUI_UpdateUI(UIController_handle uiC, float deltaTime);
EXPORT void CimpleUI_RenderUI(WindowUI_handle window, UIController_handle uiC);
EXPORT void CimpleUI_DestroyUIController(UIController_handle uiC);

// SDL_event check - returns true on windows size update
EXPORT bool ui_window_event_check(WindowUI_handle window, Arena_handle arena, StringMemory_handle sm, UIController_handle uiC);

// TextBox_handle
EXPORT TextBox_handle CimpleUI_CreateTextBox(
    Arena_handle arena,
    UIController_handle uiController,
    StringMemory_handle sm,
    uint8_t fontIndex,
    FontHolder_handle fh,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height);
EXPORT void CimpleUI_TextBoxAppendText(Arena_handle arena, StringMemory_handle sm, TextBox_handle textbox, const char* text);
EXPORT const char* CimpleUI_TextBoxGetText(TextBox_handle textbox);

// Button
EXPORT BasicButton_handle CimpleUI_CreateButton(
    Arena_handle arena,
    UIController_handle uiController,
    WindowUI_handle window,
    FontHolder_handle fh,
    uint8_t fontIndex,
    int x, int y, int width, int height,
    const char* text,
    ColorRGBA color);
EXPORT void CimpleUI_ButtonAddClickListener(Arena_handle arena, BasicButton_handle button, ButtonClickCallback callback, void* userData);
EXPORT int CimpleUI_ButtonGetState(BasicButton_handle button);

#ifdef __cplusplus
}
#endif

#endif // CIMPLE_UI_EXPORT_H
