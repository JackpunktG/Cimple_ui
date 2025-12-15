#include "Cimple_ui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>

const SDL_Color COLOR[TOTAL] =
{
    {0, 0, 0, 255},       /* BLACK */
    {255, 255, 255, 255}, /* WHITE */
    {255, 0, 0, 255},     /* RED */
    {0, 255, 0, 255},     /* GREEN */
    {0, 0, 255, 255},     /* BLUE */
    {255, 255, 0, 255},   /* YELLOW */
    {255, 0, 255, 255},   /* MAGENTA */
    {0, 255, 255, 255},   /* CYAN */
    {128, 128, 128, 255}, /* GRAY */
    {64, 64, 64, 255},    /* DARK_GRAY */
    {192, 192, 192, 255}, /* LIGHT_GRAY */
    {165, 42, 42, 255},   /* BROWN */
    {255, 165, 0, 255},   /* ORANGE */
    {255, 192, 203, 255}, /* PINK */
    {128, 0, 128, 255},   /* PURPLE */
    {50, 205, 50, 255},   /* LIME */
    {0, 0, 128, 255},     /* NAVY */
    {0, 128, 128, 255},   /* TEAL */
    {128, 128, 0, 255},   /* OLIVE */
    {128, 0, 0, 255}      /* MAROON */
};


/* Window and SDL2 Initialization Function */

