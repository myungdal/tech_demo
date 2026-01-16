// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/GridFlow/Internal/GridFlowDef.h"


class Grid;

class GridFlowChunk
{
private:
	int64_t mWidth = 0; // U축 크기
	int64_t mHeight = 0; // V축 크기

	int64_t mOffsetU = 0; // 오프셋
	int64_t mOffsetV = 0; // 오프셋

	//GridBound mGridBound; // 그리드 바운드

	GridFlowDirTable mDirTable;

private:
	GridFlowTaskMap mTaskMapDraft;
	GridFlowTaskMap mTaskMap;

private:
	uint64_t mVersion = 0;
	uint64_t mDraftVersion = 0;

public:
	explicit GridFlowChunk(int64_t width, int64_t height, int64_t offsetU, int64_t offsetV);
	virtual ~GridFlowChunk();

private:
	inline size_t GetIndex(int64_t u, int64_t v) const noexcept { return (v * mWidth + u); }

public:
	inline void IncreaseVersion() noexcept { ++mVersion; }

	inline int64_t GetWidth() const noexcept { return mWidth; }
	inline int64_t GetHeight() const noexcept { return mHeight; }

	inline int64_t GetOffsetU() const noexcept { return mOffsetU; }
	inline int64_t GetOffsetV() const noexcept { return mOffsetV; }

	//inline const GridBound& GetGridBound() const noexcept { return mGridBound; }

	GridFlowTaskMap& RefTaskMapDraft() { return mTaskMapDraft; }
	GridFlowTaskMap& RefTaskMap() { return mTaskMap; }

public:
	std::pair<GridFlowMode, GridFlowDir> CalcFlowDir(Grid& grid, int64_t fromU, int64_t fromV, int64_t toU, int64_t toV);
	
	// 디버깅 용 렌더링 사용
	std::pair<GridFlowMode, GridFlowDir> GetFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV) const;
   
private:
	friend class GridFlowBuilder;
	void ClearFlowDirs(int64_t toU, int64_t toV);
	void SetFlowDir(int64_t fromU, int64_t fromV, int64_t toU, int64_t toV, GridFlowDir dir);
};
