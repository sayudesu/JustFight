#include "SceneGameOver.h"
#include "../Util/Pad.h"
#include <DxLib.h>
#include "SceneTitle.h"

SceneGameOver::SceneGameOver()
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

void SceneGameOver::End()
{
}

SceneBase* SceneGameOver::Update()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		return new SceneTitle();
	}

	return this;
}

void SceneGameOver::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SceneGameOver", 0xffffff);
#endif
}
