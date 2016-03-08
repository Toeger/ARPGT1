#include "input_handler.h"

Input_handler::Input_handler()
{
	//WASD movement
	action_map[irr::KEY_KEY_W] = Action::go_forward;
	action_map[irr::KEY_KEY_A] = Action::go_left;
	action_map[irr::KEY_KEY_S] = Action::go_backward;
	action_map[irr::KEY_KEY_D] = Action::go_right;
	//arrow key movement
	action_map[irr::KEY_UP] = Action::go_forward;
	action_map[irr::KEY_LEFT] = Action::go_left;
	action_map[irr::KEY_DOWN] = Action::go_backward;
	action_map[irr::KEY_RIGHT] = Action::go_right;
}

bool Input_handler::OnEvent(const irr::SEvent &event)
{
	switch (event.EventType){
	case irr::EET_KEY_INPUT_EVENT:
	{
		//set what actions are happening
		continuous_actions[action_map[event.KeyInput.Key]] = event.KeyInput.PressedDown;
		//call instant action function if set
		auto f = instant_actions[action_map[event.KeyInput.Key]];
		if (f){
			f();
		}
	}
	break;
	case irr::EET_MOUSE_INPUT_EVENT:
	{
		//event.MouseInput.Wheel;
	}
	break;
	default:
	break;
	}
	return false;
}
