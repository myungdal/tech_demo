// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "MmoSync/Grid/MultiGrid.h"

// 전방 선언
struct MapGenerationSettings;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Map 생성을 위한 기본 팩토리 클래스
// 모든 맵 생성 팩토리들의 기본 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Engine;
class AsciiMap;

class MMOSYNC_API GridGeneratorBase
{
public:
	// 기본 생성자 (Engine 의존성 제거)
	GridGeneratorBase() = default;
	
	// 템플릿 생성자도 deprecated
	template<typename ConfigType>
	explicit GridGeneratorBase(const Engine& engine, const ConfigType& config)
	{
		// config는 파생 클래스에서 사용
		// static_cast로 안전하게 캐스팅 가능
		static_cast<void>(engine);
		static_cast<void>(config); // 사용하지 않는다는 경고 방지
	}
	
	virtual ~GridGeneratorBase() = default;

public:

	// ============================================================================
	// Grid Creation Methods (New - MapGenerationSettings)
	// ============================================================================
	
	
	/// MultiGrid를 생성합니다 (새로운 인터페이스)
	/// @param multiGrid 생성할 MultiGrid 객체
	/// @param settings 통합 맵 생성 설정
	virtual std::unique_ptr<AsciiMap>  CreateMultiGrid(OUT MultiGrid& multiGrid, const MapGenerationSettings& settings) = 0;

	// ============================================================================
	// Utility Methods
	// ============================================================================
	
	/// 설정이 유효한지 확인합니다 (새로운 인터페이스)
	virtual bool IsConfigValid(const MapGenerationSettings& settings) const = 0;

protected:
	/// 기본 MultiGrid Lod 구축
	void BuildMultiGridLod(MultiGrid& multiGrid);
};
