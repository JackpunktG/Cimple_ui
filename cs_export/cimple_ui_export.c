#include "cimple_ui_export.h"
#include "../lib/Cimple_ui/Cimple_ui.h"

typedef struct
{
    WindowHolder* windowController;
    StringMemory* sm;
    FontHolder* fh;
    Arena* arena;
} CimpleUI;

EXPORT void CimpleUI_QuitSDL(CimpleUI_handle cimpleUI)
{
    destroy_fonts(((CimpleUI*)cimpleUI)->fh);
    string_memory_destroy(((CimpleUI*)cimpleUI)->sm);
    arena_destroy(((CimpleUI*)cimpleUI)->arena);
    quit_SDL2_ui();
}

/* Windows functions */

EXPORT CimpleUI_handle CimpleUI_Init(int8_t maxWindows, int8_t maxFonts)
{
    Arena* mainArena = arena_init(ARENA_BLOCK_SIZE /4, 8, false);
    CimpleUI* cimpleUI = arena_alloc(mainArena, sizeof(CimpleUI), false);
    cimpleUI->windowController = window_holder_init(mainArena, maxWindows);
    cimpleUI->sm = string_memory_init(mainArena);
    cimpleUI->fh = font_holder_init(mainArena, maxFonts);
    cimpleUI->arena = mainArena;

    return (CimpleUI_handle)cimpleUI;
}

EXPORT WindowController_handle CimpleUI_CreateWindowController(
    CimpleUI_handle cimpleUI,
    const char* title,
    uint32_t width,
    uint32_t height, uint16_t uiElemMax, uint32_t FLAGS)
{
    CimpleUI* ui = (CimpleUI*)cimpleUI;
    WindowHolder* windowController = ui->windowController;
    StringMemory* sm = ui->sm;
    FontHolder* fh = ui->fh;
    return (WindowController_handle)window_controller_init(windowController, sm, fh, title, width, height, uiElemMax, FLAGS);
}

EXPORT bool CimpleUI_MultiWindowEventCheck(CimpleUI_handle cimpleUI, uint16_t* windowID)
{
    CimpleUI* ui = (CimpleUI*)cimpleUI;
    WindowHolder* windowController = ui->windowController;

    SDL_Event e;
    bool windowChanged = false;
    bool firstChange = true;
    uint16_t localWindowID = 0;
    while (SDL_PollEvent(&e) != 0)
    {
        if(multi_window_event_check(windowController, &e, &localWindowID))
        {
            windowChanged = true;
            if (windowID != NULL && firstChange)
            {
                *windowID = localWindowID;
                firstChange = false;
            }
        }
    }
    return windowChanged;
}

EXPORT void CimpleUI_MultiWindowUIUpdate(CimpleUI_handle cimpleUI, float deltaTime)
{
    multi_window_ui_update(((CimpleUI*)cimpleUI)->windowController, deltaTime);
}

EXPORT void CimpleUI_MultiWindowRender(CimpleUI_handle cimpleUI, ColorRGBA color)
{
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    multi_window_render(((CimpleUI*)cimpleUI)->windowController, sdlColor);
}

EXPORT void CimpleUI_DestroyWindowController(CimpleUI_handle cimpleUI, WindowController_handle wc)
{
    destroy_window_controller(((CimpleUI*)cimpleUI)->windowController, (WindowController*)wc);
}

EXPORT uint32_t CimpleUI_GetWindowWidth(WindowController_handle windowController)
{
    return ((WindowController*)windowController)->window->width;
}

EXPORT uint32_t CimpleUI_GetWindowHeight(WindowController_handle windowController)
{
    return ((WindowController*)windowController)->window->height;
}


// Font holder
EXPORT void CimpleUI_LoadFont(CimpleUI_handle cimpleUI, const char* fileName, uint8_t fontSize)
{
    load_fonts(((CimpleUI*)cimpleUI)->fh, fileName, fontSize);
}

/* Label_handle */
EXPORT Label_handle CimpleUI_CreateLabel(
    WindowController_handle windowController, int x, int y, int width, int height, const char* text,
    uint8_t fontIndex, uint8_t fontSize, ColorRGBA color)
{
    WindowController* w = (WindowController*)windowController;
    return (Label_handle)label_basic_init(w->arena, w->uiController, x, y, width, height, text,
                                          w->fh->fonts[fontIndex], fontSize, (SDL_Color)
    {
        color.r, color.g, color.b, color.a
    }, w->window->renderer);
}

