#include "Effekseer3DDrawer.h"
#include "EffekseerForDXLib.h"

Effekseer3DDrawer::Effekseer3DDrawer():
	m_pos(VGet(0,0,0)),
	m_handle(-1),
	m_playingEffectHandle(-1),
	m_effectDrawTime(0)
{
}

Effekseer3DDrawer::~Effekseer3DDrawer()
{
}

void Effekseer3DDrawer::Init()
{
	// エフェクトリソースを読み込む。
	m_handle = LoadEffekseerEffect("laser.efk");
}

void Effekseer3DDrawer::End()
{
	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	DeleteEffekseerEffect(m_handle);
}

void Effekseer3DDrawer::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// 定期的にエフェクトを再生する
	if (m_effectDrawTime % 60 == 0)
	{
		// エフェクトを再生する。
		m_playingEffectHandle = PlayEffekseer3DEffect(m_handle);
	}
	// 再生中のエフェクトを移動する。
	SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y, m_pos.z);
	m_effectDrawTime++;
}

void Effekseer3DDrawer::Draw()
{
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void Effekseer3DDrawer::SetPos(VECTOR pos)
{
	m_pos = pos;
}
