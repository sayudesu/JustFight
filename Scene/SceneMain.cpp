#include <DxLib.h>

#include "SceneMain.h"
#include "SceneResult.h"// リザルトシーン

#include "../Object/Camera/Camera.h"    // カメラ
#include "../Object/Player/Player.h"    // プレイヤー
#include "../Object/Enemy/Enemy.h"      // エネミー
#include "../Object/CharacterBase.h"    // キャラクター
#include "../Object/Stage/FIeldDrawer.h"// マップ描画

#include "../Util/Collision3D.h"    // 当たり判定
#include "../Util/EffekseerDrawer.h"// 3Dエフェクト
#include "../Util/Game.h"           // ゲーム基本設定
#include "../Util/Pad.h"            // パッド
#include "../Util/BloodDrawer.h"    // 血のエフェクト
#include "../Util/CharacterName.h"  // キャラクターの名前
#include "../Util/BlurScreen.h";    // 画面加工
#include "../Util/TutorialDrawer.h"
#include "../Util/Tips.h"
#include "../Util/SoundName.h"
#include "../Util/HitPos.h"

#include "../UI/UIDrawer.h";// UI描画

#include "../CSVData/SoundManager.h"

namespace
{
	// プレイヤーの番号を指定する
	constexpr int kPlayerNo = static_cast<int>(CharacterName::PLAYER);
	// エネミーの番号を指定する
	constexpr int kEnemyNo = static_cast<int>(CharacterName::ENEMY);

	// 勝敗が付いた時に描画する画像位置
	const char* const kCheckmateGraphPath = "Data/Image/UI/Checkmate.png";

	// 勝敗が決まった際にチェックメイト画像が描画されるまでのフレーム
	constexpr int kCheckmateDrawFrame = 20;

	// シーンが移行するまでのフレーム
	constexpr int kResultSceneChengeFrame = 60 * 3;

	// チェックメイト画像のサイズの最大
	constexpr float kCheckmateGraphSizeMax = 1.0f;

	// チェックメイト画像を最大サイズまで小さくする値
	constexpr float kCheckmateGraphSizeMinRate = 1.0f;

	// チェックメイト画像の角度の最終の値
	constexpr float kCheckmateGraphEndRota = 0.0f;

	// チェックメイト画像の角度の変更する値
	constexpr float kCheckMateGraphRate = 0.5f;

	// 勝敗決定後の背景のアルファ値を変更する最大値
	constexpr int kResultAlphaRateMax = 128;

	// 勝敗決定後の背景のアルファ値を変更する値
	constexpr int kResultAlphaRate = 5;

	// 勝敗決定後の背景カラー
	constexpr int kResultColor = 0x000000;
}

SceneMain::SceneMain(DifficultyData data):
	m_pUpdateFunc(nullptr),
	m_pCamera(nullptr),
	m_hCheckmate(-1),
	m_resultData(GameResultData::NONE),
	m_frameCount(0),
	m_difficultyData(data),
	m_checkmateSize(10.0f),
	m_checkmateRota(10.0f),
	m_checkmateBgBlendRate(0),
	m_checkmatePosY(0),
	m_isBlur(false)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	// シーン遷移
	m_pUpdateFunc = &SceneMain::UpdateGamePlay;

	// キャラクタークラス
	m_pCharacter[kPlayerNo] = std::make_shared<Player>(m_difficultyData, VGet(-300.0f, 300.0f, 0.0f)); 
	m_pCharacter[kEnemyNo]  = std::make_shared<Enemy>(m_difficultyData, VGet(300.0f, 300.0f, 0.0f)); 

	m_pCamera   = std::make_unique<Camera>();        // カメラクラス
	m_pField    = std::make_unique<FieldDrawer>();   // フィールド描画クラス
	m_pUi       = std::make_unique<UIDrawer>();      // UI描画クラス
	m_pTutorial = std::make_unique<TutorialDrawer>();// チュートリアルクラス

	// 初期化
	m_pCharacter[kPlayerNo]->Init();
	m_pCharacter[kEnemyNo]->Init();
	m_pCamera->Setting();
	m_pField->Init();
	m_pTutorial->Init();

	// チェックメイト画像の読み込み
	m_hCheckmate = LoadGraph(kCheckmateGraphPath);

	// UIにパラメーターの状態を渡す
	m_pUi->SetParam(
		m_pCharacter[kPlayerNo]->GetMyId(),
		m_pCharacter[kPlayerNo]->GetHp(),
		m_pCharacter[kPlayerNo]->GetMaxHp(),
		m_pCharacter[kPlayerNo]->GetStrongPower(),
		m_pCharacter[kPlayerNo]->GetkStrongAttackPowerMax(),
		m_pCharacter[kPlayerNo]->GetFightingMeter());
	m_pUi->SetParam(
		m_pCharacter[kEnemyNo]->GetMyId(),
		m_pCharacter[kEnemyNo]->GetHp(),
		m_pCharacter[kEnemyNo]->GetMaxHp(),
		m_pCharacter[kEnemyNo]->GetStrongPower(),
		m_pCharacter[kEnemyNo]->GetkStrongAttackPowerMax(),
		m_pCharacter[kEnemyNo]->GetFightingMeter());

	// スクリーン効果の初期化
	EffectScreen::GetInstance().BlurIReplayInit();	
}

