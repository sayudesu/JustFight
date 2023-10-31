#include "EffekseerDrawer.h"
#include "EffekseerForDXLib.h"

EffekseerDrawer* EffekseerDrawer::m_pInstance = nullptr;

namespace
{
	struct EffekseerData
	{
		const char* fileName;
		float size;
	};

	const EffekseerData kEffectNo[Id::Max] =
	{
		// �ʏ�K�[�h�����ꍇ
		{
			"Data/Guard.efk",
			30.0f,
		},
		// �W���X�g�K�[�h�����ꍇ
		{
			"Data/Guard2.efk",
			130.0f,
		},
		// �X�^����Ԃ̏ꍇ
		{
			"Data/Hiyoko.efk",
			30.0f,
		},
	};
}

void EffekseerDrawer::Load()
{

	for (int i = 0; i < Id::Max; i++)
	{
		m_handle[i] = LoadEffekseerEffect(kEffectNo[i].fileName, kEffectNo[i].size);
	}
}

void EffekseerDrawer::Unload()
{
	for (int i = 0; i < Id::Max; i++)
	{
		// �G�t�F�N�g���\�[�X���폜����B
		DeleteEffekseerEffect(m_handle[i]);
	}
}

void EffekseerDrawer::Update()
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	for(auto& effect : m_effectData)
	{
		// �Đ��t���[���̃J�E���g
		effect.playFrameCount++;
		if (effect.playFrameCount < effect.playFrameMax)
		{
			if (IsEffekseer3DEffectPlaying(effect.playingHandle) == -1)
			{
				effect.playingHandle = PlayEffekseer3DEffect(m_handle[effect.id]);
			}
		}

		// �Đ����̃G�t�F�N�g���ړ�����B
		SetPosPlayingEffekseer3DEffect(effect.playingHandle, effect.pos.x, effect.pos.y, effect.pos.z);
		SetRotationPlayingEffekseer3DEffect(effect.playingHandle, effect.angle.x, effect.angle.y, effect.angle.z);

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();
	}

	//if (m_effectData.size() != 0)
	//{
	//	for (int i = 0; i < m_effectData.size(); i++)
	//	{
	//		if (m_effectData[i].playFrameCount >= m_effectData[i].playFrameMax)
	//		{
	//			m_effectData.erase(m_effectData.begin() + i);
	//			printfDx("erase\n");
	//		}
	//	}
	//}
}

void EffekseerDrawer::Draw()
{
	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}

void EffekseerDrawer::Play(int& playingEffectHandle, Id id, EffectPlayType type, VECTOR pos, VECTOR size, VECTOR angle, int playFrame)
{
	EffectData data{};

	playingEffectHandle = PlayEffekseer3DEffect(m_handle[id]);

	data.playingHandle = playingEffectHandle;
	data.id    = id;
	data.type  = type;
	data.pos   = pos;
	data.size  = size;
	data.angle = angle;
	data.playFrameMax   = playFrame;
	data.playFrameCount = 0;
	m_effectData.push_back(data);
}

void EffekseerDrawer::PlayLoop()
{
//	PlayEffekseer3DEffect(m_handle[effect.id]);
}
