#ifndef UF_S_Semantic_H
#define UF_S_Semantic_H

#include <stdlib.h>
#include "BaseLib.h"

//UseFull String Semantic module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

struct Semantic {
	const char * begin;
	size_t l_begin;
	const char * end;
	size_t l_end;
	bool handle_internal;
};

struct Semantic newSemantic(const char * begin, const char * end, bool handle_internal) {
	struct Semantic semantic;
	semantic.begin = begin;
	semantic.end = end;
	semantic.handle_internal = handle_internal;
	semantic.l_begin = strlen(begin);
	semantic.l_end = strlen(end);
	return semantic;
}

struct SemanticHandler {
	const struct Semantic * sems;
	size_t count;
};

struct SemanticHandler newSemanticHandler(const struct Semantic * sems, size_t count) {
	struct SemanticHandler semantic_handler;
	semantic_handler.sems = sems;
	semantic_handler.count = count;
	return semantic_handler;
}

const char * parseSemantic
	(const char * source, const char * spliter, struct SemanticHandler sh) {
	size_t l_spl = strlen(spliter);
	const struct Semantic * sems = sh.sems;
	size_t * q = (size_t *)malloc(sizeof(size_t) * strlen(source));
	size_t q_i = 0;
	
	cycl_start:
		//printf("q_i = %lu\n", (long unsigned)q_i);
		if (*source == 0) goto cycl_end;
		if (q_i == 0) {
			//Если текущий индекс == 0, то handle_internal по умлочанию true, не надо делать проверки на end
			for (size_t i = 0; i < sh.count; i++)
				if (strncmp(source, sems[i].begin, sems[i].l_begin) == 0) {
					q_i += 1;
					q[q_i] = i;
					source += sems[i].l_begin;
					goto cycl_start;
				}	
			if (strncmp(source, spliter, l_spl) == 0) {
				free(q);
				return source + l_spl;
			}
		}
		else {
			//Иначе, делаем проверку на конец
			if (strncmp(source, sems[q[q_i]].end, sems[q[q_i]].l_end) == 0) {
				source += sems[q[q_i]].l_end;
				q_i -= 1;
				goto cycl_start;
			}
			else {
				//Если не прошло, то уже чекаем на handle_internal
				if (sems[q[q_i]].handle_internal) {		
					//Если можно - ищем начало всех семантик
					for (size_t i = 0; i < sh.count; i++)
						if (strncmp(source, sems[i].begin, sems[i].l_begin) == 0) {
							source += sems[i].l_begin;
							q_i += 1;
							q[q_i] = i;
							goto cycl_start;
						}
				}
				else {
					//Иначе - только текущей
					if (strncmp(source, sems[q[q_i]].begin, sems[q[q_i]].l_begin) == 0) {
						source += sems[q[q_i]].l_begin;
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
	free(q);
	return source;
}

struct SemanticDataHolder {
	const char * string;
	struct SemanticHandler sh;
	unsigned * stack;
	size_t stack_index;
};

const char * getSemanticCharWithDataHolder(struct SemanticDataHolder * data) {
	
	const char ** string = & data->string;
	struct SemanticHandler * sh = & data->sh;
	unsigned * stack     =   data->stack; //stack is array
	size_t * stack_index = & data->stack_index;
	
	if (**string == 0) return *string;
	
	if (*stack_index == 0) {
		//Если текущий индекс == 0, то handle_internal по умлочанию true, не надо делать проверки на end
		for (size_t i = 0; i < sh->count; i++)
			if (strncmp(*string, sh->sems[i].begin, sh->sems[i].l_begin) == 0) {
				*stack_index += 1;
				stack[*stack_index] = i;
				*string += sh->sems[i].l_begin;
				return NULL;
			}
		*string += 1;
		return *string - 1;
	}
	else {
		//Иначе, делаем проверку на конец
		if (strncmp(*string, sh->sems[stack[*stack_index]].end, sh->sems[stack[*stack_index]].l_end) == 0) {
			*string += sh->sems[stack[*stack_index]].l_end;
			*stack_index -= 1;
			return NULL;
		}
		else {
			//Если не прошло, то уже чекаем на handle_internal
			if (sh->sems[stack[*stack_index]].handle_internal) {
				//Если можно - ищем начало всех семантик
				for (size_t i = 0; i < sh->count; i++) {
					if (strncmp(*string, sh->sems[i].begin, sh->sems[i].l_begin) == 0) {
						*string += sh->sems[i].l_begin;
						*stack_index += 1;
						stack[*stack_index] = i;
						return NULL;
					}
				}
			}
			else {
				//Иначе - только текущей
				if (strncmp(*string, sh->sems[stack[*stack_index]].begin, sh->sems[stack[*stack_index]].l_begin) == 0) {
					*string += sh->sems[stack[*stack_index]].l_begin;
					*stack_index += 1;
					stack[*stack_index] = stack[*stack_index - 1];
					return NULL;
				}
			}
		}
	}
	*string += 1;
	return NULL;
}

const char * getSemanticChar
	/*
	s  - source
	q  - queue
	qi - queue index
	*/
	(const char ** s, const struct SemanticHandler * sh, unsigned * q, size_t * qi) {
	
	if (**s == 0) return *s;
	
	if (*qi == 0) {
		//Если текущий индекс == 0, то handle_internal по умлочанию true, не надо делать проверки на end
		for (size_t i = 0; i < sh->count; i++)
			if (strncmp(*s, sh->sems[i].begin, sh->sems[i].l_begin) == 0) {
				*qi += 1;
				q[*qi] = i;
				*s += sh->sems[i].l_begin;
				return NULL;
			}
		*s += 1;
		return *s - 1;
	}
	else {
		//Иначе, делаем проверку на конец
		if (strncmp(*s, sh->sems[q[*qi]].end, sh->sems[q[*qi]].l_end) == 0) {
			*s += sh->sems[q[*qi]].l_end;
			*qi -= 1;
			return NULL;
		}
		else {
			//Если не прошло, то уже чекаем на handle_internal
			if (sh->sems[q[*qi]].handle_internal) {
				//Если можно - ищем начало всех семантик
				for (size_t i = 0; i < sh->count; i++) {
					if (strncmp(*s, sh->sems[i].begin, sh->sems[i].l_begin) == 0) {
						*s += sh->sems[i].l_begin;
						*qi += 1;
						q[*qi] = i;
						return NULL;
					}
				}
			}
			else {
				//Иначе - только текущей
				if (strncmp(*s, sh->sems[q[*qi]].begin, sh->sems[q[*qi]].l_begin) == 0) {
					*s += sh->sems[q[*qi]].l_begin;
					*qi += 1;
					q[*qi] = q[*qi - 1];
					return NULL;
				}
			}
		}
	}
	*s += 1;
	return NULL;
}

struct SemanticBaseLib {
	struct SemanticHandler sh;
};

struct SemanticBaseLib newSemanticBaseLib(const struct Semantic * s, size_t c) {
	struct SemanticBaseLib base_lib;
	base_lib.sh = newSemanticHandler(s, c);
	return base_lib;
}

#endif
