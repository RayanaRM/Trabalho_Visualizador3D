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

	void helloWorld()
	{
		cout << FilePath;
	}
};

