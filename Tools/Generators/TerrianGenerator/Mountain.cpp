#include "Mountain.h"

NS_GNRT_START

bool GetChange(char from, char to)
{
	float chance = _mrandom(0, 10000) / 10000.0;
	return chance <= GetChangeF(from, to);
}
float GetChangeF(char from, char to)
{
	if (from == 'S')
	{
		if (to == 'S')
			return .8;
		else if (to == 'T')
			return .15;
		else if (to == 'C')
			return .05;
		else
			return 0;
	}
	else if (from == 'T')
	{
		if (to == 'S')
			return .88;
		else if (to == 'T')
			return .10;
		else if (to == 'C')
			return .02;
	}
	else if (from == 'C')
	{
		if (to == 'C')
			return .47;
		else if (to == 'S')
			return .21;
		else if (to == 'T')
			return .32;
		else
			return 0;
	}
}

MountainGen::MountainGen(Vector3 p,int depth) : 
	_mother(), 
	_child(), 
	_M(Vector3(), 0, 0, 0),
	_rules(), 
	_genDepth(depth)
{
	L_M_Point first('S', 0, std::move(p));
	_mother.push_back(first);
	_rules.insert(std::pair<char, const char*>('C', "^S^T^C^R"));
	_rules.insert(std::pair<char, const char*>('T', "^[<C+SRTSR-S>T]^S^[RT]^>S<C[+T^C]^[-R]"));
	_rules.insert(std::pair<char, const char*>('S',"[S^T>SR<S-TR+S]^[+STRS>S<T-S][-SST<SR>T+RS]^[>RSTS-SR+T>S]^[<STRS+S-T<SR]"));
}

MountainGen::~MountainGen()
{
	_mother.clear();
	_child.clear();
	_rules.clear();
}

void MountainGen::Init(int seed)
{
	_msrandom(seed);
}

void MountainGen::Gen()
{
	_genDepth--;
	if (_genDepth <= 0)
		return;

	_mother.clear();
	_mother = _child;
	_child.clear();
	Gen();
}

bool MountainGen::GenNextByChange(char prev, char & next)
{
	return false;
}

bool MountainGen::GenNextBlockByChane(char prev, const char * block)
{
	return false;
}

bool MountainGen::GenNextBlock(char prev, const char * block)
{
		
	return false;
}

void MountainGen::GenRock()
{
}

NS_GNRT_END