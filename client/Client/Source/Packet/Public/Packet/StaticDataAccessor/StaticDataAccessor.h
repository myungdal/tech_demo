// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// StaticDataAccessor
// 정적 데이터 뷰에 대한 접근자 클래스입니다.
// 각종 정적 데이터(아이템, 캐릭터, 맵 등)의 뷰를 관리합니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StaticDataDocLookupBase;

class PACKET_API StaticDataAccessor final
{
private:
	using StaticViewTypes = std::tuple<
		class AchievementDocLookup,
		class BuildingDocLookup,
		class CharacterDocLookup,
		class CharacterSkillDocLookup,
		class GameDocLookup,
		class ItemDocLookup,
		class ItemStatDocLookup,
		class MapDocLookup,
		class MissionDocLookup,
		class QuestDocLookup,
		class RewardDocLookup,
		class RewardGroupDocLookup,
		class StatDocLookup,
		class UserExpDocLookup
	>;

private:
	using StaticDataViewPtr = std::shared_ptr<StaticDataDocLookupBase>;
	using StaticDataViewList = std::vector<StaticDataViewPtr>;

	StaticDataViewList mStaticDataViewList;
	
public:
	DISABLE_COPY(StaticDataAccessor);
	StaticDataAccessor();

private:
	template <typename _View>
	void RegisterView(size_t idx)
	{
		using _ViewType = typename std::decay<_View>::type;

		_ViewType::mViewIdx = idx;

		std::shared_ptr<_ViewType> ptr = std::make_shared<_ViewType>();
		mStaticDataViewList.emplace_back(ptr);
	}

public:
	template <typename _View>
	std::shared_ptr<typename std::decay<_View>::type> Get() const
	{ 
		using _ViewType = typename std::decay<_View>::type;

		std::shared_ptr<StaticDataDocLookupBase> ptr = mStaticDataViewList.at(_ViewType::mViewIdx);
		std::shared_ptr<_ViewType> castedPtr = std::static_pointer_cast<_ViewType>(ptr);
		return castedPtr;
	}

public:
	void BuildView();
};

#if defined(__UNREAL__)
PACKET_API extern TSharedPtr<StaticDataAccessor> gStaticDataAccessor;
#else
inline std::shared_ptr<StaticDataAccessor> gStaticDataAccessor = nullptr;
#endif
