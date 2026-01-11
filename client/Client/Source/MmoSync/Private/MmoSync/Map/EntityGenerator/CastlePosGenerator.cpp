// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Map/EntityGeneration/CastlePosGenerator.h"

#include "MmoSync/Grid/MultiGrid.h"
#include "MmoSync/Util/EntityUtil/EntityUtil.h"
#include "MmoSync/Util/GridUtil/GridUtil.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"
#include "Common/Random/DeterministicRandom/DeterministicRandom64.h"

#include <algorithm>
#include <queue>
#include <set>

using namespace MmoSync::Map::TerrainLevel;

CastlePosGenerator::CastlePosGenerator(const MultiGrid& grid, std::deque<ColRowPair>& posList)
	: mGrid(grid), mPosList(posList)
{
}

void CastlePosGenerator::InitCastlePositions(int castlePairCount, const AsciiMap& asciiMap, uint64_t randomSeed)
{
	// Use deterministic random for consistent results
	DeterministicRandom64 random(randomSeed);

	// 필드 맵용 캐슬 위치 생성
	const int mapWidth = asciiMap.Width();
	const int mapHeight = asciiMap.Height();
	const int64_t minPairDistance = 6;  // 쌍 내부 최소 거리
	const int64_t maxPairDistance = 12; // 쌍 내부 최대 거리
	const int64_t minPairDistanceSq = MathUtil::Pow(minPairDistance);
	const int64_t maxPairDistanceSq = MathUtil::Pow(maxPairDistance);
	// 기존 지점들과의 최소 분리 거리: maxPairDistance 이상이어야 함
	// 새 쌍의 두 지점(최대 거리 maxPairDistance)이 생성되어도 기존 지점과 겹치지 않도록 보장
	const int64_t minSeparationDistance = maxPairDistance; // 기존 지점들과의 최소 분리 거리
	const int64_t minSeparationDistanceSq = MathUtil::Pow(minSeparationDistance);

	for (int i = 0; i < castlePairCount; ++i)
	{
		GridCol c0 = 0;
		GridCol c1 = 0;
		GridRow r0 = 0;
		GridRow r1 = 0;
		int attempts = 0;
		const int maxAttempts = 500; // 충분한 시도 횟수
		bool pairFound = false;

		// 순차적으로 쌍을 찾기: 조건을 만족하지 않으면 다른 랜덤 지점을 평가
		while (attempts < maxAttempts && !pairFound)
		{
			// 첫 번째 캐슬 위치 찾기 (낮은 레벨 지형만)
			c0 = static_cast<GridCol>(random(mapWidth));
			r0 = static_cast<GridRow>(random(mapHeight));
			
			if (!IsLowLevelTerrain(c0, r0, asciiMap))
			{
				attempts++;
				continue;
			}

			// 기존 지점들과의 최소 분리 거리 체크 (mCastlePositions + mPosList 모두 체크)
			bool tooCloseToExisting = false;
			
			// 이미 배치된 Castle 위치 체크
			for (const auto& existingPos : mCastlePositions)
			{
				const int64_t deltaCol = static_cast<int64_t>(c0) - static_cast<int64_t>(existingPos.first);
				const int64_t deltaRow = static_cast<int64_t>(r0) - static_cast<int64_t>(existingPos.second);
				const int64_t distanceSq = MathUtil::DistanceSq(deltaCol, deltaRow);
				
				if (distanceSq < minSeparationDistanceSq)
				{
					tooCloseToExisting = true;
					break;
				}
			}
			
			// 이미 배치된 다른 entity (Nest 등) 위치 체크
			if (!tooCloseToExisting)
			{
				for (const auto& existingPos : mPosList)
				{
					const int64_t deltaCol = static_cast<int64_t>(c0) - static_cast<int64_t>(existingPos.first);
					const int64_t deltaRow = static_cast<int64_t>(r0) - static_cast<int64_t>(existingPos.second);
					const int64_t distanceSq = MathUtil::DistanceSq(deltaCol, deltaRow);
					
					if (distanceSq < minSeparationDistanceSq)
					{
						tooCloseToExisting = true;
						break;
					}
				}
			}

			if (tooCloseToExisting)
			{
				attempts++;
				continue;
			}

			// 두 번째 캐슬 위치 찾기: 첫 번째 지점 기준 최소/최대 거리 범위 내에서 랜덤하게 선택
			// 랜덤 각도 선택 (0 ~ 360도)
			const Angle randomAngle = MathUtil::GetRandomAngle([&random](int a, int b) -> int {
				return static_cast<int>(random(static_cast<int64_t>(a), static_cast<int64_t>(b)));
			});
			
			// 랜덤 거리 선택 (minPairDistance ~ maxPairDistance)
			const int64_t randomDistance = random(minPairDistance, maxPairDistance);
			
			// 각도에서 방향 벡터 구하기 (1024 스케일)
			auto [cosVec, sinVec] = MathUtil::GetVec1024FromAngle(randomAngle);
			
			// 방향 벡터에 거리를 곱하고 1024로 나누어 실제 델타 계산
			const int64_t deltaCol = (cosVec * randomDistance) / 1024;
			const int64_t deltaRow = (sinVec * randomDistance) / 1024;
			
			// 두 번째 지점 좌표 계산
			c1 = static_cast<GridCol>(static_cast<int64_t>(c0) + deltaCol);
			r1 = static_cast<GridRow>(static_cast<int64_t>(r0) + deltaRow);

			// 맵 경계 체크
			if (c1 < 0 || r1 < 0 || c1 >= mapWidth || r1 >= mapHeight)
			{
				attempts++;
				continue;
			}

			// 지형 레벨 체크
			if (!IsLowLevelTerrain(c1, r1, asciiMap))
			{
				attempts++;
				continue;
			}

			// 실제 거리 제곱 계산 및 범위 체크 (반올림 오차 고려)
			const int64_t actualDeltaCol = static_cast<int64_t>(c0) - static_cast<int64_t>(c1);
			const int64_t actualDeltaRow = static_cast<int64_t>(r0) - static_cast<int64_t>(r1);
			const int64_t pairDistanceSq = MathUtil::DistanceSq(actualDeltaCol, actualDeltaRow);

			// 거리 제곱이 범위 안에 있어야 함 (minPairDistanceSq <= pairDistanceSq <= maxPairDistanceSq)
			if (pairDistanceSq < minPairDistanceSq || pairDistanceSq > maxPairDistanceSq)
			{
				attempts++;
				continue;
			}

			// 두 번째 지점도 기존 지점들과의 최소 분리 거리 체크 (mCastlePositions + mPosList 모두 체크)
			tooCloseToExisting = false;
			
			// 이미 배치된 Castle 위치 체크
			for (const auto& existingPos : mCastlePositions)
			{
				const int64_t deltaCol2 = static_cast<int64_t>(c1) - static_cast<int64_t>(existingPos.first);
				const int64_t deltaRow2 = static_cast<int64_t>(r1) - static_cast<int64_t>(existingPos.second);
				const int64_t distanceSq2 = MathUtil::DistanceSq(deltaCol2, deltaRow2);
				
				if (distanceSq2 < minSeparationDistanceSq)
				{
					tooCloseToExisting = true;
					break;
				}
			}
			
			// 이미 배치된 다른 entity (Nest 등) 위치 체크
			if (!tooCloseToExisting)
			{
				for (const auto& existingPos : mPosList)
				{
					const int64_t deltaCol2 = static_cast<int64_t>(c1) - static_cast<int64_t>(existingPos.first);
					const int64_t deltaRow2 = static_cast<int64_t>(r1) - static_cast<int64_t>(existingPos.second);
					const int64_t distanceSq2 = MathUtil::DistanceSq(deltaCol2, deltaRow2);
					
					if (distanceSq2 < minSeparationDistanceSq)
					{
						tooCloseToExisting = true;
						break;
					}
				}
			}

			if (tooCloseToExisting)
			{
				attempts++;
				continue;
			}

			// 경로 존재 및 교차 검사 (IsValidCastlePair 활용)
			if (!IsValidCastlePair(c0, r0, c1, r1, asciiMap))
			{
				attempts++;
				continue;
			}

			// 모든 조건을 만족하는 쌍을 찾음
			const ColRowPair cell0 = mGrid.GetCell(c0, r0);
			const ColRowPair cell1 = mGrid.GetCell(c1, r1);

			// 위치 정보 저장
			mCastlePositions.push_back(cell0);
			mCastlePositions.push_back(cell1);

			// 위치 리스트에 추가
			mPosList.push_back({c0, r0});
			mPosList.push_back({c1, r1});

			// 경로 저장 (교차 검사를 위해)
			mCastlePaths.push_back({{c0, r0}, {c1, r1}});

			pairFound = true;
		}

		// maxAttempts 내에 유효한 쌍을 찾지 못하면 건너뛰기
		if (!pairFound)
			continue;
	}
}

