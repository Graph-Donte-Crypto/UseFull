#ifndef UF_S_Slice_H
#define UF_S_Slice_H

#include "BaseLib.hpp"
#include "../Templates/Asr.hpp"
#include "../Templates/List.hpp"
#include "../Utils/stalloc.hpp"
#include "Semantic.hpp"

/*
	Строка должна уметь
		Строка должна быть в стэке
		Любое изменение строки производится со всей строкой
		Из любой строки можно pick подстроку, изменение которой изменит базовую строку
		Подстроки могут быть только вложенными, если подстроки пересекаются, то обе подстроки получают модификатор read_only


*/

namespace str {
		
	struct SliceSet;
	
	struct ReadOnlySlice;
	
	struct ReadOnlySliceSet : public uft::List<ReadOnlySlice> {
		ReadOnlySliceSet() : List<ReadOnlySlice>() {
			
		}
		~ReadOnlySliceSet() {
			
		}
		
		ReadOnlySliceSet pickBeetwenChar  (char c1, char c2);
		ReadOnlySliceSet pickBeetwenSubstr(char * substr1, char * substr2);
		ReadOnlySliceSet pickBeforeChar   (char c);
		ReadOnlySliceSet pickBeforeSubstr (char * substr);
		ReadOnlySliceSet pickAfterChar    (char c);
		ReadOnlySliceSet pickAfterSubstr  (char * substr);
		ReadOnlySliceSet pickSubstr       (char * substr);
		ReadOnlySliceSet pickChar         (char c);
		ReadOnlySliceSet pickSplitByChar  (char c);
		ReadOnlySliceSet pickSplitBySubstr(char * substr);
		
		ReadOnlySliceSet pickBeetwenCharFirst  (char c1, char c2);
		ReadOnlySliceSet pickBeetwenSubstrFirst(char * substr1, char * substr2);
		ReadOnlySliceSet pickBeforeCharFirst   (char c);
		ReadOnlySliceSet pickBeforeSubstrFirst (char * substr);
		ReadOnlySliceSet pickAfterCharFirst    (char c);
		ReadOnlySliceSet pickAfterSubstrFirst  (char * substr);
		ReadOnlySliceSet pickSubstrFirst       (char * substr);
		ReadOnlySliceSet pickCharFirst         (char c);
		
		ReadOnlySliceSet pickBeetwenCharLast  (char c1, char c2);
		ReadOnlySliceSet pickBeetwenSubstrLast(char * substr1, char * substr2);
		ReadOnlySliceSet pickBeforeCharLast   (char c);
		ReadOnlySliceSet pickBeforeSubstrLast (char * substr);
		ReadOnlySliceSet pickAfterCharLast    (char c);
		ReadOnlySliceSet pickAfterSubstrLast  (char * substr);
		ReadOnlySliceSet pickSubstrLast       (char * substr);
		ReadOnlySliceSet pickCharLast         (char c);
	};

	struct ReadOnlySlice {
		const char * ptr = nullptr;
		size_t len = 0;
		size_t size = 0;
		
        operator const char * () const { return (const char *)ptr;}
		
		void constructor(const char * ptr, size_t len, size_t size) {
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
			printf("ptr(%lu) -> %s\n    this = %lu\n    size = %lu\n    length = %lu\n", (unsigned long)(size_t)ptr, buf, (long unsigned)(size_t)this, (long unsigned)size, (long unsigned)len);
		}
		
		ReadOnlySlice() {}
		ReadOnlySlice(const char * ptr, size_t len, size_t size) 
			{constructor(ptr,         len, size       );}
		ReadOnlySlice(const char * ptr) 
			{constructor(ptr, strlen(ptr), strlen(ptr));}
		ReadOnlySlice(const char * ptr, size_t size) 
			{constructor(ptr, strlen(ptr), size       );}
		
