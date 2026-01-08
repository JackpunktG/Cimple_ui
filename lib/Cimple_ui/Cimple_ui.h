#ifndef Cimple_ui_H
#define Cimple_ui_H
#include "../arena_memory/arena_memory.h"
#include "../string_lib/string.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>


#define DEFAULT_FONT_SIZE 24

//Colours
typedef enum
{
    BLACK,
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    GRAY,
    DARK_GRAY,
    LIGHT_GRAY,
    BROWN,
    ORANGE,
    PINK,
    PURPLE,
    LIME,
    NAVY,
    TEAL,
    OLIVE,
    MAROON,
    TOTAL
} ColorEnum;
// Can input COLOR[*] to get SDL_Color
extern const SDL_Color COLOR[TOTAL];

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    uint32_t width;
    uint32_t height;
    bool fullscreen;
    bool mouseFocus;
    bool mkeyboardFocus;
    bool minimized;
} WindowUI;

#define CIMPLE_WINDOW_VSYNC (1 << 1)
#define CIMPLE_WINDOW_FULLSCREEN (1 << 2)
#define CIMPLE_WINDOW_RESIZABLE (1 << 3)

// Windows Initialization Function. Also automatically initializes SDL2, SDL2_image, and SDL2_ttf
bool init_SDL2_ui(WindowUI* window, const char* title, uint32_t width, uint32_t height, uint32_t FLAGS);
// quits SDL2 subsystems
void quit_SDL2_ui();
void destroy_window_ui(WindowUI* window);
// Updates WindowUI state based on SDL_Event input - returns true if screensize changed
bool windowUI_update(WindowUI* window, SDL_Event* e);
// Updates window fullscreen or not - returns true if changed
bool windowUI_fullscreen(WindowUI* windowUI, SDL_Event* e);
// true if window keyboard or mouse focus
bool windowUI_has_focus(WindowUI* windowUI);
// Clears the screen with the given colour
void clear_screen_with_color(SDL_Renderer* renderer, SDL_Color colour);
/* Area memory window returns for pop-ups etc */
WindowUI* create_arena_window_ui(const char* title, uint32_t width, uint32_t height, Arena* arena, uint32_t FLAGS);



typedef struct
{
    SDL_Texture* mTexture;
    uint32_t width;
    uint32_t height;
} Texture;


/* Font Holder */
typedef struct
{
    TTF_Font** fonts;
    uint8_t count;
    uint8_t maxCount;
} FontHolder;

FontHolder* font_holder_init(Arena* arena, uint8_t maxFonts);
void load_fonts(FontHolder* fh, const char* fileName, uint8_t fontSize);
void destroy_fonts(FontHolder* fh);

/* UI-Element Controller */

typedef enum
{
    NULL_ELEM,
    TEXTBOX_ELEM,
    TEXTFIELD_ELEM,
    BUTTON_BASIC_ELEM,
    POPUP_NOTICE_ELEM,
    LABEL_ELEM,
    TABPANNEL_ELEM,
    DROPDOWN_MENU_ELEM,
    IMAGE_ELEM
} UI_Element;

typedef struct
{
    void** element;
    UI_Element* type;
    uint16_t count;
    uint16_t totalElements;
    float secondCounter;
    void* elemFocus;
    UI_Element focusedType;
    /* XXX 4-byte padding */
} UIController;

//Initializes a screens UIController with the number of elements per screen for easy freeing after screen is done
UIController* ui_controller_init(Arena* arena, uint16_t totalElements);
// checks UI elements to see if it is being interacted with and controller appending text etc
void ui_event_check(Arena* arena, StringMemory* sm, UIController* uiController, WindowUI* window, SDL_Event* e);
void ui_update(UIController* uiC, float deltaTime); // Updates UI elements
void ui_render(SDL_Renderer* renderer, UIController* uiC);
// Destroys all the textures in the UIController
void ui_controller_destroy(UIController* uiC);


/* Label Element */
typedef struct
{
    SDL_FRect rect;
    Texture* text;
    bool hidden;
} Label;
// Initializes a basic label with position and size
//if fontSize is 0, uses font's max font size that fits in box
Label* label_basic_init(Arena* arena, UIController* uiController, int x, int y, int width, int hight, const char* text, TTF_Font* font, uint8_t fontSize, SDL_Color color, SDL_Renderer* renderer);
void label_basic_render(SDL_Renderer* renderer, Label* label);
//destroy of texture handled in ui_controller_destroy



/* Event Emitter */

