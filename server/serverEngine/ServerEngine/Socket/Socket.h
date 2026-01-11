// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Common/MemPool/FixedPool.h"
#include "ServerEngine/Socket/SocketBase/SocketBase.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NetworkPacket;
class PacketTemp;

template <typename _ConcreteClass, size_t _PoolSizeFactor, typename _BaseClass = SocketBase>
class Socket : public _BaseClass
{
public:
	using SocketPtrType = SocketPtr<_ConcreteClass>;

protected:
	using PoolPtr = FixedPool<_ConcreteClass, _PoolSizeFactor>;
	static inline PoolPtr mPool{};

public:
	Socket() 
	{ 
		_BaseClass::SetSocketName(typeid(_ConcreteClass).name());
	}

public:
	static _ConcreteClass* AcquireSocketImpl()
	{
		return mPool.Pop();
	}

	static void DisposeSocketImpl(_ConcreteClass* ptr)
	{
		mPool.Push(ptr);
	}

protected:
	SocketBase* AcquireSocket() final
	{
		return static_cast<SocketBase*>(_ConcreteClass::AcquireSocketImpl());
	}

	void DisposeSocket() final
	{
		_ConcreteClass::DisposeSocketImpl(static_cast<_ConcreteClass*>(this));
	}
};
