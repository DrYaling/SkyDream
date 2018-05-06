#ifndef _UNIVERSE_DEF_H
#define _UNIVERSE_DEF_H
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
typedef struct Vector3
{
public:
	int32_t x, y, z;
	Vector3() : x(0), y(0), z(0) {  }
	bool operator == (const Vector3& right)
	{
		return x == right.x && y == right.y&& z == right.z;
	}
}*pVector3;
typedef struct Vector4
{
public:
	int32_t x, y, z, w;
}*pVector4;
typedef struct Matrix4X4
{
private:
	int32_t matrix[4][4];
}*pMatrix4X4;
typedef uint32_t planetGravity;
#endif
