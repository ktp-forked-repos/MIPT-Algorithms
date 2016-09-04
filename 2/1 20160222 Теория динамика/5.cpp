#include <bits/stdc++.h>
#define dbg(x) std::cerr << #x << " = " << (x) << std::endl;

int main()
{
	freopen("input.txt", "r", stdin);

	std::string s, t;
	std::cin >> s >> t;
	size_t sLength = s.length();
	size_t tLength = t.length();

	// size_t ***dp = new size_t[nt][nt + 1][ns + 1];
	size_t ***dp = new size_t **[tLength];
	for (size_t iStart = 0; iStart < tLength; iStart++) {
		dp[iStart] = new size_t *[tLength + 1];
		for (size_t i = 0; i <= tLength; i++)
			dp[iStart][i] = new size_t[sLength + 1];
	}

	// dp[iStart] = таблица для НОП строк s и t[iStart, tLength)
	// dp[iStart][i][j] = длина НОП строк t[iStart, iStart + i) и s[0, j)
	for (size_t iStart = 0; iStart < tLength; iStart++) {
		size_t ct = tLength - iStart;
		size_t **current = dp[iStart];
		for (size_t i = 0; i <= ct; i++)
			for (size_t j = 0; j <= sLength; j++)
				current[i][j] = 0;
		for (size_t i = 1; i <= ct; i++)
			for (size_t j = 1; j <= sLength; j++)
				if (t[iStart + i - 1] == s[j - 1])
					current[i][j] = current[i - 1][j - 1] + 1;
				else
					current[i][j] =
						std::max(current[i][j - 1], current[i - 1][j]);
	}

	// size_t **answer = new size_t[nt][nt];
	size_t **answer = new size_t *[tLength + 10];
	for (size_t l = 0; l < tLength + 10; l++)
		answer[l] = new size_t[tLength + 1];

	// answer[l][r] = длина НОП s и t[l, r] (нумерация с нуля)
	// копируем из dp в answer
	for (size_t l = 0; l < tLength; l++)
		for (size_t r = l; r < tLength; r++)
			answer[l][r] = dp[l][r - l + 1][sLength];

	// delete[][][] dp;
	for (size_t iStart = 0; iStart < tLength; iStart++) {
		for (size_t i = 0; i <= tLength; i++)
			delete[] dp[iStart][i];
		delete[] dp[iStart];
	}
	delete[] dp;

	// отвечаем на запросы
	// ...
	if (1)
	for (size_t l = 0; l < tLength; l++) {
		for (size_t i = 0; i < l; i++)
			printf("%3d ", (int) (i - (int) l + 1));
		for (size_t r = l; r < tLength; r++)
			printf("%3d ", (int) answer[l][r]);
		printf("\n");
	}
	
	if (0)
	for (size_t l = 0; l < tLength; l++) {
		for (size_t r = 0; r < tLength; r++)
			printf("%3d ", (int) answer[l][r] - (int) answer[l + 1][r]);
		printf("\n");
	}
	//
	
	// delete[][] answer;
	for (size_t l = 0; l < tLength; l++)
		delete[] answer[l];
	delete[] answer;

	return 0;
}
