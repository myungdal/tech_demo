// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <variant>
#include <optional>
#include "MmoSync/Map/Configuration/FieldMapConfig.h"
#include "MmoSync/Map/Configuration/DungeonMapConfig.h"
#include "MmoSync/Map/Visualization/AsciiConstants.h"

// ASCII 문자 기반 통합 맵 클래스
// DungeonMap과 FieldMap의 기능을 통합
class AsciiMap
{
private:
	// 기본 그리드 데이터
	int mW = 0;
	int mH = 0;
	std::vector<wchar_t> mGrid; // size = mW * mH

	// 맵 타입과 설정
	MapKind mMapKind = MapKind::FIELD;
	std::optional<std::variant<FieldMapConfig, DungeonMapConfig>> mConfig;

public:
	// 기본 생성자
	AsciiMap() = default;

	// 맵 타입별 생성자
	explicit AsciiMap(MapKind kind) : mMapKind(kind) {}
	explicit AsciiMap(const FieldMapConfig& config)
		: mMapKind(MapKind::FIELD), mConfig(config) {
	}
	explicit AsciiMap(const DungeonMapConfig& config)
		: mMapKind(MapKind::DUNGEON), mConfig(config) {
	}

	// 기본 인터페이스
	void Init(int w, int h, wchar_t fill)
	{
		mW = std::max(1, w);
		mH = std::max(1, h);
		mGrid.assign(static_cast<size_t>(mW) * static_cast<size_t>(mH), fill);
	}

	int Width() const noexcept { return mW; }
	int Height() const noexcept { return mH; }

	// 맵 타입 관련
	MapKind GetMapKind() const noexcept { return mMapKind; }
	bool IsField() const noexcept { return mMapKind == MapKind::FIELD; }
	bool IsDungeon() const noexcept { return mMapKind == MapKind::DUNGEON; }

	// 설정 접근자 (타입 안전)
	template<typename ConfigType>
	const ConfigType& GetConfig() const
	{
		if (!mConfig.has_value()) {
			throw std::runtime_error("No configuration set");
		}
		return std::get<ConfigType>(mConfig.value());
	}

	template<typename ConfigType>
	void SetConfig(const ConfigType& config)
	{
		mConfig = config;
		// 맵 타입도 자동으로 설정
		if constexpr (std::is_same_v<ConfigType, FieldMapConfig>) {
			mMapKind = MapKind::FIELD;
		}
		else if constexpr (std::is_same_v<ConfigType, DungeonMapConfig>) {
			mMapKind = MapKind::DUNGEON;
		}
	}

	// 설정이 있는지 확인
	bool HasConfig() const noexcept { return mConfig.has_value(); }

	// 기본 그리드 조작
	inline bool InBounds(int x, int y) const noexcept
	{
		return (x >= 0 && y >= 0 && x < mW && y < mH);
	}

	inline wchar_t Get(int x, int y) const noexcept
	{
		if (!InBounds(x, y)) return MmoSync::Map::TerrainLevel::LEVEL7;
		return mGrid[static_cast<size_t>(y) * static_cast<size_t>(mW) + static_cast<size_t>(x)];
	}

	inline void Set(int x, int y, wchar_t c) noexcept
	{
		if (!InBounds(x, y)) return;
		mGrid[static_cast<size_t>(y) * static_cast<size_t>(mW) + static_cast<size_t>(x)] = c;
	}

	// 던전 전용 기능들 (던전 맵일 때만 사용)
	inline int CountNeighborsMatching(int x, int y, wchar_t value, int radius = 1, bool includeCenter = false) const noexcept
	{
		if (!IsDungeon()) return 0; // 던전이 아니면 0 반환

		int count = 0;
		for (int dy = -radius; dy <= radius; ++dy)
		{
			for (int dx = -radius; dx <= radius; ++dx)
			{
				if (!includeCenter && dx == 0 && dy == 0) continue;
				if (Get(x + dx, y + dy) == value) ++count;
			}
		}
		return count;
	}