typedef enum
{
    EVENT_TYPE_CLICK,
} EventType;

typedef struct
{
    EventType type;
    /* 4-byte hole */
    void* sourceObj; // pointer to the object Emmitter
} Event;

/* Callback signature: event pointer + user_data pointer. */
typedef void (*EventCallback)(const Event* ev, void* user_data);

typedef struct
{
    EventType type;
    /* 4-byte hole */
    EventCallback cb;
    void* userData;
} EventNode;

typedef struct
{
    EventNode** listeners;
    uint8_t count;
    uint8_t maxCount;
    /*6-byte padding */
} EventEmitter;

/* Create/destroy emitter */
EventEmitter* event_emitter_create(Arena* arena);
void event_emitter_destroy(EventEmitter* em);

/* Register/unregister listeners. Returns >0 id on success, 0 on failure. */
int event_emitter_add_listener(Arena* arena, void* uiElement, UI_Element type, EventCallback cb, void* userData);
void event_emitter_remove_listener(EventEmitter* em, int listener_id);

/* Emit an event to all listeners (synchronous). */
void event_emitter_emit(EventEmitter* em, const Event* ev);


/* TextBox */

typedef struct
{
    SDL_FRect rect;
    TTF_Font* font;
    Texture* texture;
    String* string;
    SDL_Color color;
    uint8_t fontSize;
    bool focused;
    bool textChanged;
    bool hidden;
    /* XXX 7-byte padding */ // do we really need the hidden??
} TextBox;

// Initializes a TextBox structure give width and hight as funtions of screen dimensions
TextBox* textbox_init(Arena* arena, UIController* uiController, StringMemory* sm, TTF_Font* font, uint8_t fontSize, SDL_Color color, float x, float y, float width, float height);
// Renders the TextBox to the given renderer
void textbox_render(SDL_Renderer* renderer, TextBox* textbox);
// Appends c style string text to the TextBox's string
void textbox_append_text(Arena* arena, StringMemory* sm, TextBox* textbox, const char* text);


/* TextField */

typedef struct
{
    SDL_FRect rect;
    TTF_Font* font;
    Texture* texture;
    String* string;
    SDL_Color color;
    uint8_t fontSize;
    bool textChanged;
    bool hidden;
} TextField;        //Textbox but can't be interacted with

// Initializes a TextField structure give width and hight as funtions of screen dimensions
TextField* textfield_init(Arena* arena, UIController* uiController, StringMemory* sm, TTF_Font* font, uint8_t fontSize, SDL_Color color, float x, float y, float width, float height);
// Renders the TextField to the given renderer
void textfield_render(SDL_Renderer* renderer, TextField* textfield);
// Appends c style string text to the TextField's string
void textfield_append_text(Arena* arena, StringMemory* sm, TextField* textfield, const char* text);


/* Button */
enum ButtonState
{
    BUTTON_STATE_NORMAL,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_HOVERED,
    BUTTON_STATE_RELEASED,
    BUTTON_TAB,
    BUTTON_TAB_SELECTED,
    BUTTON_STATE_TOTAL
};

typedef struct
{
    SDL_Rect rect;
    Texture* text;
    EventEmitter* eventEmitter; // gets one event - on click
    SDL_Color color;
    float cooldownTimer;
    enum ButtonState state;
    bool hidden;
} BasicButton;
// Initializes a basic button with position and size
// if fontSize is 0, uses font's max font size that fits in box
BasicButton* button_basic_init(Arena* arena, UIController* uiController, int x, int y, int width, int height, const char* text, TTF_Font* font, uint8_t fontSize, SDL_Color color, SDL_Renderer* renderer);
void button_basic_render(SDL_Renderer* renderer, BasicButton* button);
//destruction of texture handled in ui_controller_destroy
int button_basic_add_listener(Arena* arena, BasicButton* bb, EventCallback cb, void* userData);


/* Dropdown Menu */
enum Dropdown_State
{
    DROPDOWN_NORMAL,
    DROPDOWN_EXPANDED,
    DROPDOWN_SELECTED
};

typedef struct
{
    SDL_Rect rect;
    Texture* text;
    enum ButtonState state;
} DropdownButton;

typedef struct
{
    DropdownButton** buttons;
    SDL_Rect rect;
    Texture* text;
    enum Dropdown_State state;
    uint8_t count;
    uint8_t maxCount;
    uint8_t selectedButton;
    EventEmitter* eventEmitter; // on click on expanded menu
    bool hidden;
} DropdownMenu;

