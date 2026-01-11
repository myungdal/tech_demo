// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "MmoSync/Util/EntityUtil/EntityUtil.h"

#include "MmoSync/WorldManager/WorldUserRegistry/WorldUser.h"
#include "MmoSync/ComponentRegisterer/Components.h"
#include "MmoSync/Entity/EntityDescriptor_WARRIOR.h"
#include "MmoSync/Entity/EntityDescriptor_ARCHER.h"
#include "MmoSync/Entity/EntityDescriptor_ASSASSIN.h"
#include "MmoSync/Entity/EntityDescriptor_BAT.h"
#include "MmoSync/Entity/EntityDescriptor_BEAR.h"
#include "MmoSync/Entity/EntityDescriptor_SPIDER.h"
#include "MmoSync/Entity/EntityDescriptor_ZEUS.h"
#include "MmoSync/Entity/EntityDescriptor_HADES.h"
#include "MmoSync/Entity/EntityDescriptor_CUPID.h"
#include "MmoSync/Entity/EntityDescriptor_GOLEM.h"
#include "MmoSync/Entity/EntityDescriptor_PHOENIX.h"
#include "MmoSync/Entity/EntityDescriptor_PROJECTILE.h"
#include "MmoSync/Entity/EntityDescriptor_ROCK.h"
#include "MmoSync/Entity/EntityDescriptor_BOMB.h"
#include "MmoSync/Entity/EntityDescriptor_DRAGON.h"
#include "MmoSync/DebugControlTool/DebugInfo.h"


namespace
{
	constexpr EntityDescriptor_ROCK ROCK{};
	constexpr EntityDescriptor_BOMB BOMB{};
	constexpr EntityDescriptor_PROJECTILE PROJECTILE{};
	constexpr EntityDescriptor_WARRIOR WARRIOR{};
	constexpr EntityDescriptor_ARCHER ARCHER{};
	constexpr EntityDescriptor_ASSASSIN ASSASSIN{};
	constexpr EntityDescriptor_BAT BAT{};
	constexpr EntityDescriptor_BEAR BEAR{};
	constexpr EntityDescriptor_SPIDER SPIDER{};
	constexpr EntityDescriptor_ZEUS ZEUS{};
	constexpr EntityDescriptor_HADES HADES{};
	constexpr EntityDescriptor_CUPID CUPID{};
	constexpr EntityDescriptor_GOLEM GOLEM{};
	constexpr EntityDescriptor_PHOENIX PHOENIX{};
	constexpr EntityDescriptor_DRAGON DRAGON{};

	using EntityDescriptorArray = std::array<const EntityDescriptorBase*, static_cast<size_t>(CharacterType::MAX)>;

	// CharacterType 와 순서를 맞추지 않아도 되도록 개선하면 좋겠다
	constexpr EntityDescriptorArray gEntityDescriptorArray
	{
		&ROCK,
		&BOMB,
		&PROJECTILE,
		&WARRIOR,
		&ARCHER,
		&ASSASSIN,
		&BAT,
		&BEAR,
		&SPIDER,
		&ZEUS,
		&HADES,
		&CUPID,
		&GOLEM,
		&PHOENIX,
		&DRAGON
	};
}

EntityDescription EntityUtil::DescriptSpawner(const EntityDescriptorArg& arg)
{
	EntityDescription entityDescription;

	const size_t idx = static_cast<size_t>(arg.characterType);

	gEntityDescriptorArray[idx]->DescriptCharacterBase(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterPcNpc(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterType(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterSpawner(OUT entityDescription, arg);

	return entityDescription;
}

EntityDescription EntityUtil::DescriptSpawnee(const EntityDescriptorArg& arg)
{
	EntityDescription entityDescription;

	const size_t idx = static_cast<size_t>(arg.characterType);

	gEntityDescriptorArray[idx]->DescriptCharacterBase(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterPcNpc(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterType(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterSpawnee(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterArrivalMotion(OUT entityDescription, arg);

	return entityDescription;
}

EntityDescription EntityUtil::DescriptEntityPcNpc(const EntityDescriptorArg& arg)
{
	EntityDescription entityDescription;

	const size_t idx = static_cast<size_t>(arg.characterType);

	gEntityDescriptorArray[idx]->DescriptCharacterBase(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterPcNpc(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterType(OUT entityDescription, arg);
	gEntityDescriptorArray[idx]->DescriptCharacterArrivalMotion(OUT entityDescription, arg);

	return entityDescription;
}

EntityDescription EntityUtil::DescriptEntityNormal(const EntityDescriptorArg& arg)
{
	EntityDescription entityDescription;

	const size_t idx = static_cast<size_t>(arg.characterType);

	gEntityDescriptorArray[idx]->DescriptCharacterType(OUT entityDescription, arg);

	return entityDescription;
}
