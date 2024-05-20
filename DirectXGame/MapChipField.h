#pragma once
#include <vector>
#include <Vector3.h>
#include <Vector3.h>
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum class MapChipType {
	kBlank,
	kBlock,
};

typedef struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
} MapChipData;

class MapChipField {
public:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const float kNumBlockVertical = 1.0f;
	static inline const float kNumBlockHorizontal = 1.0f;

	MapChipData mapChipData_;
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	
};





