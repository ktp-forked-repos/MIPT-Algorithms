#ifndef BASE_H
#define BASE_H

typedef std::vector<size_t> line;
typedef std::vector<line> table;

// просто печатает матрицу
void print(table t)
{
	for (line &l : t)
	{
		for (size_t x : l)
			printf("%2zu ", x);
		printf("\n");
	}
	printf("\n");
}

// печатает критические точки (всей матрицу)
void printPoints(table t)
{
	for (size_t i = 0; i < t.size(); ++i)
	{
		for (size_t j = 0; j < t[i].size(); ++j)
			printf("%2zu", t[i][j]);
		printf("\n");
		if (i + 1 == t.size())
			break;
		printf("%c ", t[i][0] == t[i + 1][0] ? '0' : ' ');
		for (size_t j = 1; j < t[i].size(); ++j)
		{
			bool c = t[i][j] == t[i + 1][j] && t[i][j] == t[i][j - 1] && t[i][j] + 1 == t[i + 1][j - 1];
			printf("%c ", c ? char('0' + j) : ' ');
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

#endif
