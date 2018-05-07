#include "Mountain.h"
#include <time.h>
NS_GNRT_START
#define isvariables(c) (c == 'S' || c == 'T' || c == 'C' || c == 'R')
#define isconstants(c) (c == '+' || c == '-' || c == '>' || c == '<' ||c == '[' || c == ']' || c == '^')
#define isDir(c) (c == '+' || c == '-' || c == '>' || c == '<')
#define Degree2Angle 3.1415926/180.0
inline float GetDirection(char d, float curDir)
{
	if (d == '<')
		return curDir - /*std::sin*/(30 /** Degree2Angle*/);
	else if (d == '>')
		return curDir + /*std::sin*/(30 /** Degree2Angle*/);
	else if (d == '+')
		return curDir + /*std::sin*/(15 /** Degree2Angle*/);
	else if (d == '-')
		return curDir - /*std::sin*/(15 /** Degree2Angle*/);
	else return curDir;
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
		else
			return 0;
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
	else
	{
		return 0;
	}
}
bool GetChange(char from, char to)
{
	float chance = _mrandom(0, 10000) / 10000.0;
	return chance <= GetChangeF(from, to);
}
MountainGen::MountainGen(Vector3&& p, int depth) :
	_mother(),
	_child(),
	_M(),
	_rules(),
	_genDepth(depth),
	_currentDir(0),
	_currentPoint(p),
	_tmpList()
{
	/*L_M_Point first('S', 0, std::move(p));
	_mother.push_back(first);*/
	_tmpList.push_back('S');
	_rules.insert(std::pair<char, const char*>('C', "^S^T^C^R"));
	_rules.insert(std::pair<char, const char*>('T', "^[C]^S^[>S^<S^T^+T^-T]^<T^>T^+S^-S"));
	_rules.insert(std::pair<char, const char*>('S', "S^T^[+SS^-T]^[-S]^[>S]^[<S]^[+T>S^-S]^[-T]^[>T]^[<T^+S]^C^[>CT^S]^[<C]"));
}

MountainGen::~MountainGen()
{
	_mother.clear();
	_child.clear();
	_rules.clear();
}

//ignore block recusively
static bool IIgnoreBlockReverse(std::string::iterator& end, const std::string::iterator& begin)
{
	while (true)
	{
		end--;
		if (end == begin)
			return false;
		if (*end == '[')
			return true;
		if (*end == ']')
		{
			if (!IIgnoreBlockReverse(end, begin))
				return false;
		}
	}
	return true;
}
//get var before ^
static char GetMstate(std::string::iterator& end, const std::string::iterator& begin)
{
	auto i = end;
	while (true)
	{
		if (i != begin)
		{
			if (*i == ']')
			{
				if (!IIgnoreBlockReverse(i, begin))
					return 0;
			}
			else if (isvariables(*i))
				return *i;
		}
		else
		{
			return *begin;
		}
		i--;
	}
	return 0;
}
//ignore block recusively
static bool IIgnoreBlock(std::string::iterator& headItr, const std::string::iterator& end)
{
	while (true)
	{
		headItr++;
		if (headItr == end)
			return false;
		if (*headItr == ']')
			return true;
		if (*headItr == '[')
		{
			if (!IIgnoreBlock(headItr, end))
				return false;
		}
	}
	return true;
}
//foolish function to flush ^
static bool IFlushGen(char prevGenV, std::string::iterator& headItr, const std::string::iterator& end, std::vector<char> & genList)
{
	if (headItr == end)
		return false;
	auto ItrBegin = headItr;
	char tmp = *headItr;
	//^>S like state
	if (isDir(tmp))
	{
		headItr++;
		if (headItr == end)
		{
			return false;
		}
		char c = *headItr;
		if (!isvariables(c))
			return false;

		//caculate if we continue
		if (GetChange(prevGenV, c))
		{
			genList.push_back(tmp);
			genList.push_back(c);
		}
		else
		{
			//^>S like state not succeed
		}
		return true;
	}
	//^S like
	else if (isvariables(tmp))
	{
		//caculate if we continue
		if (GetChange(prevGenV, tmp))
		{
			genList.push_back(tmp);
		}
		else
		{
			//^S like state not succeed
		}
		return true;
	}
	//^[ like
	else if (tmp == '[')
	{
		headItr++;
		if (headItr == end)
		{
			return false;
		}
		bool succeedBlock = false;
		//empty block following ^
		if (*headItr == ']')
		{
			return false;
		}
		//^[S like
		else if (isvariables(*headItr))
		{
			//caculate if we continue
			if (GetChange(prevGenV, *headItr))
			{
				genList.push_back(tmp);
				genList.push_back(*headItr);
				succeedBlock = true;

			}
			else
			{
				//^>S like state not succeed
			}
		}
		//^[>S like
		else if (isDir(*headItr))
		{
			headItr++;
			if (headItr == end)
				return false;
			if (isvariables(*headItr))
			{
				//caculate if we continue
				if (GetChange(prevGenV, *headItr))
				{
					genList.push_back(tmp);
					genList.push_back(*(headItr - 1));
					genList.push_back(*headItr);
					succeedBlock = true;
				}
				else
				{
					//do nothing
				}
			}
			//not ^[>S like but ^[>] like
			else
			{
				return false;
			}
		}
		if (!succeedBlock)
		{
			if (!IIgnoreBlock(headItr, end))
				return false;
		}
		else
		{
			//find ']'
			while (true)
			{
				++headItr;
				if (headItr == end)
					return false;
				if (*(headItr) == ']')
				{
					genList.push_back(']');
					break;
				}
				//another ^ in block
				if (*headItr == '^')
				{
					auto itrTmp = headItr - 1;
					char prevV = GetMstate(itrTmp, ItrBegin);
					if (!isvariables(prevV))
						return false;
					headItr++;
					if (!IFlushGen(prevV, headItr, end, genList))
					{
						return false;
					}
				}
				else
				{
					genList.push_back(*headItr);
				}
				if (headItr == end)
					return false;
			}
		}
	}
	return true;
}
static bool FlushGen(char prevGenV, std::string& genList)
{
	if (prevGenV == '^')
	{
		LogErrorFormat("MountainGen", "content error ^ must have followed by var %s", genList.c_str());
		return false;
	}
	std::vector<std::string::iterator> removeList;
	std::vector<std::string::iterator> history;
	std::vector<char> vGen;
	vGen.clear();
	char prevV = 0;
	auto sitr = genList.begin();
	for (; sitr != genList.end(); sitr++)
	{
		if (*sitr == '^')
		{
			if (prevV == 0 && prevGenV == 0)
			{
				LogErrorFormat("MountainGen", "content error ^ must have followed by var %s", genList.c_str());
				return false;
			}
			sitr++;
			//if ^ is first,us prev gen stead
			if (prevV == 0)
				prevV = prevGenV;
			if (!IFlushGen(prevV, sitr, genList.end(), vGen))
			{
				LogErrorFormat("MountainGen", "content error ^ must have a Var following %s", genList.c_str());
				return false;
			}

		}
		else if (isvariables(*sitr))
		{
			prevV = *sitr;
			vGen.push_back(prevV);
		}
		if (sitr == genList.end())
			break;
	}
	genList.clear();
	genList.append(vGen.data(), vGen.size());
	return true;
}
bool MountainGen::Gen()
{
	std::string gen = std::string();
	for (auto c : _tmpList)
	{
		const char* rule = nullptr;
		auto ritr = _rules.find(c);
		if (ritr != _rules.end())
		{
			std::string tmp = ritr->second;
			char prev = 0;
			if (gen.size() > 0)
			{
				auto itr = gen.begin() + gen.size() - 1;
				prev = GetMstate(itr, gen.begin());
			}
			if (!FlushGen(prev, tmp))
				return false;
			gen.append(tmp);
		}
		//constants or else
		else
		{
			//do nothing at this time
		}

	}
	_tmpList.clear();
	for (int i = 0; i < gen.size(); i++)
	{
		_tmpList.push_back(gen[i]);
	}
	//FlushGen(_tmpList);
	LogFormat("MountainGen", "depth %d:\n\t%s", _genDepth, gen.c_str());
	_genDepth--;
	if (_genDepth <= 0)
		return true;
	return Gen();
}
void MountainGen::Init(int seed)
{
	_msrandom(seed);
}

