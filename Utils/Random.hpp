#ifndef UF_U_Random_H
#define UF_U_Random_H

#include <stdlib.h>
#include <initializer_list>
#include <math.h>

#include "Concepts.hpp"

//UseFull Random module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	bool chance(double percent) {
		if (percent == 0) return false;
		else return !(rand() % (int)round(1.0 / percent));
	}
	
	double randomPercent() {
		return ((double)rand()) / RAND_MAX;
	}
	
	template <typename Type>
	Type randomFromInterval(Type from, Type to) {
		return (Type) (from + randomPercent() * (to - from));
	}
	
	template <typename Type>
	Type random(std::initializer_list<Type> list) {
		return *(list.begin() + rand() % list.size());
	}
	
	template <typename Type>
	Type & random(CoCollection<Type> auto & col) {
		return col[rand() % col.length()];
	}
}

#endif