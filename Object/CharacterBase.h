#pragma once
#include <DxLib.h>
#include "../Util/CharacterName.h"
#include "../Util/BattleState.h"
#include "../Util/CharacterParameter.h"

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
	virtual void AttackTwo();
	// 強攻撃
	virtual void StrongAttack();
	// 防御
	virtual void Guard();
private:
	// 位置情報の更新
	void UpdatePos(int shiftX = 0, int shiftY = 0, int shiftZ = 0);
	// 盾に攻撃が当たって弾く場合
	void WeaponAttacksShield();
	// フレームで向かいたい座標まで動かす
	float MoveByFrame(const float relativePos, const float EndPos, const int nowFrame, const int maxFrame);

	// ジャストガード
	virtual void JustGuard();
	// ジャストガードをされた場合
	virtual void Stun();
	// 勝利した場合
	virtual void Winner();
	// 敗北した場合
	virtual void Losers();
protected:
	// 角度を取得
	virtual void SetAngle(float angle);
	virtual void SetRotMtx(MATRIX rotMtx);

	// ターゲット位置追跡移動
	virtual void TargetMove();
public:
	// 自身が誰をを返す
	CharacterName GetMyId();

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

	// 攻撃が盾に当たったかどうか
	void SetWeaponAttacksShield(const bool isShieldHit);

	// 現在の行動の情報を渡す
	void SetBattleState(BattleState state);

	// 相手の角度
	void SetTargetMtxRota(const MATRIX rot);
	void SetTargetRota(const float rot);

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

	// ターゲットの体力を取得
	void SetTargetHp(const int hp);

	// エフェクトを呼び出す
	void SetCollGuardEffect    ();// 通常防御
	void SetCollJustGuardEffect();// ジャストガード
private:
	// 3Dモデルハンドル
	int m_weaponHandle;
	int m_shieldHandle;

	// エフェクトハンドル
	int m_effectHandle;

	// 角度
	MATRIX m_rotMtx;
	MATRIX m_targetRotMtx;

	float m_targetAngle;

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
	int m_attackGapFrame;// 攻撃時の遊びのフレーム
	int m_attackFrame;   // 遊びから攻撃のフレーム
	int m_guardFrame;    // 防御するまでのフレーム
	int m_justGuardFrame;// ジャストガードのフレーム
	int m_stunFrame;     // スタン状態のフレーム
	int m_recoilFrame;   // 攻撃を弾かれば場合のフレーム

	// 敵からの攻撃をガードできたかどうか
	bool m_isResultGuard;
	bool m_isResultDamage;

	// 盾を攻撃したかどうか
	bool m_isWeaponAttacksShield;

	// スタン状態かどうか
	bool m_isStun;

	// 攻撃範囲にいるかどうか
	bool m_isAttackRange;

	// ターゲットの体力
	int m_targetHp;

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

	// ターゲットの行動を記録
	BattleState m_targetBattleState;

	// キャラクター全てのパラメーター
	CharacterParameter m_parameter;

	// コンボ技用
	int m_comboAttack;


	// ここは使わない

	float test1 = 0.0f;
	float test2 = 0.0f;
	float test3 = 0.0f;

	VECTOR testV1 = VGet(0, 0, 0);
	VECTOR testV2 = VGet(0, 0, 0);
	VECTOR testV3 = VGet(0, 0, 0);
};

