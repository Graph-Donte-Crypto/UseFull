#ifndef UF_U_LambdaHolder_H
#define UF_U_LambdaHolder_H

#include "Concepts.hpp"

//UseFull Utils Lambda Holder module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

/*

#include <concepts>
#include <stdio.h>

template <typename FUNC, typename RET>
concept CoLambda0 = 
requires (FUNC func) {
    { func() } -> std::same_as<RET>;
};

template <typename RET>	
struct AutoHolder {		
    virtual RET invoke() = 0;

    virtual ~AutoHolder() {
        
    }
};

template <typename Type, typename RET>
struct Holder : public AutoHolder<RET> {
    Type object;
    
    virtual RET invoke() {
        return object();
    }
    
    virtual ~Holder() {
        
    }
    
    Holder(Type obj, RET) {
        object = obj;
    }
}; 

template <typename RET>
struct LambdaHolder {
    AutoHolder<RET> * ptr = nullptr;
    
    RET invoke() {
        return ptr->invoke();
    }
    
    LambdaHolder(CoLambda0<RET> auto lambda) {
        RET a;
        ptr = new Holder(lambda, a);
    }
    ~LambdaHolder() {
        delete ptr;
    }
};

void callprintf() {
    printf("Some\n");
}

int main() {
    LambdaHolder<int> handle = LambdaHolder<int>([](){
		callprintf();
		return 3;
	});
	
    handle.invoke();
}

*/

namespace utils {
	
	template <typename FunctionType>
	struct AutoHolder {		
		virtual FunctionType invoke();
	
		virtual ~AutoHolder() {
			
		}
	};

	template <typename ObjectType, typename FunctionType>
	struct Holder : public AutoHolder<FunctionType> {
		ObjectType object;
		
		auto invoke() {
			return object();
		}
		
		virtual ~Holder() {
			
		}
		
		Holder(ObjectType obj) {
			object = obj;
		}
	}; 

	template <typename RET>
	struct LambdaHolder {
		AutoHolder<RET> * ptr = nullptr;
		
		RET invoke() {
			return ptr->invoke();
		}
		
		LambdaHolder(CoLambda0<RET> auto lambda) {
			RET a;
			ptr = new Holder(lambda, a);
		}
		~LambdaHolder() {
			delete ptr;
		}
	};
}

#endif