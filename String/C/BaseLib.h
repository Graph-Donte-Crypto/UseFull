#ifndef UF_S_BaseLib_H
#define UF_S_BaseLib_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
//UseFull String Base Library
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

#define C char

const C caseLower[] = { (C)0,(C)1,(C)2,(C)3,(C)4,(C)5,(C)6,(C)7,(C)8,(C)9,(C)10,(C)11,(C)12,(C)13,(C)14,(C)15,(C)16,(C)17,(C)18,(C)19,(C)20,(C)21,(C)22,(C)23,(C)24,(C)25,(C)26,(C)27,(C)28,(C)29,(C)30,(C)31,(C)32,(C)33,(C)34,(C)35,(C)36,(C)37,(C)38,(C)39,(C)40,(C)41,(C)42,(C)43,(C)44,(C)45,(C)46,(C)47,(C)48,(C)49,(C)50,(C)51,(C)52,(C)53,(C)54,(C)55,(C)56,(C)57,(C)58,(C)59,(C)60,(C)61,(C)62,(C)63,(C)64, 'a' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' , 'o' , 'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z' ,(C)91,(C)92,(C)93,(C)94,(C)95,(C)96,(C)97,(C)98,(C)99,(C)100,(C)101,(C)102,(C)103,(C)104,(C)105,(C)106,(C)107,(C)108,(C)109,(C)110,(C)111,(C)112,(C)113,(C)114,(C)115,(C)116,(C)117,(C)118,(C)119,(C)120,(C)121,(C)122,(C)123,(C)124,(C)125,(C)126,(C)127,(C)128,(C)129,(C)130,(C)131,(C)132,(C)133,(C)134,(C)135,(C)136,(C)137,(C)138,(C)139,(C)140,(C)141,(C)142,(C)143,(C)144,(C)145,(C)146,(C)147,(C)148,(C)149,(C)150,(C)151,(C)152,(C)153,(C)154,(C)155,(C)156,(C)157,(C)158,(C)159,(C)160,(C)161,(C)162,(C)163,(C)164,(C)165,(C)166,(C)167,(C)168,(C)169,(C)170,(C)171,(C)172,(C)173,(C)174,(C)175,(C)176,(C)177,(C)178,(C)179,(C)180,(C)181,(C)182,(C)183,(C)184,(C)185,(C)186,(C)187,(C)188,(C)189,(C)190,(C)191,(C)192,(C)193,(C)194,(C)195,(C)196,(C)197,(C)198,(C)199,(C)200,(C)201,(C)202,(C)203,(C)204,(C)205,(C)206,(C)207,(C)208,(C)209,(C)210,(C)211,(C)212,(C)213,(C)214,(C)215,(C)216,(C)217,(C)218,(C)219,(C)220,(C)221,(C)222,(C)223,(C)224,(C)225,(C)226,(C)227,(C)228,(C)229,(C)230,(C)231,(C)232,(C)233,(C)234,(C)235,(C)236,(C)237,(C)238,(C)239,(C)240,(C)241,(C)242,(C)243,(C)244,(C)245,(C)246,(C)247,(C)248,(C)249,(C)250,(C)251,(C)252,(C)253,(C)254,(C)255};
const C caseUpper[] = { (C)0,(C)1,(C)2,(C)3,(C)4,(C)5,(C)6,(C)7,(C)8,(C)9,(C)10,(C)11,(C)12,(C)13,(C)14,(C)15,(C)16,(C)17,(C)18,(C)19,(C)20,(C)21,(C)22,(C)23,(C)24,(C)25,(C)26,(C)27,(C)28,(C)29,(C)30,(C)31,(C)32,(C)33,(C)34,(C)35,(C)36,(C)37,(C)38,(C)39,(C)40,(C)41,(C)42,(C)43,(C)44,(C)45,(C)46,(C)47,(C)48,(C)49,(C)50,(C)51,(C)52,(C)53,(C)54,(C)55,(C)56,(C)57,(C)58,(C)59,(C)60,(C)61,(C)62,(C)63,(C)64,(C)65,(C)66,(C)67,(C)68,(C)69,(C)70,(C)71,(C)72,(C)73,(C)74,(C)75,(C)76,(C)77,(C)78,(C)79,(C)80,(C)81,(C)82,(C)83,(C)84,(C)85,(C)86,(C)87,(C)88,(C)89,(C)90,(C)91,(C)92,(C)93,(C)94,(C)95,(C)96, 'A' , 'B' , 'C' , 'D'  , 'E'  , 'F'  , 'G'  , 'H'  , 'I'  , 'J'  , 'K'  , 'L'  , 'M'  , 'N'  , 'O'  , 'P'  , 'Q'  , 'R'  , 'S'  , 'T'  , 'U'  , 'V'  , 'W'  , 'X'  , 'Y'  , 'Z'  ,(C)123,(C)124,(C)125,(C)126,(C)127,(C)128,(C)129,(C)130,(C)131,(C)132,(C)133,(C)134,(C)135,(C)136,(C)137,(C)138,(C)139,(C)140,(C)141,(C)142,(C)143,(C)144,(C)145,(C)146,(C)147,(C)148,(C)149,(C)150,(C)151,(C)152,(C)153,(C)154,(C)155,(C)156,(C)157,(C)158,(C)159,(C)160,(C)161,(C)162,(C)163,(C)164,(C)165,(C)166,(C)167,(C)168,(C)169,(C)170,(C)171,(C)172,(C)173,(C)174,(C)175,(C)176,(C)177,(C)178,(C)179,(C)180,(C)181,(C)182,(C)183,(C)184,(C)185,(C)186,(C)187,(C)188,(C)189,(C)190,(C)191,(C)192,(C)193,(C)194,(C)195,(C)196,(C)197,(C)198,(C)199,(C)200,(C)201,(C)202,(C)203,(C)204,(C)205,(C)206,(C)207,(C)208,(C)209,(C)210,(C)211,(C)212,(C)213,(C)214,(C)215,(C)216,(C)217,(C)218,(C)219,(C)220,(C)221,(C)222,(C)223,(C)224,(C)225,(C)226,(C)227,(C)228,(C)229,(C)230,(C)231,(C)232,(C)233,(C)234,(C)235,(C)236,(C)237,(C)238,(C)239,(C)240,(C)241,(C)242,(C)243,(C)244,(C)245,(C)246,(C)247,(C)248,(C)249,(C)250,(C)251,(C)252,(C)253,(C)254,(C)255};

#undef C

char * strInit(size_t size) {
	char * ptr = (char *)malloc(sizeof(char) * size);
	ptr[0] = 0;
	return ptr;
}

char * strDestroy(char ** ptr) {
	free(*ptr);
	return (*ptr = NULL);
}

long long strPosString(const char * where, const char * what) {
	long long d = strstr(where, what) - where;
	return d < 0 ? -1 : d;
}
long long strPosChar(const char * where, const char what) {
	long long d = strchr(where, what) - where;
	return d < 0 ? -1 : d;
}
long long strPosLastStringBeforePtr(const char * where, const char * what, const char * before) {
	const char * start = where;
	long long d = -1;
	size_t len = strlen(what);
	where = strstr(where, what);
	if (!where || where > before) return -1;
	d = (long long)where;
	where += len;
	while ( (where = strstr(where, what)) != NULL) {
		if (where > before) break;
		d = (long long)where;
		where += len;
	}
	return d - (long long)start;

}
long long strPosLastString(const char * where, const char * what) {
	const char * start = where;
	long long d = -1;
	size_t len = strlen(what);
	where = strstr(where, what);
	if (!where) return -1;
	d = (long long)where;
	where += len;
	while ( (where = strstr(where, what)) != NULL) {
		d = (long long)where;
		where += len;
	}
	return d - (long long)start;

}
long long strPosLastStringBeforeIndex(const char * where, const char * what, size_t index) {
   return strPosLastStringBeforePtr(where, what, where + index);
}
long long strPosLastChar(const char * where, const char what) {
	long long d = -1;
	d = strrchr(where, what) - where;
	return d < 0 ? -1 : d;
}
long long strPosLastCharBeforePtr(const char * where, const char what, const char * before) {
	const char * start = where;
	long long d = -1;
	//TODO: на месте before ставить конец строки. После вычислений - поставить оригинальный символ
	where = strchr(where, what);
	if (!where || where > before) return -1;
	d = (long long)where;
	where += 1;
	while ( (where = strchr(where, what)) != NULL) {
		if (where > before) break;
		d = (long long)where;
		where += 1;
	}
	return d - (long long)start;
}
long long strPosLastCharBeforeIndex(const char * where, const char what, size_t index) {
	return strPosLastCharBeforePtr(where, what, where + index);
}
const char * strPosMin(const char * where, const char * what) {
	char * ptrsuse[256];
	const char ** ptrs = (const char **)ptrsuse;
	size_t what_len = strlen(what) + 1;
	bool stack = true;
	if (what_len > 256) {
		ptrs = (const char **)malloc(sizeof(const char *) * what_len);
		stack = false;
	}

	for (size_t i = 0; i < what_len; i++) {
		ptrs[i] = strchr(where, what[i]);
	}
	const char * ans = ptrs[what_len - 1];
	for (size_t i = 0; i < what_len - 1; i++) {
		ans = ans > ptrs[i] ? ptrs[i] : ans;
	}

	if (!stack) free(ptrs);

	return ans;
}

char * strCopyN(char * out, const char * in, size_t n) {
	return strncpy(out, in, n);
}
char * strMove(char * out, const char * in, size_t n) {
	return (char *)memmove((void * )out, (const void *)in, n);
}
char * strExpand(char ** in, size_t n, size_t size) {
	if (size == 0) size = strlen(*in);
	char * ptr = strInit(size * n);;
	memcpy(ptr, *in, size);
	strDestroy(in);
	*in = ptr;
	return *in;
}
char * strAppstrEnd(char * out, const char * in) {
	strcat(out, in);
	return out;
}
//TODO: IDK how it works!
char * strInsert(char * out, const char * in, size_t pos, bool overwrite) {
	size_t in_len = strlen(in);
	//Right strShift for out
	size_t strShift = pos + in_len;
	long long count = strlen(out) - pos;
	if (overwrite == false) count -= in_len + 1;
	for (long long i = (long long)count; i >= 0; i--)
		out[i + strShift] = out[i + pos];
	//strInserting
	for (size_t i = 0; i < in_len; i++) out[pos+i] = in[i];
	return out;
}
//TODO: need tests
char * strShift(char * where, long long strShift) {
	memmove(where, where - strShift, strlen(where - strShift));
	return where;
}

char * strShiftPosition(char * where, long long strShift) {
	memmove(where + strShift, where, strlen(where));
	return where + strShift;
	/*
		//aaaaBaaaa
		//B 3
		//aaaa___Baaaa
		
		//aaaaBaaaa
		//B -3
		//aBaaaa
	*/
}

char * strEnd(char * in) {
	return in + strlen(in);
}
char * strReset(char ** in) {
	*in[0] = 0;
	return *in;
}
size_t strLength(const char * in) {
	return strlen(in);
}

char * strLast(char * in) {
	return in + strlen(in) - 1;
}

size_t strCountSubstr(const char * where, const char * what) {
	size_t what_len = strlen(what);
	size_t count = 0;
	while (*where != 0) {
		if (strncmp(where, what, what_len) == 0) {
			count += 1;
			where += what_len;
			continue;
		}
		where += 1;
	}
	return count;
}

size_t strCountChar(const char * where, const char what) {
	size_t count = 0;
	while (*where++ != 0) if (*where == what) count++;
	return count;
}

long long strToInt(const char * str) {
	long long ans = 0;
	int minus = -1;
	if (*str == '-') str++;
	else minus *= -1;

	while (*str) {
		ans *= 10;
		ans += *str - 48;
		str++;
	}
	return ans * minus;
}
double strToDouble(const char * str) {
	int minus = -1;

	if (*str == '-') str++;
	else minus *= -1;

	long long ans = 0;
	while (true) {
		if (*str == 0) return (double)ans;
		if (*str == '.') break;
		ans *= 10;
		ans += *str - 48;
		str++;
	}
	str++;
	double afterdot = 0;
	size_t order = 10;
	while (*str) {
		afterdot += (*str - 48) / order;
		order *= 10;
		str++;
	}
	return (afterdot + (double)ans) * minus;
}
char * strToUpperCase(char * str) {
	while (*str) {
		*str = caseUpper[(unsigned char)(*str)];
		str++;
	}
	return str;
}
char * strToLowerCase(char * str) {
	while (*str) {
		*str = caseLower[(unsigned char)(*str)];
		str++;
	}
	return str;
}

void strPrintDebug(char * str) {
	for (size_t i = 0; i < strlen(str); i++) {
		printf("%c | %i\n", str[i], str[i]);
	}
	printf("\n");
}

#endif
