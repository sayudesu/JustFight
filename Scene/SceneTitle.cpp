#include <DxLib.h>
#include "SceneDebug.h"
#include "SceneMain.h"
#include "SceneTitle.h"

#include "../Util/SlideSelect.h"
#include "../Util/SlideSelect.h"

#include "../Util/Game.h"

#include "../Util/GameObject.h"

#include "../Object/Camera/Camera.h"

#include "../Util/DifficultyData.h"

#include "../CSVData/ModelManager.h"
#include "../Util/ModelName.h"

#include "../CSVData/FontManager.h"
#include "../Util/FontSize.h"

#include "../CSVData/SoundManager.h"
#include "../Util/SoundName.h"

#include "../PoseScreen.h"

namespace 
{
	constexpr float kImageAngle = 0.0f * DX_PI_F / 180.0f;
	constexpr float kImagePosX = -450.0f;
	constexpr float kImagePosY = 0.0f;
	constexpr float kImageSize = 0.95f;

	constexpr float kImageArrowSizeChangeSpeed = 0.002f;

	// 難易度
	const char* const kNovice = "チュートリアル";
	const char* const kIntermediate = "普通";
	const char* const kExpart = "難しい";

	// オプション
	const char* const kOption = "オプション";
}

SceneTitle::SceneTitle():
	m_hTitle(-1),
	m_isInputController(false),
	m_bgPos(VGet(static_cast<float>(Game::kScreenWidth / 2), -static_cast<float>(Game::kScreenHeight / 2), 0.0f)),
	m_speed(0.1f)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_arrowSize[0] = kImageSize;
	m_arrowSize[1] = kImageSize;

	// カメラの位置が正しいかどうかを確認するための初期化
	m_isCameraStop[0] = false;
	m_isCameraStop[1] = false;
	m_isCameraStop[2] = false;
	m_isCameraStop[3] = false;

	m_hTitle = LoadGraph("Data/Image/UI/ゲーム難易度選択ベース2.png");

	m_select = std::make_unique<SlideSelect>();
	m_select->Init(2,true);

	// カメラの座標
	m_cameraPosX = 100.0f;
	m_cameraPosY = 300.0f;
	m_cameraPosZ = 100.0f;

	// カメラインスタンス
	m_camera = std::make_unique<Camera>();
	// カメラターゲット位置初期化
	m_camera->SetTargetPos(VGet(0.0f, -100.0f, -10.0f));

	// 3Dオブジェクト
	{
		// マップ
		m_pStage = std::make_unique<GameObject>(
			ModelManager::GetInstance().ModelType(ModelName::MAP3),
			VGet(0,0,0),
			VGet(0,0, 180 * DX_PI_F/ 180.0f),
			VGet(1, 1, 1));		
	}

	// 2Dオブジェクト
	{
		// 背景
		m_hBg = std::make_shared<GameObject>(
			"Data/Image/UI/ゲーム難易度選択ベース2.png",
			m_bgPos,
			kImageAngle,
			kImageSize
		);

		// 選択文字
		m_hSelect = std::make_shared<GameObject>(
			"Data/Image/UI/Select Difficulty.png",
			VGet(kImagePosX, -300.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		// 選択文字の下の飾り
		m_hDecoration = std::make_shared<GameObject>(
			"Data/Image/UI/飾り.png",
			VGet(kImagePosX, -250.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		// 難易度
		m_hNovice = std::make_shared<GameObject>(
			"Data/Image/UI/Novice.png",
			VGet(kImagePosX, 30.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);
		
		m_hIntermediate = std::make_shared<GameObject>(
			"Data/Image/UI/Intermediate.png",
			VGet(kImagePosX, 30.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		m_hExpert = std::make_shared<GameObject>(
			"Data/Image/UI/Expert.png",
			VGet(kImagePosX, 24.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		m_hImageDifficultyBg = std::make_shared<GameObject>(
			"Data/Image/UI/CharactorBg.png",
			VGet(kImagePosX + 800.0f, 30.0f, 0),
			kImageAngle,
			kImageSize + 2.0f,
			m_hBg.get()
		);

		// 矢印上向き
		m_hArrow[0] = std::make_shared<GameObject>(
			"Data/Image/UI/矢印.png",
			VGet(kImagePosX, -120.0f, 0),
			kImageAngle + 180.0f * DX_PI_F / 180.0f,
			m_arrowSize[0],
			m_hBg.get()
		);

		// 矢印下向き
		m_hArrow[1] = std::make_shared<GameObject>(
			"Data/Image/UI/矢印.png",
			VGet(kImagePosX, 180.0f, 0),
			kImageAngle,
			m_arrowSize[1],
			m_hBg.get()
		);

		// オプションの背景
		m_hOptionBack = std::make_shared<GameObject>(
			"Data/Image/UI/OptionBack.png",
			VGet(550.0f, 400.0f, 0),
			0,
			1,
			m_hBg.get()
			);

		// オプションボタン
		m_hOptionBotton = std::make_shared<GameObject>(
			"Data/Image/UI/StartBotton.png",
			VGet(430.0f, 400.0f, 0),
			0,
			1,
			m_hBg.get()
			);

		m_pStage->SetPos(VGet(0.0f, 0.0f, 0.0f));
		m_pStage->SetRotate(VGet(0.0f, 0.0f, 0.0f));
		m_pStage->Update();
	}

	m_hModel[0] = std::make_unique<GameObject>(
		ModelManager::GetInstance().ModelType(ModelName::Pawn_B),
		ConvScreenPosToWorldPos({ Game::kScreenWidth / 2,Game::kScreenHeight / 2,0 }),
		VGet(-90 * DX_PI_F / 180.0f,0,0),
		VGet(0.9f, 0.9f, 0.9f));

	m_hModel[1] = std::make_unique<GameObject>(
		ModelManager::GetInstance().ModelType(ModelName::Knight_B),
		ConvScreenPosToWorldPos({ Game::kScreenWidth / 2,Game::kScreenHeight / 2,0 }),
		VGet(-90 * DX_PI_F / 180.0f, 0, 0),
		VGet(0.9f, 0.9f, 0.9f));

	m_hModel[2] = std::make_unique<GameObject>(
		ModelManager::GetInstance().ModelType(ModelName::Queen_B),
		ConvScreenPosToWorldPos({ Game::kScreenWidth / 2,Game::kScreenHeight / 2,0 }),
		VGet(-90 * DX_PI_F / 180.0f, 0, 0),
		VGet(0.9f, 0.9f, 0.9f));

	m_modelRot[0] = 0.0f;
	m_modelRot[1] = 0.0f;
	m_modelRot[2] = 0.0f;

	m_arrowPosX[0] = m_hArrow[0]->GetPos().x;
	m_arrowPosY[0] = m_hArrow[0]->GetPos().y;

	m_arrowPosX[1] = m_hArrow[1]->GetPos().x;
	m_arrowPosY[1] = m_hArrow[1]->GetPos().y;

	m_firstArrowPosX[0] = m_hArrow[0]->GetPos().x;
	m_firstArrowPosY[0] = m_hArrow[0]->GetPos().y;

	m_firstArrowPosX[1] = m_hArrow[1]->GetPos().x;
	m_firstArrowPosY[1] = m_hArrow[1]->GetPos().y;

	m_arrowShakeX[0] = 0.0f;
	m_arrowShakeY[0] = 0.0f;

	m_arrowShakeX[1] = 0.0f;
	m_arrowShakeY[1] = 0.0f;

	m_firstEnemyBgX = m_hImageDifficultyBg->GetPos().x;
	m_firstEnemyBgY = m_hImageDifficultyBg->GetPos().y;

	m_difficulty = m_hNovice->GetSize2D();
}

void SceneTitle::End()
{
	DeleteGraph(m_hTitle);
}

SceneBase* SceneTitle::Update()
{

	SoundManager::GetInstance().Play(SoundName::TITLE,true);

	PoseScreen::GetInstance().Update();

	char deviceName[260]{};
	char productName[260]{};
#if false
	// コントローラーの接続を確認する
	if (GetJoypadName(DX_INPUT_PAD1, &deviceName[0], &productName[0]) == 0) 
	{
		m_isInputController = false;
	}
	else
	{
		m_isInputController = true;
		return this;
	}
#endif
	static float posY = 30.0f;
	if (m_isCameraStop[3])
	{
		m_select->Update();
		posY -= 1.0f;
	}

	// ボタン
	{
		static int timer = 0;
		float y = cos(timer * 0.07f) * 5.0f;
		timer++;

		// 位置の更新
		m_hArrow[0]->SetPos({ m_arrowPosX[0] + m_arrowShakeX[0],m_arrowPosY[0] + m_arrowShakeY[0] +   y,0});
		m_hArrow[1]->SetPos({ m_arrowPosX[1] + m_arrowShakeX[1],m_arrowPosY[1] + m_arrowShakeY[1] + (-y),0});

		// 上を押した場合
		if (m_select->IsUpBotton())
		{
			// 矢印を指定の最大数まで上に上昇させる
			// 最終地点に到達すると乱数で揺らす
			m_arrowPosY[0] -= 10.0f;
			if (m_arrowPosY[0] < m_firstArrowPosY[0] - 20.0f)
			{
				m_arrowPosY[0] = m_firstArrowPosY[0] - 20.0f;
				m_arrowShakeX[0] = GetRand(5) - 5;
				m_arrowShakeY[0] = GetRand(5) - 5;
			}
		}
		else
		{
			// 上を押していない場合は元の位置に戻り揺れはしない
			m_arrowPosY[0] += 15.0f;
			if (m_arrowPosY[0] > m_firstArrowPosY[0])
			{
				m_arrowPosY[0] = m_firstArrowPosY[0];
				m_arrowShakeX[0] = 0;
				m_arrowShakeY[0] = 0;
			}
		}
		// 下を押した場合
		if (m_select->IsDownBotton())
		{
			// 矢印を指定の最大数まで下に下降させる
			// 最終地点に到達すると乱数で揺らす
			m_arrowPosY[1] += 10.0f;
			if (m_arrowPosY[1] > m_firstArrowPosY[1] + 20.0f)
			{
				m_arrowPosY[1] = m_firstArrowPosY[1] + 20.0f;
				m_arrowShakeX[1] = GetRand(5) - 5;
				m_arrowShakeY[1] = GetRand(5) - 5;
			}
		}
		else
		{
			// 下を押していない場合は元の位置に戻り揺れはしない
			m_arrowPosY[1] -= 15.0f;
			if (m_arrowPosY[1] < m_firstArrowPosY[1])
			{
				m_arrowPosY[1] = m_firstArrowPosY[1];
				m_arrowShakeX[1] = 0;
				m_arrowShakeY[1] = 0;
			}
		}
	}

	// 敵の画像
	{
		// 位置の更新
		m_hImageDifficultyBg->SetPos
		(
			{
				m_firstEnemyBgX + m_enemyBgShakeX,
				m_firstEnemyBgY + m_enemyBgShakeY,
				m_hImageDifficultyBg->GetPos().x,
			}
		);

		// 選択を変更した場合
		if (m_tempSelect != m_select->GetSelect())
		{
			m_isEnemyBgShake = true;
		}
		
		// 指定のフレーム数の間揺らす
		if (m_isEnemyBgShake)
		{
			m_enemyBgshakeCount++;
			
			m_enemyBgShakeX = GetRand(5) - 5;
			m_enemyBgShakeY = GetRand(5) - 5;
			
			if (m_enemyBgshakeCount == 5)
			{
				m_enemyBgshakeCount = 0;
				m_isEnemyBgShake = false;
			}
		}
		else
		{
			m_enemyBgShakeX = 0;
			m_enemyBgShakeY = 0;
		}

		m_tempSelect = m_select->GetSelect();
	}

	// カメラ処理
	{
		m_camera->SetPos(VGet(m_cameraPosX, m_cameraPosY, m_cameraPosZ));

		// カメラがx,y,zそれぞれ停止したかをチェックする
		if (m_isCameraStop[0] && m_isCameraStop[1] && m_isCameraStop[2])
		{
			// 画面の中心に移動
			if (m_bgPos.y < static_cast<float>(Game::kScreenHeight) / 2)
			{
				m_bgPos.y += 60.0f;
				m_hBg->SetPos(m_bgPos);
			}
			else
			{
				// 画面の中心に固定
				m_bgPos.y = static_cast<float>(Game::kScreenHeight) / 2;

				m_isCameraStop[3] = true;
			}
		}

		m_speed = (m_speed * 1.1f);

		if (m_cameraPosX > 0.0f)
		{
			m_cameraPosX -= m_speed;
		}
		else
		{
			m_cameraPosX = 0.0f;
			m_isCameraStop[0] = true;
		}

		if (m_cameraPosY > 32.0f)
		{
			m_cameraPosY -= m_speed;
		}
		else
		{
			m_cameraPosY = 32.0f;
			m_isCameraStop[1] = true;
		}
	
		if (m_cameraPosZ > 10.0f)
		{
			m_cameraPosZ -= m_speed;
		}
		else
		{
			m_cameraPosZ = 10.0f;
			m_isCameraStop[2] = true;
		}

	}

	// 3Dモデルの描画
	{
		if (posY < -10.0f)
		{
			posY = -10.0f;
		}

		VECTOR  test = ConvScreenPosToWorldPos({ Game::kScreenWidth / 2 + 3900.0f,Game::kScreenHeight / 2 + 1500.0f,0 });
		for (int i = 0; i < 3; i++)
		{
			// 位置の更新
			m_hModel[i]->SetPos
			(
				{
					test.x,
					test.y + posY,
					test.z
				}
			);

			// 回転
			m_modelRot[i]++;

			// 回転の更新
			m_hModel[i]->SetRotate
			(
				{
					-90 * DX_PI_F / 180.0f,
					0,
					m_modelRot[i] * DX_PI_F / 180.0f
				}
			);

			// 3Dモデルの更新
			m_hModel[i]->Update();
		}
	}


	m_camera->Setting();

	// 背景の更新処理
	m_hBg->Update();
	m_hSelect->Update();
	m_hDecoration->Update();
	m_hOptionBack->Update();
	m_hOptionBotton->Update();

	// 難易度
	m_hNovice->Update();
	m_hIntermediate->Update();
	m_hExpert->Update();

	m_hImageDifficultyBg->Update();
	// 矢印の更新処理
	m_hArrow[0]->Update();
	m_hArrow[1]->Update();

	if (m_isCameraStop[3])
	{
		// 難易度調整
		// 弱い(チュートリアル)
		if (m_select->GetResult() == 0)
		{		
			SoundManager::GetInstance().Stop(SoundName::TITLE);
			return new SceneMain(DifficultyData::NOIVE);
		}
		// 普通
		if (m_select->GetResult() == 1)
		{
			SoundManager::GetInstance().Stop(SoundName::TITLE);
			return new SceneMain(DifficultyData::INTERMEDIATE);
		}
		// 強い
		if (m_select->GetResult() == 2)
		{
			SoundManager::GetInstance().Stop(SoundName::TITLE);
			return new SceneMain(DifficultyData::EXPERT);
		}
	}

#if false
	if (DxLib::CheckHitKey(KEY_INPUT_Z))
	{
		return new SceneDebug();
	}
#endif

	return this;
}

void SceneTitle::Draw()
{

	m_pStage->Draw();

	m_hBg->Draw();
	m_hSelect->Draw();
	m_hDecoration->Draw();

	m_hImageDifficultyBg->Draw();

	m_hArrow[0]->Draw();
	m_hArrow[1]->Draw();

	// 難易度
	if (m_select->GetSelect() == 0)
	{
		m_hNovice->Draw();
		if (m_isCameraStop[3])
		{
			m_hModel[0]->Draw();
		}

		FontManager::GetInstance().DrawString(
			m_hBg->GetPos().x + m_hNovice->GetPos().x - 150.0f,
			m_hBg->GetPos().y + m_hNovice->GetPos().y + 240.0f,
			kNovice, 0xffffff, FontSize::GENEITERAMIN_SMALL);
	}
	else if (m_select->GetSelect() == 1)
	{
		m_hIntermediate->Draw();
		if (m_isCameraStop[3])
		{
			m_hModel[1]->Draw();
		}

		FontManager::GetInstance().DrawString(
			m_hBg->GetPos().x + m_hNovice->GetPos().x - 65.0f,
			m_hBg->GetPos().y + m_hNovice->GetPos().y + 240.0f,
			kIntermediate, 0xffffff, FontSize::GENEITERAMIN_SMALL);
	}
	else if (m_select->GetSelect() == 2)
	{
		m_hExpert->Draw();		
		if (m_isCameraStop[3])
		{
			m_hModel[2]->Draw();
		}

		FontManager::GetInstance().DrawString(
			m_hBg->GetPos().x + m_hNovice->GetPos().x - 105.0f,
			m_hBg->GetPos().y + m_hNovice->GetPos().y + 240.0f,
			kExpart, 0xffffff, FontSize::GENEITERAMIN_SMALL);
	}


	// オプションボタンの描画
	m_hOptionBack->Draw();
	m_hOptionBotton->Draw();
	FontManager::GetInstance().DrawString(
		m_hBg->GetPos().x + m_hOptionBack->GetPos().x - 30.0f,
		m_hBg->GetPos().y + m_hOptionBack->GetPos().y - 20.0f,
		kOption, 0xffffff, FontSize::GENEITERAMIN_SMALL);

	// ポーズ画面の描画
	PoseScreen::GetInstance().Draw();

#if false
	// コントローラー
	if (m_isInputController)
	{
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
		DrawFormatString((Game::kScreenWidth - ((17 * 32)/2)) / 2, Game::kScreenHeight/2 - 32, 0x000000, "コントローラーを接続してください。");
	}	
#endif
}