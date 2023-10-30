#pragma once
#include <DxLib.h>

class Effekseer3DDrawer
{
public:
	Effekseer3DDrawer();
	~Effekseer3DDrawer();

	void Init(const char* effectName, float size);
	void End();
	void Update();
	void Draw();
public:
	// 現在再生されているかどうか
	int IsPlay()const;

	void SetPos(VECTOR pos);

	void SetPlay(bool play);

	void SetAngle(float angle);
private:
	int m_handle;
	int m_playingEffectHandle;

	int m_effectDrawTime;

	int m_isEffectPlay;

	VECTOR m_pos;

	float m_angle;
};

