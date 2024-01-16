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
			{"No","Name", "Volume", "Extension"},
			{"0","Click", "255", ".mp3"},
			{"1","Select", "255", ".mp3"},
			{"2","Create", "255", ".mp3"},
			{"3","Good", "255", ".mp3"},
			{"3","Bad", "255", ".mp3"},
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
			std::vector<std::string> strvec = split(line, ',');

			// データ保管
			m_data[mapKey].no        = stoi(strvec.at(0));
			m_data[mapKey].name      = strvec[1].c_str();
			m_data[mapKey].volume    = stoi(strvec.at(2));
			m_data[mapKey].extension = strvec[3].c_str();

			// ファイル位置
			std::string name = file + strvec[1] + strvec[3];
			m_handle.push_back(LoadSoundMem(name.c_str()));

			// mapにキーを決める
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

	void WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data)
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
	void Play(SoundName name)
	{
		PlaySoundMem(m_handle[name], DX_PLAYTYPE_BACK);
	}
}