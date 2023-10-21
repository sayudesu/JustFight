#pragma once
#include <DxLib.h>
#include <array>

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
protected:
	// アイドル状態
	virtual void Idle();
	// 攻撃
	virtual void Attack();
	// 防御
	virtual void Guard();
	// ジャストガード
	virtual void JustGuard();
	// ジャストガードをされた場合
	virtual void JustGuardBreak();
protected:
	virtual void SetAngle(float angle);
	virtual void SetRotMtx(MATRIX rotMtx);
public:
	// 位置
	VECTOR GetPos()const;

	// 現在の攻撃フレーム
	int GetAttackFrame()const;
	// 現在の防御フレーム
	int GetGuardFrame()const;
	// 現在のジャストガードフレーム
	int GetJustGuardFrame()const;
	// 最大攻撃フレーム
	int GetAttackFrameMax()const;
	// 最大防御フレーム
	int GetGuardFrameMax()const;
	// 最大ジャストガードフレーム
	int GetJustGuardFrameMax()const;

	// 現在のヒットポイント
	int GetHp()const;
	// 現在のスタミナ
	int GetStamina()const;
	
	// ジャストガードされたかどうか
	bool IsJustGuard()const;

	// 攻撃を受けたかどうか
	void SetDamage(bool isDamage);
	void SetJustGuard(bool isJustGuard);
	void SetJustGuardBreak(bool isJustGuardBreak);

	// スタミナの管理
	void SetStamina(float addStamina, float subStamina);
private:
	// 3Dモデルハンドル
	int m_lanceHnadle;
	int m_shieldHnadle;

	// 移動量
	// 角度
	VECTOR m_vec;
	MATRIX m_rotMtx;

	// 装備位置
	VECTOR m_vecWeapon;
	VECTOR m_vecSield;

	// 体力
	// スタミナ
	int m_hp;
	float m_stamina;

	// フレーム関連
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;
	int m_justGuardBreakFrame;

protected:
	// メンバ関数ポインタ
	void(CharacterBase::*m_pFunc)();

	// 位置
	VECTOR m_pos;
	VECTOR m_targetPos;

	// 角度
	float m_angle;

	// 今の状態
	bool m_isAttack;
	bool m_isGuard;
	bool m_isJustGuard;
	bool m_isJustGuardBreak;
};

