#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <irrlicht/irrlicht.h>

struct Input_handler : public irr::IEventReceiver{

	//set up some reasonable action map
	//TODO: overwrite with settings
	Input_handler(){
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
	//logical actions that a character can perform
	enum Action : unsigned char{
		none,
		go_forward,
		go_backward,
		go_left,
		go_right,
		size
	};
	//defines the action for discrete events. Discrete events are activated once by a keypress
	Action action_map[irr::KEY_KEY_CODES_COUNT] = {};
	//defines which continuous actions are happening. Continuous actions are happening when a key is pressed
	bool continuous_actions[Action::size] = {};
	//defines which function is called when an action occures
	void (*instant_actions[Action::size])() = {};
private:
	bool OnEvent(const irr::SEvent& event) override{
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
};

#endif // INPUT_HANDLER_H
