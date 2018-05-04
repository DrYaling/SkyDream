#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<stdlib.h>
#include <windows.h>
#include "../../Tools/Log/Log.h"
using namespace std;
typedef long long lol;
lol Mod = 1000000007;
lol pw[5010], a[5010][5010], d, n, ans;
int main()
{
	int i, j, flag, k;
	LogFormat("KOCH", "%s,%d", "aaa", 32768);
	LogFormat("KOCH1", "%s,%d", "aaa", 897);
	GlobalLogger::Clear();
	cin >> n;
	pw[0] = 1;
	for (i = 1; i <= n; i++)
		pw[i] = pw[i - 1] * 2;
	cin >> a[0][0] >> a[0][pw[n]] >> a[pw[n]][0] >> a[pw[n]][pw[n]] >> d;
	for (i = n; i >= 1; i--, (d *= 2) %= Mod)
	{
		for (j = 0; j < pw[n]; j += pw[i])
		{
			for (k = 0; k < pw[n]; k += pw[i])
			{
				int x = j + pw[i - 1], y = k + pw[i - 1];
				a[x][y] = a[j][k] + a[j + pw[i]][k] + a[j][k + pw[i]] + a[j + pw[i]][k + pw[i]] + d;
				a[x][y] %= Mod;
			}
		}
		flag = 1;
		for (j = 0; j <= pw[n]; j += pw[i - 1], flag ^= 1)
		{
			for (k = flag * pw[i - 1]; k <= pw[n]; k += pw[i])
			{
				if (j - pw[i - 1] >= 0)
					a[j][k] += a[j - pw[i - 1]][k];
				if (j + pw[i - 1] <= pw[n])
					a[j][k] += a[j + pw[i - 1]][k];
				if (k - pw[i - 1] >= 0)
					a[j][k] += a[j][k - pw[i - 1]];
				if (k + pw[i - 1] <= pw[n])
					a[j][k] += a[j][k + pw[i - 1]];
				a[j][k] += d;
				a[j][k] %= Mod;
			}
		}
	}
	for (i = 0; i <= pw[n]; i++)
		for (j = 0; j <= pw[n]; j++)
			ans += a[i][j], ans %= Mod;
	cout << ans;
	while (1) Sleep(1);
	return 0;
}