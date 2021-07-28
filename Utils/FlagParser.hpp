#ifndef UF_U_FlagParser_H
#define UF_U_FlagParser_H

#include <vector>
#include <string>
#include <string.h>

//UseFull Utils Flag Parser module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include <UseFull/Utils/Stream.hpp>

namespace utils {

	struct Flag {
		std::vector<const char *> name_variants;
		const char * desc = nullptr;
		size_t (*value_func)(const char * const *, size_t) = nullptr;
		void   (*empty_func)(             ) = nullptr;

		Flag(std::initializer_list<const char *> list
			, const char * _desc
			, size_t (*_value_func)(const char * const *,size_t)
		)
		: desc(_desc)
		, value_func(_value_func)
		, empty_func(nullptr)
		{
			name_variants.insert(name_variants.end(), list.begin(), list.end());
		}
		Flag(std::initializer_list<const char *> list
			, const char * _desc
			, void (*_empty_func)()
		)
		: desc(_desc)
		, value_func(nullptr)
		, empty_func(_empty_func)
		{
			name_variants.insert(name_variants.end(), list.begin(), list.end());
		}
		Flag()
		: desc(nullptr)
		, value_func(nullptr)
		, empty_func(nullptr)
		{}

		bool is(const char * value) {
			for (size_t i = 0; i < name_variants.size(); i++)
				if (strcmp(name_variants[i], value) == 0) return true;
			return false;
		}

		std::string getNameVariants() {
			if (name_variants.size() > 0) {
				std::string first = "[" + std::string(name_variants[0]) + "]";
				return utils::GetStream<const char *>(name_variants)
				.Ignore(1)
				.Reduce<std::string>([](auto p, auto x){ return p + " | [" + x + "]";}, first);
			}
			else return "";
		}

		size_t invoke(const char * const * argv, size_t left) {
			if (value_func) return value_func(argv, left);
			if (empty_func) empty_func();
			return 0;
		}
	};


	struct FlagParser {
		size_t argc;
		char * const * argv;
		std::vector<Flag> flags;

		FlagParser(size_t _argc, char * const * _argv)
		: argc(_argc)
		, argv(_argv)
		{}

		void execute() {
			for (size_t i = 1; i < argc; i++) {
				bool is = false;
				for (size_t j = 0; j < flags.size(); j++) {
					if (flags[j].is(argv[i])) {
						i += flags[j].invoke(argv + i, argc - i - 1);
						is = true;
						break;
					}
				}
				if (!is) printf("flag [%s] unresolved\n", argv[i]);
			}
		}
	}
	* FlagParserAutoHelpSource = nullptr;

	void FlagParserAutoHelp() {
		FlagParser & p = *FlagParserAutoHelpSource;

		utils::GetStream<Flag>(p.flags)
			.ForAll([](Flag & flag) { printf("%s => %s\n", flag.getNameVariants().c_str(), flag.desc);});
		exit(0);
	}
}

#endif
