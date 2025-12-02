#ifndef Cimple_ui_H
#define Cimple_ui_H
#include "../arena_memory/arena_memory.h"
#include "../string_lib/string.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

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

// Windows Initialization Function. Also automatically initializes SDL2, SDL2_image, and SDL2_ttf
bool init_SDL2_ui(WindowUI* window, const char* title, uint32_t width, uint32_t height, bool vsync, bool fullscreen);
// Destroys WindowUI and quits SDL2 subsystems
void destroy_SDL2_ui(WindowUI* windowUI);
// Updates WindowUI state based on SDL_Event input - returns true if screensize changed
bool windowUI_update(WindowUI* window, SDL_Event* e);
// Updates window fullscreen or not - returns true if changed
bool windowUI_fullscreen(WindowUI* windowUI, SDL_Event* e);
// Clears the screen with the given colour
void clear_screen_with_color(SDL_Renderer* renderer, SDL_Color colour);
/* Area memory window returns for pop-ups etc */
WindowUI* create_arena_window_ui(const char* title, uint32_t width, uint32_t height, Arena* arena, bool vsync, bool fullscreen);


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
void ui_event_check(Arena* arena, StringMemory* sm, UIController* uiController, SDL_Event* e);
// Updates UI elements (for caret blinking, etc)
void ui_update(UIController* uiC, float deltaTime);
void ui_render(SDL_Renderer* renderer, UIController* uiC);
// Destroys all the textures in the UIController
void ui_controller_destroy(UIController* uiC);

/* TextBox */

typedef struct
{
    SDL_FRect rect;
    TTF_Font* font;
    Texture* texture;
    String* string;
    SDL_Color color;
    uint8_t fontSize;
    uint8_t caretSpacing; // uses to place caret in correct location when mulitple lines
    bool focused;
    bool caret;
    bool textChanged;
    bool hidden;
    /* XXX 7-byte padding */ // do we really need the hidden??
} TextBox;

// Initializes a TextBox structure give width and hight as funtions of screen dimensions
TextBox* textbox_init(Arena* arena, UIController* uiController, String* string, TTF_Font* font, uint8_t fontSize, SDL_Color color, float x, float y, float width, float height);
// Renders the TextBox to the given renderer
void textbox_render(SDL_Renderer* renderer, TextBox* textbox);
// Appends c style string text to the TextBox's string
void textbox_append_text(Arena* arena, StringMemory* sm, TextBox* textbox, const char* text);



/* Button */
enum ButtonState
{
    BUTTON_STATE_NORMAL = 0,
    BUTTON_STATE_PRESSED = 1,
    BUTTON_STATE_HOVERED = 2,
    BUTTON_STATE_RELEASED = 3,
    BUTTON_STATE_TOTAL = 4
};

typedef struct
{
    Texture* texture;
    SDL_Rect spriteClip[BUTTON_STATE_TOTAL]; //  states: normal, hovered, pressed, released
    int xPos;
    int yPos;
    uint8_t totalStates;
    uint8_t currentState;
} Button;


/* Helper functions */
//Completes the destruction of the SDL2 UI system including WindowUI, UIController, StringMemory, and Arena
//Need to Close any fonts etc before calling this function
void destroy_complete_screen(WindowUI* windowUI, Arena* arena, StringMemory* sm, UIController* uiController);
#endif
