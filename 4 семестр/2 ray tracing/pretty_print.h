#ifndef RAY_TRACING_PRETTY_PRINT_H
#define RAY_TRACING_PRETTY_PRINT_H
#include <bits/stdc++.h>
using namespace std;

#include "colors.h"
const ascii::Color defaultMemberColor = ascii::cyan;

template<typename Arg1, typename Arg2, typename... Args>
ostream &printObject_(ostream &out, Arg1 &&arg1, Arg2 &&arg2, Args &&... args) {
	out << colored(arg1, defaultMemberColor) << "=" << arg2 << ", ";
	return printObject_(out, args...);
};

template<typename Arg1, typename Arg2>
ostream &printObject_(ostream &out, Arg1 &&arg1, Arg2 &&arg2) {
	return out << colored(arg1, defaultMemberColor) << "=" << arg2;
};

template<typename... Args>
ostream &printObject(ostream &out, Args &&... args) {
	out << "{";
	printObject_(out, args...);
	out << "}";
	return out;
};

#endif //RAY_TRACING_PRETTY_PRINT_H
