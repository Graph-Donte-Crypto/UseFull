#ifndef UF_U_Option_H
#define UF_U_Option_H

#include <stdlib.h>
#include <stdio.h>

#include "Concepts.hpp"

//UseFull Utils Ok module
//Version 1.1 release
//Make by Graph Don'te-Crypto

namespace utils {
	template <typename Type>
	class Ok {
	public:
		Type value;
		bool isOk = true;
		Ok(const Type & _value) : value(_value) {}
		Ok() : value() {isOk = false;}
		~Ok() = default;
		
		Ok(const Ok<Type> & o) : 
			value(o.value), 
			isOk(o.isOk) {};
		
		bool get(Type & _value) const {
			if (isOk) _value = this->value;
			else return false;
			return true;
		}
		Type ok(const char * error_message = "Ok::ok::Error isOk == false\n") const {
			if (isOk) return value;
			else {
				printf("%s", error_message); 
				exit(1);
			}
		}
		Type valueOr(Type _value) const {
			if (isOk) return this->value;
			else      return _value;
		}
		
		Type actionOr(CoLambda<Type, Nothing> auto func) const {
			if (isOk) return value;
			else      return func({});
		}
		operator bool () const { return isOk;}
		operator Type () const { return ok();}
		Ok & operator = (const Type & _value) {
			this->value = _value;
			isOk = true;
			return *this;
		}
	};
}

#endif