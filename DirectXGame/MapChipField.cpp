#include "MapChipField.h"


namespace {
std::map<std::string, MapChipType> mapChipTable = {

    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
};

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
};

void MapChipField::LoadMapChipCsv(const std::string& filePath){
	//RESET
	ResetMapChipData();
	//OPEN FILE
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	//MAPCHIP CSV
	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	//CLOSE FILE
	file.close();
	//DATA FROM CSI
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
};

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];

};

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex){
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0);
};


