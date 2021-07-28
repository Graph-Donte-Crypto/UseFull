#ifndef UF_S_StringFormatBuilder_H
#define UF_S_StringFormatBuilder_H

#include <string>
#include <string.h>

//UseFull String Format Builder module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Utils/RuntimeDebug.hpp"

namespace str {

	template <size_t size = 256>
	struct StringFormatBuilder {
		char buffer[size];
		char * pos;

		#ifdef RUNTIME_DEBUG
		void validateSize() {
            if (pos - buffer > size) {
                printf("ERROR: StringFormatBuilder<%llu> don't have enough space\n", size);
                exit(1);
            }
		}
		#endif // RUNTIME_DEBUG

		StringFormatBuilder()
            : pos(buffer)
        {}

        std::string toString() {
            return std::string(buffer, pos - buffer);
        }

        template <typename Type>
        StringFormatBuilder & append(const Type & obj, const char * format) {
            pos += sprintf(pos, format, obj);
            DEBUG(
                validateSize();
            )
            return *this;
        }

        StringFormatBuilder & append(int                value, const char * format = "%d"   ) { return append<int>               (value, format);}
        StringFormatBuilder & append(long               value, const char * format = "%ld"  ) { return append<long>              (value, format);}
        StringFormatBuilder & append(long long          value, const char * format = "%lld" ) { return append<long long>         (value, format);}
        StringFormatBuilder & append(unsigned           value, const char * format = "%u"   ) { return append<unsigned>          (value, format);}
        StringFormatBuilder & append(unsigned long      value, const char * format = "%lu"  ) { return append<unsigned long>     (value, format);}
        StringFormatBuilder & append(unsigned long long value, const char * format = "%llu" ) { return append<unsigned long long>(value, format);}
        StringFormatBuilder & append(float              value, const char * format = "%f"   ) { return append<float>             (value, format);}
        StringFormatBuilder & append(double             value, const char * format = "%lf"  ) { return append<double>            (value, format);}
        StringFormatBuilder & append(long double        value, const char * format = "%lf"  ) { return append<long double>       (value, format);}
        StringFormatBuilder & append(const char *       value, const char * format = "%s"   ) { return append<const char *>      (value, format);}
	};

	template <typename Type>
	static std::string toStringFormat(const Type & obj) {
		return StringFormatBuilder<>().append(obj).toString();
	}

	template <typename Type>
    static std::string toStringFormat(const Type & obj, const char * format) {
		return StringFormatBuilder<>().append(obj, format).toString();
	}

}

#endif
