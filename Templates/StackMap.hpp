#ifndef UF_T_StackMap_H
#define UF_T_StackMap_H

#include "../Utils/Ok.hpp"

//UseFull Template Stack Map module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto


namespace uft {
	template <typename KeyType, typename ValueType, size_t size>
	struct StackMap {
		KeyType key[size];
		ValueType value[size];
		size_t length;
		
		StackMap() {
			
		}
		~StackMap() {
			
		}
		
		StackMap add(const KeyType & kt, const ValueType & vt) {
			key[length] = kt;
			value[length] = vt;
			return *this;
		}
		
		utils::Ok<KeyType> & getKey(const ValueType & vt) {
			for (size_t i = 0; i < length; i++) {
				if (value[i] == vt) return key[i];
			}
			return {};
		}
		
		utils::Ok<ValueType> & getValue(const KeyType & kt) {
			for (size_t i = 0; i < length; i++) {
				if (key[i] == kt) return value[i];
			}
			return {};
		}
		
	};
}

#endif