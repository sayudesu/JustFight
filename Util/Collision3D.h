#pragma once
#include <DxLib.h>

class Collision3D
{
public:
	Collision3D();
	~Collision3D();

	/// <summary>
	/// ‹…‚Æ‹…‚Ì“–‚½‚è”»’è
	/// </summary>
	/// <param name="pos1">ˆÊ’u</param>
	/// <param name="pos2">ˆÊ’u</param>
	/// <param name="radius1">”¼Œa</param>
	/// <param name="radius2">”¼Œa</param>
	/// <param name="relativePos1">pos1‚©‚ç‚Ì‘Š‘ÎˆÊ’u</param>
	/// <param name="relativePos2">pos2‚©‚ç‚Ì‘Š‘ÎˆÊ’u</param>
	/// <returns>“–‚½‚Á‚½‚©‚Ç‚¤‚©</returns>
	bool IsCheckHit(VECTOR pos1, VECTOR pos2,float radius1, float radius2,MATRIX rot1, MATRIX rot2, VECTOR relativePos1, VECTOR relativePos2);
};

