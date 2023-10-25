#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>

class Player : public CharacterBase
{
public:
	Player(VECTOR pos);
	virtual ~Player();
	
	// �{�^������
	void Input()override;
private:
	// �ړ��p�֐�
	VECTOR AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	VECTOR SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	// ���p�֐�
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

