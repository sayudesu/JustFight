#include "SceneDebug.h"
#include <DxLib.h>
#include "SceneMain.h";

SceneDebug::SceneDebug()
{
}

void SceneDebug::Init()
{
	
}

void SceneDebug::End()
{
	
}

SceneBase* SceneDebug::Update()
{
	return new SceneMain();
}

void SceneDebug::Draw()
{
	DrawString(0, 0, "SceneDebug", 0xffffff);
}

