#include "SplayNode.h"
#include "SplayOperations.h"

SplayNode::SplayNode(int value)
		: left(nullptr)
		  , right(nullptr)
		  , parent(nullptr)
		  , size(1)
		  , value(value)
		  , sum(value)
		  , toPush(0)
		  , reverse(false)
		  , leftmostValue(value)
		  , rightmostValue(value)
		  , sizeSuffixDec(1)
		  , sizePrefixInc(1) {
}

bool SplayNode::isLeft() {
	assert(parent != nullptr);
	assert(!parent->needPush);
	return this == parent->left;
}

void SplayNode::setRight(SplayNode *child) {
	set(child, false);
}

void SplayNode::set(SplayNode *child, bool isLeft) {
	SplayNode *&node = (isLeft ? left : right);
	assert(node == nullptr);
	assert(!needPush);
	assert(child == nullptr || !child->needPush);
	node = child;
	update();
	if (child != nullptr)
		child->parent = this;
}

void SplayNode::setValue(int value) {
	this->value = value;
	update();
}

void SplayNode::add(int value) {
	toPush += value;
	needPush = 1;
}

void SplayNode::setReverse() {
	reverse ^= 1;
	needPush = 1;
}

// вызывается когда мы хотим получить какую-то информацию от вершины,
// чтобы она выполнила все отлоденные операции и передала их потомкам
void SplayNode::push() {
	needPush = 0;
	if (reverse) {
		if (left != nullptr)
			left->setReverse();
		if (right != nullptr)
			right->setReverse();
		std::swap(left, right);
		std::swap(sizeSuffixDec, sizePrefixInc);
		std::swap(leftmostValue, rightmostValue);
		reverse = 0;
	}
	if (toPush) {
		if (left != nullptr)
			left->add(toPush);
		if (right != nullptr)
			right->add(toPush);
		this->value += toPush;
		sum += size * (ll) toPush;
		leftmostValue += toPush;
		rightmostValue += toPush;
		toPush = 0;
	}
}

// вызывается когда меняется структура вершины (удаляются/добавляются потомки)
void SplayNode::update() {
	assert(!needPush);
	assert(!reverse);
	assert(!toPush);
	if (left != nullptr)
		left->push();
	if (right != nullptr)
		right->push();
	size = getSize(left) + 1 + getSize(right);
	sum = getSum(left) + value + getSum(right);
	leftmostValue = left == nullptr ? value : left->leftmostValue;
	rightmostValue = right == nullptr ? value : right->rightmostValue;

	sizeSuffixDec = right == nullptr ? 0 : right->sizeSuffixDec;
	if (right == nullptr || right->sizeSuffixDec == right->size && value >= right->leftmostValue) {
		++sizeSuffixDec;
		if (left != nullptr && left->rightmostValue >= value)
			sizeSuffixDec += left->sizeSuffixDec;
	}

	sizePrefixInc = left == nullptr ? 0 : left->sizePrefixInc;
	if (left == nullptr || left->sizePrefixInc == left->size && left->rightmostValue <= value) {
		++sizePrefixInc;
		if (right != nullptr && value <= right->leftmostValue)
			sizePrefixInc += right->sizePrefixInc;
	}
}

SplayNode::~SplayNode() {
	delete left;
	delete right;
}
