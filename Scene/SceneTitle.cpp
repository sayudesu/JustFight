#include "SceneTitle.h"
#include <DxLib.h>
#include "../SlideSelect.h"
#include "SceneDebug.h"
#include "SceneMain.h"
#include "../SlideSelect.h"

SceneTitle::SceneTitle():
	m_hTitle(-1)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_hTitle = LoadGraph("Data/Image/Title.png");

	m_select = std::make_unique<SlideSelect>();
	m_select->Init();
}

void SceneTitle::End()
{
	DeleteGraph(m_hTitle);
}

SceneBase* SceneTitle::Update()
{
	m_select->Update();

	if (m_select->GetResult() == 0)
	{
		printfDx("‚Ü‚¾‚ ‚è‚Ü‚¹‚ñ\n");
	}

	if (m_select->GetResult() == 1)
	{
		return new SceneMain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_Z))
	{
		return new SceneDebug();
	}

	return this;
}

void SceneTitle::Draw()
{
	m_select->Draw();

	DrawGraph(0, 0, m_hTitle, true);
#if _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}