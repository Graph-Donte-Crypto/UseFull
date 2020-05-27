#ifndef UF_T_Interface_H
#define UF_T_Interface_H

#include <functional>
#include "../Utils/Ok.h"

using namespace utils;

namespace uft {
	
	struct ISerializible {
		virtual size_t getDataSize() = 0;
		virtual void * packData(void * output) = 0;
		virtual void * unpackData(void * input) = 0;
	};
	struct IAllocatable {
		virtual void allocMemory(size_t size) = 0;
		virtual void freeMemory() = 0;
	};
		
	template<typename Type>
	struct IBaseCollection {
		virtual Type & operator[] (size_t index) const = 0;
		virtual size_t size() const = 0;
		virtual size_t length() const = 0;
		virtual IBaseCollection & foreach(std::function<void (Type *)> lambda) = 0;
	};
	
	struct <typename Type>
	struct IAllocatableCollection : 
		public virtual IBaseCollection, 
		public virtual IAllocatable, 
		public virtual ISerializible 
	{	
 		void checkSizeWithKeepValues(size_t length) {
			if (length >= size()) {
				size_t max_size = size();
				do {
					max_size *= 2;
				} while (length > max_size);
				void * buf = stalloc(getDataSize(), char);
				packData(buf);
				freeMemory();
				allocMemory(max_size);
				unpackData(buf);
			}
		}
		void checkSizeWithoutKeepValues(size_t length) {
			if (length >= size()) {
				size_t max_size = size();
				do {
					max_size *= 2;
				} while (length > max_size);
				freeMemory();
				allocMemory(max_size);
			}
		}
	};
	
	template<typename Type>
	struct IFuncCollection : public virtual IBaseCollection {
		virtual IFuncCollection & addCopy (size_t index,       Type * obj) = 0;
		IFuncCollection & addCopy         (size_t index, const Type & obj) {return addCopy(index, &obj);}
		
		virtual IFuncCollection & addCopy (                    Type * obj) = 0;
		IFuncCollection & addCopy         (              const Type & obj) {return addCopy(&obj);}
		
		
		IFuncCollection & addAllCopy (IBaseCollection<Type> & col) {
			col.foreach([this] (Type * obj) {addCopy(obj);});
			return *this;
		}
		IFuncCollection & addAllCopy (size_t index, IBaseCollection<Type> & col) {
			col.foreach([this, &index] (Type * obj) {addCopy(index++, obj);});
			return *this;
		}
		virtual IFuncCollection & remove     (size_t index) = 0;
		virtual IFuncCollection & removeAll  () = 0;
		
		IFuncCollection & removeFirst() {return remove(0);}
		IFuncCollection & removeLast () {return remove(length() - 1);}
		
		virtual Ok<size_t> indexByCondition(std::function<bool (Type *)> lambda) {
			for (size_t i = 0; i < length(); i++) 
				if (lambda( &( (*this)[i]) ) == true) return i;
			return {};
		}
	};
	
	template<typename Type>
	struct ICollectionEvent public ICollection {
		virtual void onRemove(Type * obj) = 0;
		virtual void onAdd(Type * obj) = 0;
	};

	template<typename Type>
	struct IComparable {
	public:		
		virtual bool equals(Type object) = 0;
		virtual int compare(Type object) = 0;
	};
	
}

#endif