/* Image_handle */
EXPORT Image_handle CimpleUI_CreateImage(
    WindowController_handle windowController,
    const char* path,
    int x, int y, int width, int height)
{
    WindowController* w = (WindowController*)windowController;
    return (Image_handle)image_init(w, path, x, y, width, height);
}

EXPORT void CimpleUI_SetImageOpacity(Image_handle image, uint8_t opacity)
{
    image_opacity_set((Image*)image, opacity);
}
EXPORT void CimpleUI_ImageMove(Image_handle image, int x, int y)
{
    Image* img = (Image*) image;
    if (x > -5000)
        img->rect.x = x;
    if (y > -5000)
        img->rect.y = y;
}
EXPORT void CimpleUI_ImageRenew(WindowController_handle windowController, Image_handle image, const char* path, int x, int y, int w, int h)
{
    image_renew((WindowController*)windowController, (Image*)image, path, x, y, w, h);
}




/* TextBox */
EXPORT TextBox_handle CimpleUI_CreateTextBox(
    WindowController_handle windowController,
    uint8_t fontIndex,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height)
{
    WindowController* w = (WindowController*)windowController;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    return (TextBox_handle)textbox_init(
               w->arena,
               w->uiController,
               w->sm,
               w->fh->fonts[fontIndex],
               fontSize,
               sdlColor,
               x, y, width, height
           );
}

EXPORT void CimpleUI_TextBoxAppendText(WindowController_handle windowController, TextBox_handle textbox, const char* text)
{
    WindowController* w = (WindowController*)windowController;
    textbox_append_text(w->arena, w->sm, (TextBox*)textbox, text);
}

EXPORT void CimpleUI_TextBox_Clear(TextBox_handle textbox, CimpleUI_handle cimpleUI)
{
    CimpleUI* ui = (CimpleUI*)cimpleUI;
    TextBox* tb = (TextBox*)textbox;
    string_clear(tb->string, &ui->sm, ui->arena);
    tb->textChanged = true;
};

EXPORT void CimpleUI_TextBoxGetText(TextBox_handle textbox, char* dest)
{
    TextBox* tb = (TextBox*)textbox;
    c_string_sendback(tb->string, dest);
}

EXPORT uint32_t CimpleUI_TextBoxGetTextLength(TextBox_handle textbox)
{
    TextBox* tb = (TextBox*)textbox;
    return tb->string->count +1;
}

/* TextField */
EXPORT TextField_handle CimpleUI_CreateTextField(
    WindowController_handle windowController,
    uint8_t fontIndex,
    uint8_t fontSize,
    ColorRGBA color,
    float x, float y, float width, float height)
{
    WindowController* w = (WindowController*)windowController;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    return (TextField_handle)textfield_init(
               w->arena,
               w->uiController,
               w->sm,
               w->fh->fonts[fontIndex],
               fontSize,
               sdlColor,
               x, y, width, height
           );
}

EXPORT void CimpleUI_TextFieldAppendText(WindowController_handle windowController, TextField_handle textfield, const char* text)
{
    WindowController* w = (WindowController*)windowController;
    textfield_append_text(w->arena, w->sm, (TextField*)textfield, text);
}

EXPORT void CimpleUI_TextFieldGetText(TextField_handle textfield, char* dest)
{
    TextField* tf = (TextField*)textfield;
    c_string_sendback(tf->string, dest);
}
//adds 1 for null terminator
EXPORT uint32_t CimpleUI_TextFieldGetTextLength(TextField_handle textfield)
{
    TextField* tf = (TextField*)textfield;
    return tf->string->count +1;
}

EXPORT void CimpleUI_TextField_Clear(TextField_handle textfield, CimpleUI_handle cimpleUI)
{
    CimpleUI* ui = (CimpleUI*)cimpleUI;
    TextField* tf = (TextField*)textfield;
    StringMemory* stringmem = ui->sm;
    string_clear(tf->string, &stringmem, ui->arena);
    tf->textChanged = true;
};



