#pragma once
#include <DxLib.h>
#include <deque>

#include "../CharacterBase.h"

#include "../../Util/DifficultyData.h"

class Enemy : public CharacterBase
{
public:
	Enemy(DifficultyData data,VECTOR pos);
	virtual ~Enemy();

	void Input()override;
	void InputTutorial()override;

private:
	// �^�[�Q�b�g�ɋ߂Â����ꍇ���E�ړ�������
	void MoveLeftAndRight(MATRIX mtxRot);
	// �퓬�Ԑ������߂�
	void BattleType();
	// �L�����N�^�[�̌���
	void Direction()override;
private:
	std::deque<float> m_delayFrameAngle;

	// �h��
	bool m_isCheckGuard;
	bool m_isResultGuard;
	// �ʏ�U��
	bool m_isCheckAttack;
	bool m_isAttackResult;
	// 1�t���[�������A���U���ł��Ȃ��l�ɂ���
	int m_attackContinueDelayFrameCount[2];
	int m_isAttackContinueDelay[2];

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

	int m_guardFrameCount = 0;

	// �G��p�̉�]�s��
	MATRIX m_enemyRotMtx;
};

