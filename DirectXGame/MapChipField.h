#pragma once
#include <vector>
#include <Vector3.h>
#include<cstdint>
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

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVertical = 70;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	

	uint32_t GetNumBlockVertical();

	uint32_t GetNumBlockHorizontal();

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndexSet(uint32_t xIndex, uint32_t yIndex);
};





