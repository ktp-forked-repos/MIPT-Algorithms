//
// Created by dima on 29.02.16.
//

#ifndef IDEA_20160229_SPLAY_SPLAYOPETAIONS_H
#define IDEA_20160229_SPLAY_SPLAYOPETAIONS_H

#include <utility> // pair
#include "base.h"
#include "SplayNode.h"

ll getSum(SplayNode *node);

size_t getSize(SplayNode *node);

void update(SplayNode *node);

void push(SplayNode *node);

void deAttach(SplayNode *node);

void rotate(SplayNode *node);

void splay(SplayNode *node);

SplayNode *find(SplayNode *node, size_t i);

std::pair<SplayNode *, SplayNode *> split(SplayNode *node, size_t size);

SplayNode *findMax(SplayNode *node);

SplayNode *merge(SplayNode *left, SplayNode *right);

SplayNode *findFirstLarger(SplayNode *node, int value);

// для отладки
void checkParentsPushing(SplayNode *node);

SplayNode *nextPermutation(SplayNode *node);

#endif //IDEA_20160229_SPLAY_SPLAYOPETAIONS_H
