#include "SceneTitle.h"
#include <DxLib.h>
#include "SceneDebug.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::End()
{
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
#if _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}