#include <stdlib.h>
#include <stdio.h>

int hits = 0;
int addressNumber = 0;
int exponentiate(int base, int exponent)
{
	int result = 1;
	while(exponent > 0)
	{
		result *= base;
		exponent--;
	}
	return result;
}



//CACHE SET START//////////////////////////////////////////////////////////////////////////
struct cacheSet
{
	int * * lruArray;
	int * tags;
	int numberOfTagsPerSet;
};


struct cacheSet * cacheSetNew(int numberOfTagsPerSet)
{
	struct cacheSet * result = malloc(sizeof(struct cacheSet));
	result->lruArray = malloc(sizeof(int *) * numberOfTagsPerSet);
	int i;
	for(i = 0; i < numberOfTagsPerSet; i++)
	{
		result->lruArray[i] = malloc(sizeof(int) * numberOfTagsPerSet);
	}
	result->tags = malloc(sizeof(int) * numberOfTagsPerSet);
	for(i = 0; i < numberOfTagsPerSet; i++)
	{
		result->tags[i] = -1;
	}
	result->numberOfTagsPerSet = numberOfTagsPerSet;
}


//returns -1 if false and index of tag if found
int cacheSetIsInSet(struct cacheSet * this, int tag)
{
	int i;
	int result = -1;
	for(i = 0; i < this->numberOfTagsPerSet && (result == -1); i++)
	{
		result = (this->tags[i] == tag)? i : -1;
	}
	return result;
}

int cacheSetFindLRUIndex(struct cacheSet * this)
{
	int i;
	int j;
	for(i = 0; i < this->numberOfTagsPerSet; i++)
	{
		int encounteredOne = 0;
		for(j = 0; j < this->numberOfTagsPerSet && !encounteredOne; j++)
		{
			encounteredOne = this->lruArray[i][j];
		}
		if(!encounteredOne)
		{
			return i;
		}
	}
	printf("error in cacheSetFindLRU, should never be able to reach this printf\n");
	exit(1);
}


void cacheSetUpdateLRU(struct cacheSet * this, int tagIndex)
{
	int i;

	for(i = 0; i < this->numberOfTagsPerSet; i++)
	{

		this->lruArray[tagIndex][i] = 1;
	}
	
	for(i = 0; i < this->numberOfTagsPerSet; i++)
	{
		this->lruArray[i][tagIndex] = 0;
	}
}

void cacheSetInsert(struct cacheSet * this, int tagToBeInserted)
{
	int tagIndex = cacheSetIsInSet(this, tagToBeInserted);
	if(tagIndex != -1) //i.e. if tag is already in set, we just have to update it to be most recently used
	{
				cacheSetUpdateLRU(this, tagIndex);
	}
	else
	{
		int leastRecentlyUsedTagIndex = cacheSetFindLRUIndex(this);
		this->tags[leastRecentlyUsedTagIndex] = tagToBeInserted;
		cacheSetUpdateLRU(this, leastRecentlyUsedTagIndex);
	}
	//cacheSetUpdateLRU call could be moved down here, but I figured I'd keep it this way for readability
}


//CACHE END START/////////////////////////////////////////////////////////////////////////


//CACHE START/////////////////////////////////////////////////////////////////////////
struct cache 
{
	int numberOfBitsPerAddress;
	int numberOfCacheLineOffsetBits;
	int numberOfSetBits;
	int numberOfTagsPerSet;
	int cacheSize;				//in bytes
	int addressMask;
	struct cacheSet * * sets;
};


struct cache * cacheNew(int numberOfBitsPerAddress, int numberOfCacheLineOffsetBits, int numberOfSetBits, int numberOfTagsPerSet)
{
	struct cache * result = malloc(sizeof(struct cache));
	result->numberOfBitsPerAddress = numberOfBitsPerAddress;
	result->numberOfCacheLineOffsetBits = numberOfCacheLineOffsetBits;
	result->numberOfSetBits = numberOfSetBits;
	result->numberOfTagsPerSet = numberOfTagsPerSet;
	int numberOfSets = exponentiate(2, numberOfSetBits);
	result->sets = malloc(sizeof(struct cacheSet *) * numberOfSets);
	int i;
	for(i = 0; i < numberOfSets; i++)
	{
		result->sets[i] = cacheSetNew(numberOfTagsPerSet);
	}
	result->cacheSize = exponentiate(2, numberOfCacheLineOffsetBits) * numberOfTagsPerSet * numberOfSets;
	int mask = 1;
	result->addressMask = 0;
	for(i = 0; i < numberOfBitsPerAddress; i++)
	{
		result->addressMask = result->addressMask | mask;
		mask = mask << 1;
	}
	return result;
}

void cacheAccess(struct cache * this, int address)
{
	int setNumber = ((address << (this->numberOfBitsPerAddress - this->numberOfCacheLineOffsetBits - this-> numberOfSetBits)) & this->addressMask)
				>> (this->numberOfBitsPerAddress - this->numberOfSetBits);
	int tag = address >> (this->numberOfCacheLineOffsetBits + this->numberOfSetBits);
	cacheSetInsert(this->sets[setNumber], tag);

}
	
//CACHE END/////////////////////////////////////////////////////////////////////////






int main(int argc, char * * argv)
{
	struct cache * test = cacheNew(16, 4, 0, 8);
	cacheAccess(test, 0x0000);
	cacheAccess(test, 0x0004);
	cacheAccess(test, 0x000c);
	cacheAccess(test, 0x2200);
	cacheAccess(test, 0x00d0);
	cacheAccess(test, 0x00e0);
	cacheAccess(test, 0x1130);
	cacheAccess(test, 0x0028);
	cacheAccess(test, 0x113c);
	cacheAccess(test, 0x2204);
	cacheAccess(test, 0x0010);
	cacheAccess(test, 0x0020);
	cacheAccess(test, 0x0004);
	cacheAccess(test, 0x0040);
	cacheAccess(test, 0x2208);
	cacheAccess(test, 0x0008);
	cacheAccess(test, 0x00a0);
	cacheAccess(test, 0x0004);
	cacheAccess(test, 0x1104);
	cacheAccess(test, 0x0028);
	cacheAccess(test, 0x000c);
	cacheAccess(test, 0x0084);
	cacheAccess(test, 0x000c);
	cacheAccess(test, 0x3390);
	cacheAccess(test, 0x00b0);
	cacheAccess(test, 0x1100);
	cacheAccess(test, 0x0028);
	cacheAccess(test, 0x0064);
	cacheAccess(test, 0x0070);
	cacheAccess(test, 0x00d0);
	cacheAccess(test, 0x0008);
	cacheAccess(test, 0x3394);
	return 0;
}


