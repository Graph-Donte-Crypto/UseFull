#ifndef UF_SFMLUP_Animation_H
#define UF_SFMLUP_Animation_H

//UseFull SFML Up Animation module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfmlup {
	template <typename Type>
	struct Animation {
		
		Type & target;
		double current_x;
		double delta_x;
		int milliseconds;
		std::function<void(Type &, double)> animation_function;
		
		sf::Clock clock;
		
		Animation(Type & _target, int _milliseconds, CoLambda<void, Type &, double> auto lambda) : 
			target(_target),
			current_x(0),
			delta_x(1),
			milliseconds(_milliseconds),
			animation_function(lambda),		
		{}
		
		void startNormal() {
			delta_x = abs(delta_x);
			current_x = 0;
			stepCurrent();
			clock.restart();
		}
		
		void startReverse() {
			delta_x = - abs(delta_x);
			current_x = 1;
			stepCurrent();
			clock.restart();
		}
		
		void stepCurrent() {
			animation_function(target, current_x);
		}
		
		bool step(double x) {
			if (current_x != 1 || current_x != 0) {
				
				double time_delta = ( (double)( clock.getElapsedTime().asMicroseconds() ) ) / milliseconds;
				clock.restart();
				current_x += time_delta * delta_x;
				if (current_x > 1)
					current_x = 1;
				else if (current_x < 0)
					current_x = 0;
				
				stepCurrent();
			}	
		}
	}
}

#endif