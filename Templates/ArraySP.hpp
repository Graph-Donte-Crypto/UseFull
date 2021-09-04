#ifndef UF_T_Array_SP_H
#define UF_T_Array_SP_H

#include "../Utils/StdDiagnosticIgnore.hpp"

#include <deque>
#include <vector>

#include "../Utils/StdDiagnosticIgnoreEnd.hpp"

//UseFull Template Array SP (same place) module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Utils/Bytes.hpp"
#include "../Utils/Concepts.hpp"
#include "../String/StringFormatBuilder.hpp"


namespace uft {
	using namespace utils;

	struct SPIndex {
		size_t real_block_index;
		size_t real_array_index;
		size_t fake_array_index;
		size_t fake_joint_index;
		size_t fake_alive_index;
	};

	template <typename Type, size_t increment_size>
	struct ArraySP {

		struct SPBlock {
			std::vector<Type> values;
			std::vector<bool> values_exists; 
			size_t free_spaces;
			size_t curr_free_space;

			SPBlock() 
				: values()
				, values_exists()
				, free_spaces(increment_size)
				, curr_free_space(0)
			{
				values.resize(increment_size);
				values_exists.reserve(increment_size);
				values_exists.assign(increment_size, false);
			}

			Type * tryAddNoCheck(const Type & object) {
				//write new object
				values_exists[curr_free_space] = true;
				values[curr_free_space] = object;
				Type * ptr = values.data() + curr_free_space;

				//find new free_space
				free_spaces--;
				if (free_spaces == 0)	//if no free spaces - set curr_free_space 
					curr_free_space = increment_size;
				else //else find curr_free_space
					for (; curr_free_space < increment_size; curr_free_space++)
						if (!values_exists[curr_free_space])
							break;

				return ptr;
			}

			Type * tryAdd(const Type & object) {
				if (free_spaces == 0) 
					return nullptr;
				else
					return tryAddNoCheck(object);
			}

			bool keepPointer(const Type * const object) {
				return (values.data() <= object) && (object < values.data() + increment_size);
			}

			size_t getPosition(const Type * const object) {
				return object - values.data();
			}

			bool tryRemove(const Type * const object) {
				if (!keepPointer(object) || !values_exists[getPosition(object)])
					return false;
				else {
					removeNoCheck(object);
					return true;
				}
			}

			bool tryRemoveNum(size_t num) {
				Type * ptr = values.data();
				return tryRemove(ptr + num);
			}

			void removeNoCheck(const Type * const object) {
				size_t position = getPosition(object);
				values_exists[position] = false;
				free_spaces++;

				if (position < curr_free_space) //set new curr_free_space if needed
					curr_free_space = position;
			}

			void foreach(CoLambda<void, Type &> auto lambda) {
				if (free_spaces == increment_size) return;
				else if (free_spaces == 0) {
					for (size_t i = 0; i < increment_size; i++)
						lambda(values[i]);
				}
				else {
					for (size_t i = 0; i < increment_size; i++)
						if (values_exists[i])
							lambda(values[i]);
				}
			}
		};

		std::vector<SPBlock> blocks;

		Type * tryAdd(const Type & object) {
			for (size_t i = 0; i < blocks.size(); i++)
				if (blocks[i].free_spaces > 0)
					return blocks[i].tryAddNoCheck(object);
			blocks.emplace_back();
			return blocks.back().tryAddNoCheck(object);
		}

		Type * tryAdd(const Type && object) {
			for (size_t i = 0; i < blocks.size(); i++)
				if (blocks[i].free_spaces > 0)
					return blocks[i].tryAddNoCheck(object);
			blocks.emplace_back();
			return blocks.back().tryAddNoCheck(object);
		}

		bool tryRemove(const Type * const object) {
			for (size_t i = 0; i < blocks.size(); i++)
				if (blocks[i].keepPointer(object)) {
					blocks[i].tryRemoveNoCheck(object);
					return true;
				}
			return false;
		}

		void foreach(CoLambda<void, Type &> auto lambda) {
			for (size_t i = 0; i < blocks.size(); i++)
				blocks[i].foreach(lambda);
		}
		void foreachFull(CoLambda<void, Type &, const SPIndex &> auto lambda) {
			SPIndex base;
			base.fake_joint_index = 0;
			base.fake_alive_index = 0;
			for (base.real_block_index = 0; base.real_block_index < blocks.size(); base.real_block_index++) {
				base.fake_array_index = 0;
				for (base.real_array_index = 0; base.real_array_index < increment_size; base.real_array_index++, base.fake_joint_index++) {
					lambda(blocks[base.real_block_index].values[base.real_array_index], base);
					if (blocks[base.real_block_index].values_exists[base.real_array_index]) {
						base.fake_alive_index++;
						base.fake_array_index++;
					}
				}
			}
		}

