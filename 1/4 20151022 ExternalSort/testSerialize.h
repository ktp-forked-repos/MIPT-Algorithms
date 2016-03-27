#ifndef TEST_SERIALIZE
#define TEST_SERIALIZE

#include "serialize.h"

template<class T>
void testObject(T source, const char* name)
{
	std::cout << "Test " << name << " ... ";
	std::fstream stream;
	
	stream.open("test.dat", std::ios_base::binary | std::ios_base::out);
	writeObjectToStream(source, stream);
	stream.close();
	
	T target;
	stream.open("test.dat", std::ios_base::binary | std::ios_base::in);
	readObjectFromStream(target, stream);
	stream.close();
	assert(std::remove("test.dat") == 0);
	
	std::cout << ((source == target) ? "OK" : "FAIL") << std::endl;
}

void testSerialize()
{
	std::cout << "\tSerialization:" << std::endl;
	
	char c = 'D';
	testObject(c, "char");
	
	int i = 77;
	testObject(i, "int");
	
	std::pair<int, int> pii = {77, -2015};
	testObject(pii, "pair<int, int>");
	
	std::string s = "it's string!";
	testObject(s, "string");
	
	std::pair<std::string, int> psi = {"i37164546419", 7};
	testObject(psi, "pair<string, int>");
	
	std::pair<int, std::string> pis = {2015, "jkadscbkgnsadk"};
	testObject(pis, "pair<int, string>");
	
	std::pair<std::string, std::string> pss = {"k g f h k s", "q4fwdh9s"};
	testObject(pss, "pair<string, string>");
	
	std::vector<int> vi = {1, 7, 10, 77, 123, -52143, 761925};
	testObject(vi, "vector<int>");
	
	std::vector<std::string> vs = {"string one", "very big string 415fhw6hsd 1745c91 6 79q5 tq4ty28g", "", "q"};
	testObject(vs, "vector<string>");
	
	std::vector<std::pair<int, int>> vpii = {{20, 8}, {19, 98}, {2210, 2015}};
	testObject(vpii, "vector<pair<int, int>>");
	
	std::vector<std::pair<std::string, int>> vpsi = {{"15625252", 157462}, {"-dfasuck gdc xdsf", -2015}, {"", 0}};
	testObject(vpsi, "vector<pair<string, int>>");
	
	std::vector<std::vector<int>> vvi = {{0, 1, 2, 3}, {}, {10, 100, 1000}};
	testObject(vvi, "vector<vector<int>>");
	
	std::vector<std::pair<std::vector<std::pair<std::string, std::pair<int, long long>>>, double>> complex = 
	{
		{{
			{"1, 2", {1, 2LL}}, 
			{"-100, 10LL", {-100, 10LL}}
		}, 77.77},
		{{
			{"77, 81430764571962LL", {77, 81430764571962LL}}
		}, 123.456},
		{{
			{"5419, 47326954159LL", {5419, 47326954159LL}}, 
			{"-481795, -48156156302516LL", {-481795, -48156156302516LL}}, 
			{"81597528, 0", {81597528, 0}},
			{"0, 0", {0, 0}}
		}, 2210.2015},
		{{
			{"", {0, 0LL}}
		}, 0.0}
	};
	testObject(complex, "complex object");
	
	std::cout << std::endl;
}

#endif