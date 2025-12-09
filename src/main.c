#include "../lib/Cimple_ui/Cimple_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


typedef struct
{
    UIController* uiController;
    TTF_Font* font;
} PopupNoticeData;

/* Example callback (C) */
void on_button_click(const Event* ev, void* userData)
{
    PopupNoticeData* data = (PopupNoticeData*)userData;
    popup_notice_init(data->uiController, "Here is the notice", "here i wanna button", data->font, 200, 100, COLOR[BLUE]);
}

void on_close_button(const Event* ev, void* userData)
{
    bool *quit = (bool*)userData;
    *quit = true;
}

int main(int argc, char* argv[])
{
    WindowUI windowUI;
    if (!init_SDL2_ui(&windowUI, "SDL2 UI Example", 800, 800, true, false))
    {
        printf("Failed to initialize SDL2 UI\n");
        return EXIT_FAILURE;
    }
    uint32_t screen_width = windowUI.width;
    uint32_t screen_height = windowUI.height;

    //printf("SDL Window size: %dx%d\n", windowUI.width, windowUI.height);
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;

    const char* text = "src/assets/fonts/Limelight-Regular.ttf";

    Arena* arena = arena_init(ARENA_BLOCK_SIZE, 8, true);
    StringMemory* string_memory = string_memory_init(arena);
    UIController* UIController = ui_controller_init(arena, 16);
    FontHolder* fh = font_holder_init(arena, 1);
    load_fonts(fh, text, DEFAULT_FONT_SIZE);
    TabPannel* tp = tab_pannel_init(arena, UIController, &windowUI, "Tab1|Tab2|Tab3", TABPANNEL_BUTTOM, 50, 16, fh->fonts[0], 0, COLOR[GRAY]);
    TextBox* textbox = textbox_init(arena, UIController, string_memory, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50.0f, 50.0f, 400.0f, DEFAULT_FONT_SIZE + 10);
    BasicButton* quitB = button_basic_init(arena, UIController, 700, 700, 80, 50, "Quit", fh->fonts[0], 0, COLOR[RED], windowUI.renderer);
    event_emitter_add_listener(arena, quitB, BUTTON_BASIC_ELEM, on_close_button, &quit);
    BasicButton* button = button_basic_init(arena, UIController, 500, 50, 150, 50, "Click Me lolo", fh->fonts[0], 0, COLOR[BLUE], windowUI.renderer);
    //event emmitter
    PopupNoticeData data = {UIController, fh->fonts[0]};
    event_emitter_add_listener(arena, button, BUTTON_BASIC_ELEM, on_button_click, &data);


    DropdownMenu* ddm = dropdown_menu_init(arena, UIController, windowUI.renderer, 16, "Here it comes :)", fh->fonts[0], 0, 500, 500, 200, 50, COLOR[BLACK]);
    event_emitter_add_listener(arena, ddm, DROPDOWN_MENU_ELEM, on_button_click, &data);
    dropdown_menu_populate(arena, windowUI.renderer, fh->fonts[0], ddm, "number one\nnumber two\n3333??", 0, COLOR[BLACK]);

    add_elem_to_pannel(textbox, TEXTBOX_ELEM, tp, 2);
    add_elem_to_pannel(button, BUTTON_BASIC_ELEM, tp, 2);
    add_elem_to_pannel(quitB, BUTTON_BASIC_ELEM, tp, 2);
    add_elem_to_pannel(ddm, DROPDOWN_MENU_ELEM, tp, 3);

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
            if (windowUI_update(&windowUI, &e) || windowUI_fullscreen(&windowUI, &e))
            {
                screen_width = windowUI.width;
                screen_height = windowUI.height;
            }
            ui_event_check(arena, string_memory, UIController, &windowUI, &e);
        }
        Uint32 currentTimeDelta = SDL_GetTicks();
        float deltaTime = (currentTimeDelta - lastTime) / 1000.0f;
        lastTime = currentTimeDelta;

        // Update UI
        ui_update(UIController, deltaTime);

        // Clear screen
        clear_screen_with_color(windowUI.renderer, COLOR[BLACK]);

        // Render UI
        ui_render(windowUI.renderer, UIController);
        // Update screen
        SDL_RenderPresent(windowUI.renderer);
    }

    destroy_fonts(fh);
    destroy_window(&windowUI, arena, string_memory, UIController);



    return EXIT_SUCCESS;
}




