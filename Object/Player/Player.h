#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>

class Player : public CharacterBase
{
public:
	Player(VECTOR pos);
	virtual ~Player();
	
	// ボタン入力
	void Input()override;
private:
	// 移動用関数
	VECTOR AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	VECTOR SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	// 回避用関数
	void MoveAway(float x, float z, MATRIX rotMtx);
private:
	VECTOR m_awayVec;
	VECTOR m_awayRelativePos;
	bool m_isAway;

	bool m_isUp;
	bool m_isDown;
	bool m_isLeft;
	bool m_isRight;

};

