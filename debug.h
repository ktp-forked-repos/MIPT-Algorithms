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
bool multi = 1;
bool print_time = 1;

#include <sstream>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
ostream &COUT = cout;

struct TimePrinter {
    ~TimePrinter() {
        if (print_time) {
            fprintf(stderr, "%.3f seconds\n", clock() / (float) CLOCKS_PER_SEC);
        }
    }
};

TimePrinter tp;

long long gettime() {
    static chrono::time_point<chrono::high_resolution_clock> t0 = chrono::high_resolution_clock::now();
    chrono::time_point<chrono::high_resolution_clock> t1 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();
}

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

template<template<typename...> typename C, typename... E, typename enable_if<!is_same<C<E...>, string>::value>::type * = nullptr, typename = decltype(declval<C<E...>>().begin())>
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

#define msg(s) if (!dont) COUT << ((s) + string("\n"))
#define dbg(...) myPrintLabel("", #__VA_ARGS__, __VA_ARGS__)
#define dbgt(...) myPrintLabel("\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgtt(...) myPrintLabel("\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgttt(...) myPrintLabel("\t\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgtttt(...) myPrintLabel("\t\t\t\t", #__VA_ARGS__, __VA_ARGS__)
#define dbgl(label, ...) myPrintLabel((label) + string(": "), #__VA_ARGS__, __VA_ARGS__)
#define print_clock() printf("%.3f секунд\n", clock() / (float) CLOCKS_PER_SEC)

string gettimestring() {
    long long time = gettime();
    static thread_local char buffer[100];
    sprintf(buffer, "%5.1f", time / 1e6);
    return string(buffer);
}

int getthreadnumber() {
    static atomic_int number_threads = {0};
    static thread_local int number = ++number_threads;
    return number;
}

template<typename... Args>
void myPrintLabel(string label, const char *names, Args &&... args) {
#ifdef LOCAL
    if (dont) return;
    ostringstream ss;
    if (multi) ss << "#" << getthreadnumber() << " " << gettimestring() << " " << label;
    myPrint(ss, names, args...);
    COUT << ss.str();
#endif
}

template<typename T>
void test(const T &t, string expected) {
    ostringstream out;
    out << t;
    assert(out.str() == expected);
}

void test() {
    test(vector<int>{1, 2, 3}, "{1, 2, 3}");
    test(set<int> {1, 2, 3}, "{1, 2, 3}");
    test(map<int, int> {{1, 10},
                        {2, 20}}, "{{1, 10}, {2, 20}}");
    test(pair<int, int>{1, 2}, "{1, 2}");
    test(string{"abc"}, "abc");
    test("abc", "abc");
    test(7, "7");
    test(atomic_int{7}, "7");
    test((void *) 7, "0x7");
}