#ifndef UF_S_Parser_H
#define UF_S_Parser_H

#include <cstddef>
#include <string.h>
#include "../Templates/Array.hpp"
#include "../Templates/List.hpp"

namespace str {

	const char * findNSequenceEndPlusOne(const char * source, const char * sequence, size_t n) {
		size_t sequence_length = strlen(sequence);
		for (size_t i = 0; i < n; i++) {
			if (strncmp(source, sequence, sequence_length) == 0)
				source += sequence_length;
			else
				return nullptr;
		}
		return source;
	}

	const char * findFTSequenceEndPlusOne(const char * source, const char * sequence, size_t from, size_t to) {
		size_t sequence_length = strlen(sequence);
		for (size_t i = 0; i < from; i++) {
			if (strncmp(source, sequence, sequence_length) == 0)
				source += sequence_length;
			else
				return nullptr;
		}
		for (size_t i = from; i <= to; i++) {
			if (strncmp(source, sequence, sequence_length) == 0)
				source += sequence_length;
			else
				break;
		}
		return source;
	}

	struct ParserPart {

	    enum class ElementCount {
			FromTo,
			From
		};

		/*
        FromTo(n, m)
            One        - FromTo(1, 1)
            ZeroOrOne  - FromTo(0, 1)
        From(n)
            OneOrMany  - From(1)
            ZeroOrMany - From(0)
		*/
		enum class ElementType {
			Any,
			String
		};

		struct Element {
			ElementCount element_count = ElementCount::FromTo;
			size_t from = 0, to = 0;

			Element & setCountAsFromTo(size_t from, size_t to) {
				this->from = from;
				this->to = to;
				element_count = ElementCount::FromTo;
				return *this;
			}

			Element & setCountAsFrom(size_t from) {
				this->from = from;
				element_count = ElementCount::From;
				return *this;
			}

            Element & setCountAsOne()        {return setCountAsFromTo(1, 1);}
			Element & setCountAsZeroOrOne()  {return setCountAsFromTo(0, 1);}
			Element & setCountAsOneOrMany()  {return setCountAsFrom(1);}
			Element & setCountAsZeroOrMany() {return setCountAsFrom(0);}


			ElementType element_type = ElementType::Any;
			const char * string = nullptr;

			Element & setTypeAsAny() {
				element_type = ElementType::Any;
				return *this;
			}
			Element & setTypeAsString(const char * string) {
				this->string = string;
				element_type = ElementType::String;
				return *this;
			}

			enum class CheckResult {
				ValidNormal,
				ValidMany,
				ErrorString
			};

            const char * start_ptr = nullptr;
            const char * end_ptr = nullptr;

			CheckResult isCorrect(const char * ptr, CheckResult previus) {

				constexpr auto ValidMany = CheckResult::ValidMany;
				constexpr auto ErrorString = CheckResult::ErrorString;

				size_t delta = 0;
				CheckResult returnType = CheckResult::ValidNormal;

				printf("ic Init\n");
				if (string != nullptr) printf("string = %s\n", string);
				else printf("string = nullptr\n");

				//TODO: I MAKE THIS SWITCH (element_count)

				if (element_count == element_count::FromTo) {

				}

				if (element_count == ElementCount::Many) {
					printf("ic Count = Many\n");
					if (element_type == ElementType::Any) {
						if (ptr[0] == 0)
							return ErrorString;
					}
					else {
						if (previus == ValidMany) {
							char * fs_ptr = strstr(ptr, string);
							if (fs_ptr == nullptr)
								return ErrorString;
                            start_ptr = fs_ptr;
							delta = fs_ptr - ptr;
						}
						else {
							if (strncmp(ptr, string, strlen(string)) != 0)
								return ErrorString;
						}
					}

					returnType = CheckResult::ValidMany;
				}
				else if (element_count == ElementCount::Number) {
					printf("ic Count = Number\n");
					if (element_type == ElementType::Any) {
						if (strlen(ptr) < number)
							return ErrorString;
						delta = number;
					}
					else {
						const char * fs_ptr = nullptr;
						if (previus == ValidMany) {
							if ((fs_ptr = strstr(ptr, string)) == nullptr)
								return ErrorString;
						}
						else
							fs_ptr = ptr;
                        start_ptr = fs_ptr;
						const char * ns_ptr = findNSequenceEndPlusOne(fs_ptr, string, number);
						if (ns_ptr == nullptr)
							return ErrorString;
						delta = ns_ptr - ptr;
					}
				}
				else if (element_count == ElementCount::FromTo) {
					printf("ic Count = FromTo\n");
					if (element_type == ElementType::Any) {
						size_t string_length = strlen(ptr);
						if (string_length > to)
							delta = to;
						else if (string_length >= from)
							delta = string_length;
						else
							return ErrorString;
					}
					else {
						const char * fs_ptr = nullptr;
						if (previus == ValidMany) {
							if ((fs_ptr = strstr(ptr, string)) == nullptr)
								return ErrorString;
						}
						else
							fs_ptr = ptr;

                        start_ptr = fs_ptr;

						const char * ns_ptr = findFTSequenceEndPlusOne(fs_ptr, string, from, to);
						if (ns_ptr == nullptr)
							return ErrorString;
						delta = ns_ptr - ptr;
					}
				}
				else {
					printf("ic Fatal\n");
				}

				printf("ic Return\n");

				end_ptr = ptr + delta;
				return returnType;
			}

		};

