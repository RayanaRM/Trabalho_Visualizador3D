#pragma once
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class JsonParser
{
public:
	string FilePath;

	JsonParser(string filePath)
	{
		FilePath = filePath;
	}

	string getModelPath()
	{
		std::ifstream i(FilePath);
		json j;
		i >> j;

		string modelPath = j.value("modelPath", "");

		return modelPath;
	}
};

