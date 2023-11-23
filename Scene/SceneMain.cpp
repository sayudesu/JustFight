#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"// カメラ
#include "../Object/Player/Player.h"// プレイヤー
#include "../Object/Enemy/Enemy.h"// エネミー
#include "../Util/Collision3D.h"// 当たり判定
#include "../Util/EffekseerDrawer.h"// 3Dエフェクト
#include "../Util/Game.h"// ゲーム基本設定
#include "../Object/CharacterBase.h"// キャラクター
#include "../Util/Pad.h"// パッド
#include "SceneDebug.h"// デバッグシーン
#include "SceneResult.h"// リザルトシーン
#include "../Util/BloodDrawer.h"// 血のエフェクト
#include "../FIeldDrawer.h"// マップ描画

#include "SceneGameOver.h"// ゲームオーバーシーン
#include "SceneClear.h"// ゲームクリアシーン

#include "../Util/CharacterName.h"// キャラクターの名前

#include "../DEBUG.h"// デバッグ用

SceneMain::SceneMain():
	m_pCamera(nullptr)
{
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)] = nullptr;
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]  = nullptr;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera               = std::make_unique<Camera>();							// カメラクラス
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)] = std::make_shared<Player>(VGet(-300.0f, 260.0f, 0.0f));// キャラクタークラス
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]  = std::make_shared<Enemy>(VGet(300.0f, 260.0f, 0.0f));  // キャラクタークラス
	m_pField                = std::make_unique<FieldDrawer>();			            // フィールド描画クラス

	// 初期化
	m_pCamera->Init();
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Init();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Init();
	m_pField->Init();
}

