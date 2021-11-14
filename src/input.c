#include "input.h"

#include "platform/win32_platform.h"

// static old_keys_state[BUTTON_COUNT] = {0};
// static keys_state[BUTTON_COUNT] = {0};

void input_update(input* in) {
	vec2 pos = win32_mouse_pos();
	in->mouse.x = pos.x;
	in->mouse.y = pos.y;

	uchar* keys_state = win32_keyboard_state();
	for (int i = 0; i < BUTTON_COUNT-1; i++) {
		if (keys_state[i] & 0x80) { // pressed
			if (i == VK_LBUTTON) { // left button
			 	in->mouse.left_pressed = true;
			 	in->mouse.left_released = false;
			} else if (i == VK_RBUTTON) {
			 	in->mouse.right_pressed = true;
			 	in->mouse.right_released = false;
			} else {
				in->keys[i].pressed = true;
				in->keys[i].released = false;
			}
		} else { // released
			if (i == VK_LBUTTON) { // left button
			 	in->mouse.left_pressed = false;
			 	in->mouse.left_released = true;
			} else if (i == VK_RBUTTON) {
			 	in->mouse.right_pressed = false;
			 	in->mouse.right_released = true;
			} else {
				in->keys[i].pressed = false;
				in->keys[i].released = true;
			}
		}
	}

	free(keys_state);
}