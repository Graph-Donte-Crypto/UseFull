#ifndef UF_U_Option_H
#define UF_U_Option_H

#include <stdlib.h>
#include <stdio.h>

#include "Concepts.hpp"

//UseFull Utils Ok module
//Version 1.0 release
//Make by Graph Don'te-Crypto

namespace utils {
	template <typename type>
	class Ok {
	public:
		type value;
		bool isOk = true;
		Ok(const type & value) {this->value = value;}
		Ok() {isOk = false;}
		Ok(const Ok<type> & o) {
			value = o.value;
			isOk = o.isOk;
		}
		//Ok(void) {isOk = false;}
		bool get(type & value) const {
			if (isOk) value = this->value;
			else return false;
			return true;
		}
		type ok() const {
			if (isOk) return value;
			else {
				printf("Ok::ok::Error isOk == false\n"); 
				exit(1);
			}
		}
		type valueOr(type value) const {
			if (isOk) return this->value;
			else      return value;
		}
		
		type actionOr(CoLambda<type, Nothing> auto func) const {
			if (isOk) return value;
			else      return func({});
		}
		operator bool () const { return isOk;}
		operator type () const { return ok();}
		Ok & operator = (const type & value) {
			this->value = value;
			isOk = true;
			return *this;
		}
	};
}

#endif