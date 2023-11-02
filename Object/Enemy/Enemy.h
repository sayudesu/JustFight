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
	// ターゲットに近づいた場合左右移動をする
	void MoveLeftAndRight(MATRIX mtxRot);
	// 戦闘態勢を決める
	void BattleType();
private:
	std::deque<float> m_delayFrameAngle;

	// 防御
	bool m_isCheckGuard;
	bool m_isResultGuard;
	// 通常攻撃
	bool m_isCheckAttack;
	bool m_isAttackResult;
	// 強攻撃
	bool m_isCheckStrongAttack;
	bool m_isStrongAttackResult;

	// 左右移動用
	bool m_isMoveLeft;
	bool m_isMoveRight;
};