void SceneMain::End()
{
	// 解放処理
	m_pCharacter[kPlayerNo]->End();
	m_pCharacter[kEnemyNo]->End();

	m_pField->End();
	m_pTutorial->End();

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
	// BGMの再生
	SoundManager::GetInstance().Play(SoundName::PLAY, true);

	// if 難易度が中、大だったら
	// else if 難易度が小だったら チュートリアル
	if (m_difficultyData == DifficultyData::INTERMEDIATE||
		m_difficultyData == DifficultyData::EXPERT)
	{
		m_pCharacter[kPlayerNo]->Input();
		m_pCharacter[kEnemyNo]->Input();
	}
	else if(m_difficultyData == DifficultyData::NOIVE)
	{
		m_pCharacter[kPlayerNo]->InputTutorial();
		m_pCharacter[kEnemyNo]->InputTutorial();
		m_pTutorial->SetTips(Tips::MOVE);

		if (m_pCharacter[kPlayerNo]->GetTipsMove(Tips::ATTACK))
		{
			m_pTutorial->SetTips(Tips::ATTACK);
		}
		if (m_pCharacter[kPlayerNo]->GetTipsMove(Tips::GUARD))
		{
			m_pTutorial->SetTips(Tips::GUARD);
		}

	}

	// キャラクターの更新処理
	UpdateCharacter(m_pCharacter[kPlayerNo],m_pCharacter[kEnemyNo], true);
	UpdateCharacter(m_pCharacter[kEnemyNo], m_pCharacter[kPlayerNo], false);

	// UIにパラメーターの状態を渡す
	m_pUi->SetParam(
		m_pCharacter[kPlayerNo]->GetMyId(),
		m_pCharacter[kPlayerNo]->GetHp(),
		m_pCharacter[kPlayerNo]->GetMaxHp(),
		m_pCharacter[kPlayerNo]->GetStrongPower(),
		m_pCharacter[kPlayerNo]->GetkStrongAttackPowerMax(),
		m_pCharacter[kPlayerNo]->GetFightingMeter());
	m_pUi->SetParam(
		m_pCharacter[kEnemyNo]->GetMyId(),
		m_pCharacter[kEnemyNo]->GetHp(),
		m_pCharacter[kEnemyNo]->GetMaxHp(),
		m_pCharacter[kEnemyNo]->GetStrongPower(),
		m_pCharacter[kEnemyNo]->GetkStrongAttackPowerMax(),
		m_pCharacter[kEnemyNo]->GetFightingMeter());


	// 敵の攻撃可能範囲にいるかどうか
	if (CheckModelAboutHIt(m_pCharacter[kPlayerNo], m_pCharacter[kEnemyNo]))
	{
		m_pCharacter[kEnemyNo]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[kEnemyNo]->SetAttackRange(false);
	}

	// カメラにプレイヤーとエネミーの位置を渡す
	m_pCamera->SetTargetPos(m_pCharacter[kPlayerNo]->GetPos());
	// カメラにプレイヤーの角度と位置を渡す
	m_pCamera->SetPlayerAngle(m_pCharacter[kPlayerNo]->GetAngle());
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
		if (m_pCharacter[kPlayerNo]->GetHp() <= 0) // プレイヤー
		{		
			m_frameCount++;
			if (m_frameCount > kCheckmateDrawFrame)
			{
				m_frameCount = 0;
				m_resultData = GameResultData::OVER;
			}
		}
		else if (m_pCharacter[kEnemyNo]->GetHp() <= 0) // エネミー
		{	
			m_frameCount++;
			if (m_frameCount > kCheckmateDrawFrame)
			{
				m_frameCount = 0;
				m_resultData = GameResultData::CREAR;
			}
		}

		// 場外に出た場合
		if (CheckCollMap(m_pCharacter[kPlayerNo])) // プレイヤー
		{
			m_resultData = GameResultData::OVER;
		}
		else if (CheckCollMap(m_pCharacter[kEnemyNo])) // エネミー
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

	// プレイヤーに攻撃がヒットしたかどうか
	bool isDamageBlur = m_pCharacter[kPlayerNo]->IsHitDamage() && !m_pCharacter[kPlayerNo]->IsGuard();

	// プレイヤーが回避行動しているかどうか
	bool isAwayBlur = m_pCharacter[kPlayerNo]->IsAway();

	// 攻撃がヒットしている、プレイヤーが回避している場合にブラー効果をオンにする
	m_isBlur = isDamageBlur || isAwayBlur;

	// 画面振動更新処理
	EffectScreen::GetInstance().QuakeUpdate();

	return this;
}

