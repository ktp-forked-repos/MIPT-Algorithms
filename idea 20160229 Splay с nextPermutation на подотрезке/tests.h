//
// Created by dima on 01.03.16.
//

#ifndef IDEA_20160229_SPLAY_TESTS_H
#define IDEA_20160229_SPLAY_TESTS_H

#include "base.h"
#include "VectorStruct.h"
#include "SplayStruct.h"
#include "GeneratorRandom.h"

std::vector<ll> perform(IStruct *s, std::vector<AbstractOperation *> &operations) {
	std::vector<ll> result;
	for (AbstractOperation *operation : operations)
		operation->perform(s, result);
	return result;
}

bool check(std::vector<AbstractOperation *> operations) {
	IStruct *vector = new VectorStruct();
	IStruct *splay = new SplayStruct();

	std::vector<ll> resultVector = perform(vector, operations);
	std::vector<ll> resultSplay = perform(splay, operations);
	delete vector;
	delete splay;
	for (AbstractOperation *operation : operations)
		delete operation;

	return resultVector == resultSplay;
}

#include <sys/time.h>

int getSeed() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return time.tv_sec * 1000000 + time.tv_usec;
}

size_t numberPass = 0;
size_t numberAll = 0;

void runTest(AbstractGenerator *generator, std::string name) {
	++numberAll;
	bool correct = check(generator->generate());
	delete generator;
	if (correct)
		++numberPass;
	printf("%s %s\n", correct ? "OK" : "Fail", name.c_str());
}

typedef GeneratorRandom<1, 1, 1, 1, 1> GeneratorRandomDefault;
typedef GeneratorRandom<1, 1, 0, 3, 0> GeneratorRandomNextPermutation;
typedef GeneratorRandom<1, 1, 0, 0, 3> GeneratorRandomAdd;

void runTests(int seed = getSeed()) {
	printf("Тестируем корректность. Seed: %10d\n", seed);
	srand(seed);

	runTest(new GeneratorRandomDefault(10, 0, 10), "Маленький тест");
	runTest(new GeneratorRandomDefault(1e3, 0, 1e3), "Средний тест");
	runTest(new GeneratorRandomDefault(1e3, 0, 10), "Средний тест с повторяющимися значениями");

	runTest(new GeneratorRandomDefault(1e5, -1e7, 1e7), "Огромный тест");
	runTest(new GeneratorRandomDefault(1e5, 0, 1e2), "Огромный тест с повторяющимися значениями");

	runTest(new GeneratorRandomNextPermutation(1e5, -1e7, 1e7), "Приоритет nextPermutation");
	runTest(new GeneratorRandomNextPermutation(1e5, 0, 1e2), "Приоритет nextPermutation с повторяющимися значениями");

	runTest(new GeneratorRandomAdd(1e5, -1e7, 1e7), "Приоритет add");
	runTest(new GeneratorRandomAdd(1e5, 0, 1e2), "Приоритет add с повторяющимися значениями");

	printf("\n");
	if (numberPass == numberAll)
		printf("Всё хорошо!\n");
	else
		printf("Пройдено %u из %u =(\n", numberPass, numberAll);
}

// Тесты времени
#include <chrono>

size_t getTime(AbstractGenerator *generator) {
	auto operations = generator->generate();
	auto start = std::chrono::high_resolution_clock::now();
	SplayStruct *s = new SplayStruct();
	perform(s, operations);
	delete s;
	auto end = std::chrono::high_resolution_clock::now();
	size_t result = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	delete generator;
	for (AbstractOperation *operation : operations)
		delete operation;
	return result;
}

void timeTest(int rangeMin = 0, int rangeMax = 10, int seed = getSeed()) {
	srand(seed);
	printf("Тестируем время работы. Диапозон случайных данных: [ %11d, %11d ]\n", rangeMin, rangeMax);
	for (size_t number = 1e3, exponent = 3; exponent <= 7; number *= 10, ++exponent) {
		size_t time = getTime(new GeneratorRandomDefault(number, rangeMin, rangeMax));
		printf("1e%u: %u\n", exponent, time);
	}
	printf("\n");
}

// для отладки
void findBug(int seed = 0) {
	AbstractGenerator *generator = new GeneratorRandomDefault(1e3, 0, 10);
	if (seed) {
		srand(seed);
		bool correct = check(generator->generate());
		printf("%d\n", correct);
		return;
	}

	for (int i = 1; i < 1e3; ++i) {
		int seed = getSeed();
		printf("%7d, %10d\n", i, seed);
		srand(seed);
		assert(check(generator->generate()));
	}
}

#endif //IDEA_20160229_SPLAY_TESTS_H
