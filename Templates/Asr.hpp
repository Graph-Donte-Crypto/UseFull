#ifndef UF_T_Asr_H
#define UF_T_Asr_H

#include "Interface.hpp"
#include <functional>

//UseFull Template Asr module
//Version 1.1 release
//Make by Graph Don'te-Crypto

namespace uft {
	template <typename type>
	struct Asr;

	template <typename type>
	struct AsrRecord {
	    Asr<type> * asr;
	    type * record = nullptr;
	    AsrRecord<type> * prev = nullptr, * next = nullptr;
	};

	template <typename type>
	struct Asr : public ICollection<type> {
		size_t count = 0;
		AsrRecord<type> * asr_head;
		AsrRecord<type> * asr_last;
		//ICollection
		virtual type & operator[] (size_t index) const {
			AsrRecord<type> * ans = asr_head->next; 
			for (size_t i = 0; i < index; i++) ans = ans->next;
			return *(ans->record);
		};
		virtual size_t size() const {return count;};
		virtual size_t length() const {return count;};
		Asr<type> & foreach(std::function<void (type *)> lambda) {
			for (AsrRecord<type> * i = asr_head->next; i != asr_last; i = i->next) lambda(i->record);
			return *this;
		}
		//
		void AsrDoubleLink(AsrRecord<type> * rec1, AsrRecord<type> * rec2) {
			rec1->next = rec2;
			rec2->prev = rec1;
		}

		Asr() {
			asr_head = new AsrRecord<type>;
			asr_last = new AsrRecord<type>;

			asr_head->asr = this;
			asr_last->asr = this;

			AsrDoubleLink(asr_head, asr_last);

			asr_head->record = nullptr;
			asr_head->prev = nullptr;

			asr_last->record = nullptr;
			asr_last->next = nullptr;
		}
		~Asr() {
			AsrRecord<type> * buf = asr_head->next;
			while (buf->next != nullptr) {
				buf = buf->next;
				delete buf->prev;
			}

			delete asr_head;
			delete asr_last;
		}
		AsrRecord<type> * add(type * add_object) {
			count += 1;

			AsrRecord<type> * add_record = new AsrRecord<type>;
			add_record->asr = this;
			add_record->record = add_object;

			AsrRecord<type> * local_prev = asr_last->prev;
			AsrDoubleLink(local_prev, add_record);
			AsrDoubleLink(add_record, asr_last);

			return add_record;
		}
		AsrRecord<type> * remove(AsrRecord<type> * remove_record) {
			count -= 1;

			AsrRecord<type> * local_prev = remove_record->prev;
			AsrRecord<type> * local_next = remove_record->next;
			AsrDoubleLink(local_prev,local_next);
			delete remove_record;
			return nullptr;
		}
		
			
	};
}

#endif
