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
	// ターゲットに近づいた場合左右移動をする
	void MoveLeftAndRight(MATRIX mtxRot);
	// 戦闘態勢を決める
	void BattleType();

private:
	//// 移動用関数
	//VECTOR AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	//VECTOR SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	//// 回避用関数
	//void MoveAway(float x, float z, MATRIX rotMtx);

private:
	std::deque<float> m_delayFrameAngle;

	// 防御
	bool m_isCheckGuard;
	bool m_isResultGuard;
	// 通常攻撃
	bool m_isCheckAttack;
	bool m_isAttackResult;
	// 1フレームだけ連続攻撃できない様にする
	int m_attackContinueDelayOneFrameCount;

	// 強攻撃
	bool m_isCheckStrongAttack;
	bool m_isStrongAttackResult;

	// 左右移動用
	bool m_isMoveLeft;
	bool m_isMoveRight;

	// 回避用のベクトル
	VECTOR m_awayVec;
	// 回避の移動用相対位置
	VECTOR m_awayRelativePos;
};

