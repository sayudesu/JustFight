#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>
#include <deque>

class Enemy : public CharacterBase
{
public:
	Enemy(VECTOR pos);
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


	//bool m_isUp;
	//bool m_isDown;
	//bool m_isLeft;
	//bool m_isRight;

	//// �J�������쑀��̕ύX
	//bool m_isCameraLockon;
};

