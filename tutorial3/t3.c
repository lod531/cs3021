#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int topRecursionDepth = 0;

int ackermann(int x, int y)
{
	if(x == 0)
	{
		return y+1;
	}
	else if(y == 0)
	{
		return ackermann(x-1, 1);
	}
	else
	{
		return ackermann(x-1, ackermann(x, y-1));
	}
}

int main()
{
	unsigned long microSeconds = 0;
	float milliSeconds = 0.0;
	clock_t start, end;
	start = clock();
	int BS = 0;
	int i;
	for(i = 0; i < 10000; i++)
	{
		BS+=ackermann(3, 6);
	}
	end = clock();
	microSeconds = end - start;
	microSeconds = microSeconds/(i+1);
	printf("Microseconds taken: %lu\n(average of %d runs)\n", microSeconds, i);
	return 0;
}
