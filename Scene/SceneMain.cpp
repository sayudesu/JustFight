#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"
#include "../Util/Collision3D.h"
#include "../Util/EffekseerDrawer.h"
#include "../Util/Game.h"
#include "../DEBUG.h"

#include "../Object/CharacterBase.h"

#include "../Util/Pad.h"
#include "SceneDebug.h"
#include "SceneResult.h"
#include "../Util/BloodDrawer.h"

#include "../FIeldDrawer.h"

#include "../Util/AttackData.h"

#include "../Util/EffectId.h"

namespace
{
	// キャラクターベースの配列番号
	constexpr int kPlayerNo = 0;
	constexpr int kEnemyNo = 1;
}

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pColl(nullptr)
{
	m_pCharacter[kPlayerNo] = nullptr;
	m_pCharacter[kEnemyNo] = nullptr;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera               = std::make_unique<Camera>();							// カメラクラス
	m_pCharacter[kPlayerNo] = std::make_unique<Player>(VGet(-300.0f, 260.0f, 0.0f));// キャラクタークラス
	m_pCharacter[kEnemyNo]  = std::make_unique<Enemy>(VGet(300.0f, 260.0f, 0.0f));  // キャラクタークラス
	m_pColl                 = std::make_unique<Collision3D>();						// 当たり判定クラス
	m_pField                = std::make_unique<FIeldDrawer>();			            // フィールド描画クラス

	// 初期化
	m_pCamera->Init();
	m_pCharacter[kPlayerNo]->Init();
	m_pCharacter[kEnemyNo]->Init();
	m_pField->Init();
}

void SceneMain::End()
{
	// 解放処理
	m_pCharacter[kPlayerNo]->End();
	m_pCharacter[kEnemyNo]->End();
	m_pField->End();

	for (int i = 0; i < m_pBlood.size(); i++)
	{
		// デリート処理
		delete m_pBlood[i];
		m_pBlood[i] = nullptr;
	}
}

