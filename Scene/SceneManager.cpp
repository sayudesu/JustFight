#include <DxLib.h>
#include <cassert>

#include "SceneManager.h"
#include "SceneDebug.h"
#include "SceneBase.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "SceneResult.h"

#include "../Util/Pad.h"
#include "../Util/Game.h"

#include <string>

namespace
{
	constexpr int kLoadObjectNum = 12;
	constexpr int kLoadObjectSpeed = 16;
}

SceneManager::SceneManager():
	m_pScene(nullptr),
	m_pTempScene(nullptr)
{
	std::string file = "Data/Image/Fade/";
	std::string extension = ".png";
	
	for (int i = 0; i < 26; i++)
	{
		std::string name = std::to_string(i + 1);
		std::string a = file + name + extension;
		m_hFade[i] = LoadGraph(a.c_str());
	}

	m_handle[1] = LoadGraph("Data/Image/UI/�n.png");
	m_handle[0] = LoadGraph("Data/Image/UI/�n��.png");

	m_isReverce[1] = false;
	m_isReverce[0] = true;
	std::vector<float> test;
	for (int i = 0; i < kLoadObjectNum; i++)
	{
		m_x[0].push_back(GetRand(-500.0f));
		m_y[0].push_back(GetRand(Game::kScreenHeight));

		m_x[1].push_back(GetRand(-500.0f));
		m_y[1].push_back(GetRand(Game::kScreenHeight));

		m_rota.push_back((GetRand(70) - 30) * DX_PI_F / 180.0f);

		m_isRota.push_back(false);
	}

	m_isLoading = false;

//	LoadInit();

	//for (int i = 0; i < 26; i++)
	//{
	//	for (int j = 0; j < kLoadObjectNum; j++)
	//	{
	//		for (int k = 0; k < 2; k++)
	//		{
	//			size[k][j][i] = 1.0f;
	//		}
	//	}
	//}

	m_isFade = false;
}
SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
//	m_pScene.reset(new SceneDebug);
	m_pScene.reset(new SceneTitle);

	m_pScene->Init();
}

void SceneManager::End()
{
	assert(m_pScene);
	if (!m_pScene)	return;
	for (int i = 0; i < 26; i++)
	{
		DeleteGraph(m_hFade[i]);
	}

	m_pScene->End();
}

void SceneManager::Update()
{
	assert(m_pScene);
	if (!m_pScene)return;

#if _DEBUG
	LONGLONG start = GetNowHiPerformanceCount();
#endif

	// �R���g���[���[�̍X�V����
	Pad::Update();

	if (m_isFade)
	{
		// ���[�h������������
		InitFade();
		// �X�V����
		UpdateFade();

		return;
	}
	
	m_pTempScene = m_pScene->Update();

	if (m_pTempScene != m_pScene.get())
	{
		// �t�F�C�h���J�n����
		StartFade();

		// �O�̃V�[���̏I������
		m_pScene->End();

		// �V�[����ύX����
		m_pScene.reset(m_pTempScene);

		// ����������
		m_pScene->Init();
	}


	//if (m_isSceneSet)
	//{
	//	//// ���[�h������������
	//	//InitFade();
	//	// �O�̃V�[���̏I������
	//	m_pScene->End();
	//	// �V�[����ύX����
	//	m_pScene.reset(m_pTempScene);

	//	// ����������
	//	m_pScene->Init();

	//	m_isSceneSet = false;
	//}

	//// �X�V������L���ɂ���
	//if (m_isLoading)
	//{
	//	// �X�V����
	//	UpdateFade();
	//	return;
	//}

#if _DEBUG
	m_updateTime = GetNowHiPerformanceCount() - start;
#endif
}

