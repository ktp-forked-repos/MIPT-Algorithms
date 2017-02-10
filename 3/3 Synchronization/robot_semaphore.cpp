#include <iostream>
#include <thread>
#include <atomic>
#include "semaphore.h"

/*
 * Реализация на семафоре
 * Кроме семафора ещё используется атомарный флаг --- is_next_left,
 * если он истиненн, то следующим функцию step должен выполнить первый поток, иначе второй
 * изначально значение счётчика семафора равно единице
 * потоки в цикле выполняют следующее:
 *   пытаются уменьшить счётчик семафора
 *   у одного из них это удаётся, он проверяет что он должен следующий вызывать step:
 *     если это не так, то он пытается пробудить другой поток:
 *       увеличивает значение счётчика семафора, таким образом давая возможность другому потоку его уменьшить
 *       сразу после этого сам пытается уменьшить значение счётчика (возможно стоит вставить std::this_thread::yield() между увеличивание и уменьшением)
 *     иначе он вызывает step, меняет флаг и увеличивает значение счётчика семафора, таким образом давая возможность другому потоку его уменьшить
 */

void step(std::string direction) {
	std::cout << direction << std::endl;
}

std::atomic_bool is_next_left;
Semaphore semaphore;

void robot(bool is_this_left) {
	std::string direction = is_this_left ? "left" : "right";
	if (is_this_left) {
		semaphore.post();
	}
	while (true) {
		semaphore.wait();
		while (is_this_left != is_next_left) {
			semaphore.post();
			semaphore.wait();
		}
		step(direction);
		is_next_left = !is_this_left;
		semaphore.post();
	}
}

int main() {
	std::thread thread1(robot, true);
	std::thread thread2(robot, false);
	thread1.join();
	thread2.join();
}