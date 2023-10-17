#pragma once
#include <DxLib.h>

// キャラクター専用基底クラス
class CharacterBase
{
public:
	CharacterBase(VECTOR pos);
	virtual ~CharacterBase();

	virtual void Init();
	virtual void End();
	virtual void Update();
	virtual void Draw();

private:
	virtual void UpdateInput();
	virtual void Attack();
	virtual void Guard();

private:
	// メンバ関数ポインタ
	void(CharacterBase::*m_pFunc)();
	// 位置
	VECTOR m_pos;
	// 移動量
	VECTOR m_vec;
};