void SceneManager::Draw()
{
	assert(m_pScene);
	if (!m_pScene)	return;

#if _DEBUG
	LONGLONG start = GetNowHiPerformanceCount();
#endif

	// �`��
	m_pScene->Draw();

	// �t�F�C�h��ʂ̕`��
	if (m_isLoading)
	{
		DrawFade();
	}

#if _DEBUG
	m_drawTime = GetNowHiPerformanceCount() - start;
	
	DrawString(0, Game::kScreenHeight - 48, "����", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kScreenHeight - 48 + 2, 48 + 16 - 2, Game::kScreenHeight - 32 - 2, 0x0000ff, true);

	DrawString(0, Game::kScreenHeight - 32, "�`��", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kScreenHeight - 32 + 2, 48 + 16 - 2, Game::kScreenHeight - 16 - 2, 0xff0000, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	int width = static_cast<int>(Game::kScreenWidth * rate);
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0x0000ff,true);

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<int>(Game::kScreenWidth * rate);
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0xff0000, true);
#endif
}

void SceneManager::StartFade()
{
	m_isFade = true;
}

void SceneManager::InitFade()
{
	if (!m_isLoading)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < m_x->size(); j++)
			{
				m_x[i][j] = GetRand(-500.0f);
				m_y[i][j] = GetRand(Game::kScreenHeight);
			}
		}
		m_isLoading = true;
	}
}

void SceneManager::UpdateFade()
{
	int count = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < kLoadObjectNum; j++)
		{
			if (m_rota[j] > 70 * DX_PI_F / 180.0f)
			{
				m_isRota[j] = true;
			}
			if (m_rota[j] < -70 * DX_PI_F / 180.0f)
			{
				m_isRota[j] = false;
			}

			if (m_isRota[j])
			{
				m_rota[j] -= 0.05f;
			}
			else
			{
				m_rota[j] += 0.05f;
			}

			m_x[i][j] += kLoadObjectSpeed;

			if (m_x[i][j] > Game::kScreenWidth)
			{
				count++;
			}
		}
	}

	if (count >= kLoadObjectNum)
	{
		m_isLoading = false;
		m_isSceneSet = true;
		m_isFade = false;
	}
}

void SceneManager::DrawFade()
{
	int x[26], y[26];

	////float resultPosX[2][kLoadObjectNum][26]{};
	////float resultPosY[2][kLoadObjectNum][26]{};

	for (int i = 0; i < 26; i++)
	{
		GetGraphSize(m_hFade[i], &x[i], &y[i]);

		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < kLoadObjectNum; k++)
			{
			//	size[j][k][i] += 0.00005f;
			//	m_x[j][k] = (m_x[j][k] + size[j][k][i]);
			//	resultPosX[j][k][i] = (-(static_cast<float>(x[i]) / 2.0f) + (m_x[j][k]));
			//	resultPosY[j][k][i] = (-(static_cast<float>(y[i]) / 2.0f) + (m_y[j][k]));

			//	DrawRotaGraph(m_x[j][k], m_y[j][k], 1, m_rota[k], m_handle[j], true, m_isReverce[j]);

			}
		}

	}

	m_animFadeFrameCount++;
	if (m_animFadeFrameCount == 3)
	{
		m_animFadeFrameCount = 0;
		m_animArray++;

		if (m_animArray == 26)
		{
			m_animArray = 0;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < kLoadObjectNum; j++)
		{
			for (int k = 0; k < 26; k++)
			{
			//	DrawRotaGraph(resultPosX[i][j][k], resultPosY[i][j][k], size[i][j][k], 0, m_hFade[m_animArray], true, true);
				DrawExtendGraph(m_x[i][j], m_y[i][j] - x[k], 0, m_y[i][j], m_hFade[m_animArray], true);
			//	DrawRotaGraph(m_x[i][j], m_y[i][j], 1, m_rota[j], m_handle[i], true, m_isReverce[i]);
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < kLoadObjectNum; j++)
		{
			DrawRotaGraph(m_x[i][j], m_y[i][j], 1, m_rota[j], m_handle[i], true, m_isReverce[i]);
		}
	}
}

