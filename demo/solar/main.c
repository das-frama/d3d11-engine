#include <motor.h>

int main() {
    motor_init("My Game", 1920, 1080);

    camera* cam = camera_new(0, 0, 0);

    // entity* grid = 
    grid* world_grid = grid_new(1024, 1024, 64, 64, color(.5f, .5f, .5f, .25f));

    // Main game loop.
    while (!game_should_quit()) {
        frame_begin();

        // Update window events.
        window_events();
        
        // Update input.
        input_update();
        if (input_key_released('G')) {
            game_switch_playmode();
        }
        if (input_key_pressed(27)) {
            game_should_stop_next();
        }

        // Update logic.
        camera_update(cam, frame_dt());
        grid_update(world_grid, cam);

        // Draw.
        graphics_clear(0.0f, 0.0f, 0.0f, 1.0f);
        grid_draw(world_grid);
        graphics_present();

        frame_end();
    }

    grid_delete(world_grid);
    camera_delete(cam);
    motor_close();

    return EXIT_SUCCESS;
}