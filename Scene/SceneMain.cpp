#include <DxLib.h>

#include "SceneMain.h"
#include "SceneDebug.h"// デバッグシーン
#include "SceneResult.h"// リザルトシーン

#include "../Object/Camera/Camera.h"// カメラ
#include "../Object/Player/Player.h"// プレイヤー
#include "../Object/Enemy/Enemy.h"// エネミー
#include "../Object/CharacterBase.h"// キャラクター

#include "../Util/Collision3D.h"// 当たり判定
#include "../Util/EffekseerDrawer.h"// 3Dエフェクト
#include "../Util/Game.h"// ゲーム基本設定
#include "../Util/Pad.h"// パッド
#include "../Util/BloodDrawer.h"// 血のエフェクト
#include "../Util/CharacterName.h"// キャラクターの名前

#include "../FIeldDrawer.h"// マップ描画

#include "../UI/UIDrawer.h";

#include "../DEBUG.h"// デバッグ用

#include "../BlurScreen.h";

SceneMain::SceneMain(DifficultyData data):
	m_pUpdateFunc(nullptr),
	m_pCamera(nullptr)
{
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)     ] = std::make_shared<Player>(data,VGet(-300.0f, 300.0f, 0.0f)); // キャラクタークラス
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)] = std::make_shared<Enemy> (data, VGet(300.0f, 300.0f, 0.0f)); // キャラクタークラス
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	// シーン遷移
	m_pUpdateFunc = &SceneMain::UpdateGamePlay;

	m_pCamera               = std::make_unique<Camera>();// カメラクラス
	m_pField                = std::make_unique<FieldDrawer>();// フィールド描画クラス
	m_pUi                   = std::make_unique<UIDrawer>();   // UI描画クラス

	// 初期化
	m_pCamera->Setting();
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Init();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Init();
	m_pField->Init();

	m_hCheckmate = LoadGraph("Data/Image/UI/Checkmate.png");

	m_resultData = GameResultData::NONE;
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
	// シーン遷移
	return (this->*m_pUpdateFunc)();
}

SceneBase* SceneMain::UpdateGamePlay()
{
	int player = static_cast<int>(CharacterName::PLAYER);
	int enemy = static_cast<int>(CharacterName::ENEMYNORMAL);
	// キャラクターの更新処理
	UpdateCharacter(m_pCharacter[player],m_pCharacter[enemy], true);
	UpdateCharacter(m_pCharacter[enemy], m_pCharacter[player], false);


	// UIにパラメーターの状態を渡す
	m_pUi->SetParam(
		m_pCharacter[player]->GetMyId(),
		m_pCharacter[player]->GetHp(),
		m_pCharacter[player]->GetMaxHp(),
		m_pCharacter[player]->GetStrongPower(),
		m_pCharacter[player]->GetkStrongAttackPowerMax(),
		m_pCharacter[player]->GetFightingMeter());
	m_pUi->SetParam(
		m_pCharacter[enemy]->GetMyId(),
		m_pCharacter[enemy]->GetHp(),
		m_pCharacter[enemy]->GetMaxHp(),
		m_pCharacter[enemy]->GetStrongPower(),
		m_pCharacter[enemy]->GetkStrongAttackPowerMax(),
		m_pCharacter[enemy]->GetFightingMeter());


	// 敵の攻撃可能範囲にいるかどうか
	if (CheckModelAboutHIt(m_pCharacter[player], m_pCharacter[enemy]))
	{
		m_pCharacter[enemy]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[enemy]->SetAttackRange(false);
	}

	// カメラにプレイヤーとエネミーの位置を渡す
	m_pCamera->SetTargetPos(m_pCharacter[player]->GetPos());
	// カメラにプレイヤーの角度と位置を渡す
	m_pCamera->SetPlayerAngle(m_pCharacter[player]->GetAngle());
		// カメラの更新処理
	m_pCamera->Update();

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

	// 勝敗条件処理
	{
		// HPが0になった場合
		if (m_pCharacter[player]->GetHp() == 0) // プレイヤー
		{						
			m_resultData = GameResultData::OVER;
		}
		else if (m_pCharacter[enemy]->GetHp() == 0) // エネミー
		{			
			m_resultData = GameResultData::CREAR;
		}

		// 場外に出た場合
		if (CheckCollMap(m_pCharacter[player])) // プレイヤー
		{
			m_resultData = GameResultData::OVER;
		}
		else if (CheckCollMap(m_pCharacter[enemy])) // エネミー
		{
			m_resultData = GameResultData::CREAR;
		}

		// ゲームのクリア,オーバー条件の確認
		if (m_resultData != GameResultData::NONE)
		{
			m_pUpdateFunc = &SceneMain::UpdateGameResult;
			m_frameCount = 0;
		}
	}

	// 画面振動更新処理
	EffectScreen::GetInstance().QuakeUpdate();

	return this;
}

