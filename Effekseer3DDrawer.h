#pragma once
#include <DxLib.h>

class Effekseer3DDrawer
{
public:
	Effekseer3DDrawer();
	~Effekseer3DDrawer();

	void Init();
	void End();
	void Update();
	void Draw();

	void SetPos(VECTOR pos);
private:
	int m_handle;
	int m_playingEffectHandle;

	int m_effectDrawTime;

	VECTOR m_pos;
};

