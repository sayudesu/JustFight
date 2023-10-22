#pragma once
#include <DxLib.h>

class Collision3D
{
public:
	Collision3D();
	~Collision3D();

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="pos1">�ʒu</param>
	/// <param name="pos2">�ʒu</param>
	/// <param name="radius1">���a</param>
	/// <param name="radius2">���a</param>
	/// <param name="relativePos1">pos1����̑��Έʒu</param>
	/// <param name="relativePos2">pos2����̑��Έʒu</param>
	/// <returns>�����������ǂ���</returns>
	bool IsCheckHit(VECTOR pos1, VECTOR pos2,float radius1, float radius2,MATRIX rot1, MATRIX rot2, VECTOR relativePos1, VECTOR relativePos2);
};

