#include "input_handler.h"

#include <irrlicht/irrlicht.h>

Input_handler::Input_handler() {
	//WASD movement
	key_action_map[irr::KEY_KEY_W] = Action::go_forward;
	key_action_map[irr::KEY_KEY_A] = Action::go_left;
	key_action_map[irr::KEY_KEY_S] = Action::go_backward;
	key_action_map[irr::KEY_KEY_D] = Action::go_right;
	//arrow key movement
	key_action_map[irr::KEY_UP] = Action::go_forward;
	key_action_map[irr::KEY_LEFT] = Action::go_left;
	key_action_map[irr::KEY_DOWN] = Action::go_backward;
	key_action_map[irr::KEY_RIGHT] = Action::go_right;
}

bool Input_handler::is_action_happening(Input_handler::Action action) const {
	return continuous_actions[action];
}

bool Input_handler::OnEvent(const irr::SEvent &event) {
	switch (event.EventType) {
	case irr::EET_KEY_INPUT_EVENT: {
		//set what actions are happening
		continuous_actions[key_action_map[event.KeyInput.Key]] = event.KeyInput.PressedDown;
		//call instant action function if set
		auto &f = instant_actions[key_action_map[event.KeyInput.Key]];
		if (f) {
			f();
		}
	} break;
	case irr::EET_MOUSE_INPUT_EVENT: {
		//call instant action function if set
		auto &f = instant_actions[mouse_action_map[event.MouseInput.Event]];
		if (f) {
			f();
		}
		if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL) {
			int scroll = event.MouseInput.Wheel;
			while (scroll > 0) {
				auto &f = instant_actions[mouse_wheel[Mouse_wheel_up]];
				if (f)
					f();
				scroll--;
			}
			while (scroll < 0) {
				auto &f = instant_actions[mouse_wheel[Mouse_wheel_down]];
				if (f)
					f();
				scroll++;
			}
		}
	} break;
	default:
		break;
	}
	return false;
}
