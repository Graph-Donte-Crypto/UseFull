#ifndef UF_S_Semantic_H
#define UF_S_Semantic_H

#include <stdlib.h>

//UseFull String Semantic module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

namespace str  {

	struct Semantic {
		const char * begin;
		const char * end;
		bool handle_internal;
	};
	
	Semantic NewSemantic(const char * begin, const char * end, bool handle_internal) {
		Semantic semantic;
		semantic.begin = begin;
		semantic.end = end;
		semantic.handle_internal = handle_internal;
		return semantic;
	}

	const char * parseSemantic
		(const char * source, const char * spliter, size_t sem_count, const Semantic * semantics) {
		size_t l_begin[sem_count];
		size_t l_end[sem_count];
		size_t l_spl = strlen(spliter);
		for (size_t i = 0; i < sem_count; i++) {
			l_begin[i] = strlen(semantics[i].begin);
			l_end[i] = strlen(semantics[i].end);
		}
		
		size_t q[strlen(source)];
		size_t q_i = 0;
		
		cycl_start:
			//printf("q_i = %lu\n", (long unsigned)q_i);
			if (*source == 0) goto cycl_end;
			if (q_i == 0) {
				//Если текущий индекс == 0, то handle_internal по умлочанию true, не надо делать проверки на end
				for (size_t i = 0; i < sem_count; i++)
					if (strncmp(source, semantics[i].begin, l_begin[i]) == 0) {
						q_i += 1;
						q[q_i] = i;
						source += l_begin[i];
						goto cycl_start;
					}	
				if (strncmp(source, spliter, l_spl) == 0) {
					return source + l_spl;
				}
			}
			else {
				//Иначе, делаем проверку на конец
				if (strncmp(source, semantics[q[q_i]].end, l_end[q[q_i]]) == 0) {
					source += l_end[q[q_i]];
					q_i -= 1;
					goto cycl_start;
				}
				else {
					//Если не прошло, то уже чекаем на handle_internal
					if (semantics[q[q_i]].handle_internal) {		
						//Если можно - ищем начало всех семантик
						for (size_t i = 0; i < sem_count; i++)
							if (strncmp(source, semantics[i].begin, l_begin[i]) == 0) {
								source += l_begin[i];
								q_i += 1;
								q[q_i] = i;
								goto cycl_start;
							}
					}
					else {
						//Иначе - только текущей
						if (strncmp(source, semantics[q[q_i]].begin, l_begin[q[q_i]]) == 0) {
							source += l_begin[q[q_i]];
							q_i += 1;
							q[q_i] = q[q_i - 1];
							goto cycl_start;
						}
					}
				}
			}
			source += 1;
			goto cycl_start;
		cycl_end:
		return source;
	}
}

#endif