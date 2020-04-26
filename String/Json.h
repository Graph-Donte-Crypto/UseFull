#ifndef UF_S_Json_H
#define UF_S_Json_H

#include "File.h"
#include "Semantic.h";

//UseFull String Json module
//Version 1.1 alfa
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
		JsonArray * values;
		
		void constuctor(char * bytes) {
			Semantic semantics[4];
			semantics[0] = NewSemantic("{", "}", true);
			semantics[1] = NewSemantic("(", ")", true);
			semantics[2] = NewSemantic("[", "]", true);
			semantics[3] = NewSemantic("\"", "\"", false);
			const char * ptr;
			ptr = parseSemantic(bytes, ",", 4, semantics);
			if (*ptr != 0) {
				printf("No valid Json\n");
				return;
			}
		}
		
		Json(const char * filename) {
			
		}
		~Json() {
			
		}
	};
}

#endif