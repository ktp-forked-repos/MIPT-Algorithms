#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif

struct Superblock;

struct Block {
	Superblock *superblock;
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
};

int log2i(size_t size) {
	int log = 0;
	while (size > 1) {
		size /= 2;
		++log;
	}
	return log;
}

const int NUMBER_MINBLOCKS_IN_SUPERBLOCK = 8 * 1024;
const int MINBLOCK_SIZE_DATA = 128;
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

struct GlobalAllocator {
//    суперблок --- несколько последовательных минблоков
//    минблок --- <Block> и <MINBLOCK_SIZE_DATA байт>
	vector<Superblock *> superblocks;

	Superblock *getSuperBlock() {
		assert(sizeof(char) == 1);
		Superblock *superblock = (Superblock *) malloc(sizeof(Superblock) + SUPERBLOCK_SIZE_ALL);
		superblock->dataSize = 0;
		superblocks.push_back(superblock);
		return superblock;
	}

	~GlobalAllocator() {
		for (Superblock *superblock : superblocks) {
			assert(superblock->dataSize == 0);
			free(superblock);
		}
	}
};

GlobalAllocator globalAllocator;
mutex globalAllocatorMutex;

struct LocalAllocator {
	int threadIndex;
	vector<Superblock *> superblocks;
	vector<std::list<Block *>> blocksBySize;

	LocalAllocator() : blocksBySize(NUMBER_BLOCK_TYPES) {
		static atomic_int numberLocalAllocators;
		threadIndex = numberLocalAllocators++;
	}

	Superblock *getSuperBlock() {
		dbgl("getSuperBlock", threadIndex);
		Superblock *superblock = globalAllocator.getSuperBlock();
		for (int i = 0; i < NUMBER_MINBLOCKS_IN_SUPERBLOCK; ++i) {
			Block *block = superblock->getKthMinblock(i);
			block->threadIndex = threadIndex;
			block->superblock = superblock;
		}
		superblocks.push_back(superblock);
		return superblock;
	}

	char *mtalloc(size_t size) {
		assert(size <= MINBLOCK_SIZE_DATA * NUMBER_MINBLOCKS_IN_SUPERBLOCK);
		Block *block = getBlock(size);
		if (block == nullptr) {
			// блок равного или большего размера не нашёлся
			lock_guard<mutex> lock(globalAllocatorMutex);
			Superblock *superblock = getSuperBlock();
			blocksBySize.back().push_back(superblock->getLargestBlock());
			block = getBlock(size);
		}
		dbgl("+", threadIndex, size, block);

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
			if (size <= blockSize && !blocksBySize[i].empty()) {
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
		dbgl("-", threadIndex, data);
		Block *block = Block::getBlockFromData(data);
		if (block->threadIndex == threadIndex) {
			blocksBySize[block->blockType].push_back(block);
			Superblock *superblock = block->superblock;
			superblock->dataSize -= block->getDataSize();
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
	return getLocalAllocator().mtalloc(size);
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
	test(10, 100, true);
//	test(10, 100, false);
	return 0;
}

#endif