#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

/*
 * Реализация на условных переменных
 * Кроме условной переменной и необходимого для неё мьютекса ещё используется атомарный флаг --- is_next_left,
 * если он истиненн, то следующим функцию step должен выполнить первый поток, иначе второй
 * потоки в цикле выполняют следующее:
 *   пытаются захватить мьютекс с помощью unique_lock
 *   у одного из них это удаётся, он проверяет что он должен следующий вызывать step:
 *     если это не так, то поток ждёт на условной переменной
 *     иначе он вызывает step, меняет флаг и пробуждает другой поток с помощью условной переменной
 *     (на самом деле другой поток пробудится не раньше, чем текущий выйдет из итерации while из-за unique_lock)
 */

void step(std::string direction) {
	std::cout << direction << std::endl;
}

std::atomic_bool is_next_left(true);
std::mutex m;
std::condition_variable cv;

void robot(bool is_this_left) {
	std::string direction = is_this_left ? "left" : "right";
	while (true) {
		std::unique_lock<std::mutex> lock(m);
		while (is_this_left != is_next_left) {
			cv.wait(lock);
		}
		step(direction);
		is_next_left = !is_this_left;
		cv.notify_one();
	}
}

int main() {
	std::thread thread1(robot, true);
	std::thread thread2(robot, false);
	thread1.join();
	thread2.join();
}