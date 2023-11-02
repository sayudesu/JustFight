#include "SceneClear.h"
#include "SceneTitle.h"
#include "../Util/Pad.h"
#include <DxLib.h>

SceneClear::SceneClear()
{
}

SceneClear::~SceneClear()
{
}

void SceneClear::Init()
{
}

void SceneClear::End()
{
}

SceneBase* SceneClear::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		return new SceneTitle();
	}

	return this;
}

void SceneClear::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SceneClear",0xffffff);
#endif
}
