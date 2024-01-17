#include <DxLib.h>

#include "EnemyDataManager.h"

EnemyDataManager* EnemyDataManager::m_pInstance = nullptr;

void EnemyDataManager::Load(const char* fileNamePath, std::vector<std::vector<std::string>> data)
{
	// �t�@�C������1�������ǂݍ���
	std::string line;

	std::ifstream ifs = CreateOrOpen(fileNamePath, data);

	int count = 0;
	int mapKey = 0;
	while (getline(ifs, line))
	{
		// �w�b�_�[�����͓ǂݍ��܂Ȃ�
		count++;

		if (count < 2) continue;

		// �ǂݍ��񂾍s���J���}�ŕ�������
		std::vector<std::string> strvec = Split(line, ',');

		//// �f�[�^�ۊ�
		//m_data[mapKey].no = stoi(strvec.at(0));
		//m_data[mapKey].type = strvec.at(1);
		//// �f�[�^��ǂݍ���
		//m_data[mapKey].name = strvec[2].c_str();  // ���O
		//m_data[mapKey].volume = stoi(strvec.at(3)); // ����
		//m_data[mapKey].extension = strvec[4].c_str();  // �g���q

		// map�p�̂ɃL�[�����߂�
		mapKey++;
	}
}

void EnemyDataManager::Unload()
{
	printfDx("End");
}
