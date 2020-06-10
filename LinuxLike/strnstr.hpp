#ifndef UF_LL_strnstr_H
#define UF_LL_strnstr_H

//UseFull Linux Like strnstr module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

char * strnstr(const char * source, const char * find, size_t length) {
	size_t find_l = strlen(find);
	if (!find_l) return (char *)source;
	while (length >= find_l) {
		length--;
		if (!memcmp(source, find, find_l)) return (char *)source;
		source++;
	}
	return nullptr;
}

char * strnstr_thread_unsafe(char * source, const char * find, size_t length) {
	char c = source[length];
	source[length] = 0;
	char * found = strstr(source, find);
	source[length] = c;
	return found;
}

#endif
