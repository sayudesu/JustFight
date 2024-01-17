#pragma once

#include <fstream>
#include <vector>
#include <string>

class CSVManagerBase
{
public:
	CSVManagerBase() {};
	virtual ~CSVManagerBase() {};
	
	virtual std::ifstream CreateOrOpen(const char* fileNamePath, std::vector<std::vector<std::string>> data);

	// CSVファイルにデータを書き込む関数
	virtual void WriteCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data);

	std::vector<std::string> Split(std::string& input, char delimiter);
};

