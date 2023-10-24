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
	// return	0:Ä¶’†A - 1 : Ä¶‚³‚ê‚Ä‚¢‚È‚¢A‚à‚µ‚­‚ÍÄ¶I—¹
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