bool init_SDL()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("ERROR SDL could not init! MSG: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool init_resizable_window(SDL_Window** windowUI, const char* title, uint32_t width, uint32_t height)
{
    *windowUI = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (*windowUI == NULL)
    {
        printf("ERROR init windowUI! MSG: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool init_popup_window(SDL_Window** windowUI, const char* title, uint32_t width, uint32_t height)
{
    int x, y;
    SDL_GetGlobalMouseState(&x, &y);
    *windowUI = SDL_CreateWindow(title, x - (width /2), y - (height / 2), width, height, SDL_WINDOW_SHOWN);
    if (*windowUI == NULL)
    {
        printf("ERROR init windowUI! MSG: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool init_renderer_vsync(SDL_Window* window, SDL_Renderer** renderer)
{
    *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL)
    {
        printf("ERROR init VSYNC-renderer! MSG: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("ERROR SDL_image could not init! MSG: %s\n", IMG_GetError());
            return false;
        }
    }
    return true;
}

bool init_renderer(SDL_Window* window, SDL_Renderer** renderer)
{
    *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
    {
        printf("ERROR init renderer! MSG: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("ERROR SDL_image could not init! MSG: %s\n", IMG_GetError());
            return false;
        }
    }

    return true;
}

bool init_TTF()
{
    if (TTF_Init() == -1)
    {
        printf("ERROR SDL_ttf could not init! MSG: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

bool init_SDL2_ui(WindowUI* windowUI, const char* title, uint32_t width, uint32_t height, bool vsync, bool fullscreen)
{
    if (!init_SDL()) return false;

    if (!init_resizable_window(&windowUI->window, title, width, height))
        return false;

    if (vsync)
    {
        if (!init_renderer_vsync(windowUI->window, &windowUI->renderer))
            return false;
    }
    else
    {
        if (!init_renderer(windowUI->window, &windowUI->renderer))
            return false;
    }

    windowUI->fullscreen = fullscreen;

    if (fullscreen)
    {
        SDL_SetWindowFullscreen(windowUI->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_GetWindowSize(windowUI->window, &windowUI->width, &windowUI->height);
    }
    else
    {
        windowUI->width = width;
        windowUI->height = height;
    }

    return true;
}

WindowUI* create_arena_window_popup(const char* title, uint32_t width, uint32_t height, Arena* arena)
{
    WindowUI* windowUI = arena_alloc(arena, sizeof(WindowUI), NULL);
    if (!init_popup_window(&windowUI->window, title, width, height))
        return NULL;
    if (!init_renderer(windowUI->window, &windowUI->renderer))
        return NULL;

    windowUI->fullscreen = false;
    windowUI->width = width;
    windowUI->height = height;

    return windowUI;
}

WindowUI* create_arena_window_ui(const char* title, uint32_t width, uint32_t height, Arena* arena, bool vsync, bool fullscreen)
{
    WindowUI* windowUI = arena_alloc(arena, sizeof(WindowUI), NULL);
    if (!init_SDL2_ui(windowUI, title, width, height, vsync, fullscreen))
    {
        return NULL;
    }
    return windowUI;
}

bool windowUI_fullscreen(WindowUI* windowUI, SDL_Event* e)
{
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
    {
        if(windowUI->fullscreen)
        {
            SDL_SetWindowFullscreen(windowUI->window, 0);
            SDL_RestoreWindow(windowUI->window);

            SDL_SetWindowSize(windowUI->window, windowUI->width - 100, windowUI->height -100);
            SDL_SetWindowPosition(windowUI->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            windowUI->fullscreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen(windowUI->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            windowUI->fullscreen = true;

        }
        SDL_GetWindowSize(windowUI->window, &windowUI->width, &windowUI->height);
        return true;
    }
    return false;
}

bool windowUI_update(WindowUI* window, SDL_Event* e)
{
    if (e->window.windowID != SDL_GetWindowID(window->window))
        return false;

    if (e->type != SDL_WINDOWEVENT)
        return false;

    bool size_changed = false;

    if (e->type == SDL_WINDOWEVENT)
    {
        switch (e->window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window->width = e->window.data1;
            window->height = e->window.data2;
            size_changed = true;
            break;

        case SDL_WINDOWEVENT_MINIMIZED:
            window->minimized = true;
            break;

        case SDL_WINDOWEVENT_MAXIMIZED:
        case SDL_WINDOWEVENT_RESTORED:
            window->minimized = false;
            break;

        case SDL_WINDOWEVENT_ENTER:
            window->mouseFocus = true;
            break;

        case SDL_WINDOWEVENT_LEAVE:
            window->mouseFocus = false;
            break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
            window->mkeyboardFocus = true;
            break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
            window->mkeyboardFocus = false;
            break;
        }
    }
    return size_changed;
}

bool windowUI_has_focus(WindowUI* windowUI)
{
    return windowUI->mouseFocus || windowUI->mkeyboardFocus;
}

void quit_SDL2_ui()
{
    IMG_Quit();
    SDL_Quit();
}

void destroy_window_ui(WindowUI* window)
{
    if (window->renderer != NULL)
    {
        SDL_DestroyRenderer(window->renderer);
        window->renderer = NULL;
    }

    if (window->window != NULL)
    {
        SDL_DestroyWindow(window->window);
        window->window = NULL;
    }
}

/* Font Holder */

FontHolder* font_holder_init(Arena* arena, uint8_t maxFonts)
{
    FontHolder* fb = arena_alloc(arena, sizeof(FontHolder), NULL);
    fb->fonts = arena_alloc(arena, sizeof(TTF_Font*) * maxFonts, NULL);
    fb->count = 0;
    fb->maxCount = maxFonts;

    init_TTF();
    return fb;
}

void load_fonts(FontHolder* fh, const char* fileName, uint8_t fontSize)
{
    assert(fh->count < fh->maxCount);
    TTF_Font* font = TTF_OpenFont(fileName, fontSize);
    if (font == NULL)
    {
        printf("ERROR unable to load font %s! MSG: %s\n", fileName, TTF_GetError());
        assert(false);
    }
    fh->fonts[fh->count++] = font;
}

void destroy_fonts(FontHolder* fh)
{
    for (int i = 0; i < fh->count; ++i)
    {
        TTF_CloseFont(fh->fonts[i]);
    }
    TTF_Quit();
}

/* Texture & Renderer Functions */

void clear_screen_with_color(SDL_Renderer* renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void init_texture(Texture* texture)
{
    texture->mTexture = NULL;
    texture->width = 0;
    texture->height = 0;
}

void free_texture(Texture* texture)
{
    if (texture->mTexture != NULL)
    {
        SDL_DestroyTexture(texture->mTexture);
        texture->mTexture = NULL;
        texture->width = 0;
        texture->height = 0;
    }
}

bool load_texture_from_rendered_text(Texture* texture, uint32_t wraplength, const char* text, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer)
{
    init_texture(texture);

    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wraplength);
    if (textSurface == NULL)
    {
        printf("ERROR unable to render text surface! MSG: %s\n", TTF_GetError());
        return false;
    }

    texture->mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (texture->mTexture == NULL)
    {
        printf("ERROR unable to create texture from rendered text! MSG: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return false;
    }

    texture->width = textSurface->w;
    texture->height = textSurface->h;

    SDL_FreeSurface(textSurface);
    return true;
}


/* UI-Controller functions */

UIController* ui_controller_init(Arena* arena, uint16_t totalElements)
{
    UIController* uiC = arena_alloc(arena, sizeof(UIController), NULL);
    uiC->element = arena_alloc(arena, sizeof(void*) * totalElements, NULL);
    uiC->type = arena_alloc(arena, sizeof(UI_Element) * totalElements, NULL);

    uiC->focusedType = NULL_ELEM;
    uiC->elemFocus = NULL;
    uiC->secondCounter = 0.0f;
    uiC->count = 0;
    uiC->totalElements = totalElements;

    return uiC;
}

void ui_elem_add(UIController* uiC, void* elem, UI_Element type)
{
    assert(uiC->count < uiC->totalElements);

    uiC->element[uiC->count] = elem;
    uiC->type[uiC->count] = type;
    uiC->count += 1;
}

void ui_elem_remove(UIController* uiC, uint16_t elemIndex)
{
    for (int i = elemIndex; i < uiC->count -1; ++i)
    {
        uiC->type[i] = uiC->type[i+1];
        uiC->element[i] = uiC->element[i+1];
    }
    uiC->count--;
}

bool mouse_in_intbox_check(int mx, int my, SDL_Rect* rect)
{
    return mx >= rect->x && mx <= rect->x + rect->w && my >= rect->y && my <= rect->y + rect->h;
}

bool mouse_in_box_check(int mx, int my, SDL_FRect* rect)
{
    return mx >= rect->x && mx <= rect->x + rect->w && my >= rect->y && my <= rect->y + rect->h;
}

void button_basic_click(BasicButton* bb);
void popup_button_check(BasicButton* button, SDL_Event* e);
void tab_pannel_mouseclick_check(TabPannel* tp, int mx, int my);
void dropdown_menu_mouseclick_check(UIController* uiC, DropdownMenu* ddm, int mx, int my);
void dropdown_menu_mousemovement_check(DropdownMenu* ddm, int mx, int my);

void ui_event_check(Arena* arena, StringMemory* sm, UIController* uiController, WindowUI* window, SDL_Event* e)
{
    if (e->window.windowID != SDL_GetWindowID(window->window))
    {
        for (int i = 0; i < uiController->count; ++i)
        {
            switch(uiController->type[i])
            {
            case POPUP_NOTICE_ELEM:
            {
                PopUpNotice* pn = (PopUpNotice*)uiController->element[i];
                if (e->window.windowID == SDL_GetWindowID(pn->window->window))
                    popup_button_check(pn->button, e);
                break;
            }
            default:
                break;
            }
        }
    }

    if (!window->mkeyboardFocus && !window->mouseFocus)
        return;
    // printf("UI Event Check: Event Type %d\n", e->type);

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = e->button.x;
        int my = e->button.y;
        if(uiController->focusedType == DROPDOWN_MENU_ELEM)
        {
            DropdownMenu* ddm = (DropdownMenu*)uiController->elemFocus;
            if(ddm->hidden)
            {
                uiController->elemFocus = NULL;
                uiController->focusedType = NULL_ELEM;
            }
            dropdown_menu_mouseclick_check(uiController, ddm, mx, my);
        }
        else
        {
            for(int i = 0; i < uiController->count; ++i)
            {
                switch(uiController->type[i])
                {
                case TEXTBOX_ELEM:
                {
                    TextBox* tb = (TextBox*)uiController->element[i];
                    if(tb->hidden) break;
                    if (mouse_in_box_check(mx, my, &tb->rect))
                    {
                        tb->focused = true;
                        uiController->elemFocus = tb;
                        uiController->focusedType = TEXTBOX_ELEM;
                        SDL_StartTextInput();
                        SDL_Rect input = {(int)tb->rect.x, (int)tb->rect.y, (int)tb->rect.w, (int)tb->rect.h};
                        SDL_SetTextInputRect(&input);
                    }
                    else if (tb->focused)
                    {
                        tb->focused = false; //if mouse click happens outside the box change it to false
                        uiController->elemFocus = NULL;
                        uiController->focusedType = NULL_ELEM;
                        SDL_StopTextInput();
                    }
                    break;
                }
                case BUTTON_BASIC_ELEM:
                {
                    BasicButton* bb = (BasicButton*)uiController->element[i];
                    if(bb->hidden) break;
                    if (bb->cooldownTimer <= 0 && mouse_in_intbox_check(mx, my, &bb->rect))
                    {
                        bb->state = BUTTON_STATE_PRESSED;
                        button_basic_click(bb);
                        bb->cooldownTimer = 0.5f; //500 ms cooldown
                    }
                    break;
                }
                case TABPANNEL_ELEM:
                {
                    TabPannel* tp = (TabPannel*)uiController->element[i];
                    tab_pannel_mouseclick_check(tp, mx,my);
                    break;
                }
                case DROPDOWN_MENU_ELEM:
                {
                    DropdownMenu* ddm = (DropdownMenu*)uiController->element[i];
                    if(ddm->hidden) break;
                    dropdown_menu_mouseclick_check(uiController, ddm, mx, my);
                    break;
                }
                case NULL_ELEM:
                    assert(false);
                default:
                    break;
                }
            }
        }
    }
    else if (e->type == SDL_MOUSEBUTTONUP)
    {
        int mx = e->button.x;
        int my = e->button.y;
        for(int i = 0; i < uiController->count; ++i)
        {
            switch(uiController->type[i])
            {
            case BUTTON_BASIC_ELEM:
            {
                BasicButton* bb = (BasicButton*)uiController->element[i];
                if(bb->hidden) break;
                if (mouse_in_intbox_check(mx, my, &bb->rect) && bb->state == BUTTON_STATE_PRESSED)
                    bb->state = BUTTON_STATE_HOVERED;
                else
                    bb->state = BUTTON_STATE_NORMAL;
                break;
            }

            case NULL_ELEM:
                assert(false);
            default:
                break;
            }
        }
    }
    else if (e->type == SDL_MOUSEMOTION)
    {
        int mx = e->motion.x;
        int my = e->motion.y;
        for(int i = 0; i < uiController->count; ++i)
        {
            switch(uiController->type[i])
            {
            case BUTTON_BASIC_ELEM:
            {
                if (uiController->focusedType == DROPDOWN_MENU_ELEM)
                    break;

                BasicButton* bb = (BasicButton*)uiController->element[i];
                if (bb->hidden) break;
                if (mouse_in_intbox_check(mx, my, &bb->rect))
                    bb->state = BUTTON_STATE_HOVERED;
                else
                    bb->state = BUTTON_STATE_NORMAL;
                break;
            }
            case DROPDOWN_MENU_ELEM:
            {
                DropdownMenu* ddm = (DropdownMenu*)uiController->element[i];
                if(ddm->hidden) break;
                dropdown_menu_mousemovement_check(ddm, mx, my);
                break;
            }
            case NULL_ELEM:
                assert(false);
            default:
                break;
            }
        }
    }

    if (uiController->elemFocus == NULL) return;
    if (uiController->focusedType != TEXTBOX_ELEM) return;
    //logic for inputing text to focused textbox

    if (e->type == SDL_TEXTINPUT)
    {

        switch(uiController->focusedType)
        {
        case TEXTBOX_ELEM:
        {
            TextBox* tb = (TextBox*)uiController->elemFocus;
            if (tb->hidden) break;
            assert(tb->focused);
            textbox_append_text(arena, sm, tb, e->text.text);
            break;
        }
        case NULL_ELEM:
            assert(false);
            break;
        default:
            break;
        }
    }
    else if (e->type == SDL_KEYDOWN)
    {
        if (e->key.keysym.sym == SDLK_BACKSPACE || e->key.keysym.sym == SDLK_KP_ENTER || e->key.keysym.sym == SDLK_RETURN)
        {
            switch(uiController->focusedType)
            {
            case TEXTBOX_ELEM:
            {
                TextBox* tb = (TextBox*)uiController->elemFocus;
                if (tb->hidden) break;
                assert(tb->focused);
                if (e->key.keysym.sym == SDLK_BACKSPACE)
                {
                    remove_last_char(tb->string);
                    tb->textChanged = true;
                }
                else
                {
                    uiController->elemFocus = NULL;
                    uiController->focusedType = NULL_ELEM;
                    tb->focused = false; //lose focus on enter key
                    SDL_StopTextInput();
                }
                break;

            }
            case NULL_ELEM:
                assert(false);
                break;
            default:
                break;
            }
        }
    }
}

void update_tab(TabPannel* tp);

void ui_update(UIController* uiC, float deltaTime)
{
    for (int i  = 0; i < uiC->count; ++i)
    {
        switch(uiC->type[i])
        {
        case BUTTON_BASIC_ELEM:
        {
            BasicButton* bb = (BasicButton*)uiC->element[i];
            if (bb->cooldownTimer > 0.0f) //Button cooldown update
            {
                bb->cooldownTimer -= deltaTime;
            }
            break;
        }
        case POPUP_NOTICE_ELEM:
        {
            PopUpNotice* pn = (PopUpNotice*)uiC->element[i];
            if (!pn->displaying)
            {
                popup_notice_destroy(pn);
                ui_elem_remove(uiC, i--);
                continue;
            }
            break;
        }
        case TABPANNEL_ELEM:
        {
            TabPannel* tp = (TabPannel*)uiC->element[i];
            if(tp->tabChanged)
            {
                update_tab(tp);
                tp->tabChanged = false;
            }
            break;

        }
        case NULL_ELEM:
            break;
        default:
            break;
        }
    }
}

void render_tab_pannel_buttons(TabPannel* tp, SDL_Renderer* renderer);
void dropdown_menu_render(DropdownMenu* ddm, SDL_Renderer* renderer);
void ui_render(SDL_Renderer* renderer, UIController* uiC)
{
    for(int i = 0; i < uiC->count; ++i)
    {
        switch(uiC->type[i])
        {
        case TEXTBOX_ELEM:
        {
            TextBox* tb = (TextBox*)uiC->element[i];
            textbox_render(renderer, tb);
            break;
        }
        case BUTTON_BASIC_ELEM:
        {
            BasicButton* bb = (BasicButton*)uiC->element[i];
            button_basic_render(renderer, bb);
            break;
        }
        case LABEL_ELEM:
        {
            Label* label = (Label*)uiC->element[i];
            label_basic_render(renderer, label);
            break;
        }
        case POPUP_NOTICE_ELEM:
        {
            PopUpNotice* pn = (PopUpNotice*)uiC->element[i];
            clear_screen_with_color(pn->window->renderer, COLOR[BLACK]);
            button_basic_render(pn->window->renderer, pn->button);
            label_basic_render(pn->window->renderer, pn->label);
            SDL_RenderPresent(pn->window->renderer);
            break;
        }
        case TABPANNEL_ELEM:
        {
            TabPannel* tp = (TabPannel*)uiC->element[i];
            render_tab_pannel_buttons(tp, renderer);
            break;
        }
        case DROPDOWN_MENU_ELEM:
        {
            DropdownMenu* ddm = (DropdownMenu*)uiC->element[i];
            dropdown_menu_render(ddm, renderer);
            break;
        }
        case TEXTFIELD_ELEM:
        {
            TextField* tf = (TextField*)uiC->element[i];
            textfield_render(renderer, tf);
            break;
        }
        case NULL_ELEM:
            assert(false);
            break;
        }
    }
}

void destroy_tab_pannel_buttons(TabPannel* tp);
void ui_controller_destroy(UIController* uiC)
{
    for(int i = 0; i < uiC->count; ++i)
    {
        switch(uiC->type[i])
        {
        case TEXTBOX_ELEM:
        {
            TextBox* tb = (TextBox*)uiC->element[i];
            free_texture(tb->texture);
            break;
        }
        case TEXTFIELD_ELEM:
        {
            TextField* tf = (TextField*)uiC->element[i];
            free_texture(tf->texture);
            break;
        }
        case BUTTON_BASIC_ELEM:
        {
            BasicButton* bb = (BasicButton*)uiC->element[i];
            free_texture(bb->text);
            break;
        }
        case LABEL_ELEM:
        {
            Label* label = (Label*)uiC->element[i];
            free_texture(label->text);
            break;
        }
        case TABPANNEL_ELEM:
        {
            TabPannel* tp = (TabPannel*)uiC->element[i];
            destroy_tab_pannel_buttons(tp);
            break;
        }
        case DROPDOWN_MENU_ELEM:
        {
            DropdownMenu* ddm = (DropdownMenu*)uiC->element[i];
            destroy_dropdown_menu(ddm);
            break;
        }
        case POPUP_NOTICE_ELEM:
        {
            PopUpNotice* pn = (PopUpNotice*)uiC->element[i];
            popup_notice_destroy(pn);
            break;
        }
        case NULL_ELEM:
            assert(false);
            break;
        default:
            break;
        }
    }
}

/* Label functions */

Label* label_basic_init(Arena* arena, UIController* uiController, int x, int y, int width, int height,
                        const char* text, TTF_Font* font, uint8_t fontsize, SDL_Color color,
                        SDL_Renderer* renderer)
{
    Label* l = arena_alloc(arena, sizeof(Label), NULL);
    l->text = arena_alloc(arena, sizeof(Texture), NULL);
    init_texture(l->text);
    l->rect.x = x;
    l->rect.y = y;
    l->rect.w = width;
    l->rect.h = height;
    l->hidden = false;

    if (fontsize != 0)
    {
        TTF_SetFontSize(font, fontsize);
        load_texture_from_rendered_text(l->text, width, text, font, color, renderer);
    }
    else
    {
        uint8_t i = DEFAULT_FONT_SIZE;
        do
        {
            assert(i > 0 && "Font size could not be adjusted to fit button text");

            if (i != DEFAULT_FONT_SIZE)
                free_texture(l->text);

            TTF_SetFontSize(font, i--);
            load_texture_from_rendered_text(l->text, width -20, text, font, color, renderer);
        }
        while (l->text->height >= l->rect.h);
    }

    if (uiController != NULL) ui_elem_add(uiController, l, LABEL_ELEM);

    return l;
}


void label_basic_render(SDL_Renderer* renderer, Label* label)
{
    if (label->hidden) return;

    if (label->text->mTexture != NULL)
    {
        SDL_FRect dst =
        {
            roundf(label->rect.x + (label->rect.w / 2) - (float)label->text->width / 2),
            roundf(label->rect. y + (label->rect.h / 2) - (float)label->text->height / 2),
            (float)label->text->width,
            (float)label->text->height
        };
        SDL_RenderCopyF(renderer, label->text->mTexture, NULL, &dst);
    }
}

/* Event Emitter functions */

EventEmitter* event_emitter_init(Arena* arena, uint8_t maxListeners)
{
    EventEmitter* em = arena_alloc(arena, sizeof(EventEmitter), NULL);
    em->listeners = arena_alloc(arena, sizeof(EventNode*) * maxListeners, NULL);
    em->count = 0;
    em->maxCount = maxListeners;
    return em;
}

int event_emitter_add_listener(Arena* arena, void* uiElement, UI_Element type, EventCallback cb, void* userData)
{
    if (!cb || !uiElement) return 0;

    switch (type)
    {
    case BUTTON_BASIC_ELEM:
    {
        BasicButton* bb = (BasicButton*)uiElement;
        return button_basic_add_listener(arena, bb, cb, userData);
        break;
    }
    case DROPDOWN_MENU_ELEM:
    {
        DropdownMenu* ddm = (DropdownMenu*)uiElement;
        return dropdown_menu_add_listener(arena, ddm, cb, userData);
        break;
    }
    case TEXTBOX_ELEM:
        break;
    case NULL_ELEM:
        assert(false);
        break;
    default:
        break;
    }
    return 0;
}

void event_emitter_emit(EventEmitter* em, const Event* ev)
{
    bool found = false;
    for (int i = 0; i < em->maxCount; ++i)
    {
        if (em->listeners[i]->type == ev->type)
        {
            EventNode* event = em->listeners[i];
            event->cb(ev, event->userData);
            found = true;
            break;
        }
    }
    assert(found && "No event listener found for emitted event" );
}


/* Text box functions */

TextBox* textbox_init(Arena* arena, UIController* uiC, StringMemory* sm, TTF_Font* font, uint8_t fontSize, SDL_Color color, float x, float y, float width, float height)
{
    TextBox* tb = arena_alloc(arena, sizeof(TextBox), NULL);
    tb->texture = arena_alloc(arena, sizeof(Texture), NULL);
    tb->string = string_init(arena, sm);
    init_texture(tb->texture);

    tb->fontSize = fontSize;
    tb->color = color;
    tb->font = font;
    tb->focused = false;
    tb->textChanged = false;
    tb->hidden = false;
    tb->rect.x = x;
    tb->rect.y = y;
    tb->rect.w = width;
    tb->rect.h = height;

    ui_elem_add(uiC, tb, TEXTBOX_ELEM);

    return tb;
}

void textbox_render(SDL_Renderer* renderer, TextBox* textbox)
{
    if (textbox->hidden) return;

    //string texture update
    if (textbox->textChanged && textbox->string->count > 0)
    {
        TTF_SetFontSize(textbox->font, textbox->fontSize);
        free_texture(textbox->texture);

        char tmp[textbox->string->count +1];
        c_string_sendback(textbox->string, tmp);
        load_texture_from_rendered_text(textbox->texture, (int)(textbox->rect.w - 5),tmp, textbox->font, textbox->color, renderer);

        textbox->textChanged = false;
    }

    if (textbox->string->count > 0)
    {
        if (textbox->texture->height > textbox->rect.h)
            textbox->rect.h += textbox->fontSize;
        else if (textbox->texture->height < textbox->rect.h - textbox->fontSize)
            textbox->rect.h -= textbox->fontSize;
    }
    else if (textbox->string->count == 0)
        textbox->rect.h = textbox->rect.h > textbox->fontSize + 10 ? textbox->rect.h -= textbox->fontSize : textbox->fontSize + 10;


    SDL_Color color = COLOR[GRAY];
    SDL_FRect r = textbox->rect;
    if (textbox->focused)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRectF(renderer, &r);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 120);
        SDL_RenderFillRectF(renderer, &r);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 185);
        SDL_RenderDrawRectF(renderer, &r);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
        SDL_RenderFillRectF(renderer, &r);
    }

    //string render
    if (textbox->texture->mTexture != NULL && textbox->string->count > 0)
    {
        SDL_FRect dst = {r.x + 5, r.y + (r.h / 2) - (float)textbox->texture->height / 2, (float)textbox->texture->width, (float)textbox->texture->height};
        SDL_RenderCopyF(renderer, textbox->texture->mTexture, NULL, &dst);
    }

}

void textbox_append_text(Arena* arena, StringMemory* sm, TextBox* textbox, const char* text)
{
    String* s = textbox->string;
    if (string_c_append(arena, &s, sm, text) == 1)
        textbox->string = s;

    textbox->textChanged = true;
}


/* TextField functions */

TextField* textfield_init(Arena* arena, UIController* uiC, StringMemory* sm, TTF_Font* font, uint8_t fontSize, SDL_Color color, float x, float y, float width, float height)
{
    TextField* tf = arena_alloc(arena, sizeof(TextBox), NULL);
    memset(tf, 0, sizeof(TextField));
    tf->texture = arena_alloc(arena, sizeof(Texture), NULL);
    tf->string = string_init(arena, sm);
    init_texture(tf->texture);

    tf->fontSize = fontSize;
    tf->color = color;
    tf->font = font;
    tf->textChanged = false;
    tf->hidden = false;
    tf->rect.x = x;
    tf->rect.y = y;
    tf->rect.w = width;
    tf->rect.h = height;

    ui_elem_add(uiC, tf, TEXTFIELD_ELEM);

    return tf;
}


void textfield_render(SDL_Renderer* renderer, TextField* textfield)
{
    if (textfield->hidden) return;

    //string texture update
    if (textfield->textChanged && textfield->string->count > 0)
    {
        TTF_SetFontSize(textfield->font, textfield->fontSize);
        free_texture(textfield->texture);

        char tmp[textfield->string->count +1];
        c_string_sendback(textfield->string, tmp);
        load_texture_from_rendered_text(textfield->texture, (int)(textfield->rect.w - 5),tmp, textfield->font, textfield->color, renderer);

        textfield->textChanged = false;
    }

    if (textfield->string->count > 0)
    {
        if (textfield->texture->height > textfield->rect.h)
            textfield->rect.h += textfield->fontSize;
        else if (textfield->texture->height < textfield->rect.h - textfield->fontSize)
            textfield->rect.h -= textfield->fontSize;
    }
    else if (textfield->string->count == 0)
        textfield->rect.h = textfield->rect.h > textfield->fontSize + 10 ? textfield->rect.h -= textfield->fontSize : textfield->fontSize + 10;


    SDL_Color color = COLOR[GRAY];
    SDL_FRect r = textfield->rect;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 185);
    SDL_RenderDrawRectF(renderer, &r);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 100);
    SDL_RenderFillRectF(renderer, &r);

    //string render
    if (textfield->texture->mTexture != NULL && textfield->string->count > 0)
    {
        SDL_FRect dst = {r.x + 5, r.y + (r.h / 2) - (float)textfield->texture->height / 2, (float)textfield->texture->width, (float)textfield->texture->height};
        SDL_RenderCopyF(renderer, textfield->texture->mTexture, NULL, &dst);
    }
}

void textfield_append_text(Arena* arena, StringMemory* sm, TextField* textfield, const char* text)
{
    String* s = textfield->string;
    if (string_c_append(arena, &s, sm, text) == 1)
        textfield->string = s;

    textfield->textChanged = true;
}

/* Button funtions */

BasicButton* button_basic_init(Arena* arena, UIController* uiController, int x, int y, int width, int height, const char* text, TTF_Font* font, uint8_t fontsize, SDL_Color color, SDL_Renderer* renderer)
{
    BasicButton* bb = arena_alloc(arena, sizeof(BasicButton), NULL);
    bb->text = arena_alloc(arena, sizeof(Texture), NULL);
    bb->eventEmitter = event_emitter_init(arena, 1);
    bb->color = color;
    bb->hidden = false;
    bb->rect.x = x;
    bb->rect.y = y;
    bb->rect.w = width;
    bb->rect.h = height;
    bb->cooldownTimer = 0.0f;
    bb->state = BUTTON_STATE_NORMAL;

    if (fontsize != 0)
    {
        TTF_SetFontSize(font, fontsize);
        load_texture_from_rendered_text(bb->text, width, text, font, color, renderer);
    }
    else
    {
        uint8_t i = DEFAULT_FONT_SIZE;
        do
        {
            assert(i > 0 && "Font size could not be adjusted to fit button text");

            if (i != DEFAULT_FONT_SIZE)
                free_texture(bb->text);

            TTF_SetFontSize(font, i--);
            load_texture_from_rendered_text(bb->text, width - 10, text, font, color, renderer);
        }
        while (bb->text->height >= bb->rect.h);
    }

    if (uiController != NULL) ui_elem_add(uiController, bb, BUTTON_BASIC_ELEM);

    return bb;
}

int button_basic_add_listener(Arena* arena, BasicButton* bb, EventCallback cb, void* userData)
{
    assert(bb->eventEmitter->count < bb->eventEmitter->maxCount);

    EventNode* en = arena_alloc(arena, sizeof(EventNode), NULL);
    en->cb = cb;
    en->userData = userData;
    en->type = EVENT_TYPE_CLICK;

    bb->eventEmitter->listeners[bb->eventEmitter->count++] = en;
    return 1;
}

void button_basic_click(BasicButton* bb)
{
    if (!bb) return;
    Event ev;
    ev.type = EVENT_TYPE_CLICK;
    ev.sourceObj = bb;
    event_emitter_emit(bb->eventEmitter, &ev);
}

void button_basic_render(SDL_Renderer* renderer, BasicButton* button)
{
    if (button->hidden) return;

    SDL_Color color;
    switch (button->state)
    {
    case BUTTON_STATE_NORMAL:
        color = COLOR[WHITE];
        break;
    case BUTTON_STATE_HOVERED:
        color = COLOR[LIGHT_GRAY];
        break;
    case BUTTON_STATE_PRESSED:
        color = button->color;
        break;
    case BUTTON_TAB_SELECTED:
        color = COLOR[GRAY];
        break;
    case BUTTON_TAB:
        color = COLOR[WHITE];
        break;
    default:
        color = COLOR[WHITE];
        break;
    }
    SDL_Rect r = button->rect;

    //Button draw
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &r);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a -20);
    SDL_RenderFillRect(renderer, &r);

    //Text render
    SDL_FRect dst =
    {
        roundf(r.x + ((float)r.w / 2) - (float)button->text->width / 2),
        roundf(r.y + ((float)r.h / 2) - (float)button->text->height / 2),
        (float)button->text->width,
        (float)button->text->height
    };
    SDL_RenderCopyF(renderer, button->text->mTexture, NULL, &dst);
}


/* Tab Pannel functions */


void add_elem_to_pannel(void* elem, UI_Element type, TabPannel* tabPannel, uint8_t tab)
{
    assert(tabPannel->pannelCount >= tab && tab > 0);
    Pannel* p = tabPannel->pannels[tab -1];

    assert(p->elemCount < p->maxCount);

    p->pannelElems[p->elemCount] = elem;
    p->type[p->elemCount++] = type;

    if (tab != 1)
    {
        switch (type)
        {
        case BUTTON_BASIC_ELEM:
        {
            BasicButton* bb = (BasicButton*)elem;
            bb->hidden = true;
            break;
        }
        case TEXTBOX_ELEM:
        {
            TextBox* tb = (TextBox*)elem;
            tb->hidden = true;
            break;
        }
        case LABEL_ELEM:
        {
            Label* l = (Label*)elem;
            l->hidden = true;
            break;
        }
        case DROPDOWN_MENU_ELEM:
        {
            DropdownMenu* ddm = (DropdownMenu*)elem;
            ddm->hidden = true;
            break;
        }
        case TEXTFIELD_ELEM:
        {
            TextField* tf = (TextField*)elem;
            tf->hidden = true;
            break;
        }
        case POPUP_NOTICE_ELEM:
        case TABPANNEL_ELEM:
        case NULL_ELEM:
            assert(false);
            break;
        }
    }
}

Pannel* pannel_init(Arena* arena, uint8_t elemAmount)
{
    Pannel* p = arena_alloc(arena, sizeof(Pannel), NULL);
    p->pannelElems = arena_alloc(arena, sizeof(void*) * elemAmount, NULL);
    p->type = arena_alloc(arena, sizeof(UI_Element) * elemAmount, NULL);
    p->maxCount = elemAmount;
    p->elemCount = 0;

    return p;
}

TabPannel* tab_pannel_init(Arena* arena, UIController* uiController, WindowUI* window, const char* text,
                           enum TabPannelPossition possition, int height, uint8_t elemPerPannelAmount, TTF_Font* font, uint8_t fontSize, SDL_Color color)
{

    uint8_t count = 1;
    int index = 0;
    while(text[index] != '\0')
    {
        if (text[index++] == '|')
            count++;
    }
    TabPannel* tp = arena_alloc(arena, sizeof(TabPannel), NULL);
    tp->pannelCount = count;
    tp->currentPannel = 0;
    tp->tabChanged = true;
    tp->pannels = arena_alloc(arena, sizeof(Pannel*) * count, NULL);
    for (int i = 0; i < count; ++i)
    {
        tp->pannels[i] = pannel_init(arena, elemPerPannelAmount);
    }

    tp->tabButtons = arena_alloc(arena, sizeof(BasicButton*) * count, NULL);
    int lastIndexDivide = 0;
    for (int i = 0; i < count; ++i)
    {
        char buffer[128];
        int j = lastIndexDivide;
        int k = 0;
        while (text[j] != '|' && j < index)
        {
            buffer[k++] = text[j++];
        }
        buffer[k] = '\0';
        lastIndexDivide = ++j;

        switch(possition)
        {
        case TABPANNEL_TOP:
            tp->tabButtons[i] = button_basic_init(arena, NULL, 0 + (i *  window->width / count), 0, window->width / count, height, buffer, font, fontSize, color, window->renderer);
            break;
        case TABPANNEL_BUTTOM:
            tp->tabButtons[i] = button_basic_init(arena, NULL, 0 + (i *  window->width / count), window->height - height, window->width / count, height, buffer, font, fontSize, color, window->renderer);
            break;
        }
        tp->tabButtons[i]->state = i == 0 ? BUTTON_TAB_SELECTED : BUTTON_TAB;
    }

    ui_elem_add(uiController, tp, TABPANNEL_ELEM);

    return tp;
}

void tab_pannel_mouseclick_check(TabPannel* tp, int mx, int my)
{
    int changePannel = -1;

    for (int i = 0; i < tp->pannelCount; ++i)
    {
        if(mouse_in_intbox_check(mx, my, &tp->tabButtons[i]->rect) && tp->tabButtons[i]->state == BUTTON_TAB)
        {
            tp->tabButtons[i]->state = BUTTON_TAB_SELECTED;
            tp->currentPannel = i;
            changePannel = i;
            tp->tabChanged = true;
        }
    }
    if (changePannel == -1)
        return;

    for(int i = 0; i < tp->pannelCount; ++i)
    {
        if (i == tp->currentPannel)
            tp->tabButtons[i]->state = BUTTON_TAB_SELECTED;
        else
            tp->tabButtons[i]->state = BUTTON_TAB;
    }
}

void update_tab(TabPannel* tp)
{
    for(int i = 0; i < tp->pannelCount; ++i)
    {
        Pannel* p = tp->pannels[i];
        if (i == tp->currentPannel)
        {
            for (int j = 0; j < p->elemCount; ++j)
            {
                switch (p->type[j])
                {
                case BUTTON_BASIC_ELEM:
                {
                    BasicButton* bb = (BasicButton*)p->pannelElems[j];
                    bb->hidden = false;
                    break;
                }
                case TEXTBOX_ELEM:
                {
                    TextBox* tb = (TextBox*)p->pannelElems[j];
                    tb->hidden = false;
                    break;
                }
                case LABEL_ELEM:
                {
                    Label* l = (Label*)p->pannelElems[j];
                    l->hidden = false;
                    break;
                }
                case DROPDOWN_MENU_ELEM:
                {
                    DropdownMenu* ddm = (DropdownMenu*)p->pannelElems[j];
                    ddm->hidden = false;
                    break;
                }
                case TEXTFIELD_ELEM:
                {
                    TextField* tf = (TextField*)p->pannelElems[j];
                    tf->hidden = false;
                    break;
                }
                case POPUP_NOTICE_ELEM:
                case TABPANNEL_ELEM:
                case NULL_ELEM:
                    assert(false);
                    break;
                }
            }
        }
        else
        {
            for (int j = 0; j < p->elemCount; ++j)
            {
                switch (p->type[j])
                {
                case BUTTON_BASIC_ELEM:
                {
                    BasicButton* bb = (BasicButton*)p->pannelElems[j];
                    bb->hidden = true;
                    break;
                }
                case TEXTBOX_ELEM:
                {
                    TextBox* tb = (TextBox*)p->pannelElems[j];
                    tb->hidden = true;
                    break;
                }
                case LABEL_ELEM:
                {
                    Label* l = (Label*)p->pannelElems[j];
                    l->hidden = true;
                    break;
                }
                case DROPDOWN_MENU_ELEM:
                {
                    DropdownMenu* ddm = (DropdownMenu*)p->pannelElems[j];
                    ddm->hidden = true;
                    break;
                }
                case TEXTFIELD_ELEM:
                {
                    TextField* tf = (TextField*)p->pannelElems[j];
                    tf->hidden = true;;
                    break;
                }
                case POPUP_NOTICE_ELEM:
                case TABPANNEL_ELEM:
                case NULL_ELEM:
                    assert(false);
                    break;

                }
            }
        }
    }
}

void render_tab_pannel_buttons(TabPannel* tp, SDL_Renderer* renderer)
{
    for (int i = 0; i < tp->pannelCount; ++i)
        button_basic_render(renderer, tp->tabButtons[i]);
}

void destroy_tab_pannel_buttons(TabPannel* tp)
{
    for (int i = 0; i < tp->pannelCount; ++i)
        free_texture(tp->tabButtons[i]->text);
}


/* Drop down menu funtions */

DropdownMenu* dropdown_menu_init(Arena* arena, UIController* uiController, SDL_Renderer* renderer, uint8_t maxCount, const char* label,
                                 TTF_Font* font, uint8_t fontSize, int x, int y, int w, int h, SDL_Color color)
{
    DropdownMenu* ddm = arena_alloc(arena, sizeof(DropdownMenu), NULL);
    memset(ddm, 0, sizeof(DropdownMenu));
    ddm->eventEmitter = event_emitter_init(arena, 1);
    ddm->rect.x = x;
    ddm->rect.y = y;
    ddm->rect.w = w;
    ddm->rect.h = h;
    ddm->state = DROPDOWN_NORMAL;
    ddm->selectedButton = 0;
    ddm->count = 0;
    ddm->maxCount = maxCount;
    ddm->hidden = false;

    ddm->text = arena_alloc(arena, sizeof(Texture), NULL);
    if (fontSize != 0)
    {
        TTF_SetFontSize(font, fontSize);
        load_texture_from_rendered_text(ddm->text, w, label, font, color, renderer);
    }
    else
    {
        uint8_t i = DEFAULT_FONT_SIZE;
        do
        {
            assert(i > 0 && "Font size could not be adjusted to fit button text");

            if (i != DEFAULT_FONT_SIZE)
                free_texture(ddm->text);

            TTF_SetFontSize(font, i--);
            load_texture_from_rendered_text(ddm->text, w - 10, label, font, color, renderer);
        }
        while (ddm->text->height >= ddm->rect.h);
    }

    ddm->buttons = arena_alloc(arena, sizeof(DropdownButton*) * maxCount, NULL);

    for(int i = 0; i < maxCount; ++i)
        ddm->buttons[i] = NULL;

    ui_elem_add(uiController, ddm, DROPDOWN_MENU_ELEM);

    return ddm;
}

DropdownButton* dropdown_button_init(Arena* arena, SDL_Renderer* renderer, TTF_Font* font, uint8_t fontSize, SDL_Color color,
                                     const char* text, int x, int y, int w, int h)
{
    DropdownButton* b = arena_alloc(arena, sizeof(DropdownButton), NULL);
    memset(b, 0, sizeof(DropdownButton));
    b->rect.x = x;
    b->rect.y = y;
    b->rect.w = w;
    b->rect.h = h;
    b->state = BUTTON_STATE_NORMAL;
    b->text = arena_alloc(arena, sizeof(Texture), NULL);

    if (fontSize != 0)
    {
        TTF_SetFontSize(font, fontSize);
        load_texture_from_rendered_text(b->text, w, text, font, color, renderer);
    }
    else
    {
        uint8_t i = DEFAULT_FONT_SIZE;
        do
        {
            assert(i > 0 && "Font size could not be adjusted to fit button text");

            if (i != DEFAULT_FONT_SIZE)
                free_texture(b->text);

            TTF_SetFontSize(font, i--);
            load_texture_from_rendered_text(b->text, w - 10, text, font, color, renderer);
        }
        while (b->text->height >= b->rect.h);
    }

    return b;
}

void menu_button_reinit(DropdownButton* b, SDL_Renderer* renderer, TTF_Font* font, uint8_t fontSize, SDL_Color color,
                        const char* text, int x, int y, int w, int h)
{
    b->rect.x = x;
    b->rect.y = y;
    b->rect.w = w;
    b->rect.h = h;
    b->state = BUTTON_STATE_NORMAL;

    free_texture(b->text);
    if (fontSize != 0)
    {
        TTF_SetFontSize(font, fontSize);
        load_texture_from_rendered_text(b->text, w, text, font, color, renderer);
    }
    else
    {
        uint8_t i = DEFAULT_FONT_SIZE;
        do
        {
            assert(i > 0 && "Font size could not be adjusted to fit button text");

            if (i != DEFAULT_FONT_SIZE)
                free_texture(b->text);

            TTF_SetFontSize(font, i--);
            load_texture_from_rendered_text(b->text, w - 20, text, font, color, renderer);
        }
        while (b->text->height >= b->rect.h);
    }
}

void dropdown_menu_populate(Arena* arena, SDL_Renderer* renderer, TTF_Font* font, DropdownMenu* menu,
                            const char* textString, uint8_t fontSize, SDL_Color color)
{

    int index = 0;
    uint8_t count = 1;
    while(textString[index] != '\0')
    {
        if (textString[index++] == '\n')
            count++;
    }
    assert(count < menu->maxCount);

    menu->count = 0;
    SDL_Rect r = menu->rect;
    int k = 0;

    for(int i = 0; i < count; ++i)
    {
        char buffer[256];
        int j = 0;
        while(textString[k] != '\n' && k < index)
            buffer[j++] = textString[k++];
        buffer[j] = '\0';
        ++k;

        if (menu->buttons[i] == NULL)
            menu->buttons[i] = dropdown_button_init(arena, renderer, font, fontSize, color, buffer, r.x, r.y + ((i +1) * r.h), r.w -20, r.h);
        else
            menu_button_reinit(menu->buttons[i], renderer, font, fontSize, color, buffer, r.x, r.y + ((i+1) * r.h), r.w -20, r.h);

        menu->count++;
    }
}

int dropdown_menu_add_listener(Arena* arena, DropdownMenu* ddm, EventCallback cb, void* userData)
{
    assert(ddm->eventEmitter->count < ddm->eventEmitter->maxCount);

    EventNode* en = arena_alloc(arena, sizeof(EventNode), NULL);
    en->cb = cb;
    en->userData = userData;
    en->type = EVENT_TYPE_CLICK;

    ddm->eventEmitter->listeners[ddm->eventEmitter->count++] = en;
    return 1;
}
void dropdown_menu_select(DropdownMenu* ddm)
{
    if (!ddm) return;
    Event ev;
    ev.type = EVENT_TYPE_CLICK;
    ev.sourceObj = ddm;
    event_emitter_emit(ddm->eventEmitter, &ev);
}

void dropdown_menu_mouseclick_check(UIController* uiC, DropdownMenu* ddm, int mx, int my)
{
    switch (ddm->state)
    {
    case DROPDOWN_NORMAL:
        if(mouse_in_intbox_check(mx, my, &ddm->rect))
        {
            ddm->state = DROPDOWN_EXPANDED;
            uiC->elemFocus = ddm;
            uiC->focusedType = DROPDOWN_MENU_ELEM;
        }
        break;
    case DROPDOWN_EXPANDED:
        for(int i = 0; i < ddm->count; ++i)
        {
            DropdownButton* d = ddm->buttons[i];
            if(mouse_in_intbox_check(mx, my, &d->rect))
            {
                d->state = BUTTON_STATE_PRESSED;
                ddm->selectedButton = i;
                ddm->state = DROPDOWN_SELECTED;
                dropdown_menu_select(ddm);

                uiC->elemFocus = NULL;
                uiC->focusedType = NULL_ELEM;
                return;
            }
        }
        ddm->state = DROPDOWN_NORMAL;
        uiC->elemFocus = NULL;
        uiC->focusedType = NULL_ELEM;
        break;
    case DROPDOWN_SELECTED:
        if(mouse_in_intbox_check(mx, my, &ddm->rect))
        {
            ddm->state = DROPDOWN_EXPANDED;
            uiC->elemFocus = ddm;
            uiC->focusedType = DROPDOWN_MENU_ELEM;
        }
        break;
    }
}

void select_dropdown_menu_button(DropdownMenu* ddm, uint8_t buttonIndex)
{
    assert(buttonIndex <= ddm->count);
    ddm->selectedButton = buttonIndex;
    ddm->state = DROPDOWN_SELECTED;
}

void reset_dropdown_menu(DropdownMenu* ddm)
{
    ddm->state = DROPDOWN_NORMAL;
}

void dropdown_menu_mousemovement_check(DropdownMenu* ddm, int mx, int my)
{
    switch (ddm->state)
    {
    case DROPDOWN_EXPANDED:
        for(int i = 0; i < ddm->count; ++i)
        {
            DropdownButton* d = ddm->buttons[i];
            if(mouse_in_intbox_check(mx, my, &d->rect))
                d->state = BUTTON_STATE_HOVERED;
            else
                d->state = BUTTON_STATE_NORMAL;
        }
        break;
    default:
        break;
    }
}

void draw_filled_triangle(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Color colour);
void dropdown_menu_render(DropdownMenu* ddm, SDL_Renderer* renderer)
{
    if (ddm->hidden) return;

    switch(ddm->state)
    {
    case DROPDOWN_NORMAL:
    {
        SDL_Color color = COLOR[WHITE];
        SDL_Rect r = ddm->rect;

        //Button draw
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a -20);
        SDL_RenderFillRect(renderer, &r);

        //triangle
        SDL_Point v0 = {r.x + r.w - 18, r.y + (r.h / 2) -6};
        SDL_Point v1 = {r.x + r.w - 5, r.y + (r.h / 2) -6};
        SDL_Point v2 = {r.x + r.w -12, r.y + (r.h / 2) +6};
        draw_filled_triangle(renderer, v0, v1, v2, COLOR[TEAL]);

        //Text render
        SDL_FRect dst =
        {
            roundf(r.x + (((float)r.w -20) / 2) - (float)ddm->text->width / 2),
            roundf(r.y + ((float)r.h / 2) - (float)ddm->text->height / 2),
            (float)ddm->text->width,
            (float)ddm->text->height
        };
        SDL_RenderCopyF(renderer, ddm->text->mTexture, NULL, &dst);
        break;
    }
    case DROPDOWN_EXPANDED:
    {
        for (int i = 0; i < ddm->count; ++i)
        {
            DropdownButton* d = ddm->buttons[i];
            SDL_Color color;
            if (d->state == BUTTON_STATE_HOVERED)
                color = COLOR[LIGHT_GRAY];
            else
                color = COLOR[WHITE];

            SDL_Rect r = d->rect;
            SDL_Color boarder = COLOR[DARK_GRAY];

            //Button draw
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &r);

            SDL_SetRenderDrawColor(renderer, boarder.r, boarder.g, boarder.b, boarder.a);
            SDL_RenderDrawRect(renderer, &r);

            //Text render
            SDL_FRect dst =
            {
                roundf(r.x + ((float)r.w / 2) - (float)d->text->width / 2),
                roundf(r.y + ((float)r.h / 2) - (float)d->text->height / 2),
                (float)d->text->width,
                (float)d->text->height
            };
            SDL_RenderCopyF(renderer, d->text->mTexture, NULL, &dst);

        }
        //draw top of menu
        SDL_Color color = COLOR[WHITE];
        SDL_Rect r = ddm->rect;

        //Button draw
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a -20);
        SDL_RenderFillRect(renderer, &r);

        //Text render
        SDL_FRect dst =
        {
            roundf(r.x + ((float)r.w / 2) - (float)ddm->text->width / 2),
            roundf(r.y + ((float)r.h / 2) - (float)ddm->text->height / 2),
            (float)ddm->text->width,
            (float)ddm->text->height
        };
        SDL_RenderCopyF(renderer, ddm->text->mTexture, NULL, &dst);
        break;
    }
    case DROPDOWN_SELECTED:
    {
        SDL_Color color = COLOR[WHITE];
        SDL_Rect r = ddm->rect;

        //Button draw
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a -20);
        SDL_RenderFillRect(renderer, &r);

        //triangle
        SDL_Point v0 = {r.x + r.w - 18, r.y + (r.h / 2) -6};
        SDL_Point v1 = {r.x + r.w - 5, r.y + (r.h / 2) -6};
        SDL_Point v2 = {r.x + r.w -12, r.y + (r.h / 2) +6};
        draw_filled_triangle(renderer, v0, v1, v2, COLOR[TEAL]);

        //Text render
        SDL_FRect dst =
        {
            roundf(r.x + (((float)r.w -20) / 2) - (float)ddm->buttons[ddm->selectedButton]->text->width / 2),
            roundf(r.y + ((float)r.h / 2) - (float)ddm->buttons[ddm->selectedButton]->text->height / 2),
            (float)ddm->buttons[ddm->selectedButton]->text->width,
            (float)ddm->buttons[ddm->selectedButton]->text->height
        };
        SDL_RenderCopyF(renderer, ddm->buttons[ddm->selectedButton]->text->mTexture, NULL, &dst);

        break;
    }
    }
}



int dropdown_button_selected(DropdownMenu* ddm)
{
    if (ddm->state != DROPDOWN_SELECTED)
        return -1;

    return ddm->selectedButton;
}

void destroy_dropdown_menu(DropdownMenu* ddm)
{
    free_texture(ddm->text);
    for (int i = 0; i < ddm->maxCount; ++i)
    {
        if (ddm->buttons[i] != NULL)
            free_texture(ddm->buttons[i]->text);
    }
}

/* PopUp functions */

void popup_button_check(BasicButton* bb, SDL_Event* e)
{
    //printf("Popup Button Check: Event Type %d\n", e->type);

    if (e->type == SDL_MOUSEMOTION)
    {
        int mx = e->motion.x;
        int my = e->motion.y;
        if (mouse_in_intbox_check(mx, my, &bb->rect))
            bb->state = BUTTON_STATE_HOVERED;
        else
            bb->state = BUTTON_STATE_NORMAL;
    }
    else if (e->type == SDL_MOUSEBUTTONUP)
    {
        int mx = e->button.x;
        int my = e->button.y;
        if (mouse_in_intbox_check(mx, my, &bb->rect) && bb->state == BUTTON_STATE_PRESSED)
            bb->state = BUTTON_STATE_HOVERED;
        else
            bb->state = BUTTON_STATE_NORMAL;
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = e->button.x;
        int my = e->button.y;
        if (mouse_in_intbox_check(mx, my, &bb->rect))
        {
            bb->state = BUTTON_STATE_PRESSED;
            button_basic_click(bb);
        }
    }
}


void notice_button_click(const Event* ev, void* userData)
{
    PopUpNotice* p = (PopUpNotice*)userData;
    p->displaying = false;
}


PopUpNotice* popup_notice_init(UIController* uiC, const char* notice, const char* button, TTF_Font* font, uint32_t width, uint32_t height,SDL_Color color)
{
    Arena* arena = arena_init(232, 8, false);

    PopUpNotice* popup = arena_alloc(arena, sizeof(PopUpNotice), NULL);
    popup->arena = arena;
    popup->window = create_arena_window_popup("PopUp", width, height, arena);
    popup->label = label_basic_init(arena, NULL, 0, 0, width, height / 2, notice, font, 0, color, popup->window->renderer);
    popup->button = button_basic_init(arena, NULL, width / 4, height / 2, width / 3, height / 3, button, font, 0, color, popup->window->renderer);
    popup->displaying = true;

    event_emitter_add_listener(arena, popup->button, BUTTON_BASIC_ELEM, notice_button_click, popup);
    //printf("size of popup %zu\n", popup->arena->current->used);
    //printf("size of allocated %zu\n", popup->arena->totalAllocated);


    ui_elem_add(uiC, popup, POPUP_NOTICE_ELEM);

    return popup;
}

void popup_notice_destroy(PopUpNotice* popup)
{
    free_texture(popup->label->text);
    free_texture(popup->button->text);
    destroy_window_ui(popup->window);

    arena_destroy(popup->arena);
}


/* Geometric Shapes Rendering */

/* Draws a triangle where the bottom is one vertex (v2) and top edge is v0-v1 (v0.y == v1.y) */
static void draw_flat_top_triangle(SDL_Renderer* renderer, SDL_Point v0, SDL_Point v1, SDL_Point v2)
{
    double invslope1 = (double)(v2.x - v0.x) / (v2.y - v0.y);
    double invslope2 = (double)(v2.x - v1.x) / (v2.y - v1.y);

    double curx1 = v2.x;
    double curx2 = v2.x;

    for (int y = v2.y; y >= v0.y; --y)
    {
        int x_start = (int)ceil(fmin(curx1, curx2));
        int x_end   = (int)floor(fmax(curx1, curx2));
        if (x_end >= x_start)
            SDL_RenderDrawLine(renderer, x_start, y, x_end, y);

        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

/* Public function: fills a triangle specified by three points */
void draw_outlined_triangle(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Color colour)
{
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
    SDL_RenderDrawLine(renderer, p3.x, p3.y, p1.x, p1.y);
}


/* Helper: swap two SDL_Points */
static void swap_point(SDL_Point* a, SDL_Point* b)
{
    SDL_Point tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Draws a triangle where the top is one vertex (v0) and bottom edge is v1-v2 (v1.y == v2.y) */
static void draw_flat_bottom_triangle(SDL_Renderer* renderer, SDL_Point v0, SDL_Point v1, SDL_Point v2)
{
    double invslope1 = (double)(v1.x - v0.x) / (v1.y - v0.y);
    double invslope2 = (double)(v2.x - v0.x) / (v2.y - v0.y);

    double curx1 = v0.x;
    double curx2 = v0.x;

    for (int y = v0.y; y <= v1.y; ++y)
    {
        int x_start = (int)ceil(fmin(curx1, curx2));
        int x_end   = (int)floor(fmax(curx1, curx2));
        if (x_end >= x_start)
            SDL_RenderDrawLine(renderer, x_start, y, x_end, y);

        curx1 += invslope1;
        curx2 += invslope2;
    }
}

/* Public function: fills a triangle specified by three points */
void draw_filled_triangle(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Color colour)
{
    if (!renderer) return;

    /* Set draw color */
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

    /* Sort points by y ascending: p1.y <= p2.y <= p3.y */
    SDL_Point v0 = p1, v1 = p2, v2 = p3;
    if (v0.y > v1.y) swap_point(&v0, &v1);
    if (v1.y > v2.y) swap_point(&v1, &v2);
    if (v0.y > v1.y) swap_point(&v0, &v1);

    /* Handle degenerate triangles (line or point) */
    if (v0.y == v2.y)
    {
        /* All on same horizontal line: draw a horizontal line between min x and max x */
        int minx = v0.x, maxx = v0.x;
        minx = SDL_min(minx, v1.x);
        maxx = SDL_max(maxx, v1.x);
        minx = SDL_min(minx, v2.x);
        maxx = SDL_max(maxx, v2.x);
        SDL_RenderDrawLine(renderer, minx, v0.y, maxx, v0.y);
        return;
    }

    /* If middle vertex shares same y as top -> flat-top triangle */
    if (v0.y == v1.y)
    {
        /* Ensure v0 is left and v1 is right on the top edge */
        if (v0.x > v1.x) swap_point(&v0, &v1);
        draw_flat_top_triangle(renderer, v0, v1, v2);
    }
    else if (v1.y == v2.y) /* flat-bottom triangle */
    {
        /* Ensure v1 is left and v2 is right on the bottom edge */
        if (v1.x > v2.x) swap_point(&v1, &v2);
        draw_flat_bottom_triangle(renderer, v0, v1, v2);
    }
    else
    {
        /* General triangle -- split into a flat-bottom and flat-top at y = v1.y */
        double alpha = (double)(v1.y - v0.y) / (double)(v2.y - v0.y);
        double split_x = v0.x + alpha * (v2.x - v0.x);
        SDL_Point vi = { (int)round(split_x), v1.y };

        /* Determine which side vi should be on (left or right) by comparing x */
        if (vi.x <= v1.x)
        {
            /* vi is left of v1 -> lower triangle v0,vi,v1 is flat-bottom; upper triangle vi,v1,v2 is flat-top */
            draw_flat_bottom_triangle(renderer, v0, vi, v1);
            /* For the top part we need vi,v1,v2 but top edge must be in order left/right */
            if (vi.x > v1.x) swap_point(&vi, &v1);
            draw_flat_top_triangle(renderer, vi, v1, v2);
        }
        else
        {
            /* vi is right of v1 */
            draw_flat_bottom_triangle(renderer, v0, v1, vi);
            if (v1.x > vi.x) swap_point(&v1, &vi);
            draw_flat_top_triangle(renderer, v1, vi, v2);
        }
    }
}


/* Destruction helper functions */
void destroy_sdl2_ui_complete(StringMemory* sm, FontHolder* fh, Arena* arena)
{
    quit_SDL2_ui();

    string_memory_destroy(sm);
    destroy_fonts(fh);
    arena_destroy(arena);
}

void destroy_window(WindowUI* windowUI, Arena* window_arena, StringMemory* sm, UIController* uiController)
{
    ui_controller_destroy(uiController);
    destroy_window_ui(windowUI);
    arena_destroy(window_arena);
}


/* Multi window functions */

WindowHolder* window_holder_init(Arena* mainArena, uint16_t maxCount)
{
    WindowHolder* wh = arena_alloc(mainArena, sizeof(WindowHolder), false);
    memset(wh, 0, sizeof(WindowHolder));
    wh->maxCount = maxCount;
    wh->count = 0;
    wh->windowController = arena_alloc(mainArena, sizeof(WindowController*) * maxCount, false);

    return wh;
};

WindowController* window_controller_init(WindowHolder* wh, StringMemory* sm, FontHolder* fh, const char* title, uint16_t width, uint16_t height, uint8_t uiElemMax)
{
    Arena* arena = arena_init(ARENA_BLOCK_SIZE, 8, false);
    WindowController* wc = arena_alloc(arena, sizeof(WindowController), false);
    memset(wc, 0, sizeof(WindowController));
    wc->window = create_arena_window_ui(title, width, height, arena, true, false);
    wc->uiController = ui_controller_init(arena, uiElemMax);
    wc->fh = fh;
    wc->sm = sm;
    wc->arena = arena;

    assert(wh->maxCount > wh->count);
    wh->windowController[wh->count++] = wc;

    return wc;
}

bool multi_window_event_check(WindowHolder* wh, SDL_Event* e, uint16_t* windowIndex)
{
    bool windowchanged = false;
    bool firstChange = true;
    for (int i = 0; i < wh->count; ++i)
    {
        WindowController* wc = wh->windowController[i];
        windowchanged = windowUI_update(wc->window, e);
        if (windowchanged && firstChange && windowIndex != NULL)
        {
            *windowIndex = i;
            firstChange = false;
        }

        ui_event_check(wc->arena, wc->sm, wc->uiController, wc->window, e);
    }
    return !firstChange;
}

void multi_window_ui_update(WindowHolder* wh, float deltaTime)
{
    for(int i = 0; i < wh->count; ++i)
        ui_update(wh->windowController[i]->uiController, deltaTime);
}

void multi_window_render(WindowHolder* wh, SDL_Color color)
{
    for (int i = 0; i < wh->count; ++i)
    {
        WindowController* wc = wh->windowController[i];
        clear_screen_with_color(wc->window->renderer, color);
        ui_render(wc->window->renderer, wc->uiController);
        SDL_RenderPresent(wc->window->renderer);
    }
}

void destroy_window_controller(WindowHolder* wh, WindowController* wc)
{
    for (int i = 0; i < wh->count; ++i)
    {
        if(wh->windowController[i] == wc)
        {
            destroy_window(wc->window, wc->arena, wc->sm, wc->uiController);
            wh->windowController[i] = wh->windowController[--wh->count];
            return;
        }
    }
    assert(false && "Window to destory not found");
}
