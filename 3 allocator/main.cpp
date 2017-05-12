#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif

struct Superblock;

struct Block {
	Superblock *superblock;
	Block *nextBlockSameSize;
	int threadIndex;
	int blockType;

	char *getBlockData();

	Block *getBlockAfterThis(int blockType);

	static Block *getBlockFromData(char *pointer);

	int getDataSize();
};

struct Superblock {
	int dataSize;

	char *getSuperblockData();

	Block *getLargestBlock();

	Block *getKthMinblock(int k);

	bool hasBlock(Block *pBlock);
};

int log2i(size_t size) {
	int log = 0;
	while (size > 1) {
		size /= 2;
		++log;
	}
	return log;
}

const int NUMBER_MINBLOCKS_IN_SUPERBLOCK = 1024;
const int MINBLOCK_SIZE_DATA = 64;
const int MINBLOCK_SIZE_ALL = MINBLOCK_SIZE_DATA + sizeof(Block);
const int SUPERBLOCK_SIZE_DATA = NUMBER_MINBLOCKS_IN_SUPERBLOCK * MINBLOCK_SIZE_DATA;
const int SUPERBLOCK_SIZE_ALL = NUMBER_MINBLOCKS_IN_SUPERBLOCK * MINBLOCK_SIZE_ALL;
const int NUMBER_BLOCK_TYPES = log2i(MINBLOCK_SIZE_DATA * NUMBER_MINBLOCKS_IN_SUPERBLOCK) + 1;
const int BLOCK_TYPE_MIN = log2i(MINBLOCK_SIZE_DATA);

Block *Block::getBlockAfterThis(int blockType) {
	return (Block *) ((char *) this + (1 << blockType) + sizeof(Block) * (1 << (blockType - BLOCK_TYPE_MIN)));
}

char *Block::getBlockData() {
	return (char *) this + sizeof(Block);
}

Block *Block::getBlockFromData(char *data) {
	return (Block *) (data - sizeof(Block));
}

int Block::getDataSize() {
	return 1 << blockType;
}

char *Superblock::getSuperblockData() {
	return (char *) this + sizeof(Superblock);
}

Block *Superblock::getKthMinblock(int k) {
	return (Block *) (getSuperblockData() + k * MINBLOCK_SIZE_ALL);
}

Block *Superblock::getLargestBlock() {
	return (Block *) getSuperblockData();
}

bool Superblock::hasBlock(Block *block) {
	char *data = getSuperblockData();
	return data <= (char *) block && (char *) block < data + SUPERBLOCK_SIZE_ALL;
}

struct GlobalAllocator {
//    суперблок --- несколько последовательных минблоков
//    минблок --- <Block> и <MINBLOCK_SIZE_DATA байт>
	vector<Superblock *> superblocksAll;
	vector<Superblock *> superblocksEmpty;

	Superblock *getSuperBlock() {
		assert(sizeof(char) == 1);

		if (superblocksEmpty.empty()) {
			Superblock *superblock = (Superblock *) malloc(sizeof(Superblock) + SUPERBLOCK_SIZE_ALL);
			if (superblock == nullptr) {
				throw new runtime_error("malloc returns nullptr");
			}
			superblock->dataSize = 0;
			superblocksAll.push_back(superblock);
			return superblock;
		} else {
			Superblock *superblock = superblocksEmpty.back();
			superblocksEmpty.pop_back();
			assert(superblock->dataSize == 0);
			return superblock;
		}
	}

	void returnSuperBlock(Superblock *superblock) {
		assert(superblock->dataSize == 0);
		superblocksEmpty.push_back(superblock);
	}

	~GlobalAllocator() {
		for (Superblock *superblock : superblocksAll) {
			assert(superblock->dataSize == 0);
			free(superblock);
		}
	}
};

GlobalAllocator globalAllocator;
mutex globalAllocatorMutex;

struct ListOfBlocks {
	Block *head = nullptr;

	void push_back(Block *block) {
		block->nextBlockSameSize = head;
		head = block;
	}

	Block *back() {
		return head;
	}

	void pop_back() {
		assert(head != nullptr);
		head = head->nextBlockSameSize;
	}

	bool empty() {
		return head == nullptr;
	}

	template<typename Lambda>
	void remove_if(Lambda lambda) {
		while (head != nullptr && lambda(head)) {
			head = head->nextBlockSameSize;
		}

		for (Block *block = head; block != nullptr; block = block->nextBlockSameSize) {
			while (block->nextBlockSameSize != nullptr && lambda(block->nextBlockSameSize)) {
				block->nextBlockSameSize = block->nextBlockSameSize->nextBlockSameSize;
			}
		}
	}
};

struct LocalAllocator {
	int threadIndex;
	vector<Superblock *> superblocks;
//	vector<std::list<Block *>> blocksBySize;
	vector<ListOfBlocks> blocksBySize;

	LocalAllocator() : blocksBySize(NUMBER_BLOCK_TYPES) {
		static atomic_int numberLocalAllocators;
		threadIndex = numberLocalAllocators++;
	}

