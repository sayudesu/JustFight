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
	// 入力
	virtual void Input();
	// アイドル状態
	virtual void Idle();
	// 攻撃
	virtual void Attack();
	// 防御
	virtual void Guard();
public:
	// 現在の攻撃フレーム
	int GetAttackFrame();
	// 現在の防御フレーム
	int GetGuardFrame();
	// 現在のジャストガードフレーム
	int GetJustGuardFrame();
	// 最大攻撃フレーム
	int GetAttackFrameMax();
	// 最大防御フレーム
	int GetGuardFrameMax();
	// 最大ジャストガードフレーム
	int GetJustGuardFrameMax();

	// 現在のヒットポイント
	int GetHp();
	// 現在のスタミナ
	int GetStamina();
	
	// 攻撃を受けたかどうか
	void SetDamage(bool isDamage);
	// スタミナの管理
	void SetStamina(int addStamina, int subStamina);
protected:
	// メンバ関数ポインタ
	void(CharacterBase::*m_pFunc)();

	// 3Dモデルハンドル
	int m_lanceHnadle;
	int m_shieldHnadle;

	// 位置
	// 移動量
	VECTOR m_pos;
	VECTOR m_vec;

	// 装備位置
	VECTOR m_posWeapon;
	VECTOR m_posSield;

	// 体力
	// スタミナ
	int m_hp;
	int m_stamina;

	// 今の状態
	bool m_isAttack;
	bool m_isGuard;

	// フレーム関連
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;
};

