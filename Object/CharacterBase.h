#pragma once
#include <DxLib.h>
#include "../Util/CharacterName.h"
#include "../Util/AttackData.h"
#include "../Util/BattleState.h"

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
	virtual void Input() = 0;
protected:
	// アイドル状態
	virtual void Idle();
	// 攻撃
	virtual void Attack();
	// 強攻撃
	virtual void StrongAttack();
	// 防御
	virtual void Guard();
	// ジャストガード
	virtual void JustGuard();
	// ジャストガードをされた場合
	virtual void Stun();
	// 勝利した場合
	virtual void Winner();
	// 敗北した場合
	virtual void Losers();
private:
	// 位置情報の更新
	virtual void UpdatePos(int shiftX = 0, int shiftY = 0, int shiftZ = 0);
protected:
	// 角度を取得
	virtual void SetAngle(float angle);
	virtual void SetRotMtx(MATRIX rotMtx);

	// ターゲット位置追跡移動
	virtual void TargetMove();
public:
	// 自身が誰をを返す
	CharacterName GetMyId();
	AttackData GetMyAttackId();

	// 現在の行動の情報を渡す
	BattleState GetBattleState();

	// 位置
	VECTOR GetPos      ()const;// 自身
	VECTOR GetWeaponPos()const;// 武器
	VECTOR GetSieldPos ()const;// 盾

	// 角度
	MATRIX GetRot ()const;// 行列
	float GetAngle()const;// ラジアン

	// モデルの当たり判定用半径
	float GetModelRadius()const;
	
	// 装備の判定用半径
	float GetWeaponAttackRadius()const;
	float GetSieldRadius()const;

	// 装備の相対位置
	VECTOR GetWeaponAttackRelative()const;
	VECTOR GetSieldRelative()const;

	// フレーム関係
	int GetAttackFrame      ()const;// 現在の攻撃フレーム
	int GetGuardFrame       ()const;// 現在の防御フレーム
	int GetJustGuardFrame   ()const;// 現在のジャストガードフレーム
	int GetAttackFrameMax   ()const;// 最大攻撃フレーム
	int GetGuardFrameMax    ()const;// 最大防御フレーム
	int GetJustGuardFrameMax()const;// 最大ジャストガードフレーム
	int GetStunFrameMax     ()const;// 最大スタンのフレーム数

	// ゲージ関係
	int GetHp             ()const;// 現在のヒットポイント
	float GetFightingMeter()const;// 戦闘に必要なメーター
	
	// ジャストガードできたかどうか
	bool IsJustGuard()const;

	// スタン状態かどうか
	bool IsStun()const;

	// 攻撃範囲に入っているかどうか
	bool IsAttackRange()const;

	// 現在の行動の情報を渡す
	void SetBattleState(BattleState state);

	// 相手の角度
	void SetTargetRota(const MATRIX rot);

	// 攻撃範囲に入っているかどうか
	void SetAttackRange(const bool isRange);

	// 攻撃を受けたかどうか
	void SetDamage(const bool isDamage);
	// ガードが成功しかたどうか
	void SetGuard(const bool isGuard);
	// ジャストガードが成功したかどうか
	void SetJustGuard(const bool isJustGuard);
	// スタンをするかどうか
	void SetStun(const bool isStun);

	// 戦いに必要な特殊なメーターを変更する
	void SetFightingMeter(const float fightingMeter);

	// 戦いに必要な特殊なメーターを管理する
	void FightingMeter();

	// ターゲットの位置を取得する
	void SetTargetPos(const VECTOR pos);
private:
	// 3Dモデルハンドル
	int m_weaponHandle;
	int m_shieldHandle;

	// 角度
	MATRIX m_rotMtx;
	MATRIX m_enemyRotMtx;

	// 装備
	VECTOR m_vecWeapon;
	VECTOR m_vecSield;

	// 一時的な武器の位置
	VECTOR m_tempWeaponPos;

	// 体力
	// 戦いに必要な特殊なメーター
	int m_hp;
	float m_tempFightingMeter;
	float m_fightingMeter;

	// フレーム関連
	int m_attackGapFrame;
	int m_attackFrame;
	int m_guardFrame;
	int m_justGuardFrame;
	int m_stunFrame;

	// 敵からの攻撃をガードできたかどうか
	bool m_isResultGuard;
	bool m_isResultDamage;

	// スタン状態かどうか
	bool m_isStun;

	// 攻撃範囲にいるかどうか
	bool m_isAttackRange;

	// 現在の行動を記録
	BattleState m_battleState;
protected:
	// メンバ関数ポインタ
	void(CharacterBase::*m_pFunc)();

	// 位置
	VECTOR m_pos;
	VECTOR m_vec;
	VECTOR m_targetPos;

	// 角度
	float m_angle;

	// 状態
	bool m_isAttack;
	bool m_isStrongAttack;
	bool m_isGuard;
	bool m_isJustGuard;

	// 回避
	bool m_isAway;

	// ターゲットの距離
	VECTOR m_targetRange;

	CharacterName m_myId;
	AttackData m_attackId;

	// ターゲットの行動を記録
	BattleState m_targetBattleState;
};

