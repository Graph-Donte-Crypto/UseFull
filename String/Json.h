#ifndef UF_S_Json_H
#define UF_S_Json_H

#include "File.h"

//UseFull String Json module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {
	
	enum class JsonGeneralType {
		Record,
		Array,
		Number,
		Literal,
		String
	};
	
	
	struct JsonGeneral {
		JsonGeneralType type;
	};
	
	struct JsonRecord : public JsonGeneral {
		char ** name;
		JsonGeneral * value;
	};
	struct JsonArray : public JsonGeneral {
		JsonGeneral * values;
	};
	struct JsonNumber : public JsonGeneral {
		double value;
	};
	struct JsonLiteral : public JsonGeneral {
		bool isNull;
		bool isTrue;
	};
	struct JsonString : public JsonGeneral {
		char * value;
	};
	
	
	
	
	struct Json {
		JsonRecord * values;
		Json(const char * filename) {
			
		}
		~Json() {
			
		}
	};
}

#endif