#ifndef UF_SFMLUp_GUI_Checkbox_H
#define UF_SFMLUp_GUI_Checkbox_H

//UseFull SFML Up Gui Checkbox module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "Button.hpp"

namespace sfup { namespace gui {

using namespace math;

struct Checkbox : public Button {
	
	static Frame frame_true, frame_false;
	
	bool value = false;
	
	Checkbox(const Codir<2> & codir) : Button(codir) {
		frame = Frame(1, codir.center());
	}
	Checkbox(const Codir<2> & codir, bool value) : Button(codir) {
		frame = Frame(1, codir.center());
		this->value = value;
	}
	
	void switchValue() {
		XY center = frame.center;
		if (value)
			frame = frame_false;
		else 
			frame = frame_true;
		frame.center = center;
		
		value = !value;
	}
	
	void actionRealized() {
		switchValue();
		Button::actionRealized();
	}
	void moveRelative(const XY & delta) {
		Button::moveRelative(delta);
	}
};

struct CheckboxInitValuesStruct {
	CheckboxInitValuesStruct() {
		Checkbox::frame_true
			.addPoint({1, 5})
			.addPoint({4, 9})
			.addPoint({9, 2})
			.addBreak();
		Checkbox::frame_false
			.addPoint({1, 1})
			.addPoint({9, 9})
			.addBreak()
			.addPoint({9, 1})
			.addPoint({1, 9})
			.addBreak();
	}
	
} CheckboxInitValues;
	
}}

#endif