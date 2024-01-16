#pragma once
#include <vector>
#include <string>
#include <map>

#include "SoundName.h"

// �T�E���h�}�l�[�W���[�̃V���O���g�[���N���X

class SoundManager
{
private:
	SoundManager() = default;
	static SoundManager* m_pInstance;
public:
	virtual~SoundManager() = default;
	// �R�s�[�R���X�g���N�^�̋֎~
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	// �C���X�^���X�̍쐬
	static SoundManager& GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new SoundManager();
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

	std::vector<std::string> Split(std::string& input, char delimiter);

	// CSV�t�@�C���Ƀf�[�^���������ފ֐�
	void WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data);

	/// <summary>
	/// �T�E���h�̍Đ�
	/// </summary>
	/// <param name="name">�T�E���h�̖��O</param>
	/// <param name="repeat">�J��Ԃ��Đ����邩�ǂ���</param>
	void Play(SoundName name,bool repeat = false);


private:
	enum Type
	{
		SE,
		BGM
	};
	// �T�E���h�ۑ��`���f�[�^
	struct SoundData
	{
		int no;
		Type type;
		std::string name;
		int volume;
		std::string extension;
	};

	// �T�E���h�̃f�[�^���i�[
	std::map<int, SoundData> m_data;
	// �n���h��
	std::vector<int> m_handle;
};