bool CastlePosGenerator::IsLowLevelTerrain(GridCol col, GridRow row, const AsciiMap& asciiMap)
{
	// 맵 경계 검사
	if (col < 0 || row < 0 || col >= asciiMap.Width() || row >= asciiMap.Height())
		return false;
	
	// 캐슬 생성 가능한 지형인지 조사 (LEVEL0, LEVEL1, LEVEL2만 허용)
	// wchar_t와 TerrainLevel 상수는 ASCII 값이므로 직접 비교하면 안 됨
	wchar_t tile = asciiMap.Get(col, row);
	return (tile == LEVEL0 || tile == LEVEL1 || tile == LEVEL2);
}

bool CastlePosGenerator::HasValidPath(GridCol fromCol, GridRow fromRow, GridCol toCol, GridRow toRow, const AsciiMap& asciiMap)
{
	// 간단한 BFS 경로 찾기
	const int mapWidth = asciiMap.Width();
	const int mapHeight = asciiMap.Height();
	
	// 방문 체크용 맵
	std::vector<std::vector<bool>> visited(mapHeight, std::vector<bool>(mapWidth, false));
	
	// 4방향 이동
	const int dx[] = {1, -1, 0, 0};
	const int dy[] = {0, 0, 1, -1};
	
	std::queue<std::pair<int, int>> q;
	q.push({fromCol, fromRow});
	visited[fromRow][fromCol] = true;
	
	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();
		
		// 목적지에 도달했으면 경로 존재
		if (x == toCol && y == toRow)
			return true;
		
		// 4방향 탐색
		for (int i = 0; i < 4; ++i)
		{
			int nx = x + dx[i];
			int ny = y + dy[i];
			
			// 경계 검사
			if (nx < 0 || ny < 0 || nx >= mapWidth || ny >= mapHeight)
				continue;
			
			// 이미 방문했으면 스킵
			if (visited[ny][nx])
				continue;
			
			// 가능한 지형이 아니면 스킵
			if (!IsLowLevelTerrain(GridCol(nx), GridRow(ny), asciiMap))
				continue;
			
			visited[ny][nx] = true;
			q.push({nx, ny});
		}
	}
	
	return false; // 경로를 찾지 못함
}

