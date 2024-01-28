#pragma once
#include <DxLib.h>

#include "../CharacterBase.h"

#include "../../Util/DifficultyData.h"

#include "../../Tips.h"

class Player : public CharacterBase
{
public:
	Player(DifficultyData data,VECTOR pos);
	virtual ~Player();
	
	// ボタン入力
	void Input()override;
	void InputTutorial()override;

private:
	// キャラクターの向き
	void Direction()override;
	void InputMove();
	void InputAttack();
	void InputGuard();

	// 移動用関数
	VECTOR AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	VECTOR SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos);
	// 回避用関数
	void MoveAway(float x, float z, MATRIX rotMtx);
private:
	// メンバ関数ポインタ
	void(Player::* m_pMyFunc)();
	// 回避用のベクトル
	VECTOR m_awayVec;
	// 回避の移動用相対位置
	VECTOR m_awayRelativePos;

	bool m_isUp;
	bool m_isDown;
	bool m_isLeft;
	bool m_isRight;

	// カメラ操作操作の変更
	bool m_isCameraLockon;

	// コントローラー用変数
	DINPUT_JOYSTATE m_padInput;

	// プレイヤーの回転行列
	MATRIX m_platerRotMtx;

	// 動いているかどうか
	bool m_isMove;
};

