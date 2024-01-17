#include <DxLib.h>

#include "EnemyDataManager.h"

EnemyDataManager* EnemyDataManager::m_pInstance = nullptr;

void EnemyDataManager::Load(const char* fileNamePath, std::vector<std::vector<std::string>> data)
{
	// ファイルから1文字ずつ読み込む
	std::string line;

	std::ifstream ifs = CreateOrOpen(fileNamePath, data);

	int count = 0;
	int mapKey = 0;
	while (getline(ifs, line))
	{
		// ヘッダー部分は読み込まない
		count++;

		if (count < 2) continue;

		// 読み込んだ行をカンマで分割する
		std::vector<std::string> strvec = Split(line, ',');

		//// データ保管
		//m_data[mapKey].no = stoi(strvec.at(0));
		//m_data[mapKey].type = strvec.at(1);
		//// データを読み込む
		//m_data[mapKey].name = strvec[2].c_str();  // 名前
		//m_data[mapKey].volume = stoi(strvec.at(3)); // 音量
		//m_data[mapKey].extension = strvec[4].c_str();  // 拡張子

		// map用のにキーを決める
		mapKey++;
	}
}

void EnemyDataManager::Unload()
{
	printfDx("End");
}
