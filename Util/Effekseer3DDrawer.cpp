#include "Effekseer3DDrawer.h"
#include "EffekseerForDXLib.h"

Effekseer3DDrawer::Effekseer3DDrawer():
	m_pos(VGet(0,0,0)),
	m_handle(-1),
	m_playingEffectHandle(-1),
	m_effectDrawTime(0),
	m_angle(0)
{
}

Effekseer3DDrawer::~Effekseer3DDrawer()
{
}

void Effekseer3DDrawer::Init(const char* effectName,float size)
{
	// エフェクトリソースを読み込む。
	m_handle = LoadEffekseerEffect(effectName, size);
}

void Effekseer3DDrawer::End()
{
	StopEffekseer3DEffect(m_handle);
	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(m_handle);
}

void Effekseer3DDrawer::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	if (m_isEffectPlay)
	{
		// 定期的にエフェクトを再生する
		if (m_effectDrawTime % 60 == 0)
		{
			// エフェクトを再生する。
			m_playingEffectHandle = PlayEffekseer3DEffect(m_handle);
		}
		// 再生中のエフェクトを移動する。
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y, m_pos.z);
		SetRotationPlayingEffekseer3DEffect(m_playingEffectHandle, 0, m_angle, 0);
		m_effectDrawTime++;
	}
	else
	{
		m_effectDrawTime = 0;
	}
	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void Effekseer3DDrawer::Draw()
{
//	static const int a = LoadGraph("Data/Background.png");
//	DrawExtendGraph(0, 0, 1920, 1080, a, true);

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

int Effekseer3DDrawer::IsPlay()const
{
	return IsEffekseer3DEffectPlaying(m_playingEffectHandle);
}

void Effekseer3DDrawer::SetPos(VECTOR pos)
{
	m_pos = pos;
}

void Effekseer3DDrawer::SetPlay(bool play)
{
	m_isEffectPlay = play;
}

void Effekseer3DDrawer::SetAngle(float angle)
{
	m_angle = angle;
}
