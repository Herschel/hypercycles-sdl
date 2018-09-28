// MIKE TODO: Implement these.

int fixmul(int a, int b)
{
	// 16.16?
	return  ((long long)a * (long long)b) >> 16;
}

int fixmul2(int a, int b)
{
	// 8.24?
	return  ((long long)a * (long long)b) >> 24;
}

int fixdiv(int a, int b)
{
	return ((long long)a * (1 << 16)) / b;
}

int shf1(int a)
{
	return a >> 22;
}

int qrshift(int a, int b)
{
	return a >> b;
}
