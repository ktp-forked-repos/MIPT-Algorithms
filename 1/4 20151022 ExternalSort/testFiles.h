#ifndef TEST_FILES
#define TEST_FILES

#include "fileStorage.h"
#include "generator.h"

template<class T>
void test(std::string className, T (*generator)(), unsigned int number)
{
	std::cout << "Test " << number << " " << className << " ... ";
	
	T* data = new T[number];
	for (unsigned int i = 0; i < number; ++i)
		data[i] = (*generator)();
	
	FileStorage<T>* file = new FileStorage<T>();
	
	file->startWrite();
	for (unsigned int i = 0; i < number; ++i)
		file->write(data[i]);
	file->stop();
	
	file->startRead();
	for (unsigned int i = 0; i < number; ++i)
	{
		assert(file->hasNext());
		assert(data[i] == file->read());
	}
	assert(!file->hasNext());
	file->stop();
	
	delete[] data;
	delete file;
	
	std::cout << "OK" << std::endl;
}

template<class T>
void testOnSetNumbers(std::string className, T (*generator)(), std::vector<unsigned int> numbers)
{
	std::cout << className << ":" << std::endl;
	for (unsigned int number : numbers)
		test(className, generator, number);
	std::cout << std::endl;
}

void testFiles()
{
	std::cout << "\tFiles:" << std::endl;
	
	std::vector<unsigned int> defaultSetNumbers = {0, 1, 10, 1024, 1000000};
	std::vector<unsigned int> smallSetNumbers = {0, 1, 10, 77};
	
	testOnSetNumbers("chars", generateChar, defaultSetNumbers);
	testOnSetNumbers("ints", generateInt, defaultSetNumbers);
	testOnSetNumbers("long", generateLong, defaultSetNumbers);
	testOnSetNumbers("strings", generateString, smallSetNumbers);
}

#endif