bool CastlePosGenerator::PathsIntersect(
	const ColRowPair& path1Start, const ColRowPair& path1End, 
	const ColRowPair& path2Start, const ColRowPair& path2End
)
{
	// 간단한 선분 교차 검사 (Bresenham 알고리즘 기반)
	// 실제로는 더 정교한 교차 검사가 필요할 수 있음
	
	// 두 선분이 같은 점을 공유하는지 검사
	if ((path1Start == path2Start) || (path1Start == path2End) || 
	    (path1End == path2Start) || (path1End == path2End))
		return true;
	
	// 간단한 교차 검사: 두 선분의 중점이 너무 가까우면 교차로 간주
	int mid1X = (path1Start.first + path1End.first) / 2;
	int mid1Y = (path1Start.second + path1End.second) / 2;
	int mid2X = (path2Start.first + path2End.first) / 2;
	int mid2Y = (path2Start.second + path2End.second) / 2;
	
	int distanceSq = (mid1X - mid2X) * (mid1X - mid2X) + (mid1Y - mid2Y) * (mid1Y - mid2Y);
	const int minDistanceSq = 4; // 최소 거리 제곱
	
	return distanceSq < minDistanceSq;
}

bool CastlePosGenerator::IsValidCastlePair(GridCol c0, GridRow r0, GridCol c1, GridRow r1, const AsciiMap& asciiMap)
{
	// 경로 존재 검사
	if (!HasValidPath(c0, r0, c1, r1, asciiMap))
		return false;
	
	// 기존 경로들과 교차 검사
	for (const auto& existingPath : mCastlePaths)
	{
		if (PathsIntersect({c0, r0}, {c1, r1}, existingPath.first, existingPath.second))
			return false;
	}
	
	return true;
}
