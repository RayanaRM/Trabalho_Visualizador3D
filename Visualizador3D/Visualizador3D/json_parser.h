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

	vector <Objeto> getObjetos()
	{
		vector <Objeto> objetos;

		std::ifstream i(FilePath);
		nlohmann::json j;
		i >> j;

		for (int i = 0; i < j["objeto"].size(); i++) {
			glm::vec3 positionTranslate;
			positionTranslate[0] = j["objeto"][i]["positionTranslate"][0];
			positionTranslate[1] = j["objeto"][i]["positionTranslate"][1];
			positionTranslate[2] = j["objeto"][i]["positionTranslate"][2];

			objetos.push_back(Objeto((int)j["objeto"][i]["id"], (bool)j["objeto"][i]["isSelected"], (bool)j["objeto"][i]["hasCurves"], (float)j["objeto"][i]["escala"], positionTranslate, (string)j["objeto"][i]["path"]));
		}

		return objetos;
	}
};
