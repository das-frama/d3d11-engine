#include "input.h"

#include "platform/win32_platform.h"

static byte old_keys_state[BUTTON_COUNT] = {0};
static byte keys_state[BUTTON_COUNT] = {0};

static input g_input = {0};

void input_update() {
	// Update mouse.
	vec2 cur_pos = win32_mouse_pos();

	if (!g_input.is_init) {
		g_input.mouse.x = cur_pos.x;
 		g_input.mouse.y = cur_pos.y;
 		g_input.is_init = true;
	}

	if (g_input.enabled && (cur_pos.x != g_input.mouse.x || cur_pos.y != g_input.mouse.y)) {
		g_input.mouse.has_movement = true;
		g_input.mouse.dx = cur_pos.x - g_input.mouse.x;
		g_input.mouse.dy = cur_pos.y - g_input.mouse.y;
	} else {
		g_input.mouse.has_movement = false;
		g_input.mouse.dx = 0;
		g_input.mouse.dy = 0;
	}

	if (g_input.enabled) {
		float x = g_input.screen_area.left + g_input.screen_area.width / 2.0f;
		float y = g_input.screen_area.top + g_input.screen_area.height / 2.0f;
		win32_set_mouse_pos(x, y);
		g_input.mouse.x = x;
		g_input.mouse.y = y;
	} else {
		g_input.mouse.x = cur_pos.x;
		g_input.mouse.y = cur_pos.y;
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

void input_set_screen_area(rect r) {
	g_input.screen_area = r;
}

bool input_key_pressed(uchar key_code) {
	return g_input.keys[key_code].pressed;
}

bool input_key_released(uchar key_code) {
	return g_input.keys[key_code].released;
}