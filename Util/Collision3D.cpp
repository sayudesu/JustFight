#include "Collision3D.h"
#include <cmath>

Collision3D::Collision3D()
{
}

Collision3D::~Collision3D()
{
}

bool Collision3D::IsCheckHit(VECTOR pos1, VECTOR pos2, float radius1, float radius2,MATRIX rot1, MATRIX rot2, VECTOR relativePos1, VECTOR relativePos2)
{
	// 行列を使った座標変換
	VECTOR TempPos1 = VTransform(relativePos1, rot1);
	TempPos1 = VAdd(pos1, TempPos1);

	// 行列を使った座標変換
	VECTOR TempPos2 = VTransform(relativePos2, rot2);
	TempPos2 = VAdd(pos2, TempPos2);

	// ベクトルの減算
	const VECTOR vec = VSub(TempPos1, TempPos2);
	// ベクトルのサイズ
//  const float del = sqrt(VSquareSize(vec));
	const float del = VSize(vec);
//	const float del = VSquareSize(vec);

	if (del < radius1 + radius2)
	{
		return true;
	}
#if _DEBUG
	//DrawSphere3D(TempPos1, radius1, 20, 0x000000, 0x000000, false);
	//DrawSphere3D(TempPos2, radius2, 20, 0xffffff, 0x000000, false);
#endif	
	return false;
}