void MountainGen::Start()
{
	Init(time(nullptr));
	Gen();
	//normal L-system translation
	//relocate
	size_t rpos = 0;
	std::vector<char> _gen;
	char tmp = 0;
	while (CheckRP(rpos))
	{
		char c = Next(rpos);
		switch (c)
		{
		case '^':
		{
			throw 0;
			return;
		}
		case '[':
		{
			MemoryPoint m = MemoryPoint(std::move(_currentPoint), c, _currentDir, rpos);
			GenNextBlock(c, rpos);
			_currentDir = m.i;
			_currentPoint = m.v;
			break;
		}
		default:
			GenNext(c, rpos);
			break;
		}
	}
	for (auto m : _mother)
	{
		LogFormat("MountainGen_Ret", "idx %d,\t\tvector3(%.2f,\t\t%.2f,\t\t%.2f),\t\t\t\tvar %c", m.idx, m.v.x, m.v.y, m.v.z, m.z);
	}
}
//gen block map
//ensure that rpos is [
size_t MountainGen::GenNextBlock(char prev, size_t& rpos)
{
	while (true)
	{
		char c = Next(rpos);
		if (c == ']')
			return 0;
		else if (c == '[')
		{
			MemoryPoint m = MemoryPoint(std::move(_currentPoint), c, _currentDir, rpos);
			GenNextBlock(c, rpos);
		}
		else
		{
			GenNext(c, rpos);
		}
	}
	return 0;
}
void MountainGen::GenNext(char v, size_t& rpos)
{
	char first = v;
	if (isvariables(first))
	{
		GenVar(first, rpos);
	}
	else if (isDir(first))
	{
		char second = Next(rpos);
		_currentDir = GetDirection(first, _currentDir);
		if (isvariables(second))
		{
			GenVar(second, rpos);
		}
		else
		{
			LogErrorFormat("MountainGen", "GenNextFail at pos %d", rpos);
		}
	}
	else
	{
		LogErrorFormat("MountainGen", "GenNextFail at pos %d", rpos);
	}
}
void MountainGen::GenVar(char v, size_t rpos)
{
	float x = _currentPoint.x + NORMAL_STEP * std::cos(_currentDir);
	float y = _currentPoint.y + NORMAL_STEP * std::sin(_currentDir);
	float height = 0;
	if (v == 'C')
		height -= _mrandom(10, 8000);
	else if (v == 'T')
		height += _mrandom(40, 1800);
	else if (v == 'S')
		height += _mrandom(-1200, 1200);
	float z = _currentPoint.z + height;
	_currentPoint.x = x;
	_currentPoint.y = y;
	_currentPoint.z = z;
	Vector3 p3(x, y, z);
	L_M_Point p = L_M_Point(v, rpos-1, std::move(p3));
	_mother.push_back(p);
}
void MountainGen::GenRock()
{
}

NS_GNRT_END