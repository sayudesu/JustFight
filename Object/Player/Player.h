#pragma once
#include <DxLib.h>

#include "../CharacterBase.h"

#include "../../DifficultyData.h"

class Player : public CharacterBase
{
public:
	Player(DifficultyData data,VECTOR pos);
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
	// ���p�̃x�N�g��
	VECTOR m_awayVec;
	// ����̈ړ��p���Έʒu
	VECTOR m_awayRelativePos;

	bool m_isUp;
	bool m_isDown;
	bool m_isLeft;
	bool m_isRight;

	// �J�������쑀��̕ύX
	bool m_isCameraLockon;
};