	void FloodFill(int startX, int startY, wchar_t passable, std::vector<std::pair<int, int>>& outTiles) const
	{
		outTiles.clear();
		if (!IsDungeon()) return; // 던전이 아니면 아무것도 하지 않음
		if (!InBounds(startX, startY)) return;
		if (Get(startX, startY) != passable) return;

		const size_t total = static_cast<size_t>(mW) * static_cast<size_t>(mH);
		std::vector<uint8_t> visited(total, 0);

		auto toIndex = [&](int x, int y) -> size_t
			{
				return static_cast<size_t>(y) * static_cast<size_t>(mW) + static_cast<size_t>(x);
			};

		std::queue<std::pair<int, int>> q;
		q.emplace(startX, startY);
		visited[toIndex(startX, startY)] = 1;

		static const int dx[4] = { 1, -1, 0, 0 };
		static const int dy[4] = { 0, 0, 1, -1 };

		// Safety limits to prevent infinite loops
		const size_t maxTiles = static_cast<size_t>(mW) * static_cast<size_t>(mH);
		const size_t maxIterations = std::min(maxTiles, static_cast<size_t>(50000));
		size_t iterationCount = 0;

		while (!q.empty() && iterationCount < maxIterations)
		{
			auto [cx, cy] = q.front();
			q.pop();
			iterationCount++;

			outTiles.emplace_back(cx, cy);

			// Prevent excessive memory usage
			if (outTiles.size() > maxTiles * 0.8)
			{
				break;
			}

			for (int dir = 0; dir < 4; ++dir)
			{
				const int nx = cx + dx[dir];
				const int ny = cy + dy[dir];
				if (!InBounds(nx, ny)) continue;

				const size_t ni = toIndex(nx, ny);

				if (visited[ni]) continue;
				if (Get(nx, ny) != passable) continue;

				// Mark as visited BEFORE adding to queue
				visited[ni] = 1;
				q.emplace(nx, ny);
			}
		}
	}

	// Optimized version that uses shared visited array
	void FloodFillWithVisited(int startX, int startY, wchar_t passable, std::vector<std::pair<int, int>>& outTiles, std::vector<uint8_t>& visited) const
	{
		outTiles.clear();
		if (!IsDungeon()) return; // 던전이 아니면 아무것도 하지 않음
		if (!InBounds(startX, startY)) return;
		if (Get(startX, startY) != passable) return;

		auto toIndex = [&](int x, int y) -> size_t
			{
				return static_cast<size_t>(y) * static_cast<size_t>(mW) + static_cast<size_t>(x);
			};

		// Check if already visited
		if (visited[toIndex(startX, startY)]) return;

		std::queue<std::pair<int, int>> q;
		q.emplace(startX, startY);
		visited[toIndex(startX, startY)] = 1;

		static const int dx[4] = { 1, -1, 0, 0 };
		static const int dy[4] = { 0, 0, 1, -1 };

		// Safety limits
		const size_t maxTiles = static_cast<size_t>(mW) * static_cast<size_t>(mH);
		const size_t maxIterations = std::min(maxTiles, static_cast<size_t>(50000));
		size_t iterationCount = 0;

		while (!q.empty() && iterationCount < maxIterations)
		{
			auto [cx, cy] = q.front();
			q.pop();
			iterationCount++;

			outTiles.emplace_back(cx, cy);

			// Prevent excessive memory usage
			if (outTiles.size() > maxTiles * 0.8)
			{
				break;
			}

			for (int dir = 0; dir < 4; ++dir)
			{
				const int nx = cx + dx[dir];
				const int ny = cy + dy[dir];
				if (!InBounds(nx, ny)) continue;

				const size_t ni = toIndex(nx, ny);

				if (visited[ni]) continue;
				if (Get(nx, ny) != passable) continue;

				// Mark as visited BEFORE adding to queue
				visited[ni] = 1;
				q.emplace(nx, ny);
			}
		}
	}

	// Convert to lines for printing/saving
	std::vector<std::wstring> ToLines() const
	{
		std::vector<std::wstring> out;
		out.reserve(static_cast<size_t>(mH));
		for (int y = 0; y < mH; ++y)
		{
			const wchar_t* row = &mGrid[static_cast<size_t>(y) * static_cast<size_t>(mW)];
			out.emplace_back(row, row + mW);
		}
		return out;
	}

	// 팩토리 메서드들
	static AsciiMap CreateFieldMap(const FieldMapConfig& config)
	{
		AsciiMap map(config);
		return map;
	}

	static AsciiMap CreateDungeonMap(const DungeonMapConfig& config)
	{
		AsciiMap map(config);
		return map;
	}

};