/* Button functions */
EXPORT BasicButton_handle CimpleUI_CreateButton(
    WindowController_handle windowController,
    uint8_t fontIndex,
    uint8_t fontSize,
    int x, int y, int width, int height,
    const char* text,
    ColorRGBA color)
{
    WindowController* w = (WindowController*)windowController;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    return (BasicButton_handle)button_basic_init(
               w->arena,
               w->uiController,
               x, y, width, height,
               text,
               w->fh->fonts[fontIndex],
               fontSize,
               sdlColor,
               w->window->renderer
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

EXPORT void CimpleUI_ButtonAddClickListener(WindowController_handle windowController, BasicButton_handle button, ButtonClickCallback callback, void* userData)
{
    // Store the callback in a way that can be called from C#
    button_basic_add_listener(((WindowController*)windowController)->arena, (BasicButton*)button, button_callback_wrapper, (void*)callback);
}

EXPORT int CimpleUI_ButtonGetState(BasicButton_handle button)
{
    return ((BasicButton*)button)->state;
}


EXPORT TabPannel_handle CimpleUI_CreateTabPannel(
    WindowController_handle windowController,
    const char* tabNames,
    int tabPosition,
    uint8_t fontIndex,
    uint8_t fontSize,
    int height,
    ColorRGBA color, int elemPerTab)
{
    WindowController* w = (WindowController*)windowController;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};

    return (TabPannel_handle)tab_pannel_init(
               w->arena,
               w->uiController,
               w->window,
               tabNames,
               (enum TabPannelPossition)tabPosition,
               height,
               elemPerTab,
               w->fh->fonts[fontIndex],
               fontSize,
               sdlColor
           );
}

EXPORT void CimpleUI_AddElementToTabPannel(
    TabPannel_handle tabPannel,
    void* elem,
    int elemType,
    int tabIndex)
{
    add_elem_to_pannel(elem, elemType, (TabPannel*)tabPannel, tabIndex);
}


/* Drop down menu */
EXPORT DropdownMenu_handle CimpleUI_dropdown_menu_init(
    WindowController_handle windowController, uint8_t maxCount, const char* label, uint8_t fontIndex, uint8_t fontSize,
    int x, int y, int w, int h, ColorRGBA color)
{
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    WindowController* wC = (WindowController*)windowController;
    return (DropdownMenu_handle)dropdown_menu_init(
               wC->arena, wC->uiController,
               wC->window->renderer, maxCount, label, wC->fh->fonts[fontIndex], fontSize,
               x, y, w, h, sdlColor);
}

EXPORT void CimpleUI_dropdown_menu_populate(
    WindowController_handle windowController, DropdownMenu_handle ddm,
    const char* textString, uint8_t fontIndex, uint8_t fontSize, ColorRGBA color)
{
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    WindowController* w = (WindowController*)windowController;
    dropdown_menu_populate(w->arena, w->window->renderer, w->fh->fonts[fontIndex], (DropdownMenu*)ddm,
                           textString, fontSize, sdlColor);
}
static void dropdown_menu_callback_wrapper(const Event* ev, void* user_data)
{
    ButtonClickCallback callback = (ButtonClickCallback)user_data;
    if (callback)
    {
        callback(ev->sourceObj);
    }
}

EXPORT void CimpleUI_dropdown_menu_add_listener(
    WindowController_handle windowController, DropdownMenu_handle ddm, ButtonClickCallback callback,
    void* userData)
{
    WindowController* w = (WindowController*)windowController;
    dropdown_menu_add_listener(w->arena, (DropdownMenu*)ddm, button_callback_wrapper, (void*)callback);
}

EXPORT int CimpleUI_dropdown_menu_get_state(DropdownMenu_handle ddm)
{
    return ((DropdownMenu*)ddm)->state;
}

EXPORT int CimpleUI_dropdown_button_selected(DropdownMenu_handle ddm)
{
    return dropdown_button_selected((DropdownMenu*)ddm);
}
EXPORT void CimpleUI_reset_dropdown_menu(DropdownMenu_handle ddm)
{
    reset_dropdown_menu((DropdownMenu*)ddm);
}
EXPORT void CimpleUI_select_dropdown_menu_button(DropdownMenu_handle ddm, uint8_t buttonIndex)
{
    select_dropdown_menu_button((DropdownMenu*)ddm, buttonIndex);
}

/* Popup Notice */
EXPORT void CimpleUI_PopupNoticeInit(
    WindowController_handle windowController,
    const char* noticeText,
    const char* buttonText,
    uint8_t fontIndex,
    int width,
    int height,
    ColorRGBA color)
{
    WindowController* w = (WindowController*)windowController;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    popup_notice_init(w->uiController, noticeText, buttonText, w->fh->fonts[fontIndex], width, height, sdlColor);
}


