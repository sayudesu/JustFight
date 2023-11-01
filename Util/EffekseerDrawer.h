#pragma once
#include <array>
#include <map>
#include <DxLib.h>
#include "EffectId.h"
#include <vector>

class EffekseerDrawer
{
private:
	EffekseerDrawer() = default;
	static EffekseerDrawer* m_pInstance;
public:
	virtual~EffekseerDrawer() = default; 
	// �R�s�[�R���X�g���N�^�̋֎~
	EffekseerDrawer(const EffekseerDrawer&) = delete;
	EffekseerDrawer& operator=(const EffekseerDrawer&) = delete;
	EffekseerDrawer(EffekseerDrawer&&) = delete;
	EffekseerDrawer& operator=(EffekseerDrawer&&) = delete;

	// �C���X�^���X�̍쐬
	static EffekseerDrawer& GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new EffekseerDrawer;
		}
		return *m_pInstance;
	}

	// �������
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

public:
	// ���[�h
	void Load();
	// �A�����[�h
	void Unload();

	void Update();
	void Draw();

	// �G�t�F�N�V�A�̍Đ�
	void Play(int& playingEffectHandle,Id id,EffectPlayType type, VECTOR pos,VECTOR size, VECTOR angle,int playFrame = -1);
private:
	std::array<int,Id::Max> m_handle;

	// �G�t�F�N�g�Đ��p�n���h��
	int m_playingEffectHandle;

	std::vector<EffectData> m_effectData;
};

