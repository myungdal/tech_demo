// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

import DbServer;
import ServerEngineDb;

#include "DbPacketHandlerMail.h"

#include "DbServer/PacketTransactor/UserAndPacketTransactor.h"


namespace
{
	class Transactor_CD_REQ_MAIL_READ final : public UserAndPacketTransactor
	{
	private:
		SocketDbFromFront& mFrontSocket;
		CD_REQ_MAIL_READ& mRp;

		MailIdList mReadMailIdList;

	public:
		explicit Transactor_CD_REQ_MAIL_READ(
			DbUser& dbUser, DbUserContext& userContext,
			SocketDbFromFront& frontSocket,
			CD_REQ_MAIL_READ& rp
		)
			:
			UserAndPacketTransactor(dbUser, userContext, rp.GetHeader()),
			mFrontSocket(frontSocket),
			mRp(rp),
			mReadMailIdList()
		{
		}

	private:
		Result OnUpdate() override
		{
			const PacketSize count = mRp.Get_MailIdList_size();
			const MailId* list = mRp.Get_MailIdList();

			for (PacketSize i = 0; i < count; ++i)
			{
				const MailId mailId = list[i];
				if (INVALID_UUID == mailId)
					continue;

				SP_MAIL_UPDATE_READ spMailUpdateRead(
					GetUserDbSession(),
					mailId.GetData(),
					1,
					tClock.GetGlobalNowTs()
				);

				Result result = DbUtil::ExecuteSp(spMailUpdateRead);
				if (Result::SUCCEEDED != result)
					return result;

				mReadMailIdList.emplace_back(mailId);
			}

			return Result::SUCCEEDED;
		}

		void OnFinish() override
		{
			DbSocketUtil::SendToClient<DC_ACK_MAIL_READ::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
			wp.GetHeader().SetDbSeq(AddDbSeq());
			wp.SetValues(
				mReadMailIdList.data(),
				static_cast<PacketSize>(mReadMailIdList.size()),
				GetUserCacheDiffWp()
			);
		}

		void OnError() override
		{
			DbSocketUtil::SendToClient<DC_ACK_MAIL_READ::Writer> wp(mFrontSocket, ACK, mRp, GetResult());
			wp.SetValues(
				nullptr,
				0,
				USER_CACHE_DIFF_WRITER(nullptr)
			);
		}

		void OnLog() override
		{
		}
	};
}


// 우편 목록 요청 받음
HandleResult DbPacketHandlerMail::OnPacket(CD_REQ_MAIL_LIST& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	_WILL_USE(rp);

	// NOTE:
	// 현재 프로토콜(DC_ACK_MAIL_LIST)이 페이로드를 가지지 않아, 여기서 DB 조회 결과를 내려줄 수 없다.
	// Mail 회귀 테스트(7번)를 위해서는 resource/packet.xlsx에서 DC_ACK_MAIL_LIST에 mailList(또는 mailIdList)를 추가하고
	// 코드 자동생성 후, 이 핸들러에서 목록을 채우도록 확장해야 한다.

	DbSocketUtil::SendToClient<DC_ACK_MAIL_LIST::Writer> wp(socket, ACK, rp, Result::SUCCEEDED);
	return HandleResult::OK;
}

// 우편 읽음 요청 받음
HandleResult DbPacketHandlerMail::OnPacket(CD_REQ_MAIL_READ& rp, MAYBE_UNUSED SocketDbFromFront& socket)
{
	const UserId userId = rp.GetHeader().GetUserId();
	DbUserPtr dbUser = gDbUserManager->FindUser(userId);
	if (nullptr == dbUser)
	{
		DbSocketUtil::SendToClient<DC_ACK_MAIL_READ::Writer> wp(socket, ACK, rp, Result::RETRY_LATER);
		wp.SetValues(
			nullptr,
			0,
			USER_CACHE_DIFF_WRITER(nullptr)
		);
		return HandleResult::OK;
	}

	_ASSERT_CRASH(tWorkingWorker == dbUser);

	DbUserContext userContext = dbUser->GetContext();
	Transactor_CD_REQ_MAIL_READ transactor(*dbUser, userContext, socket, rp);
	transactor.Run();

	return HandleResult::OK;
}

