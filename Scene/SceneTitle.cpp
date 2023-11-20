#include "SceneTitle.h"
#include <DxLib.h>
#include "SceneDebug.h"

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
}

void SceneTitle::End()
{
	DeleteGraph(m_hTitle);
}

SceneBase* SceneTitle::Update()
{
	if (DxLib::CheckHitKey(KEY_INPUT_Z))
	{
		return new SceneDebug();
	}
	return this;
}

void SceneTitle::Draw()
{
	DrawGraph(0, 0, m_hTitle, true);
#if _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}