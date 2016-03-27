#include <cstdio>
#include <iostream>
#include <ctime>

void mergeSort(int *start, int *finish, int *buffer)
{
	if (finish - start <= 1)
		return;
	int *middle = start + (finish - start) / 2;
	mergeSort(start, middle, buffer);
	mergeSort(middle, finish, buffer);
	
	for (int *it1 = start, *it2 = middle, *itBuffer = buffer; it1 != middle || it2 != finish; )
		if (it1 == middle || (it2 != finish && (*it2 < *it1)))
		{
			*itBuffer = *it2;
			itBuffer++;
			it2++;
		}
		else
		{
			*itBuffer = *it1;
			itBuffer++;
			it1++;
		}
	for (int *it = start, *itBuffer = buffer; it != finish; it++, itBuffer++)
		*it = *itBuffer;
}

int main()
{
	freopen("input.txt", "r", stdin);
	int n;
	std::cin >> n;
	int a[n];
	for (int i = 0; i < n; i++)
		std::cin >> a[i];
	
	int buffer[n];
	mergeSort(a, a + n, buffer);
	for (int i = 0; i < n; i++)
		std::cout << a[i] << " ";
	std::cout << std::endl;
	
	return 0;
}