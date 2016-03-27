#include <iostream>

unsigned int findRepetitionsCount(const char* s, char c1, char c2)
{
	if (s[0] == 0)
		return 0;
	unsigned int count = 0;
	for (unsigned int i = 0; s[i + 1] != 0; i++)
		if (s[i] == c1 && s[i + 1] == c2)
			count++;
	return count;
}

int main()
{
	std::cout << findRepetitionsCount("abacabadabarfda", 'a', 'b') << std::endl;
	return 0;
}