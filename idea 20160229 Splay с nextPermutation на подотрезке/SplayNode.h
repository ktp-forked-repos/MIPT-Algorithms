//
// Created by dima on 29.02.16.
//

#ifndef IDEA_20160229_SPLAY_SPLAYNODE_H
#define IDEA_20160229_SPLAY_SPLAYNODE_H

#include "base.h"

class SplayNode {
public:
	SplayNode *left;
	SplayNode *right;
	SplayNode *parent;
	size_t size;
	int value;
	ll sum;
	ll toPush;
	size_t sizeSuffixDec;
	size_t sizePrefixInc;
	int leftmostValue;
	int rightmostValue;
	bool reverse;
	bool needPush;

	SplayNode(int value);

	bool isLeft();

	void setRight(SplayNode *child);

	void set(SplayNode *child, bool isLeft);

	void setValue(int value);

	void add(int value);

	void setReverse();

	void push();

	void update();

	~SplayNode();
};

#endif //IDEA_20160229_SPLAY_SPLAYNODE_H
