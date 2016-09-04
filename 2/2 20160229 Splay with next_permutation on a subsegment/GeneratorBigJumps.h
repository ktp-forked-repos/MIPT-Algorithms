#ifndef IDEA_20160229_SPLAY_GENERATORBIGJUMPS_H
#define IDEA_20160229_SPLAY_GENERATORBIGJUMPS_H

class GeneratorBigJumps : public AbstractGenerator {
public:
	GeneratorBigJumps(size_t n = 10, int rangeMin = -10, int rangeMax = 10)
			: AbstractGenerator(n, rangeMin, rangeMax) {
	}

	virtual std::vector<AbstractOperation *> generate() {
		currentSize = 0;
		result.clear();

		while (result.size() < n) {
			size_t numberInsert = rand() % 20000;
			for (size_t i = 0; i < numberInsert; ++i)
				add(new OperationInsert(this));
			addRandomOperations(1000);
		}
		return result;
	}

	void addRandomOperations(size_t number) {
		for (size_t i = 0; i < number; ++i) {
			OperationType type = static_cast<OperationType>(rand() % OPERATION_TYPE_NUMBER);
			switch (type) {
				case INSERT:
					add(new OperationInsert(this));
					break;
				case GET_SUM:
					add(new OperationGetSum(this));
					break;
				case SET:
					add(new OperationSet(this));
					break;
				case NEXT_PERMUTATION:
					add(new OperationNextPermutation(this));
					break;
				case ADD:
					add(new OperationAdd(this));
					break;
			}
		}
	}
};

#endif //IDEA_20160229_SPLAY_GENERATORBIGJUMPS_H