	Superblock *getSuperBlock() {
		Superblock *superblock = globalAllocator.getSuperBlock();
		for (int i = 0; i < NUMBER_MINBLOCKS_IN_SUPERBLOCK; ++i) {
			Block *block = superblock->getKthMinblock(i);
			block->threadIndex = threadIndex;
			block->superblock = superblock;
			block->blockType = -1;
			block->nextBlockSameSize = nullptr;
		}
		superblocks.push_back(superblock);
		return superblock;
	}

	void returnSuperBlock(Superblock *superblock) {
		auto it = find(superblocks.begin(), superblocks.end(), superblock);
		assert(it != superblocks.end());
		superblocks.erase(it);
		for (ListOfBlocks &listOfBlocks : blocksBySize) {
			listOfBlocks.remove_if([superblock](Block *block) { return superblock->hasBlock(block); });
		}

		lock_guard<mutex> lock(globalAllocatorMutex);
		globalAllocator.returnSuperBlock(superblock);
	}

	char *mtalloc(size_t size) {
		assert(size <= SUPERBLOCK_SIZE_DATA);
		Block *block = getBlock(size);
		if (block == nullptr) {
			// блок равного или большего размера не нашёлся
			lock_guard<mutex> lock(globalAllocatorMutex);
			Superblock *superblock = getSuperBlock();
			blocksBySize.back().push_back(superblock->getLargestBlock());
			block = getBlock(size);
			assert(block != nullptr);
		}

		Superblock *superblock = block->superblock;
		superblock->dataSize += block->getDataSize();
		return block->getBlockData();
	}

	int divideBlockToFitSize(Block *block, int blockType, size_t size) {
//        пока блое не является минблоком и размер данных укладывается в вдвое меньший блок
		while (blockType > BLOCK_TYPE_MIN && size <= (1 << blockType) / 2) {
//            делим блок на две части
			--blockType;
			blocksBySize[blockType].push_back(block->getBlockAfterThis(blockType));
		}
		return blockType;
	}

	Block *getBlock(size_t size) {
		for (int i = BLOCK_TYPE_MIN; i < NUMBER_BLOCK_TYPES; ++i) {
			int blockSize = 1 << i;
			if ((int) size <= blockSize && !blocksBySize[i].empty()) {
				Block *block = blocksBySize[i].back();
				blocksBySize[i].pop_back();
				block->blockType = divideBlockToFitSize(block, i, size);
				block->threadIndex = threadIndex;
				return block;
			}
		}
		return nullptr;
	}

	void mtfree(char *data) {
		if (data == nullptr) {
			return;
		}
		Block *block = Block::getBlockFromData(data);
		if (block->threadIndex == threadIndex) {
			blocksBySize[block->blockType].push_back(block);
			Superblock *superblock = block->superblock;
			superblock->dataSize -= block->getDataSize();
			if (superblock->dataSize == 0) {
				returnSuperBlock(superblock);
			}
		} else {
			assert(false);
		}
	}
};

LocalAllocator &getLocalAllocator() {
	static thread_local LocalAllocator allocator;
	return allocator;
}

extern void *mtalloc(size_t size) {
	try {
		return getLocalAllocator().mtalloc(size);
	} catch (runtime_error) {
		return nullptr;
	}
}

extern void mtfree(void *data) {
	return getLocalAllocator().mtfree((char *) data);
}

#ifdef LOCAL

void thread_function(int numberOperations, bool useCustomMalloc) {
	void *(*mallocUsed)(size_t);
	void (*freeUsed)(void *);
	if (useCustomMalloc) {
		mallocUsed = mtalloc;
		freeUsed = mtfree;
	} else {
		mallocUsed = malloc;
		freeUsed = free;
	}

	const int maxBlockSize = 1024 * 16;
	vector<void *> datas;
	for (int iOperation = 0; iOperation < numberOperations; ++iOperation) {
		if (datas.empty() || rand() % 2 == 0) {
			datas.push_back(mallocUsed(rand() % maxBlockSize + 1));
		} else {
			int i = rand() % datas.size();
			freeUsed(datas[i]);
			datas[i] = datas.back();
			datas.pop_back();
		}
	}

	for (void *&data : datas) {
		freeUsed(data);
	}
}

void test(int numberThreads, int numberOperations, bool useCustomMalloc) {
	vector<thread> threads;
	for (int i = 0; i < numberThreads; ++i) {
		threads.emplace_back(thread_function, numberOperations, useCustomMalloc);
	}

	for (thread &t : threads) {
		t.join();
	}
}

int main() {
	int num_iterations = 10;
	int num_items = 100000;
	for (int i = 0; i < num_iterations; ++i) {
		vector<void *> pointers(num_items);
		for (void *&pointer : pointers) {
			pointer = mtalloc(rand() % 100);
		}

		for (void *pointer : pointers) {
			mtfree(pointer);
		}

		for (ListOfBlocks list  : getLocalAllocator().blocksBySize) {
			assert(list.empty());
		}
	}
	return 0;
}

#endif