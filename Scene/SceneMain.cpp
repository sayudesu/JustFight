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
	m_pField                = std::make_unique<FIeldDrawer>();			            // フィールド描画クラス

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
	{
		// プレイヤーの入力情報
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Input();
		// プレイヤー更新処理
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Update();

		// キャラクター攻撃判定処理
		// プレイヤーが攻撃した場合
		if (m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState() == BattleState::ATTACK ||
			m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState() == BattleState::ATTACKTWO)
		{
			UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::PLAYER)],
				m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		}
		// ターゲットのHPを取得
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetTargetHp(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetHp());
	}
	{
		// プレイヤーの入力情報
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Input();
		// プレイヤー更新処理
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Update();
		// キャラクター攻撃判定処理
	// プレイヤーが攻撃した場合
		if (m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState() == BattleState::ATTACK ||
			m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState() == BattleState::ATTACKTWO)
		{
			UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)],
				m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
		}
		// ターゲットのHPを取得
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetTargetHp(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp());
	}

	{
		static int frame = 0;
		frame++;
		printfDx("state = %d frame = %d\n",m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState(), frame);
	}
	
	// ターゲットの戦闘の状態を受け取る
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetBattleState(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState());
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetBattleState(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState());

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
		CheckWeaponAndModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		CheckWeaponAndModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);

		CheckWeaponAndShieldHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		CheckWeaponAndShieldHIt(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);

		CheckWeaponAndBodyHit(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		CheckWeaponAndBodyHit(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
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

bool SceneMain::CheckWeaponAndBodyHit(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetCollWeaponPos(), chara2->GetCollPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetWeaponAttackRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndShieldHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetCollWeaponPos(), chara2->GetShieldPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetShieldRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetCollWeaponPos(), chara2->GetCollPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetPos(), chara2->GetCollPos(),
		chara1->GetModelRadius(), chara2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

// 1が攻撃をする側
// 2が攻撃を受ける側
void SceneMain::UpdateCharacter(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	// ジャストガードが成功したかどうか
	chara2->SetJustGuard(false);

	// 回転角度を取得
	chara2->SetTargetMtxRota(chara1->GetRot());

	// ターゲットの回転行列を受け取る
	chara2->SetTargetMtxRota(chara1->GetRot());


	// ターゲットの角度を受け取る
	chara2->SetTargetRota(chara1->GetAngle());

	// ターゲットの位置を受け取る
	chara2->SetTargetPos(chara1->GetPos());
#if true
	// 攻撃フレームが最大数かどうか
	if (chara2->GetAttackFrame() == chara2->GetAttackFrameMax() - 1)
	{
	
	}
	//// ジャストガード
	//if (chara1->GetJustGuardFrame() > 0 &&
	//	chara1->GetJustGuardFrame() < chara1->GetJustGuardFrameMax())
	//{
	//	// ジャストガードが成功しているので盾に当たっていても体に当たっていても
	//	// 無敵判定になる
	//	if ((CheckWeaponAndShieldHIt(chara1, chara2)) || (CheckWeaponAndBodyHit(chara1, chara2)))
	//	{
	//		// ジャストガードが成功したかどうか
	//		chara1->SetJustGuard(true);

	//		// ジャストガードされた側は
	//		// 戦いに必要な特殊なメーターを減らす
	//		chara2->SetFightingMeter(-8.0f);
	//		// ジャストガードに成功した側は
	//		// 戦いに必要な特殊なメーターを増やす
	//		chara1->SetFightingMeter(8.0f);

	//		chara1->SetCollJustGuardEffect();

	//		// 振動開始
	//		StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);
	//	}
	//}  

	// 攻撃状態だったら

	// 通常ガードが出来るかどうか
	if (chara1->GetGuardFrame() == chara1->GetGuardFrameMax()) 
	{
		//攻撃が盾に当たったかどうか
		if (CheckWeaponAndShieldHIt(chara1, chara2))
		{
			// ガード成功したかどうか
			chara1->SetGuardKnockBack(true,-20);

			// 戦いに必要な特殊なメーターを減らす
			chara1->SetFightingMeter(-10.0f);

			chara1->SetCollGuardEffect();

			chara2->SetWeaponAttacksShield(true);
		}
		return;
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
			StartJoypadVibration(DX_INPUT_PAD1, 1000 / 3, 1000 / 2, -1);
		}
		return;
	}
#endif
}

