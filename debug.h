#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
bool dont = 0;

void nop() {}

template<typename Iterator>
ostream &write(ostream &out, Iterator begin, Iterator end) {
	if (begin == end) {
		return out << "{}";
	}
	out << '{' << *begin;
	for (Iterator it = ++begin; it != end; ++it) {
		out << ", " << *it;
	}
	return out << '}';
}

template<template<typename...> typename C, typename... E, typename enable_if<!is_same<C<E...>, basic_string<char>>::value>::type * = nullptr>
ostream &operator<<(ostream &out, C<E...> c) {
	return write(out, c.begin(), c.end());
}

template<typename Iterator>
ostream &operator<<(ostream &out, pair<Iterator, Iterator> c) {
	return write(out, c.first, c.second);
}

template<typename Arg1>
void myPrint(ostream &out, const char *name, Arg1 &&arg1) {
	out << name << "=" << arg1 << endl;
}

template<typename Arg1, typename... Args>
void myPrint(ostream &out, const char *names, Arg1 &&arg1, Args &&... args) {
	const char *comma = strchr(names + 1, ',');
	out.write(names, comma - names) << "=" << arg1;
	myPrint(out, comma, args...);
}

#define dbg(...) myPrint(#__VA_ARGS__, __VA_ARGS__)

template<typename... Args>
void myPrint(const char *names, Args &&... args) {
#ifdef LOCAL
	if (dont) return;
	ostringstream ss;
	myPrint(ss, names, args...);
	cerr << ss.str();

	// or for sequential programms
	// myPrint(cerr, names, args...);
#endif
}

#define msg(s) cout << (s + std::string("\n"))
#define dbgt(...) myPrintLabel("\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgtt(...) myPrintLabel("\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgttt(...) myPrintLabel("\t\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgtttt(...) myPrintLabel("\t\t\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgl(label, ...) myPrintLabel((label + string(": ")), #__VA_ARGS__, __VA_ARGS__)

template<typename... Args>
void myPrintLabel(string label, const char *names, Args &&... args) {
#ifdef LOCAL
	if (dont) return;
	ostringstream ss;
	myPrint(ss, names, args...);
	cerr << label + ss.str();
#endif
}
