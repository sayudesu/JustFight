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
	// �G�t�F�N�g���\�[�X��ǂݍ��ށB
	m_handle = LoadEffekseerEffect("laser.efk");
}

void Effekseer3DDrawer::End()
{
	// �G�t�F�N�g���\�[�X���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	DeleteEffekseerEffect(m_handle);
}

void Effekseer3DDrawer::Update()
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	// ����I�ɃG�t�F�N�g���Đ�����
	if (m_effectDrawTime % 60 == 0)
	{
		// �G�t�F�N�g���Đ�����B
		m_playingEffectHandle = PlayEffekseer3DEffect(m_handle);
	}
	// �Đ����̃G�t�F�N�g���ړ�����B
	SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y, m_pos.z);
	m_effectDrawTime++;
}

void Effekseer3DDrawer::Draw()
{
	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}

void Effekseer3DDrawer::SetPos(VECTOR pos)
{
	m_pos = pos;
}