SceneBase* SceneMain::UpdateGameResult()
{
	// 指定フレームの後にリザルト画面に移動する
	m_frameCount++;
	if (m_frameCount == 60 * 3)
	{
		clsDx();
		return new SceneResult(m_resultData);

	}
	if (Pad::IsTrigger(PAD_INPUT_1))
	{	
		clsDx();
		return new SceneResult(m_resultData);
	}

#if _DEBUG
	if (m_resultData == GameResultData::CREAR)
	{
		printfDx("勝利\n");
	}
	else if (m_resultData == GameResultData::OVER)
	{
		printfDx("敗北\n");
	}
	else
	{
		printfDx("バグ");
	}
#endif

	return this;
}


void SceneMain::Draw()
{
	// 
	EffectScreen::GetInstance().QuakePreRenderBlurScreen();
	EffectScreen::GetInstance().ClearScreen();
	
	// DxLibの仕様上SetDrawScreenでカメラの位置などの設定が
	// 初期化されるのでここで再指定
	m_pCamera->Setting();
	// 上と同様初期化させるのでもう一度再設定する
	EffekseerDrawer::GetInstance().EffekseerSync();

	// マップの描画
	m_pField->Draw();

	// キャラクターの描画
	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}
	
	// 血しぶきの描画(仮)
	for (auto& blood : m_pBlood)
	{
		blood->Draw();
	}
	
#if _DEBUG
	DEBUG::Field();
	DEBUG::FrameMeter("P体力", 100, 50, m_pCharacter[0]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E体力", 100, 100, m_pCharacter[1]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("Pスタミナ", 100, 150, m_pCharacter[0]->GetFightingMeter(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("Eスタミナ", 100, 200, m_pCharacter[1]->GetFightingMeter(), 100, 15, 0xffff00);

	DEBUG::FrameMeter("P攻撃フレーム", 100, 250, m_pCharacter[0]->GetAttackFrameMax(), m_pCharacter[0]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("P防御フレーム", 100, 300, m_pCharacter[0]->GetGuardFrameMax(), m_pCharacter[0]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pCharacter[0]->GetJustGuardFrameMax(), m_pCharacter[0]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E攻撃フレーム", 100, 400, m_pCharacter[1]->GetAttackFrameMax(), m_pCharacter[1]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E防御フレーム", 100, 450, m_pCharacter[1]->GetGuardFrameMax(), m_pCharacter[1]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pCharacter[1]->GetJustGuardFrameMax(), m_pCharacter[1]->GetJustGuardFrame(), 30, 0xffffff);
	DrawString(0, 0, "SceneMain", 0xffffff);
#endif

	// 画面エフェクトの更新処理
	EffectScreen::GetInstance().ScreenBack();
	EffectScreen::GetInstance().QuakePostRenderBlurScreen();

	// UIの描画
	m_pUi->Draw();

	// 勝敗がついた場合描画する
	if (m_pUpdateFunc == &SceneMain::UpdateGameResult)
	{
		DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, 0.0f, m_hCheckmate, true);
	}
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

bool SceneMain::CheckCollMap(std::shared_ptr<CharacterBase> character)
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
		// 当たった情報キャラクターにを渡す
		character->SetFieldHit();
		return false;
	}

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
		// ゲームオーバー
		return true;
	}

	// 当たり判定情報の後始末
	MV1CollResultPolyDimTerminate(HitPolyDim);

	return false;
}