		ReadOnlySlice(const ReadOnlySlice & slice) {
			ptr = slice.ptr;
			len = slice.len;
			size = slice.size;
		}
		~ReadOnlySlice() {
			
		}

		ReadOnlySlice pick(size_t from, size_t to) {
			return ReadOnlySlice(ptr + from, strnlen(ptr + from, to - from), to - from);
		}
		ReadOnlySliceSet pickBeetwenChar  (char c1, char c2) {
			ReadOnlySliceSet set;
			
			long long index1 = 0, index2 = 0;
			
			while (true) {
				index1 = str::Pos(ptr + index2, c1);
				if (index1 == -1) return set;
				index2 = str::Pos(ptr + index1 + 1, c2);
				if (index2 == -1) return set;
				set.addCopy(pick(index1, index2));
			}
			
		}
		ReadOnlySliceSet pickBeetwen(char * substr1, char * substr2) {
			ReadOnlySliceSet set;
			
			long long index1 = 0, index2 = 0;
			
			while (true) {
				index1 = str::Pos(ptr + index2, substr1);
				if (index1 == -1) return set;
				index2 = str::Pos(ptr + index1 + 1, substr2);
				if (index2 == -1) return set;
				set.addCopy(pick(index1, index2));
			}
		}
		ReadOnlySliceSet pickBefore (char c) {
			ReadOnlySliceSet set;
			long long index = str::Pos(ptr, c);
			
			while (index != -1) {
				set.addCopy(pick(0, index));
				index = str::Pos(ptr + index + 1, c);
			}
			return set;
		}
		ReadOnlySliceSet pickBefore (char * substr) {
			ReadOnlySliceSet set;
			long long index = str::Pos(ptr, substr);
			
			while (index != -1) {
				set.addCopy(pick(0, index));
				index = str::Pos(ptr + index + 1, substr);
			}
			return set;
		}
		ReadOnlySliceSet pickAfter  (char c) {
			ReadOnlySliceSet set;
			long long index = str::Pos(ptr, c);
			
			while (index != -1) {
				set.addCopy(pick(index, len));
				index = str::Pos(ptr + index + 1, c);
			}
			return set;
		}
		ReadOnlySliceSet pickAfter  (char * substr) {
			ReadOnlySliceSet set;
			long long index = str::Pos(ptr, substr);
			
			while (index != -1) {
				set.addCopy(pick(index, len));
				index = str::Pos(ptr + index + 1, substr);
			}
			return set;
		}
		ReadOnlySliceSet pickSubstr (char * substr) {
			ReadOnlySliceSet set;
			size_t s_len = strlen(substr);
			long long index = str::Pos(ptr, substr);
			
			while (index != -1) {
				set.addCopy(pick(index, index + s_len));
				index = str::Pos(ptr + index + 1, substr);
			}
			return set;
		}
		ReadOnlySliceSet pickChar   (char c) {
			ReadOnlySliceSet set;
			long long index = str::Pos(ptr, c);
			
			while (index != -1) {
				set.addCopy(pick(index, index + 1));
				index = str::Pos(ptr + index + 1, c);
			}
			return set;
		}
		ReadOnlySliceSet pickSplitByChar  (char c) {
			ReadOnlySliceSet set;
			long long index1 = 0;
			long long index2 = str::Pos(ptr, c);
			
			while (index2 != -1) {
				set.addCopy(pick(index1, index2 - 1));
				index1 = index2 + 1;
				index2 = str::Pos(ptr + index1, c);
			}
			return set;
		}
		ReadOnlySliceSet pickSplitBySubstr(char * substr) {
			ReadOnlySliceSet set;
			size_t s_len = strlen(substr);
			long long index1 = 0;
			long long index2 = str::Pos(ptr, substr);
			
			while (index2 != -1) {
				set.addCopy(pick(index1, index2 - 1));
				index1 = index2 + s_len + 1;
				index2 = str::Pos(ptr + index1, substr);
			}
			return set;
		}
		
		
		Ok<ReadOnlySlice> pickBeetwenFirst(char c1, char c2) {
			long long i1 = str::Pos(ptr, c1);
			long long i2 = str::Pos(ptr + i1 + 1, c2);
			if (i1 == -1 || i2 == -1) return {};
			else return pick(i1 + 1, i2 - 1);
		}
		Ok<ReadOnlySlice> pickBeetwenFirst(char * substr1, char * substr2) {
			long long i1 = str::Pos(ptr, substr1);
			long long i2 = str::Pos(ptr + i1 + strlen(substr1), substr2);
			if (i1 == -1 || i2 == -1) return {};
			else return pick(i1 + strlen(substr1), i2 - 1);
		}
		Ok<ReadOnlySlice> pickBeforeFirst (char c) {
			long long i = str::Pos(ptr, c);
			if (i == -1) return {};
			return pick(0, i - 1);
		}
		Ok<ReadOnlySlice> pickBeforeFirst (char * substr) {
			long long i = str::Pos(ptr, substr);
			if (i == -1) return {};
			return pick(0, i - 1);
		}
		Ok<ReadOnlySlice> pickAfterFirst  (char c) {
			long long i = str::Pos(ptr, c);
			if (i == -1) return {};
			return pick(i + 1, len);
		}
		Ok<ReadOnlySlice> pickAfterFirst  (char * substr) {
			long long i = str::Pos(ptr, substr);
			if (i == -1) return {};
			return pick(i + strlen(substr), len);
		}
		Ok<ReadOnlySlice> pickCharFirst   (char c) {
			long long i = str::Pos(ptr, c);
			if (i == -1) return {};
			return pick(i, i + 1);
		}
		Ok<ReadOnlySlice> pickSubstrFirst (char * substr) {
			long long i = str::Pos(ptr, substr);
			if (i == -1) return {};
			return pick(i, i + strlen(substr));
		}
		
		
		Ok<ReadOnlySlice> pickBeetwenLast (char c1, char c2) {
			long long i1 = str::PosLast(ptr, c1);
			long long i2 = str::PosLast(ptr + i1 + 1, c2);
			if (i1 == -1 || i2 == -1) return {};
			else return pick(i1 + 1, i2 - 1);
		}
		Ok<ReadOnlySlice> pickBeetwenLast (char * substr1, char * substr2) {
			long long i1 = str::PosLast(ptr, substr1);
			long long i2 = str::PosLast(ptr + i1 + strlen(substr1), substr2);
			if (i1 == -1 || i2 == -1) return {};
			else return pick(i1 + strlen(substr1), i2 - 1);
		}
		Ok<ReadOnlySlice> pickBeforeLast  (char c) {
			long long i = str::PosLast(ptr, c);
			if (i == -1) return {};
			return pick(0, i - 1);
		}
		Ok<ReadOnlySlice> pickBeforeLast  (char * substr) {
			long long i = str::PosLast(ptr, substr);
			if (i == -1) return {};
			return pick(0, i - 1);
		}
		Ok<ReadOnlySlice> pickAfterLast   (char c) {
			long long i = str::PosLast(ptr, c);
			if (i == -1) return {};
			return pick(i + 1, len);
		}
		Ok<ReadOnlySlice> pickAfterLast   (char * substr) {
			long long i = str::PosLast(ptr, substr);
			if (i == -1) return {};
			return pick(i + strlen(substr), len);
		}
		Ok<ReadOnlySlice> pickCharLast    (char c) {
			long long i = str::PosLast(ptr, c);
			if (i == -1) return {};
			return pick(i, i + 1);
		}
		Ok<ReadOnlySlice> pickSubstrLast  (char * substr) {
			long long i = str::PosLast(ptr, substr);
			if (i == -1) return {};
			return pick(i, i + strlen(substr));
		}
	};
	
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

}

#endif
