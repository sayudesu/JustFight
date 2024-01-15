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
	// フォルダを作成するディレクトリのパス
	const wchar_t* folderPath = L"Data/CSV";

	// フォルダを作成
	if (!CreateDirectoryW(folderPath, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		// フォルダの作成に失敗した場合の処理
		// 既にフォルダが存在する場合		
	}

	// ファイルを開く
	std::ifstream ifs("Data/CSV/SoundData.csv");

	// ファイルが存在しない場合は作成する
	if (!ifs.is_open())
	{
		std::ofstream ofs("Data/CSV/SoundData.csv");

		// ファイルを即座に閉じる
		ofs.close();

		// 作成したファイルを再度開く
		ifs.open("Data/CSV/SoundData.csv");
	}

	// 書き込むデータ
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

	// CSVファイルにデータを書き込む
	WriteCSV("Data/CSV/SoundData.csv", data);

	// ファイルから1文字ずつ読み込む
	std::string line;

	int count = 0;
	int mapKey = 0;
	while (getline(ifs, line))
	{
		// ヘッダー部分は読み込まない
		count++;

		if (count < 2) continue;

		// 読み込んだ行をカンマで分割する
		std::vector<std::string> strvec = Split(line, ',');

		// データ保管
		m_data[mapKey].no = stoi(strvec.at(0));

		// 文字列でSEかBGMを分ける
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

		// ファイル位置
		std::string name = kFile + strvec[2] + strvec[4];
		// サウンドのメモリ読み込み
		m_handle.push_back(LoadSoundMem(name.c_str()));
		// サウンドの音量を指定
		ChangeVolumeSoundMem(m_data[mapKey].volume, m_handle.back());

		// mapにキーを決める
		mapKey++;
	}
}

void SoundManager::Unload()
{
	// 読み込んだサウンドを削除処理
	InitSoundMem();
}

std::vector<std::string> SoundManager::Split(std::string& input, char delimiter)
{
	// 入力文字列をストリームに変換
	std::istringstream stream(input);

	// 分割された各フィールドを格納するための変数
	std::string field;

	// 分割された文字列を格納するベクター
	std::vector<std::string> result;

	// ストリームから1つずつフィールドを取り出し、ベクターに追加
	while (getline(stream, field, delimiter))
	{
		result.push_back(field);
	}

	// 分割された文字列のベクターを返す
	return result;
}

void SoundManager::WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data)
{
	std::ofstream ofs(filename);

	// ファイルが正しく開かれたか確認
	if (!ofs.is_open())
	{
		return;
	}

	// データをCSV形式でファイルに書き込む
	for (const auto& row : data)
	{
		for (size_t i = 0; i < row.size(); ++i)
		{
			ofs << row[i];
			if (i != row.size() - 1)
			{
				// カンマでセルを区切る
				ofs << ",";
			}
		}

		// 改行で行を区切る
		ofs << "\n";
	}

	// ファイルを閉じる
	ofs.close();
}

// サウンドを再生
void SoundManager::Play(SoundName name, bool repeat)
{
	if (!repeat)
	{
		PlaySoundMem(m_handle[name], DX_PLAYTYPE_BACK);
	}
	else
	{
		// 再生していなかったら
		if (!CheckSoundMem(m_handle[name]))
		{
			PlaySoundMem(m_handle[name], DX_PLAYTYPE_BACK);
		}
	}
}
