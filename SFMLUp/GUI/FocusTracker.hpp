#ifndef UF_SFMLUp_GUI_FocusTracker_H
#define UF_SFMLUp_GUI_FocusTracker_H

//UseFull SFML Up Gui Focus Tracker module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../../Math/Shape.hpp"
#include "../../Math/Intersect.hpp"
#include "../Event.hpp"

namespace sfup { namespace gui {

using namespace math;

struct FocusTracker {
	
	inline static void * focus = nullptr;
	inline static void * focus_next_turn = nullptr;
	
	static void nextTurn() {
		focus = focus_next_turn;
		focus_next_turn = nullptr;
	}
	
	bool prev_focused = false;
	struct State {
		bool pressed = false;
		bool focused = false;
	};	

	/*
	При вызове action функции, 
		state_before - состояние автомата до вызова функции

		
		
		
	*/

	/*
	При вызове action функции:
	state_before - состояние автомата до вызова функции;
	записывать в state_before не имеет практического смысла, имеет смысл только считывать
	*/
	State state_before;
	/*
	При вызове action функции:
	state_after - состояние автомата которое будет после вызова функции;
	Можно как записывать (новое состояние будет применено после конца вызова функциии), так и считывать
	*/
	State state_after;

	bool & pressed = state_before.pressed;
	bool & focused = state_before.focused;

	Codir<2> codir_focus;
	XY       focus_offset = XY({0, 0});
	
	virtual void moveRelative(const XY & delta) {
		focus_offset += delta;
		//codir_focus += delta;
	}

	FocusTracker(const Codir<2> & c) : codir_focus(c) {
		
	}
	virtual ~FocusTracker() {
		
	}

	virtual void actionNotFocused() {}
	virtual void actionNotFocusing() {}
	virtual void actionFocused() {}
	virtual void actionFocusing() {}
	virtual void actionPressed() {}
	virtual void actionPressing() {}
	virtual void actionRealized() {}


	/*
	Магический код, который реализует алгоритм смены фокуса мышки на объекте, 
	с возможностью редактирования и чтения текущих состояний в вызываемых action функциях
	*/
	#define EXEC(some) {state_after = state_before; { some }; state_before = state_after;}

	void checkFocus(const XY & mouse) {
		if (focus == nullptr || focus == this) {

			focused = checkPointInCodir(mouse, codir_focus + focus_offset);

			if (!focused) {
				EXEC(
					state_after.pressed = false;
					if (prev_focused) actionNotFocused();
					actionNotFocusing();
				)
			}
			else {
				focus = this;
				focus_next_turn = this;
				if (!pressed) if ((pressed = Event.getEvent(EventType::MouseButtonPressedLeft)) == true) EXEC(actionPressed();)
				if (!pressed) {
					if (!prev_focused) EXEC(actionFocused();)
					EXEC(actionFocusing();)
				}
				else {
					if (Event.getEvent(EventType::MouseButtonReleasedLeft)) {
						EXEC(state_after.pressed = false; actionRealized();)
					}
					else EXEC(actionPressing();)
				}
			}

			prev_focused = focused;

		}
	}

	#undef EXEC
};

}}

#endif