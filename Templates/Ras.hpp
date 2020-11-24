#ifndef UF_T_Ras_H
#define UF_T_Ras_H

#include "Array.hpp"

#include <queue>

//UseFull Templates Random Access Storage module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace uft {
	template <typename Type>
	void clearQueue(std::queue<Type> & q){
	   std::queue<Type> empty;
	   std::swap(q, empty);
	}

	template <typename T>
	class Ras;

	template <typename T>
	struct RasRec {
		Ras<T> * storage;
		T * record = nullptr;
		bool deleted = false;
		size_t pos;
		RasRec<T> * remove();
	};

	template <typename T>
	class Ras {
	public:
		size_t * current = nullptr;
		std::queue<size_t> emp_pos;
		uft::Array<RasRec<T> *> set = uft::Array<RasRec<T> *>(4); 
		size_t & length = set.length;
		void * owner = nullptr;
		
		Ras() {
			
		}
		~Ras() {
			
		}
		Ras<T> * flush() {
			set.removeAll();
			clearQueue(emp_pos);
			return this;
		}
		Ras<T> * flushHard() {
			for (size_t i = 0; i < set.length; i++) {
				delete set[i]->record;
				delete set[i];
			}
			clearQueue(emp_pos);
			set.removeAll();
			return this;
		}
		bool contains(T * ptr) {
			for (size_t i = 0; i < set.length; i++) 
				if (set[i]->record == ptr) return true;
			return false;
		}
		RasRec<T> * add(T * ptr) {
			RasRec<T> * buf = new RasRec<T>;

			buf->storage = this;
			buf->pos = set.length;
			buf->record = ptr;

			set.addCopy(buf);
			return buf;
		}

		void _fixArray() {
			while (emp_pos.size()) {
				size_t bnum = emp_pos.front();
				emp_pos.pop();
				while (set[set.length - 1]->deleted) {
					delete set[set.length - 1];
					set.length -= 1;
					if (set.length == 0) {
						clearQueue(emp_pos);
						return;
					}
				}
				if (bnum >= set.length) continue;
				set[bnum] = set[set.length - 1];
				set[bnum]->pos = bnum;
				set[set.length - 1] = nullptr;
				set.length -= 1;
			}
		}
		RasRec<T> * remove(RasRec<T> * ptr) {
			if (ptr->storage != this) {
				printf("Ras::removeWeak::ptr->storage != this\n");
				exit(1);
			}
			else {
				size_t ibuf = ptr->pos;
				bool active = true;
				if (current != nullptr) 
					active = ibuf > *current;
				if (active) {
					delete ptr;
					if (ibuf != set.length - 1) {
						set[ibuf] = set[set.length - 1];
						set[ibuf]->pos = ibuf;
					}
					set[set.length - 1] = nullptr;
					set.length -= 1;
				}
				else {
					set[ptr->pos]->deleted = true; //?
					emp_pos.push(ibuf);
				}
			}
			return nullptr;
		}
		RasRec<T> * removeWeak(RasRec<T> * ptr) {
			if (ptr->storage != this) {
				printf("Ras::removeWeak::ptr->storage != this\n");
				exit(1);
			}
			else {
				size_t ibuf = ptr->pos;
				delete ptr;
				if (ibuf != set.length - 1) {
					set[ibuf] = set[set.length - 1];
					set[ibuf]->pos = ibuf;
				}
				set[set.length - 1] = nullptr;
				set.length -= 1;
			}
			return nullptr;
		}
		T * operator [] (size_t i) const {return set[i]->record;}
	};

	template <typename T>
	RasRec<T> * RasRec<T>::remove() {
		if (storage != nullptr) return storage->remove(this);
		else exit(1);
	}
}

#endif