#include "Cimple_ui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
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

    if (!init_TTF()) return false;

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

void destroy_SDL2_ui(WindowUI* window)
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
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

/* Font Holder */

FontHolder* font_holder_init(Arena* arena, uint8_t maxFonts)
{
    FontHolder* fb = arena_alloc(arena, sizeof(FontHolder), NULL);
    fb->fonts = arena_alloc(arena, sizeof(TTF_Font*) * maxFonts, NULL);
    fb->count = 0;
    fb->maxCount = maxFonts;
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

bool mouse_in_intbox_check(int mx, int my, SDL_Rect* rect)
{
    return mx >= rect->x && mx <= rect->x + rect->w && my >= rect->y && my <= rect->y + rect->h;
}

bool mouse_in_box_check(int mx, int my, SDL_FRect* rect)
{
    return mx >= rect->x && mx <= rect->x + rect->w && my >= rect->y && my <= rect->y + rect->h;
}

void button_basic_click(BasicButton* bb);

void ui_event_check(Arena* arena, StringMemory* sm, UIController* uiController, SDL_Event* e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = e->button.x;
        int my = e->button.y;
        for(int i = 0; i < uiController->count; ++i)
        {
            switch(uiController->type[i])
            {
            case TEXTBOX_ELEM:
            {
                TextBox* tb = (TextBox*)uiController->element[i];
                if (mouse_in_box_check(mx, my, &tb->rect))
                {
                    tb->focused = true;
                    tb->caret = true;
                    uiController->elemFocus = tb;
                    uiController->focusedType = TEXTBOX_ELEM;
                    SDL_StartTextInput();
                    SDL_Rect input = {(int)tb->rect.x, (int)tb->rect.y, (int)tb->rect.w, (int)tb->rect.h};
                    SDL_SetTextInputRect(&input);
                }
                else if (tb->focused)
                {
                    tb->focused = false; //if mouse click happens outside the box change it to false
                    tb->caret = false;
                    uiController->elemFocus = NULL;
                    uiController->focusedType = NULL_ELEM;
                    SDL_StopTextInput();
                }
                break;
            }
            case BUTTON_BASIC_ELEM:
            {
                BasicButton* bb = (BasicButton*)uiController->element[i];
                if (mouse_in_intbox_check(mx, my, &bb->rect))
                {
                    bb->state = BUTTON_STATE_PRESSED;
                    button_basic_click(bb);
                }
                break;
            }
            case NULL_ELEM:
                assert(false);
                break;
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
                if (mouse_in_intbox_check(mx, my, &bb->rect) && bb->state == BUTTON_STATE_PRESSED)
                    bb->state = BUTTON_STATE_HOVERED;
                else
                    bb->state = BUTTON_STATE_NORMAL;
                break;
            }
            case TEXTBOX_ELEM:
            case NULL_ELEM:
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
                BasicButton* bb = (BasicButton*)uiController->element[i];
                if (mouse_in_intbox_check(mx, my, &bb->rect))
                    bb->state = BUTTON_STATE_HOVERED;
                else
                    bb->state = BUTTON_STATE_NORMAL;
                break;
            }
            case TEXTBOX_ELEM:
            case NULL_ELEM:
                break;
            }
        }
    }

    if (uiController->elemFocus == NULL) return;

    if (e->type == SDL_TEXTINPUT)
    {

        switch(uiController->focusedType)
        {
        case TEXTBOX_ELEM:
        {
            TextBox* tb = (TextBox*)uiController->elemFocus;
            assert(tb->focused);
            textbox_append_text(arena, sm, tb, e->text.text);
            break;
        }
        case NULL_ELEM:
            assert(false);
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
            }
        }
    }
}

void ui_update(UIController* uiC, float deltaTime)
{
    if (uiC->elemFocus != NULL)
    {
        uiC->secondCounter += deltaTime;
        {
            switch(uiC->focusedType)
            {
            case TEXTBOX_ELEM:
            {
                TextBox* tb = (TextBox*)uiC->elemFocus;
                assert(tb->focused);
                {
                    if (uiC->secondCounter >= 0.5f)
                        tb->caret = !tb->caret;

                }
                break;
            }
            case NULL_ELEM:
                assert(false);
                break;
            }
        }
        if (uiC->secondCounter >= 0.5f)
            uiC->secondCounter = 0.0f;
    }
    else
    {
        uiC->secondCounter = 0.0f;
    }
}

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
        case NULL_ELEM:
            assert(false);
            break;
        }
    }
}

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
        case BUTTON_BASIC_ELEM:
        {
            BasicButton* bb = (BasicButton*)uiC->element[i];
            free_texture(bb->text);
            break;
        }
        case NULL_ELEM:
            assert(false);
            break;
        }
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
    case TEXTBOX_ELEM:
        break;
    case NULL_ELEM:
        assert(false);

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

TextBox* textbox_init(Arena* arena, UIController* uiC, String* string, TTF_Font* font, uint8_t fontSize, SDL_Color color, float x, float y, float width, float height)
{
    TextBox* tb = arena_alloc(arena, sizeof(TextBox), NULL);
    tb->texture = arena_alloc(arena, sizeof(Texture), NULL);
    init_texture(tb->texture);

    tb->fontSize = fontSize;
    tb->color = color;
    tb->font = font;
    tb->string = string;
    tb->focused = false;
    tb->caret = false;
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


    //caret render
    /*
    if(textbox->focused && textbox->caret)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 185);
        SDL_RenderDrawLineF(renderer, r.x + 2 + (textbox->texture->width), r.y + r.h - (textbox->fontSize + 2), r.x +2 + (textbox->texture->width),r.y +r.h -3);
        SDL_RenderDrawLineF(renderer, r.x + 3 + (textbox->texture->width), r.y + r.h - (textbox->fontSize + 2), r.x +3 + (textbox->texture->width),r.y +r.h -3);
    }
    */
}

void textbox_append_text(Arena* arena, StringMemory* sm, TextBox* textbox, const char* text)
{
    String* s = textbox->string;
    if (string_c_append(arena, &s, sm, text) == 1)
        textbox->string = s;

    textbox->textChanged = true;
}


/* Button funtions */

BasicButton* button_basic_init(Arena* arena, UIController* uiController, int x, int y, int width, int height, const char* text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer)
{
    BasicButton* bb = arena_alloc(arena, sizeof(BasicButton), NULL);
    bb->text = arena_alloc(arena, sizeof(Texture), NULL);
    bb->eventEmitter = event_emitter_init(arena, 1);
    bb->color = color;
    bb->rect.x = x;
    bb->rect.y = y;
    bb->rect.w = width;
    bb->rect.h = height;
    bb->state = BUTTON_STATE_NORMAL;

    uint8_t i = DEFAULT_FONT_SIZE;
    do
    {
        assert(i > 0 && "Font size could not be adjusted to fit button text");

        if (i != DEFAULT_FONT_SIZE)
            free_texture(bb->text);

        TTF_SetFontSize(font, i--);
        load_texture_from_rendered_text(bb->text, width - 20, text, font, color, renderer);
    }
    while (bb->text->height >= bb->rect.h);

    ui_elem_add(uiController, bb, BUTTON_BASIC_ELEM);
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
    SDL_RenderCopy(renderer, button->text->mTexture, NULL, &button->rect);
}

void destroy_window(WindowUI* windowUI, Arena* arena, StringMemory* sm, UIController* uiController)
{
    string_memory_destroy(sm);
    ui_controller_destroy(uiController);
    arena_destroy(arena);
    destroy_SDL2_ui(windowUI);
}
