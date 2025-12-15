#include "../lib/Cimple_ui/Cimple_ui.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    UIController* uiController;
    TTF_Font* font;
} PopupNoticeData;

/* Example callback */
void on_button_click(const Event* ev, void* userData)
{
    PopupNoticeData* data = (PopupNoticeData*)userData;
    popup_notice_init(data->uiController, "Here is the notice", "here  wanna button", data->font, 200, 100, COLOR[BLUE]);
}

void on_close_button(const Event* ev, void* userData)
{
    bool *quit = (bool*)userData;
    *quit = true;
}

int main(int argc, char* argv[])
{
    //printf("SDL Window size: %dx%d\n", wc[]->window.width, wc[]->window.height);
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;

    const char* text = "src/assets/fonts/Limelight-Regular.ttf";

    Arena* mainArena = arena_init(456, 8, false);
    StringMemory* string_memory = string_memory_init(mainArena);
    FontHolder* fh = font_holder_init(mainArena, 1);
    load_fonts(fh, text, DEFAULT_FONT_SIZE);
    WindowHolder* wh = window_holder_init(mainArena, 32);

    WindowController* wc = window_controller_init(wh, string_memory, fh, "one", 800, 600, 16);

    {
        Arena* arena = wc->arena;
        UIController* UIController = wc->uiController;
        TabPannel* tp = tab_pannel_init(arena, UIController, wc->window, "Tab1|Tab2|Tab3", TABPANNEL_BUTTOM, 50, 16, fh->fonts[0], 0, COLOR[GRAY]);
        TextBox* textbox = textbox_init(arena, UIController, string_memory, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50.0f, 50.0f, 400.0f, DEFAULT_FONT_SIZE + 10);
        TextField* textfield = textfield_init(arena, UIController, string_memory, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50, 50, 400, DEFAULT_FONT_SIZE + 10);
        BasicButton* quitB = button_basic_init(arena, UIController, 700, 700, 80, 50, "Quit", fh->fonts[0], 0, COLOR[RED], wc->window->renderer);
        event_emitter_add_listener(arena, quitB, BUTTON_BASIC_ELEM, on_close_button, &quit);
        BasicButton* button = button_basic_init(arena, UIController, 500, 50, 150, 50, "Click Me lolo", fh->fonts[0], 0, COLOR[BLUE], wc->window->renderer);
        //event emmitter
        PopupNoticeData data = {UIController, fh->fonts[0]};
        event_emitter_add_listener(arena, button, BUTTON_BASIC_ELEM, on_button_click, &data);


        DropdownMenu* ddm = dropdown_menu_init(arena, UIController, wc->window->renderer, 16, "Here it comes :)", fh->fonts[0], 0, 500, 500, 200, 50, COLOR[BLACK]);
        event_emitter_add_listener(arena, ddm, DROPDOWN_MENU_ELEM, on_button_click, &data);
        dropdown_menu_populate(arena, wc->window->renderer, fh->fonts[0], ddm, "number one\nnumber two\n3333??", 0, COLOR[BLACK]);

        add_elem_to_pannel(textbox, TEXTBOX_ELEM, tp, 2);
        add_elem_to_pannel(button, BUTTON_BASIC_ELEM, tp, 2);
        add_elem_to_pannel(quitB, BUTTON_BASIC_ELEM, tp, 2);
        add_elem_to_pannel(ddm, DROPDOWN_MENU_ELEM, tp, 3);
        add_elem_to_pannel(textfield, TEXTFIELD_ELEM, tp, 1);
    }

    WindowController* wc1 = window_controller_init(wh, string_memory, fh, "two", 640, 480, 16);
    {
        Arena* arena = wc1->arena;
        UIController* UIController = wc1->uiController;
        TabPannel* tp = tab_pannel_init(arena, UIController, wc1->window, "Tab1|Tab2|Tab3", TABPANNEL_BUTTOM, 50, 16, fh->fonts[0], 0, COLOR[GRAY]);
        TextBox* textbox = textbox_init(arena, UIController, string_memory, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50.0f, 50.0f, 400.0f, DEFAULT_FONT_SIZE + 10);
        TextField* textfield = textfield_init(arena, UIController, string_memory, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50, 50, 400, DEFAULT_FONT_SIZE + 10);
        BasicButton* quitB = button_basic_init(arena, UIController, 700, 700, 80, 50, "Quit", fh->fonts[0], 0, COLOR[RED], wc1->window->renderer);
        event_emitter_add_listener(arena, quitB, BUTTON_BASIC_ELEM, on_close_button, &quit);
        BasicButton* button = button_basic_init(arena, UIController, 500, 50, 150, 50, "Click Me lolo", fh->fonts[0], 0, COLOR[BLUE], wc1->window->renderer);
        //event emmitter
        PopupNoticeData data = {UIController, fh->fonts[0]};
        event_emitter_add_listener(arena, button, BUTTON_BASIC_ELEM, on_button_click, &data);


        DropdownMenu* ddm = dropdown_menu_init(arena, UIController, wc1->window->renderer, 16, "Here it comes :)", fh->fonts[0], 0, 500, 500, 200, 50, COLOR[BLACK]);
        event_emitter_add_listener(arena, ddm, DROPDOWN_MENU_ELEM, on_button_click, &data);
        dropdown_menu_populate(arena, wc1->window->renderer, fh->fonts[0], ddm, "number one\nnumber two\n3333??", 0, COLOR[BLACK]);

        add_elem_to_pannel(textbox, TEXTBOX_ELEM, tp, 2);
        add_elem_to_pannel(button, BUTTON_BASIC_ELEM, tp, 2);
        add_elem_to_pannel(quitB, BUTTON_BASIC_ELEM, tp, 2);
        add_elem_to_pannel(ddm, DROPDOWN_MENU_ELEM, tp, 3);
        add_elem_to_pannel(textfield, TEXTFIELD_ELEM, tp, 1);
    }


    Uint32 lastTime = SDL_GetTicks();
    while (!quit)
    {

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.key.keysym.sym == SDLK_c && e.type == SDL_KEYDOWN)
            {
                quit = true;
            }
            uint16_t windowID = 0;
            if(multi_window_event_check(wh, &e, &windowID))
            {
            }

        }
        Uint32 currentTimeDelta = SDL_GetTicks();
        float deltaTime = (currentTimeDelta - lastTime) / 1000.0f;
        lastTime = currentTimeDelta;

        // Update UI
        multi_window_ui_update(wh, deltaTime);

        // Multi window rending function has clear - render and then present all together
        multi_window_render(wh, COLOR[BLACK]);

    }

    while(wh->count > 0)
    {
        destroy_window_controller(wh, wh->windowController[0]);
    }

    destroy_sdl2_ui_complete(string_memory, fh, mainArena);



    return EXIT_SUCCESS;
}




