#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <irrlicht/irrlicht.h>
#include <functional>

struct Input_handler : public irr::IEventReceiver{

	//set up some reasonable action map
	//TODO: overwrite with settings
	Input_handler();
	//logical actions that a character can perform
	enum Action : unsigned char{
		none,
		go_forward,
		go_backward,
		go_left,
		go_right,
		camera_left,
		camera_right,
		camera_forward,
		camera_backward,
		camera_up,
		camera_down,
		camera_zoom_out,
		camera_zoom_in,
		size
	};
	//defines the action for discrete events. Discrete events are activated once by a keypress
	Action key_action_map[irr::KEY_KEY_CODES_COUNT] = {};
	Action mouse_action_map[irr::EMIE_COUNT] = {};
	enum Mouse_wheel_action{Mouse_wheel_up, Mouse_wheel_down, Mouse_wheel_count};
	Action mouse_wheel[Mouse_wheel_count] = {};
	//defines which continuous actions are happening. Continuous actions are happening when a key is pressed
	bool continuous_actions[Action::size] = {};
	//defines which function is called when an action occures
	std::function<void()> instant_actions[Action::size];
private:
	bool OnEvent(const irr::SEvent& event) override;
};

#endif // INPUT_HANDLER_H
