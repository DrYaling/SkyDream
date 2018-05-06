#ifndef _MOUNTAIN_H
#define _MOUNTAIN_H
#include "../../Generators/generator.h"

	NS_GNRT_START
/*
variables:S T R C
constants:[]+-<>^   +为顺时针15°，-为逆时针15°，<为逆时针30°，>为顺时针30°，^为概率事件
axiom: S
rules:(T->^[<C+SRTSR-S>T]^S^[RT]^>S<C[+T^C]^[-R]),
（C->^S^T^C^R）
（S->[S^T>SR<S-TR+S]^[+STRS>S<T-S][-SST<SR>T+RS]^[>RSTS-SR+T>S]^[<STRS+S-T<SR])

*/
#define NORMAL_STEP 5*1000 //meter
static inline int _mrandom(int min, int max)
{
	if (min >= max)
		return min;
	return std::rand() % (max - min) + min;
}
static inline void _msrandom(int seed)
{
	std::srand(seed);
}
#define ROCK_STEP _mrandom(100,1200)
//Lsystem_MountainGen_Point
struct L_M_Point {
public:
	char z;
	Vector3 v;
	int idx;
	explicit L_M_Point(char _z, int _index, Vector3&& _v)
	{
		z = _z;
		v = _v;
		idx = _index;
	}
};
struct MemoryPoint {
public:
	explicit MemoryPoint(Vector3&& _v, char _c, float _r, int _index)
	{
		v = _v;
		c = _c;
		r = _r;
		i = _index;
	}
	bool operator == (const MemoryPoint& right)
	{
		return c == right.c && i == right.i&& std::abs(r - right.r) < 0.00000001F && v == right.v;
	}
	char c;//操作符
	float r;//方向
	int i;
	Vector3 v;
};
static MemoryPoint mp_zero = MemoryPoint(Vector3(), 0, 0, 0);
class MountainGen
{
public:
	explicit MountainGen(Vector3 p, int depth);
	~MountainGen();
	void Init(int seed);
	void Gen();
private:
	//概率生成下一个，如果没生成成功，返回false，类似于执行了一个[]
	bool GenNextByChange(char prev, char& next);
	//概率生成下一个Block
	bool GenNextBlockByChane(char prev, const char* block);
	//生成下一个Block
	bool GenNextBlock(char prev, const char* block);
	//生成岩石
	void GenRock();
	std::vector<L_M_Point> _mother;
	std::vector<L_M_Point> _child;
	std::map<char, const char*> _rules;
	int _genDepth;
	MemoryPoint _M;
};
NS_GNRT_END

#endif
