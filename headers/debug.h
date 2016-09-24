#ifndef LOCAL
void nop() {}
#define dbg(...) nop
#define dbgl(...) nop
#define dbgt(...) nop
#define dbgtt(...) nop
#define dbgttt(...) nop
#define dbgtttt(...) nop
#endif

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

bool dont = 1;

template<typename T>
ostream &operator<<(ostream &out, vector<T> v) {
	out << '{';
	for (size_t i = 0; i < v.size(); ++i)
		out << (i == 0 ? "" : ", ") << v[i];
	return out << '}';
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
