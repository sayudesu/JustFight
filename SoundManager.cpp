#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iostream>

#include "SoundManager.h"

SoundManager* SoundManager::m_pInstanceSound = nullptr;

namespace
{
	const std::string kFile = "Data/Sound/";
}

void SoundManager::Load()
{
	// �t�H���_���쐬����f�B���N�g���̃p�X
	const wchar_t* folderPath = L"Data/CSV";

	// �t�H���_���쐬
	if (!CreateDirectoryW(folderPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		// �t�H���_�̍쐬�Ɏ��s�����ꍇ�̏���
		// ���Ƀt�H���_�����݂���ꍇ		
	}

	// �t�@�C�����J��
	std::ifstream ifs("Data/CSV/SoundData.csv");

	// �t�@�C�������݂��Ȃ��ꍇ�͍쐬����
	if (!ifs.is_open())
	{
		std::ofstream ofs("Data/CSV/SoundData.csv");

		// �t�@�C���𑦍��ɕ���
		ofs.close();

		// �쐬�����t�@�C�����ēx�J��
		ifs.open("Data/CSV/SoundData.csv");
	}

	// �������ރf�[�^
	std::vector<std::vector<std::string>> data =
	{
		{"No","Type","Name", "Volume", "Extension"},
		{"0","SE","Attack", "255", ".mp3"},
		{"1","SE","Guard", "255", ".mp3"},
		{"2","SE","GuardResult", "255", ".mp3"},
		{"3","SE","JustGurad", "255", ".mp3"},
		{"4","SE","SpeedMove2", "255", ".mp3"},
		{"5","SE","Stun", "255", ".mp3"},
		{"6","SE","Damage", "255", ".mp3"},
	};

	// CSV�t�@�C���Ƀf�[�^����������
	WriteCSV("Data/CSV/SoundData.csv", data);

	// �t�@�C������1�������ǂݍ���
	std::string line;

	int count = 0;
	int mapKey = 0;
	while (getline(ifs, line))
	{
		// �w�b�_�[�����͓ǂݍ��܂Ȃ�
		count++;

		if (count < 2) continue;

		// �ǂݍ��񂾍s���J���}�ŕ�������
		std::vector<std::string> strvec = Split(line, ',');

		// �f�[�^�ۊ�
		m_data[mapKey].no = stoi(strvec.at(0));

		// �������SE��BGM�𕪂���
		if (strvec.at(1) == "SE")
		{
			m_data[mapKey].type = Type::SE;
		}
		else if(strvec.at(1) == "BGM")
		{
			m_data[mapKey].type = Type::BGM;
		}

		m_data[mapKey].name = strvec[2].c_str();
		m_data[mapKey].volume = stoi(strvec.at(3));
		m_data[mapKey].extension = strvec[4].c_str();

		// �t�@�C���ʒu
		std::string name = kFile + strvec[2] + strvec[4];
		// �T�E���h�̃������ǂݍ���
		m_handle.push_back(LoadSoundMem(name.c_str()));
		// �T�E���h�̉��ʂ��w��
		ChangeVolumeSoundMem(m_data[mapKey].volume, m_handle.back());

		// map�ɃL�[�����߂�
		mapKey++;
	}
}

void SoundManager::Unload()
{
	// �ǂݍ��񂾃T�E���h���폜����
	InitSoundMem();
}

std::vector<std::string> SoundManager::Split(std::string& input, char delimiter)
{
	// ���͕�������X�g���[���ɕϊ�
	std::istringstream stream(input);

	// �������ꂽ�e�t�B�[���h���i�[���邽�߂̕ϐ�
	std::string field;

	// �������ꂽ��������i�[����x�N�^�[
	std::vector<std::string> result;

	// �X�g���[������1���t�B�[���h�����o���A�x�N�^�[�ɒǉ�
	while (getline(stream, field, delimiter))
	{
		result.push_back(field);
	}

	// �������ꂽ������̃x�N�^�[��Ԃ�
	return result;
}

void SoundManager::WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data)
{
	std::ofstream ofs(filename);

	// �t�@�C�����������J���ꂽ���m�F
	if (!ofs.is_open())
	{
		return;
	}

	// �f�[�^��CSV�`���Ńt�@�C���ɏ�������
	for (const auto& row : data)
	{
		for (size_t i = 0; i < row.size(); ++i)
		{
			ofs << row[i];
			if (i != row.size() - 1)
			{
				// �J���}�ŃZ������؂�
				ofs << ",";
			}
		}

		// ���s�ōs����؂�
		ofs << "\n";
	}

	// �t�@�C�������
	ofs.close();
}

// �T�E���h���Đ�
void SoundManager::Play(SoundName name, bool repeat)
{
	if (!repeat)
	{
		PlaySoundMem(m_handle[name], DX_PLAYTYPE_BACK);
	}
	else
	{
		// �Đ����Ă��Ȃ�������
		if (!CheckSoundMem(m_handle[name]))
		{
			PlaySoundMem(m_handle[name], DX_PLAYTYPE_BACK);
		}
	}
}
