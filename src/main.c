#include "../lib/Cimple_ui/Cimple_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


/* Example callback (C) */
void on_button_click(const Event* ev, void* user_data)
{
    TextBox* tb = (TextBox*)user_data;
    tb->string->count = 0;
}

int main(int argc, char* argv[])
{
    WindowUI windowUI;
    if (!init_SDL2_ui(&windowUI, "SDL2 UI Example", 1000, 1000, true, false))
    {
        printf("Failed to initialize SDL2 UI\n");
        return EXIT_FAILURE;
    }
    uint32_t screen_width = windowUI.width;
    uint32_t screen_height = windowUI.height;

    printf("SDL Window size: %dx%d\n", windowUI.width, windowUI.height);
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;

    const char* text = "src/assets/fonts/Limelight-Regular.ttf";

    Arena* arena = arena_init(ARENA_BLOCK_SIZE, 8, true);
    StringMemory* string_memory = string_memory_init(arena);
    UIController* UIController = ui_controller_init(arena, 3);
    FontHolder* fh = font_holder_init(arena, 1);
    load_fonts(fh, text, DEFAULT_FONT_SIZE);
    TextBox* textbox = textbox_init(arena, UIController, string_memory, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50.0f, 50.0f, 400.0f, DEFAULT_FONT_SIZE + 10);
    BasicButton* button = button_basic_init(arena, UIController, 500, 50, 150, 50, "Click Me lolo", fh->fonts[0], COLOR[BLUE], windowUI.renderer);
    Label* label = label_basic_init(arena, UIController, 900, 900, 400, 50, "Press C to quit", fh->fonts[0], COLOR[GREEN], windowUI.renderer);
    //event emmitter
    event_emitter_add_listener(arena, button, BUTTON_BASIC_ELEM, on_button_click, textbox);

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
            ui_event_check(arena, string_memory, UIController, &e);
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




