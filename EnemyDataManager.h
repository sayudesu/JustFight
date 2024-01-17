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
	// コピーコンストラクタの禁止
	EnemyDataManager(const EnemyDataManager&) = delete;
	EnemyDataManager& operator=(const EnemyDataManager&) = delete;
	EnemyDataManager(EnemyDataManager&&) = delete;
	EnemyDataManager& operator=(EnemyDataManager&&) = delete;

	// インスタンスの作成
	static EnemyDataManager& GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new EnemyDataManager();
		}
		return *m_pInstance;
	}

	// 解放処理
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
private:
	// ロード
	void Load(const char* fileNamePath, std::vector<std::vector<std::string>> data);
	// アンロード
	void Unload();
private:
	enum Type
	{
		SE,
		BGM
	};
	// サウンド保存形式データ
	struct SoundData
	{
		int no;
		Type type;
		std::string name;
		int volume;
		std::string extension;
	};

	// サウンドのデータを格納
	std::map<int, SoundData> m_data;
};