	protected:
		void defragment_blocks(CoLambda<void, Type *, Type *> auto lambda) {

			std::deque<SPBlock *> free_spaces;

			for (size_t i = 0; i < blocks.size(); i++)
				free_spaces.push_back(blocks.data() + i);

			sort(free_spaces.begin(), free_spaces.end(), [](SPBlock * a, SPBlock * b) {
				return a->free_spaces > b->free_spaces;
			});

			while (free_spaces.size() > 0) {

				SPBlock * temp = nullptr;

				SPBlock * max = nullptr;
				while (free_spaces.size() > 0) {
					temp = free_spaces.front();
					if (temp->free_spaces != increment_size) {
						max = temp;
						break;
					}
					free_spaces.pop_front();
				}
				if (max == nullptr)
					break;

				SPBlock * min = nullptr;
				while (free_spaces.size() > 0) {
					temp = free_spaces.back();
					if (temp->free_spaces != 0) {
						min = temp;
						break;
					}
					free_spaces.pop_back();
				}
				if (min == nullptr)
					break;

				if (min == max)
					break;

				size_t elements_count = increment_size - max->free_spaces;
				if (elements_count <= min->free_spaces) {
					for (size_t i = 0; i < increment_size; i++) {
						if (max->values_exists[i]) {
							max->values_exists[i] = false;
							Type * ptr = min->tryAddNoCheck(max->values[i]);
							lambda(max->values.data() + i, ptr);
						}
					}
					max->free_spaces = increment_size;
					max->curr_free_space = 0;
					free_spaces.pop_front();
				}
				else {
					max->free_spaces += min->free_spaces;
					for (size_t i = 0; (i < increment_size) && (min->free_spaces > 0); i++) {
						if (max->values_exists[i]) {
							max->values_exists[i] = false;
							Type * ptr = min->tryAddNoCheck(max->values[i]);
							lambda(max->values.data() + i, ptr);
						}
					}
					free_spaces.pop_back();
				}

			}

		}

		void defragment_reorder_one_block(CoLambda<void, Type *, Type *> auto lambda) {
			SPBlock * holder = nullptr;
			for (size_t i = 0; i < blocks.size(); i++) {
				if (blocks[i].free_spaces != 0 && blocks[i].free_spaces != increment_size) {
					holder = blocks.data() + i;
					break;
				}
			} 
			if (holder != nullptr) {

				size_t index_forward = 0;
				size_t index_backward = increment_size - 1;

				while (true) {
					while (holder->values_exists[index_forward])
						index_forward++;
					while (!holder->values_exists[index_backward])
						index_backward--;
					if (index_forward >= index_backward)
						break;
					std::swap(holder->values[index_forward], holder->values[index_backward]);
					std::swap(holder->values_exists[index_forward], holder->values_exists[index_backward]);

					lambda(holder->values.data() + index_backward, holder->values.data() + index_forward);
				}
			}
		}

		void defragment_reorder_all_blocks() {
			SPBlock * not_full_block = nullptr;
			size_t full_blocs = 0;

			for (size_t i = 0; i < blocks.size(); i++) {
				if (blocks[i].free_spaces == 0)
					full_blocs++;
				else if (blocks[i].free_spaces != increment_size)
					not_full_block = blocks.data() + i;
			}

			size_t index_forward = 0;
			size_t index_backward = blocks.size() - 1;

			if (not_full_block) {
				if (full_blocs == blocks.size() - 1) {
					std::swap(blocks.back(), *not_full_block);
					return;
				}
				else if (full_blocs == 0) {
					std::swap(*blocks.begin(),  *not_full_block);
					return;
				}
				else {
					std::swap(blocks.back(), *not_full_block);
					index_backward--;
				}
			}
			else {
				if (full_blocs == blocks.size())
					return;
			}

			while (true) {
				while (blocks[index_forward].free_spaces == 0) 
					index_forward++;
				while (blocks[index_backward].free_spaces == increment_size) 
					index_backward--;
				if (index_forward >= index_backward)
					break;
				std::swap(blocks[index_forward], blocks[index_backward]);
			}

			if (not_full_block != nullptr && blocks[index_forward].free_spaces == increment_size)
				std::swap(blocks[index_forward], blocks.back());
		}
	public:

		void defragment(CoLambda<void, Type *, Type *> auto lambda) {
			defragment_blocks(lambda);
			defragment_reorder_one_block(lambda);
			defragment_reorder_all_blocks();
		}

		void defragment() {
			defragment_blocks([](Type *, Type *){});
			defragment_reorder_one_block([](Type *, Type *){});
			defragment_reorder_all_blocks();
		}

	};
}

#endif
