#ifndef UF_S_Slice_H
#define UF_S_Slice_H

#include "BaseLib.h"
#include "../Templates/Asr.h"
#include "../Templates/List.h"
#include "../Utils/stalloc.h"

/*
	Строка должна уметь
		Строка должна быть в стэке
		Любое изменение строки производится со всей строкой
		Из любой строки можно pick подстроку, изменение которой изменит базовую строку
		Подстроки могут быть только вложенными, если подстроки пересекаются, то обе подстроки получают модификатор read_only


*/

namespace str {
		
	template <typename T>
	struct List {
		
	};
		
	struct SliceSet;
	
	struct Slice {
		bool read_only = false;
		char * ptr = nullptr;
		size_t len = 0;
		size_t size = 0;
		Slice * parent = nullptr;
		uft::Asr<Slice> slices;
		uft::AsrRecord<Slice> * slices_record = nullptr;

		void constructor(char * ptr, size_t len, size_t size) {
			this->ptr = ptr;
			this->len = len;
			this->size = size;
		}
		
		void print() {
			char * buf = stalloc(size + 1, char);
			memcpy(buf, ptr, size);
			buf[size] = 0;
			printf("%s\n", buf);
		}
		
		void _printInfo() {
			char * buf = stalloc(size + 1, char);
			memcpy(buf, ptr, size);
			buf[size] = 0;
			printf("ptr(%lu) -> %s\n    this = %lu\n    parent = %lu\n    size = %lu\n    length = %lu\n    read_only = %i\n", (unsigned long)(size_t)ptr, buf, (long unsigned)(size_t)this, (long unsigned)(size_t)(parent), (long unsigned)size, (long unsigned)len, read_only);
		}

		Slice() {}
		Slice(      char * ptr, size_t len, size_t size) {constructor(ptr, len, size);}
		Slice(const char * ptr) {
			read_only = true;
			constructor((char *)ptr, strlen(ptr), strlen(ptr));
		}
		Slice(char * ptr) {constructor(ptr, strlen(ptr), strlen(ptr));}
		Slice(char * ptr, size_t size) {constructor(ptr, strlen(ptr), size);}
		
		Slice(const Slice & slice) {
			read_only = slice.read_only;
			ptr = slice.ptr;
			len = slice.len;
			size = slice.size;
			parent = slice.parent;
			slices = slice.slices;
			slices_record = parent->slices.add(this);
		}
		~Slice() {
			if (slices_record) slices_record = slices.remove(slices_record);
		}

		Slice & operator = (const Slice & slice) {
			set(slice);
			return *this;
		}

		/*
			QWEHelloworld
			QWE ello orld
			     ll
		
			QWEHel___loworld
			QWE ello orld
			     ll
		
		*/

		void recursRightShift(char * from, size_t count) {
			slices.foreach( [&from, &count] (Slice * slice) {				
				if (slice->ptr + slice->size - 1 > from) {
					slice->ptr += count;
					slice->recursRightShift(from, count);
				}
				else if (from > slice->ptr && from < slice->ptr + slice->size) {
					slice->size += count;
					slice->len += count;
					slice->recursRightShift(from, count);
				}
			});
		}
		
		Slice rightShift(char * from, size_t count) {
			if (parent) parent->rightShift(from, count);
			else {
				//_printInfo();
				//RightShift TODO: CHECK
				str::ShiftPosition(from, count);
				size += count;
				len += count;
				//
				//_printInfo();
				recursRightShift(from, count);
			}
			return pick(from - ptr, from - ptr + count);
		}
		void leftShift(char * from, size_t count) {

		}

		Slice & set(const Slice & slice) {
			if (slices.count == 0) {
				if (size < slice.size)      rightShift(ptr + size - 1, slice.size - size);
				else if (size > slice.size) leftShift (ptr + size - 1, size - slice.size);
				
				memcpy(ptr, slice.ptr, size);
				len = slice.len;
			}
			return *this;
		}
		Slice & set(      char * ptr) {return set(Slice(ptr));}
		Slice & set(const char * ptr) {return set(Slice(ptr));}

		Slice & unpick() {
			if (slices_record) {
				read_only = true;
				slices_record = slices.remove(slices_record);
			}
			return *this;
		}

		Slice pick(size_t from, size_t to) {
			Slice slice(ptr + from, strnlen(ptr + from, to - from), to - from);
			slice.read_only = read_only;
			slice.parent = this;
			slice.slices_record = slices.add(&slice);
			return slice;
		}
		SliceSet pickBeetwenChar  (char c1, char c2);
		SliceSet pickBeetwenSubstr(char * substr1, char * substr2);
		SliceSet pickBeforeChar   (char c);
		SliceSet pickBeforeSubstr (char * substr);
		SliceSet pickAfterChar    (char c);
		SliceSet pickAfterSubstr  (char * substr);
		SliceSet pickSubstr       (char * substr);
		SliceSet pickChar         (char c);
		SliceSet pickSplitByChar  (char c);
		SliceSet pickSplitBySubstr(char * substr);
		
		Slice pickBeetwenCharFirst  (char c1, char c2);
		Slice pickBeetwenSubstrFirst(char * substr1, char * substr2);
		Slice pickBeforeCharFirst   (char c);
		Slice pickBeforeSubstrFirst (char * substr);
		Slice pickAfterCharFirst    (char c);
		Slice pickAfterSubstrFirst  (char * substr);
		Slice pickSubstrFirst       (char * substr);
		Slice pickCharFirst         (char c);
		
		Slice pickBeetwenCharLast  (char c1, char c2);
		Slice pickBeetwenSubstrLast(char * substr1, char * substr2);
		Slice pickBeforeCharLast   (char c);
		Slice pickBeforeSubstrLast (char * substr);
		Slice pickAfterCharLast    (char c);
		Slice pickAfterSubstrLast  (char * substr);
		Slice pickSubstrLast       (char * substr);
		Slice pickCharLast         (char c);
	};
	
	struct SliceSet : List<Slice> {
		
	};

	template <size_t SIZE>
	struct SliceBuf : public Slice {
		char buf[SIZE];
		SliceBuf() : Slice() {
			buf[0] = 0;
			ptr = buf;
			size = SIZE;
		}
	};

}

#endif
