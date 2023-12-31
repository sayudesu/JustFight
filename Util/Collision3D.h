#pragma once
#include <DxLib.h>

namespace Coll
{
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="pos1">位置</param>
	/// <param name="pos2">位置</param>
	/// <param name="radius1">半径</param>
	/// <param name="radius2">半径</param>
	/// <returns>当たったかどうか</returns>
	bool IsCheckHit(VECTOR pos1, VECTOR pos2,float radius1, float radius2);
};

