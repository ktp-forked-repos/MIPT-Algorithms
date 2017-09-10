#ifndef GENERATOR
#define GENERATOR

char generateLowerCaseLetter()
{
	return static_cast<char>('a' + rand() % 26);
}

char generateUpperCaseLetter()
{
	return static_cast<char>('A' + rand() % 26);
}

char generateCharDigit()
{
	return static_cast<char>('0' + rand() % 10);
}

char generateChar()
{
	std::vector<char> chars = {generateLowerCaseLetter(), generateUpperCaseLetter(), generateCharDigit(), ',', '.', ':', ';', '\'', '"', '!', '!', '?'};
	return chars[rand() % chars.size()];
}

int generateInt()
{
	return (rand() << 16) + rand();
}

long long generateLong()
{
	long long ret = generateInt();
	return ret + generateInt();
}

double generateDouble()
{
	return static_cast<double>(generateLong()) + static_cast<double>(generateLong()) / 1e18;
}

std::string generateString()
{
	std::string string;
	string.resize((rand()));
	for (size_t i = 0; i < string.length(); ++i)
		string[i] = generateChar();
	return string;
}

#endif