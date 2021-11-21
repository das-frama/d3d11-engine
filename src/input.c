#include "input.h"

#include "platform/win32_platform.h"

static byte old_keys_state[BUTTON_COUNT] = {0};
static byte keys_state[BUTTON_COUNT] = {0};

static input g_input = {0};

void input_update() {
	// Update mouse.
	vec2 cur_pos = win32_mouse_pos();

	if (cur_pos.x != g_input.mouse.x || cur_pos.y != g_input.mouse.y) {
		g_input.mouse.has_movement = true;
	} else {
		g_input.mouse.has_movement = false;
	}

	g_input.mouse.x = cur_pos.x;
	g_input.mouse.y = cur_pos.y;

	if (g_input.enabled) {
		int w = 0, h = 0;
		win32_size(&w, &h);
		float x = w / 2.0f;
		float y = h / 2.0f;
		win32_set_mouse_pos(x, y);
	}

	// Update keyboard.
	win32_keyboard_state(keys_state);
	for (int i = 0; i < BUTTON_COUNT-1; i++) {
		if (keys_state[i] & 0x80) { // pressed
			if (i == VK_LBUTTON) { // left button
			 	g_input.mouse.left_pressed = true;
			 	g_input.mouse.left_released = false;
			} else if (i == VK_RBUTTON) {
			 	g_input.mouse.right_pressed = true;
			 	g_input.mouse.right_released = false;
			} else {
				g_input.keys[i].pressed = true;
				g_input.keys[i].released = false;
			}
		} else { // released
			if (keys_state[i] != old_keys_state[i]) {
				if (i == VK_LBUTTON) { // left button
				 	g_input.mouse.left_pressed = false;
				 	g_input.mouse.left_released = true;
				} else if (i == VK_RBUTTON) {
				 	g_input.mouse.right_pressed = false;
				 	g_input.mouse.right_released = true;
				} else {
					g_input.keys[i].pressed = false;
					g_input.keys[i].released = true;
				}
			} else {
				g_input.keys[i].pressed = false;
				g_input.keys[i].released = false;
			}
		}
	}

	memcpy(old_keys_state, keys_state, sizeof(byte) * BUTTON_COUNT);
}

input* input_get() {
	return &g_input;
}

void input_enable(bool enable) {
	g_input.enabled = enable;
}

bool input_key_pressed(uchar key_code) {
	return g_input.keys[key_code].pressed;
}

bool input_key_released(uchar key_code) {
	return g_input.keys[key_code].released;
}