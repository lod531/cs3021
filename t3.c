#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int topRecursionDepth = 0;

int ackermann(int x, int y, int recursionDepth)
{
	if(recursionDepth > topRecursionDepth)
	{
		topRecursionDepth = recursionDepth;
	}
	if(x == 0)
	{
		return y+1;
	}
	else if(y == 0)
	{
		return ackermann(x-1, 1, recursionDepth+1);
	}
	else
	{
		return ackermann(x-1, ackermann(x, y-1, recursionDepth+1), recursionDepth+1);
	}
}

int main()
{
	unsigned long microSeconds = 0;
	float milliSeconds = 0.0;
	clock_t start, end;
	start = clock();


	int result = ackermann(3, 6, 0);
	printf("%d %d\n", topRecursionDepth, result);
	end = clock();
	microSeconds = end - start;
	printf("Microseconds taken: %lu\n", microSeconds);
	return 0;
}
