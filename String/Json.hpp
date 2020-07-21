#ifndef UF_S_Json_H
#define UF_S_Json_H

#include "File.hpp"
#include "Semantic.hpp";

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
	
	
	struct JsonRecord {
		char * key;
		JsonGeneral * value;
	};
	
	struct Json {
		JsonRecord * records;
		
		void constuctor(char * bytes) {
			Semantic semantics[4];
			semantics[0] = NewSemantic("{", "}", true);
			semantics[1] = NewSemantic("(", ")", true);
			semantics[2] = NewSemantic("[", "]", true);
			semantics[3] = NewSemantic("\"", "\"", false);
			SemanticHandler sem_handler = newSemanticHandler(semantics, 4);
			const char * ptr;
			ptr = parseSemantic(bytes, ",", sem_handler);
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
	
	void jsonTestUsage() {
		
		/*
		
		{ 
		  "first_name" : "Sammy",
		  "last_name" : "Shark",
		  "location" : "Ocean",
		  "websites" : [ 
			{
			  "description" : "work",
			  "URL" : "https://www.digitalocean.com/"
			},
			{
			  "desciption" : "tutorials",
			  "URL" : "https://www.digitalocean.com/community/tutorials"
			}
		  ],
		  "social_media" : [
			{
			  "description" : "twitter",
			  "link" : "https://twitter.com/digitalocean"
			},
			{
			  "description" : "facebook",
			  "link" : "https://www.facebook.com/DigitalOceanCloudHosting"
			},
			{
			  "description" : "github",
			  "link" : "https://github.com/digitalocean"
			}
		  ]
		}
		
		*/
		
		Json json_file("File.json");
		json_file.errorAction([&json_file](){
			printf("Json was break\n  Error: %s\n", json_file.error_message);
			
		});
		printf("%s\n", json_file.get("first_name").asRecord().value);
		printf("%s\n", json_file.get("last_name").asRecord().value);
		printf("%s\n", json_file.get("location").asRecord().value);
		JsonArray & array = json_file.get("websites").asArray();
		for (size_t i = 0; i < array.length; i++) {
			if (array[i].isJson) {
				JsonArray & inner = array[i].asJson().asArray();
				for (size_t i = 0; i < inner.length; i++) {
					printf("k: %s\n v: %s\n", inner[i].asRecord().key, inner[i].asRecord().value);
				}
			}
			else {
				printf("%s\n", array[i].value);
			}
		}
		
	}
}

#endif