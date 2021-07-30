#ifndef UF_SFMLUp_GUI_BaseGui_H
#define UF_SFMLUp_GUI_BaseGui_H

#include "../../Math/Shape.hpp"
#include "../Event.hpp"
#include "../Drawer.hpp"
#include "../../Templates/Ras.hpp"
#include "FocusTracker.hpp"

//UseFull SFML Up Gui Base module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup { namespace gui {

struct BaseGui : public FocusTracker, public sf::RenderTexture {

	uft::RasRec<BaseGui> * ras_record = nullptr;
	uft::Ras<BaseGui> elements;

	Ok<BaseGui *> getByName(const char * name) {
		if (strcmp(name, unique_name) == 0) return this;
		else {
			Ok<BaseGui *> ret;
			for (size_t i = 0; i < elements.length; i++) {
				ret = elements[i]->getByName(name);
				if (ret.isOk) return ret;
			}
		}
		return {};
	}

	void addParent(uft::Ras<BaseGui> & set) {ras_record = set.add(this);}
	void addParent(BaseGui * parent) {addParent(parent->elements);}
	void addChild(BaseGui * child) {child->ras_record = elements.add(child);}
	void removeParent() {if (ras_record != nullptr) ras_record = ras_record->remove();}

	virtual void remove() {
		removeParent();
		delete this;
	}

	virtual ~BaseGui() {

	}

	Codir<2> codir;
	bool active = true;
	const char * unique_name = nullptr;

	sf::Color color_background = sf::Color::White;
	sf::Color color_outline    = sf::Color::Black;

	BaseGui(const Codir<2> & c) : FocusTracker(c), sf::RenderTexture() {
		codir = c;
		elements.owner = this;
		XY size = codir.size();
		sf::RenderTexture::create(size[0], size[1]);
	}
	BaseGui(const Codir<2> & c, const Codir<2> & c_focus) : FocusTracker(c_focus), sf::RenderTexture() {
		codir = c;
		elements.owner = this;
		XY size = codir.size();
		sf::RenderTexture::create(size[0], size[1]);
	}

	XY getAbsoluteOffset() {
		XY offset = {0, 0};
		if (ras_record) {
			BaseGui * current_owner = (BaseGui *)ras_record->storage->owner;
			while (current_owner) {
				offset += current_owner->codir.left_up;
				current_owner = (BaseGui *)current_owner->ras_record->storage->owner;
			}
		}
		return offset + codir.left_up;
	}

	Codir<2> getAbsoluteCodir() {
		return codir + getAbsoluteOffset();
	}



	XY left_down() const {
		return {codir.left_up[0], codir.right_down[1]};

	}
	XY right_up() const {
		return {codir.right_down[0], codir.left_up[1]};
	}

	void moveRelative(const XY & delta) {
		FocusTracker::moveRelative(delta);
		codir += delta;
	}
	virtual void drawSelfForElements() {
		for (size_t i = 0; i < elements.length; i++) {
			elements[i]->drawSelf();
			elements[i]->drawTo(*this);
		}
	}
	virtual void drawSelf() {
		Drawer.drawCodirFilled(*this, Codir<2>({0, 0}, codir.size()), color_outline, color_background);
	}
	void drawTo(sf::RenderTarget & target) {
		sf::RenderTexture::display();
		sf::Sprite sprite(sf::RenderTexture::getTexture());
		sprite.setPosition(sf::Vector2f(codir.left_up[0], codir.left_up[1]));
		target.draw(sprite);
		sf::RenderTexture::clear(sf::Color::Black);
	}
	void actionForElements() {
		void * focus_buf = nullptr;
		if (pressed == false) {
			focus_buf = FocusTracker::focus;
			FocusTracker::focus = nullptr;
		}
		for (size_t i = 0; i < elements.length; i++) {
			elements[i]->action();
		}
		if (FocusTracker::focus == nullptr)
			FocusTracker::focus = focus_buf;
	}
	virtual void action() {

	}
};

}}

#endif
