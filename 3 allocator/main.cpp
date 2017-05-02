#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif

struct BlockInfo {
    void *superblock;
    int threadIndex;
    int blockType;
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
const int MINBLOCK_SIZE_ALL = MINBLOCK_SIZE_DATA + sizeof(BlockInfo);
const int SUPERBLOCK_SIZE_DATA = NUMBER_MINBLOCKS_IN_SUPERBLOCK * MINBLOCK_SIZE_DATA;
const int SUPERBLOCK_SIZE_ALL = NUMBER_MINBLOCKS_IN_SUPERBLOCK * MINBLOCK_SIZE_ALL;
const int NUMBER_BLOCK_TYPES = log2i(MINBLOCK_SIZE_DATA * NUMBER_MINBLOCKS_IN_SUPERBLOCK) + 1;
const int BLOCK_TYPE_MIN = log2i(MINBLOCK_SIZE_DATA);

struct GlobalAllocator {
//    суперблок --- несколько последовательных минблоков
//    минблок --- <BlockInfo> и <MINBLOCK_SIZE_DATA байт>
    vector<void *> superblocks;

    void *getSuperBlock() {
        void *superblock = malloc(SUPERBLOCK_SIZE_ALL);
        superblocks.push_back(superblock);
        return superblock;
    }
};

GlobalAllocator globalAllocator;
mutex globalAllocatorMutex;

struct LocalAllocator {
    int threadIndex;
    vector<void *> superblocks;
    vector<std::list<void *>> blocksBySize;

    LocalAllocator() : blocksBySize(NUMBER_BLOCK_TYPES) {
        static atomic_int numberLocalAllocators;
        threadIndex = numberLocalAllocators++;
    }

    void *getSuperBlock() {
        dbgl("getSuperBlock", threadIndex);
        void *superblock = globalAllocator.getSuperBlock();
        for (int i = 0; i < NUMBER_MINBLOCKS_IN_SUPERBLOCK; ++i) {
            void *block = superblock + i * MINBLOCK_SIZE_ALL;
            BlockInfo &blockInfo = *((BlockInfo *) block);
            blockInfo.threadIndex = threadIndex;
            blockInfo.superblock = block;
        }
        return superblock;
    }

    void *mtalloc(size_t size) {
        assert(size <= MINBLOCK_SIZE_DATA * NUMBER_MINBLOCKS_IN_SUPERBLOCK);
        void *block = getBlock(size);
        if (block == nullptr) {
            // блок равного или большего размера не нашёлся
            lock_guard<mutex> lock(globalAllocatorMutex);
            void *superBlock = getSuperBlock();
            blocksBySize.back().push_back(superBlock);
            block = getBlock(size);
        }

        dbgl("+", threadIndex, size, block);
        BlockInfo &blockInfo = *((BlockInfo *) block);
        assert(blockInfo.threadIndex == threadIndex);
        return block + sizeof(BlockInfo);
    }

    int divideBlockToFitSize(void *block, int blockType, size_t size) {
//        пока блое не является минблоком и размер данных укладывается в вдвое меньший блок
        while (blockType > BLOCK_TYPE_MIN && size <= (1 << blockType) / 2) {
//            делим блок на две части
            --blockType;
            blocksBySize[blockType].push_back(block + (1 << blockType) + sizeof(BlockInfo) * (1 << (blockType - BLOCK_TYPE_MIN)));
        }
        return blockType;
    }

    void *getBlock(size_t size) {
        for (int i = BLOCK_TYPE_MIN; i < NUMBER_BLOCK_TYPES; ++i) {
            int blockSize = 1 << i;
            if (size <= blockSize && !blocksBySize[i].empty()) {
                void *block = blocksBySize[i].back();
                blocksBySize[i].pop_back();
                BlockInfo &blockInfo = *((BlockInfo *) block);
                blockInfo.blockType = divideBlockToFitSize(block, i, size);
                return block;
            }
        }
        return nullptr;
    }

    void mtfree(void *pointer) {
        dbgl("-", threadIndex, pointer);
        void *block = pointer - sizeof(BlockInfo);
        BlockInfo &blockInfo = *((BlockInfo *) block);
        if (blockInfo.threadIndex == threadIndex) {
            blocksBySize[blockInfo.blockType].push_back(block);
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

extern void mtfree(void *pointer) {
    return getLocalAllocator().mtfree(pointer);
}

#ifdef LOCAL

int number_threads = 10;
int number_operations = 100;

void thread_function() {
    vector<void *> datas;
    for (int iOperation = 0; iOperation < number_operations; ++iOperation) {
        if (datas.empty() || rand() % 2 == 0) {
            datas.push_back(mtalloc(rand() % SUPERBLOCK_SIZE_DATA + 1));
        } else {
            int i = rand() % datas.size();
            mtfree(datas[i]);
            datas[i] = datas.back();
            datas.pop_back();
        }
    }

    for (void *&data : datas) {
        mtfree(data);
    }
}

void test() {
    vector<thread> threads;
    for (int i = 0; i < number_threads; ++i) {
        threads.emplace_back(thread_function);
    }

    for (thread &t : threads) {
        t.join();
    }
}

int main() {
    test();
    return 0;
}

#endif