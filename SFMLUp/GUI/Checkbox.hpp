#ifndef UF_SFMLUp_GUI_Checkbox_H
#define UF_SFMLUp_GUI_Checkbox_H

//UseFull SFML Up Gui Checkbox module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "Button.hpp"

namespace sfup { namespace gui {

using namespace math;

struct Checkbox : public Button {
	
	Frame frame_true, frame_false;

	static
	struct CheckboxTemplatesStruct {

		inline static Frame frame_template_ok;
		inline static Frame frame_template_x;

		CheckboxTemplatesStruct() {
			frame_template_ok
				.addPoint({1, 5})
				.addPoint({4, 9})
				.addPoint({9, 2})
				.addBreak();
			frame_template_x
				.addPoint({1, 1})
				.addPoint({9, 9})
				.addBreak()
				.addPoint({9, 1})
				.addPoint({1, 9})
				.addBreak();	
		}
	} Templates;
	
	bool value = false;

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

	void actionRealized() override {
		switchValue();
		Button::actionRealized();
	}
	void moveRelative(const XY & delta) override {
		Button::moveRelative(delta);
	}
};
	
}}

#endif