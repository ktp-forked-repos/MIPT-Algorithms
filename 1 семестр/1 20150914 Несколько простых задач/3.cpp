#include <cstdio>
#include <iostream>
#include <ctime>

#include <bits/stdc++.h>

void generateFiles()
{
	srand(time(0));
	for (int exponent = 4; exponent <= 7; exponent++)
	{
		std::string file = char('0' + exponent) + std::string(".txt");
		freopen(file.c_str(), "w", stdout);
		int n = 1;
		for (int i = 0; i < exponent; i++)
			n *= 10;
		for (int i = 0; i < n; i++)
			printf("%.5lf\n", ((rand() << 16) + rand()) / double(1000 + rand()));
	}
}

double getTimeCin(std::string fileName, int n)
{
	freopen(fileName.c_str(), "r", stdin);
	int timeStart = clock();
	float f;
	for (int i = 0; i < n; i++)
		std::cin >> f;
	int timeEnd = clock();
	return (timeEnd - timeStart) / (double) CLOCKS_PER_SEC;
}

double getTimeScanf(std::string fileName, int n)
{
	freopen(fileName.c_str(), "r", stdin);
	int timeStart = clock();
	float f;
	for (int i = 0; i < n; i++)
		scanf("%f", &f);
	int timeEnd = clock();
	return (timeEnd - timeStart) / (double) CLOCKS_PER_SEC;
}

int main()
{
	//generateFiles();
	
	for (int exponent = 4; exponent <= 7; exponent++)
	{
		int n = 1;
		for (int i = 0; i < exponent; i++)
			n *= 10;
		double timeCin = getTimeCin(char('0' + exponent) + std::string(".txt"), n);
		double timeScanf = getTimeScanf(char('0' + exponent) + std::string(".txt"), n);
		printf("time cin: \t%.3f seconds\n", timeCin);
		printf("time scanf: \t%.3f seconds\n", timeScanf);
		printf("profit: \t%.1f\n", timeCin / timeScanf);
		printf("\n");
	}
	
	return 0;
}