#include "SDL2_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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

    const char* text = "Limelight-Regular.ttf";
    const uint8_t DEFAULT_FONT_SIZE = 24;

    Arena* arena = arena_init(ARENA_BLOCK_SIZE, 8, NULL);
    StringMemory* string_memory = string_memory_init(arena);
    String* string = string_init(arena, string_memory);
    UIController* UIController = ui_controller_init(arena, 1);
    FontHolder* fh = font_holder_init(arena, 1);
    load_fonts(fh, text, DEFAULT_FONT_SIZE);
    TextBox* textbox = textbox_init(arena, UIController, string, fh->fonts[0], DEFAULT_FONT_SIZE, COLOR[WHITE], 50.0f, 50.0f, 400.0f, DEFAULT_FONT_SIZE + 10);

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
    destroy_complete_screen(&windowUI, arena, string_memory, UIController);



    return EXIT_SUCCESS;
}




