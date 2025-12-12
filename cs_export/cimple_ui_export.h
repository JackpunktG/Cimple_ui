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
typedef struct TextField_Opaque* TextField_handle;
typedef struct BasicButton_Opaque* BasicButton_handle;
typedef struct Label_Opaque* Label_handle;
typedef struct TabPannel_Opaque* TabPannel_handle;
typedef struct DropdownMenu_Opaque* DropdownMenu_handle;

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

// SDL Quit - init is handled in first window creation
EXPORT void CimpleUI_QuitSDL();

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
EXPORT void CimpleUI_DestroyStringMemory(StringMemory_handle sm);

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
EXPORT bool CimpleUI_event_check(WindowUI_handle window, Arena_handle arena, StringMemory_handle sm, UIController_handle uiC);

// Label_handle
EXPORT Label_handle CimpleUI_CreateLabel(WindowUI_handle window, Arena_handle arena, UIController_handle uiController, int x, int y, int width, int height, const char* text, FontHolder_handle fh, uint8_t fontIndex, uint8_t fontSize, ColorRGBA color);

// TextBox_handle
EXPORT TextBox_handle CimpleUI_CreateTextBox(
    Arena_handle arena,
    UIController_handle uiController,
    StringMemory_handle sm,
    FontHolder_handle fh,
    uint8_t fontIndex,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height);
EXPORT void CimpleUI_TextBoxAppendText(Arena_handle arena, StringMemory_handle sm, TextBox_handle textbox, const char* text);
EXPORT void CimpleUI_TextBoxGetText(TextBox_handle textbox, char* dest);
//adds 1 for null terminator
EXPORT uint32_t CimpleUI_TextBoxGetTextLength(TextBox_handle textbox);
EXPORT void CimpleUI_TextBox_Clear(TextBox_handle textbox, StringMemory_handle sm, Arena_handle arena);

// TextField_handle
EXPORT TextField_handle CimpleUI_CreateTextField(
    Arena_handle arena,
    UIController_handle uiController,
    StringMemory_handle sm,
    FontHolder_handle fh,
    uint8_t fontIndex,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height);
EXPORT void CimpleUI_TextFieldAppendText(Arena_handle arena, StringMemory_handle sm, TextField_handle textfield, const char* text);
EXPORT void CimpleUI_TextFieldGetText(TextField_handle textfield, char* dest);
//adds 1 for null terminator
EXPORT uint32_t CimpleUI_TextFieldGetTextLength(TextField_handle textfield);
EXPORT void CimpleUI_TextField_Clear(TextField_handle textfield, StringMemory_handle sm, Arena_handle arena);

// Button
EXPORT BasicButton_handle CimpleUI_CreateButton(
    Arena_handle arena,
    UIController_handle uiController,
    WindowUI_handle window,
    FontHolder_handle fh,
    uint8_t fontIndex,
    uint8_t fontSize,
    int x, int y, int width, int height,
    const char* text,
    ColorRGBA color);
EXPORT void CimpleUI_ButtonAddClickListener(Arena_handle arena, BasicButton_handle button, ButtonClickCallback callback, void* userData);
EXPORT int CimpleUI_ButtonGetState(BasicButton_handle button);

//Tab Pannel
EXPORT TabPannel_handle CimpleUI_CreateTabPannel(
    Arena_handle arena,
    UIController_handle uiController,
    WindowUI_handle window,
    const char* tabNames,
    int tabPosition,
    FontHolder_handle fh,
    uint8_t fontIndex,
    uint8_t fontSize,
    int height,
    ColorRGBA color, int elemPerTab);

EXPORT void CimpleUI_AddElementToTabPannel(
    TabPannel_handle tabPannel,
    void* elem,
    int elemType,
    int tabIndex);

EXPORT void CimpleUI_PopupNoticeInit(
    UIController_handle uiController,
    const char* noticeText,
    const char* buttonText,
    FontHolder_handle fh,
    uint8_t fontIndex,
    int width,
    int height,
    ColorRGBA color);

/* Drop down menu */
EXPORT DropdownMenu_handle CimpleUI_dropdown_menu_init(
    Arena_handle arena, UIController_handle uiController,
    WindowUI_handle window, uint8_t maxCount, const char* label,
    FontHolder_handle fh, uint8_t fontIndex, uint8_t fontSize,
    int x, int y, int w, int h, ColorRGBA color);

EXPORT void CimpleUI_dropdown_menu_populate(
    Arena_handle arena, WindowUI_handle window, DropdownMenu_handle ddm,
    const char* textString, FontHolder_handle fh, uint8_t fontIndex, uint8_t fontSize, ColorRGBA color);

EXPORT void CimpleUI_dropdown_menu_add_listener(
    Arena_handle arena, DropdownMenu_handle ddm, ButtonClickCallback callback,
    void* userData);

EXPORT int CimpleUI_dropdown_menu_get_state(DropdownMenu_handle ddm);
EXPORT int CimpleUI_dropdown_button_selected(DropdownMenu_handle ddm);
EXPORT void CimpleUI_select_dropdown_menu_button(DropdownMenu_handle ddm, uint8_t buttonIndex);
EXPORT void CimpleUI_reset_dropdown_menu(DropdownMenu_handle ddm);


#ifdef __cplusplus
}
#endif

#endif // CIMPLE_UI_EXPORT_H