SceneBase* SceneMain::UpdateGameResult()
{
	// 指定フレームの後にリザルト画面に移動する
	m_frameCount++;

	// ボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// BGMの停止
		SoundManager::GetInstance().Stop(SoundName::PLAY);
		return new SceneResult(m_resultData, m_difficultyData);
	}

	// 指定したフレームまでカウントが進むと
	if (m_frameCount >= kResultSceneChengeFrame)
	{
		// BGMの停止
		SoundManager::GetInstance().Stop(SoundName::PLAY);
		return new SceneResult(m_resultData, m_difficultyData);
	}

	// 画像の位置を動かす計算
	m_checkmatePosY = cosf(static_cast<float>(m_frameCount) * 0.07f) * 100.0f + static_cast<float>(Game::kScreenHeight) / 2.0f - 160.0f;

	// 予めでかくしたサイズを1にする
	if (m_checkmateSize > kCheckmateGraphSizeMax)
	{
		m_checkmateSize -= kCheckmateGraphSizeMinRate;
	}
	// 予め変更した角度を0にする
	if (m_checkmateRota > kCheckmateGraphEndRota)
	{
		m_checkmateRota -= kCheckMateGraphRate;
	}

	// ブレンド率を変更
	if (m_checkmateBgBlendRate < kResultAlphaRateMax)
	{
		m_checkmateBgBlendRate += kResultAlphaRate;
	}

	return this;
}


void SceneMain::Draw()
{	
	// 新しい画面の作成
	EffectScreen::GetInstance().BlurPreRenderBlurScreen();	
	// 画面をクリアにする
	EffectScreen::GetInstance().ClearScreen();		
	
	// DxLibの仕様上SetDrawScreenでカメラの位置などの設定が
	// 初期化されるのでここで再指定
	m_pCamera->Setting();

	// 上と同様初期化させるのでもう一度再設定する
	EffekseerDrawer::GetInstance().EffekseerSync();

	// マップの描画
	m_pField->Draw();

	// 血しぶきの描画
	for (auto& blood : m_pBlood)
	{
		blood->Draw();
	}
	// キャラクターの描画
	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}

	// ブラー効果、画面の振動効果を描画する
	EffectScreen::GetInstance().BlurPostRenderBlurScreen(m_isBlur);

	// UIの描画
	m_pUi->Draw();

	if (m_difficultyData == DifficultyData::NOIVE)
	{
		// チュートリアル用描画
		m_pTutorial->Draw();
	}

	// 勝敗がついた場合描画する
	if (m_pUpdateFunc == &SceneMain::UpdateGameResult)
	{
		// アルファ値を変更してゲーム画面を暗くする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_checkmateBgBlendRate);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kResultColor, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 勝敗判別画像
		DrawRotaGraph(Game::kScreenWidth / 2, m_checkmatePosY, m_checkmateSize, m_checkmateRota, m_hCheckmate, true);
	}
}

