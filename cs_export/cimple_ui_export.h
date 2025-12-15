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
typedef struct CimpleUI_Opaque* CimpleUI_handle;
typedef struct WindowController_Opaque* WindowController_handle;

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
EXPORT void CimpleUI_QuitSDL(CimpleUI_handle cimpleUI);

//Start up functions
EXPORT CimpleUI_handle CimpleUI_Init(int8_t maxWindows, int8_t maxFonts);

// Multi-window supported display helpers
EXPORT WindowController_handle CimpleUI_CreateWindowController(
    CimpleUI_handle cimpleUI,
    const char* title,
    uint32_t width,
    uint32_t height,
    uint16_t uiElemMax);
EXPORT bool CimpleUI_MultiWindowEventCheck(CimpleUI_handle cimpleUI, uint16_t* windowID);
EXPORT void CimpleUI_MultiWindowUIUpdate(CimpleUI_handle cimpleUI, float deltaTime);
EXPORT void CimpleUI_MultiWindowRender(CimpleUI_handle cimpleUI, ColorRGBA color);
EXPORT void CimpleUI_DestroyWindowController(CimpleUI_handle cimpleUI, WindowController_handle wc);
EXPORT uint32_t CimpleUI_GetWindowWidth(WindowController_handle windowController);
EXPORT uint32_t CimpleUI_GetWindowHeight(WindowController_handle windowController);

// Load font
EXPORT void CimpleUI_LoadFont(CimpleUI_handle cimpleUI, const char* fileName, uint8_t fontSize);

// Label_handle
EXPORT Label_handle CimpleUI_CreateLabel(WindowController_handle windowController, int x, int y, int width, int height, const char* text, uint8_t fontIndex, uint8_t fontSize, ColorRGBA color);

// TextBox_handle
EXPORT TextBox_handle CimpleUI_CreateTextBox(
    WindowController_handle windowController,
    uint8_t fontIndex,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height);
EXPORT void CimpleUI_TextBoxAppendText(WindowController_handle windowController, TextBox_handle textbox, const char* text);
EXPORT void CimpleUI_TextBoxGetText(TextBox_handle textbox, char* dest);
//adds 1 for null terminator
EXPORT uint32_t CimpleUI_TextBoxGetTextLength(TextBox_handle textbox);
EXPORT void CimpleUI_TextBox_Clear(TextBox_handle textbox, CimpleUI_handle cimpleUI);

// TextField_handle
EXPORT TextField_handle CimpleUI_CreateTextField(
    WindowController_handle windowController,
    uint8_t fontIndex,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height);
EXPORT void CimpleUI_TextFieldAppendText(WindowController_handle windowController, TextField_handle textfield, const char* text);
EXPORT void CimpleUI_TextFieldGetText(TextField_handle textfield, char* dest);
//adds 1 for null terminator
EXPORT uint32_t CimpleUI_TextFieldGetTextLength(TextField_handle textfield);
EXPORT void CimpleUI_TextField_Clear(TextField_handle textfield, CimpleUI_handle cimpleUI);

// Button
EXPORT BasicButton_handle CimpleUI_CreateButton(
    WindowController_handle windowController,
    uint8_t fontIndex,
    uint8_t fontSize,
    int x, int y, int width, int height,
    const char* text,
    ColorRGBA color);
EXPORT void CimpleUI_ButtonAddClickListener(WindowController_handle windowController, BasicButton_handle button, ButtonClickCallback callback, void* userData);
EXPORT int CimpleUI_ButtonGetState(BasicButton_handle button);

//Tab Pannel
EXPORT TabPannel_handle CimpleUI_CreateTabPannel(
    WindowController_handle windowController,
    const char* tabNames,
    int tabPosition,
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
    WindowController_handle windowController,
    const char* noticeText,
    const char* buttonText,
    uint8_t fontIndex,
    int width,
    int height,
    ColorRGBA color);

/* Drop down menu */
EXPORT DropdownMenu_handle CimpleUI_dropdown_menu_init(
    WindowController_handle windowController, uint8_t maxCount, const char* label,
    uint8_t fontIndex, uint8_t fontSize,
    int x, int y, int w, int h, ColorRGBA color);

EXPORT void CimpleUI_dropdown_menu_populate(
    WindowController_handle windowController, DropdownMenu_handle ddm,
    const char* textString, uint8_t fontIndex, uint8_t fontSize, ColorRGBA color);

EXPORT void CimpleUI_dropdown_menu_add_listener(
    WindowController_handle windowController, DropdownMenu_handle ddm, ButtonClickCallback callback,
    void* userData);

EXPORT int CimpleUI_dropdown_menu_get_state(DropdownMenu_handle ddm);
EXPORT int CimpleUI_dropdown_button_selected(DropdownMenu_handle ddm);
EXPORT void CimpleUI_select_dropdown_menu_button(DropdownMenu_handle ddm, uint8_t buttonIndex);
EXPORT void CimpleUI_reset_dropdown_menu(DropdownMenu_handle ddm);




#ifdef __cplusplus
}
#endif

#endif // CIMPLE_UI_EXPORT_H
