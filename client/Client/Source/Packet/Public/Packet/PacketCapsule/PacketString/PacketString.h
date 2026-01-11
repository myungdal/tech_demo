// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "Packet/PacketCapsule/PacketString/Internal/PacketStringBuffer.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 패킷에 사용되는 문자열을 풀링하여 사용하기 위한 클래스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Type>
class PacketString final
{
private:
	mutable _Type* mBuf = nullptr;

public:
	PacketString()
	{
		mBuf = reinterpret_cast<_Type*>(PacketStringBuffer::Pop(__FUNCTIONW__));
	}

	explicit PacketString(const PacketString& val)
	{
		mBuf = val.mBuf;
		const_cast<PacketString&>(val).mBuf = nullptr;
	}

	~PacketString()
	{
		if (nullptr != mBuf)
		{
			PacketStringBuffer::Push(reinterpret_cast<uint8_t*>(mBuf));
			mBuf = nullptr;
		}
	}

public:
	PacketString& operator=(const PacketString& val)
	{
		mBuf = val.mBuf;
		val.mBuf = nullptr;

		return *this;
	}

public:
	_Type* operator*() { return mBuf; }
};

using Buf_wchar_t = PacketString<wchar_t>;
using Buf_char = PacketString<char>;
