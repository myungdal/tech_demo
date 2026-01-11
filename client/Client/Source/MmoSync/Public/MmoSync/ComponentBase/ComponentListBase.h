// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/Serializer/Serializer.h"
#include "Common/Serializer/Deserializer.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ComponentList 의 Base 클래스.
// Component 에 접근할 때, ComponentType 별 ComponentList 를 사용한다
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ComponentBase;

class ComponentListBase
{
protected:
	std::string mName;

public:
	DISABLE_COPY(ComponentListBase);
	explicit ComponentListBase(const char* name)
		:
		mName(name)
	{
	}

	virtual ~ComponentListBase() = default;

public:
	virtual void Serialize(Serializer& serializer) = 0;
	virtual void Deserialize(Deserializer& deserializer) = 0;

public:
	virtual void DestroyComponent(EntityId entityId) = 0;

public:
	virtual const ComponentBase& GetComponentBase(EntityId entityId) const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component Instance 들을 관리한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Component>
class ComponentList final : public ComponentListBase
{
private:
	using ComponentPool = CompactionPool<_Component, ComponentId>;
	ComponentPool mComponentPool;

	// EntityId 로 ComponentId 를 얻는다
	using EntityComponentView = std::vector<ComponentId, MallocAllocator<ComponentId>>;
	EntityComponentView mEntityComponentView;

public:
	DISABLE_COPY(ComponentList);
#if defined(__UNREAL__)
	ComponentList()
		:
		ComponentListBase("")
	{
		static_assert(std::is_trivially_destructible_v<_Component>, "_Component must be trivially destructible");
	}
#else
	// + 7은 "struct " 의 글자 수
	ComponentList()
		:
		ComponentListBase(typeid(_Component).name() + 7)
	{
	}
#endif

public:
	void Serialize(Serializer& serializer) override
	{
		mComponentPool.Serialize(serializer);

		const EntityCount size = static_cast<EntityCount>(mEntityComponentView.size());
		serializer.WriteValue(size);
		serializer.WriteBinary(reinterpret_cast<uint8_t*>(mEntityComponentView.data()), sizeof(ComponentId) * size);
	}

	void Deserialize(Deserializer& deserializer) override
	{
		mComponentPool.Deserialize(deserializer);

		EntityCount size = deserializer.ReadValue<EntityCount>();
		mEntityComponentView.resize(size);
		deserializer.ReadBinary(reinterpret_cast<uint8_t*>(mEntityComponentView.data()), sizeof(ComponentId) * size);
	}

	// Component 생성 & 파괴
public:
	void DestroyComponent(const EntityId entityId) override
	{
		const ComponentId componentId = mEntityComponentView[entityId];
		mComponentPool.Push(componentId);

		mEntityComponentView[entityId] = INVALID_COMPONENT_ID;
	}

	_Component& CreateComponent(const EntityId entityId)
	{
		const ComponentId componentId = mComponentPool.Pop();

		const size_t resize = _IDX(entityId + 1llu);
		if (mEntityComponentView.size() < resize)
		{
			mEntityComponentView.resize(resize, INVALID_COMPONENT_ID);
		}
		mEntityComponentView[entityId] = componentId;

		_Component& component = mComponentPool.GetData(componentId);
		return component;
	}

public:
	const ComponentBase& GetComponentBase(const EntityId entityId) const override
	{
		const ComponentId componentId = mEntityComponentView[entityId];
		const _Component& component = mComponentPool.GetData(componentId);
		return component;
	}

	_Component& GetComponent(const EntityId entityId)
	{
		const ComponentId componentId = mEntityComponentView[entityId];
		_Component& component = mComponentPool.GetData(componentId);
		return component;
	}

	_Component& operator[](const EntityId entityId)
	{
		const ComponentId componentId = mEntityComponentView[entityId];
		_Component& component = mComponentPool.GetData(componentId);
		return component;
	}

	template<typename _Func>
	void ForEach(_Func func)
	{
		for (ComponentId i = 0; i < mComponentPool.GetDataList().size(); ++i)
		{
			if (!mComponentPool.HasData(i))
				continue;

			func(mComponentPool.GetData(i));
		}
	}
};

