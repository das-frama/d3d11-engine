#include "input.h"

#include "platform/win32_platform.h"

static uchar old_keys_state[BUTTON_COUNT] = {0};
static uchar keys_state[BUTTON_COUNT] = {0};

void input_update(input* in) {
	// Update mouse.
	vec2 cur_pos = win32_mouse_pos();

	if (cur_pos.x != in->mouse.x || cur_pos.y != in->mouse.y) {
		in->mouse.has_movement = true;
	} else {
		in->mouse.has_movement = false;
	}

	in->mouse.x = cur_pos.x;
	in->mouse.y = cur_pos.y;

	int w, h;
	win32_size(&w, &h);
	win32_set_mouse_pos(vec2_new(w / 2.0f, h / 2.0f));

	// Update keyboard.
	win32_keyboard_state(keys_state);
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
			if (keys_state[i] != old_keys_state[i]) {
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
			} else {
				in->keys[i].pressed = false;
				in->keys[i].released = false;
			}
		}
	}

	memcpy(old_keys_state, keys_state, sizeof(uchar) * BUTTON_COUNT);
}