SceneBase* SceneMain::Update()
{
	// キャラクター更新処理
	m_pCharacter[kPlayerNo]->Input();
	m_pCharacter[kPlayerNo]->Update();

	// キャラクターの操作処理
	m_pCharacter[kEnemyNo]->Input();
	m_pCharacter[kEnemyNo]->Update();

	// カメラの更新処理
	m_pCamera->Update();

	// 敵の攻撃可能範囲にいるかどうか
	if (CheckModelAboutHIt(m_pCharacter[kPlayerNo].get(), m_pCharacter[1].get()))
	{
		m_pCharacter[kEnemyNo]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[kEnemyNo]->SetAttackRange(false);
	}

	{
		// 血のエフェクトを更新
		for (auto& blood : m_pBlood)
		{
			blood->Update();
		}
		for (int i = 0; i < m_pBlood.size(); i++)
		{
			if (m_pBlood[i]->IsGetErase())
			{
				// デリート処理
				delete m_pBlood[i];
				m_pBlood[i] = nullptr;
				// 要素の削除
				m_pBlood.erase(m_pBlood.begin() + i);
			}
		}
	}

	// カメラにプレイヤーとエネミーの位置を渡す
	m_pCamera->SetTargetPos(m_pCharacter[kPlayerNo]->GetPos());
	// カメラにプレイヤーの角度と位置を渡す
	m_pCamera->SetPlayerAngle(m_pCharacter[kPlayerNo]->GetAngle());

	// ターゲットの戦闘の状態を受け取る
	m_pCharacter[kPlayerNo]->SetBattleState(m_pCharacter[kEnemyNo ]->GetBattleState());
	m_pCharacter[kEnemyNo ]->SetBattleState(m_pCharacter[kPlayerNo]->GetBattleState());

	// ターゲットの位置を受け取る
	m_pCharacter[kPlayerNo]->SetTargetPos(m_pCharacter[kEnemyNo]->GetPos());
	m_pCharacter[kEnemyNo]->SetTargetPos(m_pCharacter[kPlayerNo]->GetPos());

	// キャラクター攻撃判定処理
	UpdateCharacter(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
	UpdateCharacter(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());


	{
#if _DEBUG
		CheckWeaponAndModelAboutHIt(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
		CheckWeaponAndModelAboutHIt(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());

		CheckWeaponAndSieldHIt(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
		CheckWeaponAndSieldHIt(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());

		CheckWeaponAndBodyHit(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
		CheckWeaponAndBodyHit(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());
#endif
	}
	
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			clsDx();
			return new SceneDebug();
		}
	}
	
	{
		//if (m_pCharacter[kPlayerNo]->GetHp() == 0 || m_pCharacter[kEnemyNo]->GetHp() == 0)
		//{
		//	static int count = 0;
		//	count++;
		//	if (count == 60 * 2)
		//	{
		//		count = 0;
		//		clsDx();
		//		return new SceneResult();
		//	}
		//}
	}

	return this;
}

void SceneMain::Draw()
{
	m_pField->Draw();

	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}
	
	for (auto& blood : m_pBlood)
	{
		blood->Draw();
	}

	DEBUG::Field();
#if _DEBUG
	DEBUG::FrameMeter("P体力", 100, 50, m_pCharacter[0]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E体力", 100, 100, m_pCharacter[1]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("Pスタミナ", 100, 150, m_pCharacter[0]->GetFightingMeter(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("Eスタミナ", 100, 200, m_pCharacter[1]->GetFightingMeter(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P攻撃フレーム", 100, 250, m_pCharacter[0]->GetAttackFrameMax(), m_pCharacter[0]->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P防御フレーム", 100, 300, m_pCharacter[0]->GetGuardFrameMax(), m_pCharacter[0]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pCharacter[0]->GetJustGuardFrameMax(), m_pCharacter[0]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E攻撃フレーム", 100, 400, m_pCharacter[1]->GetAttackFrameMax(), m_pCharacter[1]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E防御フレーム", 100, 450, m_pCharacter[1]->GetGuardFrameMax(), m_pCharacter[1]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pCharacter[1]->GetJustGuardFrameMax(), m_pCharacter[1]->GetJustGuardFrame(), 30, 0xffffff);

	
	DrawString(0, 0, "SceneMain", 0xffffff);
#endif

}

bool SceneMain::CheckWeaponAndBodyHit(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetWeaponAttackRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndSieldHIt(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetSieldPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetSieldRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 0.0f, -50.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetModelRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetPos(), chara2->GetPos(),
		chara1->GetModelRadius(), chara2->GetModelRadius(),
		chara1->GetRot(), chara2->GetRot(),
		VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

void SceneMain::UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2)
{
	// ジャストガードが成功したかどうか
	chara1->SetJustGuard(false);

	// 回転角度を取得
	chara1->SetTargetRota(chara2->GetRot());

	// 攻撃フレームが最大数かどうか
	if (chara2->GetAttackFrame() == chara2->GetAttackFrameMax() - 1)
	{
		// ジャストガード
		if (chara1->GetJustGuardFrame() > 0 &&
			chara1->GetJustGuardFrame() < chara1->GetJustGuardFrameMax())
		{
			// ジャストガードが成功しているので盾に当たっていても体に当たっていても
			// 無敵判定になる
			if ((CheckWeaponAndSieldHIt(chara1, chara2)) || (CheckWeaponAndBodyHit(chara1, chara2)))
			{
				// ジャストガードが成功したかどうか
				chara1->SetJustGuard(true);

				// ジャストガードした際のエフェクト再生
				EffekseerDrawer::GetInstance().Play(
					handle, Id::JustGuard,
					EffectPlayType::NORMAL,
					VGet(chara1->GetPos().x, chara1->GetPos().y + 100.0f, chara1->GetPos().z),
					VGet(0, 0, 0),
					VGet(0, 0, 0));

				// ジャストガードされた側は
				// 戦いに必要な特殊なメーターを減らす
				chara2->SetFightingMeter(-10.0f);

				// ジャストガードに成功した側は
				// 戦いに必要な特殊なメーターを増やす
				chara1->SetFightingMeter(30.0f);
	
				// 振動開始
				StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);
			}
		}
		// 通常ガードが出来るかどうか
		else if (chara1->GetGuardFrame() == chara1->GetGuardFrameMax())
		{
			//攻撃が盾に当たったかどうか
			if (CheckWeaponAndSieldHIt(chara1, chara2))
			{
				// ガード成功したかどうか
				chara1->SetGuard(true);
				// 強い攻撃が来た場合
				if (chara2->GetMyAttackId() == AttackData::STRONG)
				{
					// ターゲットをスタン状態にする
					chara1->SetStun(true);
				}

				// ガード際のエフェクト再生
				EffekseerDrawer::GetInstance().Play(
					handle, Id::Guard,
					EffectPlayType::NORMAL,
					chara1->GetSieldPos(),
					VGet(0, 0, 0),
					VGet(0, chara1->GetAngle(), 0));

				// 戦いに必要な特殊なメーターを減らす
				chara1->SetFightingMeter(-10.0f);

			}
		}
		else
		{
			// 攻撃が当たったかどうか
			if (CheckWeaponAndBodyHit(chara1, chara2))
			{
				// ダメージを与える
				chara1->SetDamage(true);

				// 戦いに必要な特殊なメーターを減らす
				chara1->SetFightingMeter(-10.0f);

				for (int i = 0; i < 30; i++)
				{
					m_pBlood.push_back(new BloodDrawer(VGet(chara1->GetPos().x, chara1->GetPos().y + 100.0f, chara1->GetPos().z)));
					m_pBlood.back()->Init(i);
				}
				// 振動開始
				StartJoypadVibration(DX_INPUT_PAD1, 1000/3, 1000/2, -1);
			}
		}
	}
}

