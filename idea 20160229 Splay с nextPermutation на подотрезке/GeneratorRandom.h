//
// Created by dima on 29.02.16.
//

#ifndef IDEA_20160229_SPLAY_GENERATORRANDOM_H
#define IDEA_20160229_SPLAY_GENERATORRANDOM_H

#include "AbstractGenerator.h"
#include "GeneratorOperations.h"

#include <algorithm>

template<size_t pInsert, size_t pGetSum, size_t pSet, size_t pNextPermutation, size_t pAdd, size_t firstInsert = 1>
class GeneratorRandom : public AbstractGenerator {
public:
	GeneratorRandom(size_t n = 10, int rangeMin = 0, int rangeMax = 10)
			: AbstractGenerator(n, rangeMin, rangeMax) {
	}

	virtual std::vector<AbstractOperation *> generate() {
		currentSize = 0;
		result.clear();

		for (size_t i = 0; i < firstInsert; ++i)
			add(new OperationInsert(this));

		std::vector<size_t> probs = {pInsert, pGetSum, pSet, pNextPermutation, pAdd};
		size_t sum = std::accumulate(probs.begin(), probs.end(), 0u);

		bool debug = 0;
		for (size_t i = firstInsert; i < n; ++i) {
			size_t current = rand() % sum;
			OperationType type;
			for (size_t iOperation = 0; iOperation < probs.size(); ++iOperation)
				if (current < probs[iOperation]) {
					type = static_cast<OperationType>(iOperation);
					break;
				}
				else
					current -= probs[iOperation];
			//OperationType type = static_cast<OperationType>(rand() % 5);
			switch (type) {
				case INSERT:
					if (debug) printf("insert\n");
					add(new OperationInsert(this));
					break;
				case GET_SUM:
					if (debug) printf("sum\n");
					add(new OperationGetSum(this));
					break;
				case SET:
					if (debug) printf("set\n");
					add(new OperationSet(this));
					break;
				case NEXT_PERMUTATION:
					if (debug) printf("np\n");
					add(new OperationNextPermutation(this));
					break;
				case ADD:
					if (debug) printf("add\n");
					add(new OperationAdd(this));
					break;
				default:
					assert(0);
			}
		}
		return result;
	}
};

#endif //IDEA_20160229_SPLAY_GENERATORRANDOM_H