		char * splitByParserPart(const char * string, const char ** after_ptr, bool flagMany) {
			constexpr auto ValidNormal = Element::CheckResult::ValidNormal;
			constexpr auto ValidMany   = Element::CheckResult::ValidMany;
			constexpr auto ErrorString = Element::CheckResult::ErrorString;

			auto result = flagMany ? ValidMany : ValidNormal;
			size_t i = flagMany ? 1 : 0;

			printf("sbpp Init\n");

            start_ptr = string;

			if (flagMany) {
				result = elements[0].isCorrect(string, &string, &start_ptr, result);
				if (result == ErrorString) {
					*after_ptr = nullptr;
					return nullptr;
				}
			}

			printf("sbpp Many\n");

            const char * before__to_remove = nullptr;

			for (; i < elements.length; i++) {
				printf("sbpp =>  %lli\n", i);
				result = elements[i].isCorrect(string, &string, &before__to_remove, result);
				printf("sbpp =>  %lli  end\n", i);
				if (result == ErrorString) {
					*after_ptr = nullptr;
					return nullptr;
				}
			}

			printf("sbpp Cycl\n");

			*after_ptr = string;
            end_ptr = string;
			if (result == ValidMany) {
                last_element_count = ElementCount::Many;
                need_postprocessing = true;
			}
            else
                return createResultString();
		}

		uft::Array<Element> elements = uft::Array<Element>(2);
		size_t normal_mode = 666;
		ElementCount last_element_count = ElementCount::Number;

		const char * start_ptr = nullptr;
		const char * end_ptr = nullptr;
		char * result_string = nullptr;

		bool need_postprocessing = false;

		char * createResultString() {
            size_t string_length = end_ptr - start_ptr;
            result_string = new char[string_length + 1];
            memcpy(result_string, start_ptr, string_length);
            result_string[string_length] = 0;
            return result_string;
		}

		bool lastIsMany() {
            return last_element_count == ElementCount::Many;
		}

		ParserPart() {}

		ParserPart & operator = (const ParserPart & part) {
            elements.flushAndCopyFrom(part.elements);
            normal_mode = part.normal_mode;
            last_element_count = part.last_element_count;
            start_ptr = part.start_ptr;
            end_ptr = part.end_ptr;
            result_string = part.result_string;
            need_postprocessing = part.need_postprocessing;
		}
	};

	struct Parser {

		const char * string_after_parts = nullptr;

		struct ParserPartResult {
			ParserPart part;
			bool is_ok = false;
			ParserPartResult() {

			}
			ParserPartResult(const ParserPart & part) {
				this->part = part;
			}

			char * getAnswer() {
                return part.result_string;
			}
		};

		char * getAnswer(size_t i) {
            return parts[i].getAnswer();
		}
		uft::List<ParserPartResult> parts = uft::List<ParserPartResult>();

		void addPart(const ParserPart & part) {
			parts.addCopy(ParserPartResult(part));
		}

		void parseString(const char * string) {
			string_after_parts = string;
			char * ptr = nullptr;
			printf("length = %lli\n", parts.length);
			bool isMany = false;
			for (size_t i = 0; i < parts.length; i++) {
				printf("  =>  %llu\n", i);
				ptr = parts[i].part.splitByParserPart(string_after_parts, &string_after_parts, isMany);
				printf("  =>  %llu  end\n", i);
				//Ошибка, если splitByParserPart вернул nullptr, но не need_postprocessing
				if (ptr == nullptr) {
                    if (!parts[i].part.need_postprocessing) return;
				}
				//Если предыдущий объект запросил постпроцессинг
				if (i > 0 && parts[i - 1].part.need_postprocessing) {
                    parts[i - 1].part.end_ptr = parts[i].part.start_ptr;
                    parts[i - 1].part.createResultString();
				}
				isMany = parts[i].part.lastIsMany();
				parts[i].is_ok = true;
			}

			if (parts[parts.length - 1].part.need_postprocessing) {
                string_after_parts += strlen(string_after_parts);
                parts[parts.length - 1].part.end_ptr = string_after_parts;
                parts[parts.length - 1].part.createResultString();
			}
		}
	};
}

#endif
