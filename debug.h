/*
#ifdef LOCAL
#include "debug.h"
#else
void nop() {}
#define msg(...) nop()
#define dbg(...) nop()
#define dbgt(...) nop()
#define dbgl(...) nop()
#endif
*/
bool dont = 0;

#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
ostream &COUT = cout;

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

template<typename T1 = int, typename T2 = int, typename ...>
struct getPairType {
	typedef pair<T1, T2> type;
};

template<typename T = int, typename ...>
struct getAtomicType {
	typedef atomic<T> type;
};

template<template<typename...> typename C, typename... E, typename enable_if<!is_same<C<E...>, string>::value && !is_same<C<E...>, typename getPairType<E...>::type>::value && !is_same<C<E...>, typename getAtomicType<E...>::type>::value>::type * = nullptr>
ostream &operator<<(ostream &out, C<E...> c) {
	return write(out, c.begin(), c.end());
}

template<typename Iterator, typename = decltype(*declval<Iterator &>(), void(), ++declval<Iterator &>(), void())>
ostream &operator<<(ostream &out, pair<Iterator, Iterator> c) {
	return write(out, c.first, c.second);
}

template<typename A, typename B>
ostream &operator<<(ostream &out, pair<A, B> p) {
	return out << '{' << p.first << ", " << p.second << '}';
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
	COUT << ss.str();

	// or for sequential programms
	// myPrint(COUT, names, args...);
#endif
}

#define msg(s) if (!dont) COUT << (s + std::string("\n"))
#define dbgt(...) myPrintLabel("\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgtt(...) myPrintLabel("\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgttt(...) myPrintLabel("\t\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgtttt(...) myPrintLabel("\t\t\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgl(label, ...) myPrintLabel((label + string(": ")), #__VA_ARGS__, __VA_ARGS__)
#define print_clock() printf("%.3f секунд\n", clock() / (float) CLOCKS_PER_SEC)

template<typename... Args>
void myPrintLabel(string label, const char *names, Args &&... args) {
#ifdef LOCAL
	if (dont) return;
	ostringstream ss;
	myPrint(ss, names, args...);
	COUT << label + ss.str();
#endif
}

template<typename T>
void test(T t, string expected) {
	ostringstream out;
	out << t;
	assert(out.str() == expected);
}

void test() {
	test(vector<int>{1, 2, 3}, "{1, 2, 3}");
	test(set<int>{1, 2, 3}, "{1, 2, 3}");
	test(map<int, int>{{1, 10},
	                   {2, 20}}, "{{1, 10}, {2, 20}}");
	test(pair<int, int>{1, 2}, "{1, 2}");
	test(string{"abc"}, "abc");
	test("abc", "abc");
}