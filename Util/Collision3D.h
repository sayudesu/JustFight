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
	/// <param name="relativePos1">pos1からの相対位置</param>
	/// <param name="relativePos2">pos2からの相対位置</param>
	/// <returns>当たったかどうか</returns>
	bool IsCheckHit(VECTOR pos1, VECTOR pos2,float radius1, float radius2,MATRIX rot1, MATRIX rot2, VECTOR relativePos1, VECTOR relativePos2);
};

