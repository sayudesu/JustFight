#include "Collision3D.h"
#include <cmath>

namespace Coll
{
	bool Coll::IsCheckHit(VECTOR pos1, VECTOR pos2, float radius1, float radius2)
	{
		// ベクトルの減算
		const VECTOR vec = VSub(pos1, pos2);
		// ベクトルのサイズ
	//  const float del = sqrt(VSquareSize(vec));
		const float del = VSize(vec);
	//	const float del = VSquareSize(vec);
	
		if (del < radius1 + radius2)
		{
			return true;
		}
#if _DEBUG
		DrawSphere3D(pos1, radius1, 20, 0xffffff, 0xffffff, false);
		DrawSphere3D(pos2, radius2, 20, 0xffffff, 0xffffff, false);
#endif	
		return false;
	}
}
