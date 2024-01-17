#pragma once

#include <map>
#include <vector>
#include <string>

#include "CSVManagerBase.h"

class EnemyDataManager final : public CSVManagerBase
{
private:
	EnemyDataManager() = default;
	static EnemyDataManager* m_pInstance;
public:
	virtual~EnemyDataManager() = default;
	// �R�s�[�R���X�g���N�^�̋֎~
	EnemyDataManager(const EnemyDataManager&) = delete;
	EnemyDataManager& operator=(const EnemyDataManager&) = delete;
	EnemyDataManager(EnemyDataManager&&) = delete;
	EnemyDataManager& operator=(EnemyDataManager&&) = delete;

	// �C���X�^���X�̍쐬
	static EnemyDataManager& GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new EnemyDataManager();
		}
		return *m_pInstance;
	}

	// �������
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
private:
	// ���[�h
	void Load(const char* fileNamePath, std::vector<std::vector<std::string>> data);
	// �A�����[�h
	void Unload();
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
};

