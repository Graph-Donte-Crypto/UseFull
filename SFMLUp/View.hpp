#ifndef UF_SFMLUP_View_H
#define UF_SFMLUP_View_H

#include <concepts>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//UseFull SFML Up View module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Math/Vector.hpp"
#include "../Math/Shape.hpp"

/*
 How to use:

 while (true) {
	 ...

	 clear()

	 WorldView.step()

	 Draw world ...

	 GuiView.step()

	 Draw gui ...

	 window.display();

	 ...

 }


 */

namespace sfup {
	using namespace math;

	template <typename Type>
	concept CoCustomView =
	requires (Type obj, sf::RenderWindow * window, size_t width, size_t height) {
		{ obj.use() } -> std::same_as<void>;
		{ obj.view } -> std::convertible_to<sf::View>;
		{ obj.reset(window, width, height) } -> std::same_as<void>;
	};

	struct WorldViewStruct {

		enum class TargetType {
			Vector,
			Codir,
			None
		} target = TargetType::None;

		void * follow_up_target = nullptr;

		sf::RenderWindow * window = nullptr;

		sf::View view;

		Codir<2> current_view;
		Codir<2> default_view;

		double scale = 1;
		double current_scale = 1;
		size_t deceleration = 3;

		WorldViewStruct()
			: current_view({0, 0}, {0, 0})
			, default_view({0, 0}, {0, 0})
		{}

		void reset(sf::RenderWindow * _window, size_t width, size_t height) {
			window = _window;
			view.reset(sf::FloatRect(0.0f, 0.0f, width, height));
			window->setView(view);

			default_view.right_down = {(double)width, (double)height};
			default_view -= default_view.center();

			current_view.right_down = {(double)width, (double)height};
			current_view -= current_view.center();
		}

		void followUp(XY & target_object) {
			target = TargetType::Vector;
			follow_up_target = &target_object;
		}
		void followUp(Codir<2> & target_object) {
			target = TargetType::Codir;
			follow_up_target = &target_object;
		}
		void stopFollowing() {
			target = TargetType::None;
		}

		void update() {
			if (window == nullptr) {
				printf("window == nullptr for WorldView\n");
				exit(0);
			}
			if (target != TargetType::None) {
				XY new_center;
				if (target == TargetType::Vector) {
					XY & target_object = *((XY *)follow_up_target);
					new_center = (target_object + (deceleration - 1) * current_view.center()) / deceleration;
				}
				else if (target == TargetType::Codir) {
					Codir<2> & target_object = *((Codir<2> *)follow_up_target);
					new_center = (target_object.center() + (deceleration - 1) * current_view.center()) / deceleration;
				}
				else {
					printf("target != TargetType::None, but target == TargetType::Center != and target == TargetType::Codir\n");
					exit(1);
				}
				current_view = default_view * current_scale + new_center;
				current_scale += (scale - current_scale) / deceleration;
				XY size = current_view.size();

				view.setSize(size[0], size[1]);
				view.setCenter(new_center[0], new_center[1]);
			}
		}

		void use() {
			window->setView(view);
		}
	}
	WorldView;

	struct GuiViewStruct {
		sf::View view;
		Codir<2> current_view;
		sf::RenderWindow * window = nullptr;

		void reset(sf::RenderWindow * _window, size_t width, size_t height) {
			window = _window;

			view.reset(sf::FloatRect(0.0f, 0.0f, width, height));
			window->setView(view);

			current_view.left_up = {0, 0};
			current_view.right_down = {(double)width, (double)height};
		}

		void use() {
			window->setView(view);
		}
	}
	GuiView;
}

#endif
