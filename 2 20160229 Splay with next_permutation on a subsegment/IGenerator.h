#ifndef IDEA_20160229_SPLAY_IGENERATOR_H
#define IDEA_20160229_SPLAY_IGENERATOR_H

class IGenerator {
public:
	virtual void createRandomLR(size_t &l, size_t &r) = 0;

	virtual size_t getRandomI(bool isIndexInsert = false) = 0;

	virtual int getRandomValue() = 0;

	virtual ~IGenerator() {
	}
};

#endif //IDEA_20160229_SPLAY_IGENERATOR_H
