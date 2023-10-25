#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"
#include "../Util/Collision3D.h"
#include "../Util/Effekseer3DDrawer.h"
#include "../Util/Game.h"
#include "../DEBUG.h"

#include "../Object/CharacterBase.h"

#include "../Util/Pad.h"
#include "SceneDebug.h"
#include "SceneResult.h"

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pColl(nullptr)
{
	m_pCharacter[0] = nullptr;
	m_pCharacter[1] = nullptr;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera       = std::make_unique<Camera>();
	m_pCharacter[0] = std::make_unique<Player>(VGet(-300.0f, 0.0f, 0.0f));
	m_pCharacter[1] = std::make_unique<Enemy> (VGet( 300.0f, 0.0f, 0.0f));
	m_pColl         = std::make_unique<Collision3D>();
	m_pEffect[0]    = std::make_unique<Effekseer3DDrawer>();
	m_pEffect[1]    = std::make_unique<Effekseer3DDrawer>();

	m_pCamera->Init();

	m_pCharacter[0]->Init();
	m_pCharacter[1]->Init();

	m_pEffect[0]->Init("Data/Guard.efk",30.0f);
	m_pEffect[1]->Init("Data/Guard2.efk", 130.0f);
}

void SceneMain::End()
{
	m_pCharacter[0]->End();
	m_pCharacter[1]->End();
	m_pEffect[0]->End();
	m_pEffect[1]->End();
}

SceneBase* SceneMain::Update()
{
	for (auto& character : m_pCharacter)
	{
		character->Update();
		character->Input();
	}


	m_pCamera->Update();

	for (auto& effect : m_pEffect)
	{
		effect->Update();

		if (!effect->IsPlay())
		{
			effect->SetPlay(false);
		}
	}

	// カメラにプレイヤーとエネミーの位置を渡す
	m_pCamera->SetTargetPos(m_pCharacter[0]->GetPos());
	m_pCamera->SetLockonPos(m_pCharacter[1]->GetPos());

	// カメラにプレイヤーの角度と位置を渡す
	m_pCamera->SetPlayerAngle(m_pCharacter[0]->GetAngle());

	m_pCharacter[1]->SetTargetPos(m_pCharacter[0]->GetPos());
	m_pCharacter[0]->SetTargetPos(m_pCharacter[1]->GetPos());

	UpdateCharacter(m_pCharacter[0].get(), m_pCharacter[1].get());
	UpdateCharacter(m_pCharacter[1].get(), m_pCharacter[0].get());
	

	if (Pad::isTrigger(PAD_INPUT_1))
	{
		clsDx();
		return new SceneDebug();
	}
	
	if (m_pCharacter[0]->GetHp() == 0 || m_pCharacter[1]->GetHp() == 0)
	{
		static int count = 0;
		count++;
		if (count == 60 * 5)
		{
			count = 0;
			return new SceneResult();
		}
	}

	return this;
}

void SceneMain::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xAAAAAA,true);

	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}

	for (auto& effect : m_pEffect)
	{
		effect->Draw();
	}

#if _DEBUG
	DEBUG::FrameMeter("P体力", 100, 50, m_pCharacter[0]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E体力", 100, 100, m_pCharacter[1]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("Pスタミナ", 100, 150, m_pCharacter[0]->GetStamina(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("Eスタミナ", 100, 200, m_pCharacter[1]->GetStamina(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P攻撃フレーム", 100, 250, m_pCharacter[0]->GetAttackFrameMax(), m_pCharacter[0]->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P防御フレーム", 100, 300, m_pCharacter[0]->GetGuardFrameMax(), m_pCharacter[0]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pCharacter[0]->GetJustGuardFrameMax(), m_pCharacter[0]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E攻撃フレーム", 100, 400, m_pCharacter[1]->GetAttackFrameMax(), m_pCharacter[1]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E防御フレーム", 100, 450, m_pCharacter[1]->GetGuardFrameMax(), m_pCharacter[1]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pCharacter[1]->GetJustGuardFrameMax(), m_pCharacter[1]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::Field();
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
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 0.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

void SceneMain::UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2)
{
	// ジャストガードが成功したかどうか
	chara1->SetJustGuard(false);
	// ジャストガードされたかどうか
	chara1->SetJustGuardBreak(chara2->IsJustGuard());
	// 回転角度を取得
	chara1->SetRota(chara2->GetRot());
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
				// スタミナを減らす
				chara1->SetStamina(30.0f, 0.0f);
				// ジャストガードが成功したかどうか
				chara1->SetJustGuard(true);
				// 振動開始
				StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

				m_pEffect[1]->SetPlay(true);
				m_pEffect[1]->SetAngle(chara1->GetAngle());
				m_pEffect[1]->SetPos(VGet(chara1->GetPos().x, chara1->GetPos().y + 100.0f, chara1->GetPos().z));
			}
		}
		// 通常ガードが出来るかどうか
		else if (chara1->GetGuardFrame() == chara1->GetGuardFrameMax())
		{
			//攻撃が盾に当たったかどうか
			if (CheckWeaponAndSieldHIt(chara1, chara2))
			{
				// スタミナを減らす
				chara1->SetStamina(0.0f, 10.0f);
				// ガード成功したかどうか
				chara1->SetGuard(true);

				m_pEffect[0]->SetPlay(true);
				m_pEffect[0]->SetAngle(chara1->GetAngle());
				m_pEffect[0]->SetPos(chara1->GetSieldPos());
			}
		}
		else
		{
			// 攻撃が当たったかどうか
			if (CheckWeaponAndBodyHit(chara1, chara2))
			{
				// ダメージを与える
				chara1->SetDamage(true);
				// 振動開始
				StartJoypadVibration(DX_INPUT_PAD1, 1000/3, 1000/2, -1);
			}
		}
	}
}

