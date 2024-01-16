#pragma once
#include <DxLib.h>
#include <deque>

#include "../CharacterBase.h"

#include "../../DifficultyData.h"

class Enemy : public CharacterBase
{
public:
	Enemy(DifficultyData data,VECTOR pos);
	virtual ~Enemy();
	void Input()override;
private:
	// �^�[�Q�b�g�ɋ߂Â����ꍇ���E�ړ�������
	void MoveLeftAndRight(MATRIX mtxRot);
	// �퓬�Ԑ������߂�
	void BattleType();

private:
	//// �ړ��p�֐�
	//VECTOR AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	//VECTOR SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	//// ���p�֐�
	//void MoveAway(float x, float z, MATRIX rotMtx);

private:
	std::deque<float> m_delayFrameAngle;

	// �h��
	bool m_isCheckGuard;
	bool m_isResultGuard;
	// �ʏ�U��
	bool m_isCheckAttack;
	bool m_isAttackResult;
	// 1�t���[�������A���U���ł��Ȃ��l�ɂ���
	int m_attackContinueDelayOneFrameCount;

	// ���U��
	bool m_isCheckStrongAttack;
	bool m_isStrongAttackResult;

	// ���E�ړ��p
	bool m_isMoveLeft;
	bool m_isMoveRight;

	// ���p�̃x�N�g��
	VECTOR m_awayVec;
	// ����̈ړ��p���Έʒu
	VECTOR m_awayRelativePos;
};

