#pragma once
#include <DxLib.h>
const struct CharacterParameter
{
	int attackFrameMax;   // 攻撃最大フレーム
	int attackFrameGapMax;// 攻撃予兆の最大フレーム
	int attackRotalFrame; // 攻撃フレームの合計

	int strongAttackFrameMax;   // 強攻撃最大フレーム
	int strongAttackFrameGapMax;// 強攻撃予兆の最大フレーム
	int strongAttackRotalFrame; // 強攻撃フレームの合計

	int guardFrameMax;    // ガード最大フレーム
	int justGuardFrameMax;// ジャストガード最大フレーム

	int stunFrameMax;// スタン状態の最大フレーム

	int hpMax;           // 最大体力
	int fightingMeterMax;// 最大戦闘に必要なメーターの最大値

	VECTOR weaponRelativePos;// 武器
	VECTOR sieldRelativePos; // 盾

	float weaponAttackRadius;// 武器の半径
	float sieldRadius;       // 盾の半径
	float modelRadius;       // モデルの当たり判定用範囲

	VECTOR weaponAttackPos;// 武器の攻撃時の判定の相対位置
	VECTOR knockBackPos;  // ガードしている時に攻撃を受けた場合のノックバックで移動する相対位置

	float weaponBackSpeed;// 剣が元の位置に戻る速度
	float sieldBackSpeed; // 盾が元の位置に戻る速度
}; 