void SceneMain::End()
{
	// 解放処理
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->End();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->End();
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
	CheckCollMap(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
	CheckCollMap(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
	{
		// プレイヤーの入力情報
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Input();
		// プレイヤー更新処理
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Update();

		//if (m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState() == BattleState::ATTACK ||
		//	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState() == BattleState::ATTACKTWO)
		//{
		//}
		UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::PLAYER)],
			m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		// ターゲットのHPを取得
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetTargetHp(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetHp());
		// ターゲットの位置を受け取る
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetTargetPos(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetPos());
	}
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetBattleState(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState());
	// ジャストガードOFF
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetJustGuard(false);


	{
		// プレイヤーの入力情報
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Input();
		// プレイヤー更新処理
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Update();
		// キャラクター攻撃判定処理
		//if (m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState() == BattleState::ATTACK ||
		//	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState() == BattleState::ATTACKTWO)
		//{
		//}
		UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)],
			m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
		// ターゲットのHPを取得
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetTargetHp(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp());
		// ターゲットの位置を受け取る
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetTargetPos(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetPos());
	}
	// ターゲットの戦闘の状態を受け取る
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetBattleState(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState());
	// ジャストガードOFF
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetJustGuard(false);


	{
#if false
		static int frame = 0;
		frame++;
		printfDx("state = %d frame = %d\n",m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState(), frame);
#endif
	}
	

	// カメラの更新処理
	m_pCamera->Update();

	// 敵の攻撃可能範囲にいるかどうか
	if (CheckModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]))
	{
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetAttackRange(false);
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
	m_pCamera->SetTargetPos(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetPos());
	// カメラにプレイヤーの角度と位置を渡す
	m_pCamera->SetPlayerAngle(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetAngle());

	{
#if true
	//	CheckWeaponAndModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
	//	CheckWeaponAndModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);

	//	CheckWeaponAndShieldHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
	//	CheckWeaponAndShieldHIt(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);

	//	CheckWeaponAndBodyHit(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
	//	CheckWeaponAndBodyHit(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
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
		if (m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp() == 0)
		{
			static int count = 0;
			count++;
			if (count == 60 * 2)
			{
				count = 0;
				clsDx();
				return new SceneGameOver();
			}
		}
		else if(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetHp() == 0)
		{
			static int count = 0;
			count++;
			if (count == 60 * 2)
			{
				count = 0;
				clsDx();
				return new SceneClear();
			}
		}
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

bool SceneMain::CheckWeaponAndBodyHit(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetCollWeaponPos(), character2->GetCollPos(),
		character1->GetWeaponAttackRadius(), character2->GetWeaponAttackRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndShieldHIt(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetCollWeaponPos(), character2->GetShieldPos(),
		character1->GetWeaponAttackRadius(), character2->GetShieldRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndModelAboutHIt(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetCollWeaponPos(), character2->GetCollPos(),
		character1->GetWeaponAttackRadius(), character2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckModelAboutHIt(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetPos(), character2->GetCollPos(),
		character1->GetModelRadius(), character2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

void SceneMain::CheckCollMap(std::shared_ptr<CharacterBase> character)
{
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;

	// モデルとカプセルとの当たり判定
	HitPolyDim = MV1CollCheck_Capsule(
		m_pField->GetHandle(),
		-1,
		character->GetCapsulePosDown(),
		character->GetCapsulePosUp(),
		character->GetCapsuleRadius());

	// 当たったかどうかで処理を分岐
	if (HitPolyDim.HitNum >= 1)
	{
		character->SetFieldHit();
	}

	//// 当たり判定情報の後始末
	//MV1CollResultPolyDimTerminate(HitPolyDim);

	//MV1_COLL_RESULT_POLY_DIM HitPolyDim;

	// モデルとカプセルとの当たり判定
	HitPolyDim = MV1CollCheck_Capsule(
		m_pField->GetHandle2(),
		-1,
		character->GetCapsulePosDown(),
		character->GetCapsulePosUp(),
		character->GetCapsuleRadius());

	// 当たったかどうかで処理を分岐
	if (HitPolyDim.HitNum >= 1)
	{
		character->SetFieldHit();
	}

	// 当たり判定情報の後始末
	MV1CollResultPolyDimTerminate(HitPolyDim);
}

// 1が攻撃をする側
// 2が攻撃を受ける側
void SceneMain::UpdateCharacter(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	// 回転角度を取得
	character2->SetTargetMtxRota(character1->GetRot());

#if true
	// 攻撃フレームが最大数かどうか
	if (character2->GetAttackFrame() == character2->GetAttackFrameMax() - 1)
	{
	
	}
	//// ジャストガード
	//if (character1->GetJustGuardFrame() > 0 &&
	//	character1->GetJustGuardFrame() < character1->GetJustGuardFrameMax())
	//{
	//	// ジャストガードが成功しているので盾に当たっていても体に当たっていても
	//	// 無敵判定になる
	//	if ((CheckWeaponAndShieldHIt(character1, character2)) || (CheckWeaponAndBodyHit(character1, character2)))
	//	{
	//		// ジャストガードが成功したかどうか
	//		character1->SetJustGuard(true);

	//		// ジャストガードされた側は
	//		// 戦いに必要な特殊なメーターを減らす
	//		character2->SetFightingMeter(-8.0f);
	//		// ジャストガードに成功した側は
	//		// 戦いに必要な特殊なメーターを増やす
	//		character1->SetFightingMeter(8.0f);

	//		character1->SetCollJustGuardEffect();

	//		// 振動開始
	//		StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);
	//	}
	//}  


	// 通常ガードが出来るかどうか
	if (character1->GetGuardFrame() == character1->GetGuardFrameMax())
	{
		// 攻撃状態だったら
		//攻撃が盾に当たったかどうか
		if (CheckWeaponAndShieldHIt(character2, character1))
		{
			// ガード成功したかどうか
			character1->SetGuardKnockBack(true, -20);

			// 戦いに必要な特殊なメーターを減らす
			character1->SetFightingMeter(-0.1f);

			character1->SetCollGuardEffect();

			character2->SetWeaponAttacksShield(true);
		}
		return;
	}
	else if (character1->GetBattleState() == BattleState::ATTACK || character1->GetBattleState() == BattleState::ATTACKTWO)
	{
		// 攻撃が当たったかどうか
		if (CheckWeaponAndBodyHit(character1, character2))
		{
			// ダメージを与える
			character2->SetDamage(true);

			// 戦いに必要な特殊なメーターを減らす
			character1->SetFightingMeter(-0.1f);

			for (int i = 0; i < 10; i++)
			{
				m_pBlood.push_back(new BloodDrawer(VGet(character2->GetPos().x, character2->GetPos().y + 100.0f, character2->GetPos().z)));
				m_pBlood.back()->Init(i);
			}

			// 振動開始
			StartJoypadVibration(DX_INPUT_PAD1, 1000 / 3, 1000 / 2, -1);
		}
		return;
	}
#endif
}

