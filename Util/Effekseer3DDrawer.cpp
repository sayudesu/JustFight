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
	// �G�t�F�N�g���\�[�X��ǂݍ��ށB
	m_handle = LoadEffekseerEffect(effectName, size);
}

void Effekseer3DDrawer::End()
{
	StopEffekseer3DEffect(m_handle);
	// �G�t�F�N�g���\�[�X���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	DeleteEffekseerEffect(m_handle);
}

void Effekseer3DDrawer::Update()
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	if (m_isEffectPlay)
	{
		// ����I�ɃG�t�F�N�g���Đ�����
		if (m_effectDrawTime % 60 == 0)
		{
			// �G�t�F�N�g���Đ�����B
			m_playingEffectHandle = PlayEffekseer3DEffect(m_handle);
		}
		// �Đ����̃G�t�F�N�g���ړ�����B
		SetPosPlayingEffekseer3DEffect(m_playingEffectHandle, m_pos.x, m_pos.y, m_pos.z);
		SetRotationPlayingEffekseer3DEffect(m_playingEffectHandle, 0, m_angle, 0);
		m_effectDrawTime++;
	}
	else
	{
		m_effectDrawTime = 0;
	}
	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();
}

void Effekseer3DDrawer::Draw()
{
//	static const int a = LoadGraph("Data/Background.png");
//	DrawExtendGraph(0, 0, 1920, 1080, a, true);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
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
