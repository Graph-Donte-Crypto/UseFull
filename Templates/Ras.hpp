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
		std::queue<size_t> empty_positions;
		uft::Array<RasRec<T> *> set = uft::Array<RasRec<T> *>(4); 
		size_t & length = set.length;
		void * owner = nullptr;
		
		Ras() {
			
		}
		~Ras() {
			
		}
		Ras<T> * flush() {
			set.removeAll();
			clearQueue(empty_positions);
			return this;
		}
		Ras<T> * flushHard() {
			for (size_t i = 0; i < set.length; i++) {
				delete set[i]->record;
				delete set[i];
			}
			clearQueue(empty_positions);
			set.removeAll();
			return this;
		}
		bool contains(T * element) {
			for (size_t i = 0; i < set.length; i++) 
				if (set[i]->record == element) return true;
			return false;
		}
		RasRec<T> * add(T * element) {
			RasRec<T> * buf = new RasRec<T>;

			buf->storage = this;
			buf->pos = set.length;
			buf->record = element;

			set.addCopy(buf);
			return buf;
		}

		void _fixArray() {
			while (empty_positions.size()) {
				size_t empty_position = empty_positions.front();
				empty_positions.pop();
				while (set[set.length - 1]->deleted) {
					delete set[set.length - 1];
					set.length -= 1;
					if (set.length == 0) {
						clearQueue(empty_positions);
						return;
					}
				}
				if (empty_position >= set.length) continue;
				set[empty_position] = set[set.length - 1];
				set[empty_position]->pos = empty_position;
				set[set.length - 1] = nullptr;
				set.length -= 1;
			}
		}
		RasRec<T> * remove(RasRec<T> * element) {
			if (element->storage != this) {
				printf("Ras::remove::ptr->storage != this\n");
				exit(1);
			}
			else {
				size_t element_position = element->pos;
				bool active = true;
				if (current != nullptr) 
					active = element_position > *current;
				if (active) {
					delete element;
					if (element_position != set.length - 1) {
						set[element_position] = set[set.length - 1];
						set[element_position]->pos = element_position;
					}
					set[set.length - 1] = nullptr;
					set.length -= 1;
				}
				else {
					set[element->pos]->deleted = true; //?
					empty_positions.push(element_position);
				}
			}
			return nullptr;
		}
		RasRec<T> * removeWeak(RasRec<T> * element) {
			if (element->storage != this) {
				printf("Ras::removeWeak::ptr->storage != this\n");
				exit(1);
			}
			else {
				size_t element_position = element->pos;
				delete element;
				if (element_position != set.length - 1) {
					set[element_position] = set[set.length - 1];
					set[element_position]->pos = element_position;
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