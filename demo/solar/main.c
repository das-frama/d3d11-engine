#include <motor.h>

int main() {
    motor_init("My Game", 1920, 1080);

    // Entities.
    camera* cam  = camera_new(0, 0, 0);
    skybox* sky  = skybox_new();
    grid*   grid = grid_new(1024, 1024, 64, 64, color(.5f, .5f, .5f, 0.8f));


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
        skybox_update(sky, cam);
        grid_update(grid, cam);

        // Draw.
        graphics_clear(0.0f, 0.0f, 0.0f, 1.0f);
        skybox_draw(sky);
        grid_draw(grid);
        graphics_present();

        frame_end();
    }

    grid_delete(grid);
    skybox_delete(sky);
    camera_delete(cam);
    motor_close();

    return EXIT_SUCCESS;
}