// 当たり判定
// 武器と体の判定
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

// 武器と盾の判定
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

// 武器と体範囲の判定
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

// 体範囲と体範囲の判定
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

// 地面のモデルとプレイヤーの判定
bool SceneMain::CheckCollMap(std::shared_ptr<CharacterBase> character)
{
	character->IsCheckHitWall(false,HitPos::NONE);
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

#if _DEBUG

		if (fabs(HitPolyDim.Dim->Normal.x) > 0.9f)
		{					
			if (HitPolyDim.Dim->Position->x < character->GetPos().x + character->GetModelRadius() / 2)
			{
			//	printfDx("横 = X+\n");
				character->IsCheckHitWall(true,HitPos::XP);
			}			
		}
		if (fabs(HitPolyDim.Dim->Normal.z) > 0.9f)
		{
			if (HitPolyDim.Dim->Position->z < character->GetPos().z + character->GetModelRadius() / 2)
			{
				//	printfDx("横 = Z+\n");
				character->IsCheckHitWall(true, HitPos::ZP);
			}
		}
#endif
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
		// 当たった情報キャラクターにを渡す
		character->SetFieldHit();
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
	// プレイヤー更新処理
	character1->Update();

	// ターゲットのHPを取得
	character1->SetTargetHp(character2->GetHp());
	// ターゲットの位置を受け取る
	character1->SetTargetPos(character2->GetPos());

	// ターゲットの戦闘の状態を受け取る
	character1->SetBattleState(character2->GetBattleState());

	// 回転角度を取得
	character2->SetTargetMtxRota(character1->GetRot());

	// ガード成功いない状態
	character1->SetWeaponAttacksShield(false);

	// ジャストガード処理
	// 攻撃が当たっていた場合
	// 相手がスタン状態ではない場合
	// 自身がガード状態の場合
	if (CheckWeaponAndShieldHIt(character2, character1) && 
		character2->GetBattleState() != BattleState::STUN && 
		character1->GetBattleState() == BattleState::GUARD)
	{
		// ジャストガードフレーム
		if(character1->GetGuardFrame() < character1->GetJustGuardFrameMax())
		{
			// ジャストガードが成功したかどうか
			character1->SetJustGuard(true);

			// エフェクトを再生
			character1->SetCollJustGuardEffect();

			// 戦いに必要な特殊なメーターを減らす
			character2->SetFightingMeter(-110.0f);

			// 振動開始
			StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

			return;
		}
	}

	// 通常ガード処理
	// 通常ガードが出来るかどうか
	if (character1->GetGuardFrame() > character1->GetGuardFrameMax() &&
		character2->GetBattleState() != BattleState::STUN)
	{
		// 攻撃状態だったら
		// 攻撃が盾に当たったかどうか
		if (CheckWeaponAndShieldHIt(character2, character1))
		{
			// ノックバック
			character1->SetGuardKnockBack(true, -20);

			// 強攻撃するための力を溜める
			character1->SetStrongPower(20.0f);

			character1->SetCollGuardEffect();

			character1->SetWeaponAttacksShield(true);

			return;
		}		
	}
	else
	{
		const bool isAttack = character1->GetBattleState()       == BattleState::ATTACK;
		const bool isAttackTow = character1->GetBattleState()    == BattleState::ATTACKTWO;
		const bool isStrongAttack = character1->GetBattleState() == BattleState::STRONGATTACK;

		// 攻撃を与える処理
		if (isAttack || isAttackTow || isStrongAttack)
		{
			// 攻撃が当たったかどうか
			if (CheckWeaponAndBodyHit(character1, character2))
			{
				// ダメージを与える
				character2->SetDamage(true);

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

				// ガードしていなかったら
				const bool isGruad = character2->GetBattleState() != BattleState::GUARD;
				if (isGruad)
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
	}
#endif
}

