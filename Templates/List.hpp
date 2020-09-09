#ifndef UF_T_List_H
#define UF_T_List_H

#include "Interface.hpp"

//UseFull Template List module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace uft {
	template <typename type>
	struct List {

		//CHECKED
		template <typename>
		struct ListRec {
			type * value = nullptr;
			void setptr(type * val) {value = val;}
			ListRec<type> * next = nullptr;
			ListRec<type> * prev = nullptr;
		};

		template <typename>
		struct ListRecCopy : public ListRec<type> {
			type   value_r;
			ListRecCopy() { ListRec<type>::setptr(&value_r);}
		};

		ListRec<type> * list_head = nullptr;
		ListRec<type> * list_last = nullptr;
		size_t length = 0;
		template <typename>
		struct ListCursor {
			size_t index = 0;
			List<type> * current = nullptr;
		};


		void ListDoubleLink(ListRec<type> * rec1, ListRec<type> * rec2) {
			rec1->next = rec2;
			rec2->prev = rec1;
		}


		List() {
			list_head = new ListRec<type>;
			list_last = new ListRec<type>;

			ListDoubleLink(list_head, list_last);
		}
		//
		~List() {
			ListRec<type> * buf = list_head->next;
			while (buf->next != nullptr) {
				buf = buf->next;
				delete buf->prev;
			}

			delete list_head;
			delete list_last;
		}
		//CHECKED

		type * get(size_t index) const {
			return & ((*this)[index]);
		}

		type & operator[] (size_t index) const {
			ListRec<type> * record = list_head->next;
			for (size_t i = 0; i < index; i++)
				record = record->next;
			return *(record->value);
		};

		List<type> & foreach(std::function<void (type *)> lambda) {
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) lambda(i->value);
			return *this;
		}
		List<type> & addCopy    (const type * obj) {
			length += 1;

			ListRec<type> * add_record = new ListRecCopy<type>;
			*(add_record->value) = *obj;

			ListRec<type> * local_prev = list_last->prev;
			ListDoubleLink(local_prev, add_record);
			ListDoubleLink(add_record, list_last);

			return *this;
		}
		List<type> & addCopy    (const type & obj) {
		    length += 1;

			ListRec<type> * add_record = new ListRecCopy<type>;
			*(add_record->value) = obj;

			ListRec<type> * local_prev = list_last->prev;
			ListDoubleLink(local_prev, add_record);
			ListDoubleLink(add_record, list_last);

			return *this;
        }
		List<type> & addRef     (const type * obj) {
			length += 1;

			ListRec<type> * add_record = new ListRec<type>;
			add_record->value = obj;

			ListRec<type> * local_prev = list_last->prev;
			ListDoubleLink(local_prev, add_record);
			ListDoubleLink(add_record, list_last);

			return *this;
		}
		//
		List<type> & addCopy    (size_t index, const type * obj) {
			length += 1;

			ListRec<type> * add_record = new ListRecCopy<type>;
			*(add_record->value) = *obj;

			ListRec<type> * local_next = list_head->next;
			for (size_t i = 0; i < index; i++) local_next = local_next->next;

			ListDoubleLink(list_head, add_record);
			ListDoubleLink(add_record, local_next);

			return *this;
		}
		List<type> & addCopy    (size_t index, const type & obj) {
			return addCopy(index, &obj);
		}
		List<type> & addRef     (size_t index, const type * obj) {
			length += 1;

			ListRec<type> * add_record = new ListRec<type>;
			add_record->value = obj;

			ListRec<type> * local_next = list_head->next;
			for (size_t i = 0; i < index; i++) local_next = local_next->next;

			ListDoubleLink(list_head, add_record);
			ListDoubleLink(add_record, local_next);

			return *this;
		}
		//CHECKED
		/*
		List<type> & addAllCopy (ICollection<type> & col) {
			col.foreach([this](type * obj){this->addCopy(obj);});
			return *this;
		}
		List<type> & addAllRef  (ICollection<type> & col) {
			col.foreach([this](type * obj){this->addRef(obj);});
			return *this;
		};
		*/
		//
		//TODO: REALIZE
		/*
		List<type> & addAllCopy (size_t index, ICollection<type> & col) {
			return *this;
		}
		List<type> & addAllRef  (size_t index, ICollection<type> & col) {
			return *this;
		}
		*/
		//
		//CHECKED
		void removeListRecord(ListRec<type> * remove_record) {
			if (remove_record) {
				length -= 1;
				ListRec<type> * local_prev = remove_record->prev;
				ListRec<type> * local_next = remove_record->next;
				ListDoubleLink(local_prev, local_next);
				delete remove_record;
			}
		}

		void removeListRecordSave(ListRec<type> * & remove_record) {
			if (remove_record) {
				length -= 1;
				ListRec<type> * local_prev = remove_record->prev;
				ListRec<type> * local_next = remove_record->next;
				ListDoubleLink(local_prev, local_next);
				remove_record = local_prev;
			}
		}

		List<type> & remove     (size_t index) {
			ListRec<type> * remove_record = list_head->next;
			for (size_t i = 0; i < index; i++) remove_record = remove_record->next;
			removeListRecord(remove_record);
			return *this;
		};

		List<type> & removeFirst() {return remove(0);}
		List<type> & removeLast () {return remove(length - 1);}
		//
		List<type> & removeFirst(const type * obj) {
			removeListRecord(recOf(obj));
			return *this;
		}
		List<type> & removeLast (const type * obj) {
			removeListRecord(recOfLast(obj));
			return *this;
		}

		List<type> & removeAll  (const type * obj) {
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) {
				if ( *(i->value) == * (type *)obj) {
					//TODO: make removeListRecordSave
					//void removeListRecordSave(ListRec<type> * & rec)
					//if remove, rec = rec->next
					i = i->prev;
					removeListRecord(i->next);
					//
				}
			}
			return *this;
		}

		List<type> & removeAll  () {
			ListRec<type> * buf = list_head->next;
			while (buf->next != nullptr) {
				buf = buf->next;
				delete buf->prev;
			}
			ListDoubleLink(list_head, list_last);
			return *this;
		}
		Ok<size_t>      indexOf     (const type * obj) const {
			Ok<size_t> ans;
			size_t length = 0;
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) {
				if ( *(i->value) == *obj) return ans = length;
				length++;
			}
		}
		Ok<size_t>      indexOfLast (const type * obj) const {
			Ok<size_t> ans;
			size_t length = 0;
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) {
				if ( *(i->value) == *obj) return length;
				length++;
			}
			return ans;
		}
		ListRec<type> * recOf       (const type * obj) const {
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) {
				if ( *(i->value) == *obj) return i;
			}
			return nullptr;
		}
		ListRec<type> * recOfRef            (const type * obj) const {
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) {
				if ( i->value == obj) return i;
			}
			return nullptr;
		}
		ListRec<type> * recOfLast   (const type * obj) const {
			ListRec<type> * ans = nullptr;
			for (ListRec<type> * i = list_head->next; i != list_last; i = i->next) {
				if ( *(i->value) == *obj) ans = i;
			}
			return ans;
		}
	};
}

#endif
