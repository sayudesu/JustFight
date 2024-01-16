#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <map>
#include <windows.h>
#include <iostream>

#include "SoundManager.h"


namespace
{
	std::string file = "Data/Sound/";
}

namespace
{
	struct SoundData
	{
		int no;
		std::string name;
		int volume;
		std::string extension;
	};

	std::map<int, SoundData> m_data;

	std::vector<int> m_handle;
}

namespace SoundManager
{
	void Init()
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
			{"No","Name", "Volume", "Extension"},
			{"0","Click", "255", ".mp3"},
			{"1","Select", "255", ".mp3"},
			{"2","Create", "255", ".mp3"},
			{"3","Good", "255", ".mp3"},
			{"3","Bad", "255", ".mp3"},
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
			std::vector<std::string> strvec = split(line, ',');

			// �f�[�^�ۊ�
			m_data[mapKey].no        = stoi(strvec.at(0));
			m_data[mapKey].name      = strvec[1].c_str();
			m_data[mapKey].volume    = stoi(strvec.at(2));
			m_data[mapKey].extension = strvec[3].c_str();

			// �t�@�C���ʒu
			std::string name = file + strvec[1] + strvec[3];
			m_handle.push_back(LoadSoundMem(name.c_str()));

			// map�ɃL�[�����߂�
			mapKey++;
		}
	}

	void End()
	{

	}
	void Update()
	{

	}

	std::vector<std::string> split(std::string& input, char delimiter)
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

	void WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data)
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
	void Play(SoundName name)
	{
		PlaySoundMem(m_handle[name], DX_PLAYTYPE_BACK);
	}
}