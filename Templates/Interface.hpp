#ifndef UF_T_Interface_H
#define UF_T_Interface_H

#include <functional>
#include "../Utils/Ok.hpp"
#include "../Utils/stalloc.hpp"
#include "../Utils/Concepts.hpp"

//UseFull Template Interface module
//Version 1.1 release
//Make by Graph Don'te-Crypto

namespace uft {
	using namespace utils;
	
	struct CollectionStructV2 {
		template <typename Coll>
			requires CoSerializible<Coll> && CoAllocatable<Coll> && CoSizeable<Coll>
		void checkSizeWithKeepValues(Coll & coll, size_t length) {
			if (length >= coll.size()) {
				size_t max_size = coll.size();
				do {
					max_size *= 2;
				} while (length > max_size);
				size_t length = coll.getDataSize();
				void * buf = stalloc(length, char);
				coll.packData(buf);
				coll.freeMemory();
				coll.allocMemory(max_size);
				coll.unpackData(buf, length);
			}
		}
		template <typename Coll>
			requires CoAllocatable<Coll> && CoSizeable<Coll>
		void checkSizeWithoutKeepValues(Coll & coll, size_t length) {
			if (length >= coll.size()) {
				size_t max_size = coll.size();
				do {
					max_size *= 2;
				} while (length > max_size);
				coll.freeMemory();
				coll.allocMemory(max_size);
			}
		}
	} CollectionV2;
	
	template <typename Type, typename Coll>
		requires CoCollection<Coll, Type>
	struct CollectionFunctions {
		static void addCopy(CoCollection<Type> auto & coll, size_t index, const Type & obj) {
			coll.addCopy(index, &obj);
		}
		static void addCopy(CoCollection<Type> auto & coll, const Type & obj) {
			coll.addCopy(&obj);
		}
		static void foreach(CoCollection<Type> auto & coll, CoLambda<void, Type *> auto lambda) {
			for (size_t i = 0; i < coll.length(); i++) lambda(&(coll[i]));
		}
		static void addAllCopy(CoCollection<Type> auto & coll, CoCollection<Type> auto & inputColl) {
			foreach(inputColl, [&coll] (Type * obj) {coll.addCopy(obj);});
		}
		static void addAllCopy (CoCollection<Type> auto & coll, size_t index, CoCollection<Type> auto & inputColl) {
			foreach(inputColl, [&coll, &index] (Type * obj) {coll.addCopy(index++, obj);});
		}
		static void removeFirst(CoCollection<Type> auto & coll) {return coll.remove(0);}
		static void removeLast (CoCollection<Type> auto & coll) {return coll.remove(coll.length() - 1);}
		
		static Ok<size_t> indexByCondition(CoCollection<Type> auto & coll, CoLambda<bool, Type *> auto lambda) {
			for (size_t i = 0; i < coll.length(); i++) 
				if (lambda(&(coll[i])) == true) return i;
			return {};
		}
	};
		
	template<typename Type>
	struct IBaseCollection {
		virtual Type & operator[] (size_t index) const = 0;
		virtual size_t size() const = 0;
		virtual size_t length() const = 0;
		
		IBaseCollection & foreach(CoLambda<void, Type *> auto lambda) {
			for (size_t i = 0; i < length(); i++) lambda(&((*this)[i]));
			return *this;
		}
	};
	
	struct CollectionStruct {
		template <typename Type, typename Coll>
			requires CoSerializible<Coll> && CoAllocatable<Coll> && std::derived_from<Coll, IBaseCollection<Type>>
		void checkSizeWithKeepValues(Coll & coll, size_t length) {
			if (length >= coll.size()) {
				size_t max_size = coll.size();
				do {
					max_size *= 2;
				} while (length > max_size);
				size_t length = coll.getDataSize();
				void * buf = stalloc(length, char);
				coll.packData(buf);
				coll.freeMemory();
				coll.allocMemory(max_size);
				coll.unpackData(buf, length);
			}
		}
		template <typename Type, typename Coll>
			requires CoAllocatable<Coll> && std::derived_from<Coll, IBaseCollection<Type>>
		void checkSizeWithoutKeepValues(Coll & coll, size_t length) {
			if (length >= coll.size()) {
				size_t max_size = coll.size();
				do {
					max_size *= 2;
				} while (length > max_size);
				coll.freeMemory();
				coll.allocMemory(max_size);
			}
		}
	} Collection;
	
	template<typename Type>
	struct IFuncCollection : public virtual IBaseCollection<Type> {
		
		virtual size_t length() const = 0;
		
		virtual IFuncCollection & addCopy (size_t index, const Type * obj) = 0;
		IFuncCollection & addCopy         (size_t index, const Type & obj) {return addCopy(index, &obj);}
		
		virtual IFuncCollection & addCopy (              const Type * obj) = 0;
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
		IFuncCollection & removeLast () {return remove(IBaseCollection<Type>::length() - 1);}
		
		Ok<size_t> indexByCondition(CoLambda<bool, Type *> auto lambda) {
			for (size_t i = 0; i < length(); i++) 
			//for (size_t i = 0; i < 3; i++) 
				if (lambda(&((*this)[i])) == true) return i;
			return {};
		}
	};
	
	template<typename Type>
	struct ICollectionEvent : public IFuncCollection<Type> {
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