// 1が攻撃をする側
// 2が攻撃を受ける側
void SceneMain::UpdateCharacter(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2,bool isPlayer)
{
#if true

	// プレイヤーの入力情報
	character1->Input();
	// プレイヤー更新処理
	character1->Update();

	// ターゲットのHPを取得
	character1->SetTargetHp(character2->GetHp());
	// ターゲットの位置を受け取る
	character1->SetTargetPos(character2->GetPos());

	// ターゲットの戦闘の状態を受け取る
	character1->SetBattleState(character2->GetBattleState());

	// ジャストガードOFF
//	character1->SetJustGuard(false);

	// 回転角度を取得
	character2->SetTargetMtxRota(character1->GetRot());

	// ガード成功いない状態
	character1->SetWeaponAttacksShield(false);

	// ジャストガード処理
	// 攻撃が当たっていた場合
	if ((CheckWeaponAndShieldHIt(character2, character1)))
	{
		// ジャストガードフレーム
		if (character1->GetJustGuardFrame() > 0 &&
			character1->GetJustGuardFrame() < character1->GetJustGuardFrameMax())
		{
			// ジャストガードが成功したかどうか
			character1->SetJustGuard(true);

			// エフェクトを再生
			character1->SetCollJustGuardEffect();

			// 戦いに必要な特殊なメーターを減らす
			character2->SetFightingMeter(-30.0f);

			// 振動開始
			StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

			return;
		}
	}

	// 通常ガード処理
	// 通常ガードが出来るかどうか
	if (character1->GetGuardFrame() == character1->GetGuardFrameMax())
	{
		// 攻撃状態だったら
		//攻撃が盾に当たったかどうか
		if (CheckWeaponAndShieldHIt(character2, character1))
		{
			// ノックバック
			character1->SetGuardKnockBack(true, -20);

			// 強攻撃するための力を溜める
			character1->SetStrongPower(20.0f);

			// 戦いに必要な特殊なメーターを減らす
			character1->SetFightingMeter(-3.0f);

			character1->SetCollGuardEffect();

			character1->SetWeaponAttacksShield(true);

			return;
		}		
	}


	const bool a = character1->GetBattleState() == BattleState::ATTACK;
	const bool b = character1->GetBattleState() == BattleState::ATTACKTWO;
	const bool c = character1->GetBattleState() == BattleState::STRONGATTACK;
	// 攻撃を与える処理
	if (a || b || c)
	{
		// 攻撃が当たったかどうか
		if (CheckWeaponAndBodyHit(character1, character2))
		{
			// ダメージを与える
			character2->SetDamage(true);

			// 戦いに必要な特殊なメーターを減らす
			character1->SetFightingMeter(-0.1f);

			// ノックバック
			if (character1->GetBattleState() == BattleState::STRONGATTACK)
			{
				character2->SetGuardKnockBack(true, -30);
			}
			else
			{
				character2->SetGuardKnockBack(true, -10);
			}

			int color = 0xffffff;
			if (isPlayer)
			{
				color = 0x000000;
			}
			else
			{
				color = 0xffffff;
			}

			const bool d = character2->GetBattleState() != BattleState::GUARD;
			if (d)
			{
				for (int i = 0; i < 100; i++)
				{
					m_pBlood.push_back(new BloodDrawer(VGet(character2->GetPos().x, character2->GetPos().y + 100.0f, character2->GetPos().z), color));
					m_pBlood.back()->Init(i);
				}
			}

			// 振動開始
			StartJoypadVibration(DX_INPUT_PAD1, 1000 / 3, 1000 / 2, -1);

			return;
		}		
	}
#endif
}

