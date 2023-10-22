#include "Collision3D.h"

Collision3D::Collision3D()
{
}

Collision3D::~Collision3D()
{
}

bool Collision3D::IsCheckHit(VECTOR pos1, VECTOR pos2, float radius1, float radius2,MATRIX rot1, MATRIX rot2, VECTOR relativePos1, VECTOR relativePos2)
{

	VECTOR TempPos1 = VTransform(relativePos1, rot1);
	TempPos1 = VAdd(pos1, TempPos1);
	DrawSphere3D(TempPos1, radius1, 20, 0x000000, 0x000000, true);

	VECTOR TempPos2 = VTransform(relativePos2, rot2);
	TempPos2 = VAdd(pos2, TempPos2);
	DrawSphere3D(TempPos2, radius2, 20, 0xffffff, 0xffffff, true);

	const VECTOR vec = VSub(TempPos1, TempPos2);
	const float del = VSize(vec);

	if (del < radius1 + radius2)
	{
		return true;
	}

	return false;
}
