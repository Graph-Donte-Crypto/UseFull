//prototype

template <typename Type, typename UnsignedX = size_t>
struct ArrayList : public ICollection {
	
	Type * data;
	UnsignedX * next;
	UnsignedX * prev;
	
	UnsignedX size;
	UnsignedX length;
	UnsignedX index_start;
	UnsignedX index_end;
	
	struct IndexerSupport;
	
	struct IndexerSupportStruct {
		UnsignedX current;
		Type get(UnsignedX i) {
			
		}
	} IndexerSupport;
	
	
	
	
	void allocMemoru(UnsignedX size) {
		data = new Type[size];
		next = new UnsignedX[size];
		prev = new UnsignedX[size];
	}
	
	void * packData(void * output) {
		output = utils::toBytes(output, data, length * sizeof(Type));
		output = utils::toBytes(output, next, length * sizeof(UnsignedX));
		output = utils::toBytes(output, prev, length * sizeof(UnsignedX));
		return output;
	}
	
	void * unpackData(void * input) {
		input = utils::fromBytes(output, data, length * sizeof(Type));
		input = utils::fromBytes(output, next, length * sizeof(UnsignedX));
		input = utils::fromBytes(output, prev, length * sizeof(UnsignedX));
		return input;
	} 
	
	void freeMemory() {
		delete[] data;
		delete[] next;
		delete[] prev;
	}
	
	void upSizeAndKeepValues(size_t new_size) {
		
	}
	
	
	ArrayList() {
		
	}
	
	//IBaseCollection
	//NEED TEST
	//NEED IndexerSupport
	virtual type & operator[] (UnsignedX index) const {
		UnsignedX cur_index = index_start;
		for (UnsignedX i = 0; i < index; i++) {
			cur_index = next[cur_index];
		}
		return data[cur_index];
	};
	//
	virtual UnsignedX size() const {return size;}
	virtual UnsignedX length() const {return size;}
	virtual IBaseCollection & foreach(std::function<void (type *)> lambda) {
		UnsignedX size = size();
		for (UnsignedX i = 0; i < size; i++) lambda((*this)[i]);
		return *this;
	}
	
	//ICollection
	
};