DropdownMenu* dropdown_menu_init(Arena* arena, UIController* uiController, SDL_Renderer* renderer, uint8_t maxCount, const char* label,
                                 TTF_Font* font, uint8_t fontSize, int x, int y, int w, int h, SDL_Color color);
void dropdown_menu_populate(Arena* arena, SDL_Renderer* renderer, TTF_Font* font, DropdownMenu* menu,
                            const char* textString, uint8_t fontSize, SDL_Color color);
//gives the index of the selected button -1 on none selected
int dropdown_button_selected(DropdownMenu* ddm);
//resets to normal state
void reset_dropdown_menu(DropdownMenu* ddm);
//selects button at index
void select_dropdown_menu_button(DropdownMenu* ddm, uint8_t buttonIndex);
int dropdown_menu_add_listener(Arena* arena, DropdownMenu* ddm, EventCallback cb, void* userData);
//update render und destory handled by ui_controller
void destroy_dropdown_menu(DropdownMenu* ddm);


/* Helper functions */
//Completes the destruction of the SDL2 UI system including  StringMemory, and Main area. All windows should be destroyed before this call
void destroy_sdl2_ui_complete(StringMemory* sm, FontHolder* fh, Arena* arena);
//destorys window and all its elements in uiController
void destroy_window(WindowUI* windowUI, Arena* window_arena, StringMemory* sm, UIController* uiController);

/* Tab Pannel */
/* Allowing in one window mutliple tabs of differing elements being displayed when clicked */

enum TabPannelPossition
{
    TABPANNEL_TOP,
    TABPANNEL_BUTTOM
};

typedef struct
{
    void** pannelElems;
    UI_Element* type;
    uint8_t elemCount;
    uint8_t maxCount;
} Pannel;

typedef struct
{
    BasicButton** tabButtons;
    uint8_t pannelCount;
    uint8_t currentPannel;
    bool tabChanged;
    Pannel** pannels;
} TabPannel;

// Create dynamic amount tab pannel - const char text is "pannel1|Pannel2|etc..."
TabPannel* tab_pannel_init(Arena* arena, UIController* uiController, WindowUI* window, const char* text,
                           enum TabPannelPossition possition, int height, uint8_t elemPerPannelAmount,
                           TTF_Font* font, uint8_t fontSize, SDL_Color color);
// adding elems to tab - tab starting at 1 indexed
void add_elem_to_pannel(void* elem, UI_Element type, TabPannel* tabPannel, uint8_t tab);
//destroy handled by UIcontoller

/* Pop-ups */
typedef struct
{
    WindowUI* window;
    Label* label;
    BasicButton* button;
    Arena* arena;
    bool displaying;
} PopUpNotice;

//basic pop up destory itself on button press
PopUpNotice* popup_notice_init(UIController* uiC, const char* notice, const char* button, TTF_Font* font, uint32_t width, uint32_t height, SDL_Color color);
void popup_notice_destroy(PopUpNotice* popup);


/* Multi Window UI Helper Fuctions */

typedef struct
{
    WindowUI* window;
    Arena* arena;
    UIController* uiController;
    StringMemory* sm;
    FontHolder* fh;
} WindowController;

typedef struct
{
    WindowController** windowController;
    uint16_t count;
    uint16_t maxCount;
} WindowHolder;

//destruction is with mainArena
WindowHolder* window_holder_init(Arena* mainArena, uint16_t maxCount);
WindowController* window_controller_init(WindowHolder* wh, StringMemory* sm, FontHolder* fh, const char* title, uint16_t width, uint16_t height, uint8_t uiElemMax, uint32_t FLAGS);
// check events for all windows - returns true if any window changed size - windowID is set to changed window index
//windowID can be NULL if not needed
bool multi_window_event_check(WindowHolder* wh, SDL_Event* e, uint16_t* windowID);
void multi_window_ui_update(WindowHolder* wh, float deltaTime);
void multi_window_render(WindowHolder* wh, SDL_Color color);
//destory window and then replaces that slot with the last on created!
void destroy_window_controller(WindowHolder* wh, WindowController* wc);


/* IMG Element */
typedef struct
{
    SDL_FRect rect;
    Texture* texture;
    bool hidden;
} Image;
// w & h can be null and then it takes the w & h from texture
Image* image_init(WindowController* windowController, const char* path, int x, int y, int w, int h);
// opacity 0-255
void image_opacity_set(Image* img, uint8_t opacity);
//Rendering and destroying handled with windowController


#endif
