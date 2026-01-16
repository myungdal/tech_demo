// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

#include "CoreMinimal.h"


namespace CronUtil
{
	using Uint8 = unsigned char;

	struct CronTrait
	{
		static constexpr Uint8 CRON_MIN_SECONDS = 0;
		static constexpr Uint8 CRON_MAX_SECONDS = 59;

		static constexpr Uint8 CRON_MIN_MINUTES = 0;
		static constexpr Uint8 CRON_MAX_MINUTES = 59;

		static constexpr Uint8 CRON_MIN_HOURS = 0;
		static constexpr Uint8 CRON_MAX_HOURS = 23;

		static constexpr Uint8 CRON_MIN_DAYS_OF_WEEK = 0;
		static constexpr Uint8 CRON_MAX_DAYS_OF_WEEK = 6;

		static constexpr Uint8 CRON_MIN_DAYS_OF_MONTH = 1;
		static constexpr Uint8 CRON_MAX_DAYS_OF_MONTH = 31;

		static constexpr Uint8 CRON_MIN_MONTHS = 1;
		static constexpr Uint8 CRON_MAX_MONTHS = 12;

		static const Uint8 CRON_MAX_YEARS_DIFF = 4;

		static const TArray<FString> DAYS;
		static const TArray<FString> MONTHS;
	};

	struct CronException
	{
		FString mMessage;
		explicit CronException(const TCHAR* message) : mMessage(message) {}
	};

	enum class DescriptionCategory : size_t
	{
		SEC = 0,		// 초
		MIN,			// 분
		HOUR,			// 시
		DAY,			// 일
		MONTH,			// 월
		WEEKDAY,		// 요일
		MAX
	};

	enum class DescriptionType : size_t
	{
		ONE = 0,		// 단일
		ALL,			// 모든
		RANGE,			// 범위
		LIST,			// 나열
		REPEAT,			// 반복
		MAX,
		ERROR = MAX
	};

	struct Description
	{
		static constexpr DescriptionType TYPE = DescriptionType::ERROR;
		DescriptionType mType = DescriptionType::ERROR;
		Description() = default;
		explicit Description(DescriptionType type) : mType(type) {}
	};

	struct DescriptionOne : Description
	{
		static constexpr DescriptionType TYPE = DescriptionType::ONE;
		Uint8 mNumber = 0;
		DescriptionOne(Uint8 number) : Description(DescriptionType::ONE), mNumber(number) {}
	};

	struct DescriptionAll : Description
	{
		static constexpr DescriptionType TYPE = DescriptionType::ALL;
		DescriptionAll() : Description(DescriptionType::ALL) {}
	};

	struct DescriptionRange : Description
	{
		static constexpr DescriptionType TYPE = DescriptionType::RANGE;
		Uint8 mFrom = 0;
		Uint8 mTo = 0;
		DescriptionRange(Uint8 from, Uint8 to) : Description(DescriptionType::RANGE), mFrom(from), mTo(to) {};
	};

	struct DescriptionList : Description
	{
		static constexpr DescriptionType TYPE = DescriptionType::LIST;
		TArray<Uint8> mList;
		DescriptionList(TArray<Uint8>&& list) : Description(DescriptionType::LIST), mList(MoveTemp(list)) {};
	};

	struct DescriptionRepeat : Description
	{
		static constexpr DescriptionType TYPE = DescriptionType::REPEAT;
		Uint8 mStart = 0;
		Uint8 mInterval = 0;
		DescriptionRepeat(Uint8 start, Uint8 interval) : Description(DescriptionType::REPEAT), mStart(start), mInterval(interval) {};
	};

	class WriterBase
	{
	public:
		virtual ~WriterBase() = default;

		virtual void Write(const CronUtil::DescriptionOne&, FString&) const {}
		virtual void Write(const CronUtil::DescriptionAll&, FString&) const {}
		virtual void Write(const CronUtil::DescriptionRange&, FString&) const {}
		virtual void Write(const CronUtil::DescriptionList&, FString&) const {}
		virtual void Write(const CronUtil::DescriptionRepeat&, FString&) const {}
	};

	class CronDescriptor
	{
	private:
		const WriterBase* mWriter[static_cast<size_t>(DescriptionCategory::MAX)];
		const Description* mDescriptions[static_cast<size_t>(DescriptionCategory::MAX)];

	public:
		explicit CronDescriptor(
			const WriterBase* sec,
			const WriterBase* min,
			const WriterBase* hour,
			const WriterBase* day,
			const WriterBase* month,
			const WriterBase* weekday
		);
		~CronDescriptor();

	private:
		void Clear();
		bool Validate() const;

	public:
		FString Parse(const FString& expr);

		template<typename _Description>
		const _Description* GetDescription(DescriptionCategory category) const
		{
			if (false == Validate())
				return nullptr;

			if (DescriptionCategory::MAX <= category)
				return nullptr;

			size_t i = static_cast<size_t>(category);
			if (_Description::TYPE != mDescriptions[i]->mType)
				return nullptr;

			return static_cast<const _Description*>(mDescriptions[i]);
		}

	private:
		void Write(DescriptionCategory category, FString& result) const;
		bool IsMeaningfulForWriter(DescriptionCategory category) const;
		const Description* SetField(const FString& value, const Uint8 minval, const Uint8 maxval) const;
		const Description* SetDaysOfWeek(FString value) const;
		const Description* SetDaysOfMonth(FString value) const;
		const Description* SetMonth(FString value) const;

		FString ReplaceOrdinals(FString text, const TArray<FString>& replacement) const;
		TPair<Uint8, Uint8> MakeRange(const FString& field, const Uint8 minval, const Uint8 maxval) const;
	};

	FString DescriptChar(const FString& cronText);
}

#if 0

#include "Util/CronUtil.h"

class CronWriterBase : public CronUtil::WriterBase
{
protected:
	FString mUnitStr = TEXT("");

public:
	explicit CronWriterBase(const TCHAR* unitStr)
		:
		mUnitStr(unitStr)
	{
	}

public:
	void Write(const CronUtil::DescriptionOne& d, FString& result) const override
	{
		result += FString::Printf(TEXT("%s%s"), *Conv(d.mNumber), *mUnitStr);
	}

	void Write(const CronUtil::DescriptionAll& d, FString& result) const override
	{
		result += FString::Printf(TEXT("매%s"), *mUnitStr);
	}

	void Write(const CronUtil::DescriptionRange& d, FString& result) const override
	{
		result += FString::Printf(TEXT("%s%s~%s%s"), *Conv(d.mFrom), *mUnitStr, *Conv(d.mTo), *mUnitStr);
	}

	void Write(const CronUtil::DescriptionList& d, FString& result) const override
	{
		bool first = true;
		for (auto v : d.mList)
		{
			if (first) first = false;
			else result += TEXT(", ");
			result += FString::Printf(TEXT("%s%s"), *Conv(v), *mUnitStr);
		}
	}

	void Write(const CronUtil::DescriptionRepeat& d, FString& result) const override
	{
		result += FString::Printf(TEXT("%s%s 시작으로 매 %s%s 마다"), *Conv(d.mStart), *mUnitStr, *Conv(d.mInterval), *mUnitStr);
	}

protected:
	virtual FString Conv(CronUtil::Uint8 value) const { return FString::FromInt(value); }
};

class CronWriterSec : public CronWriterBase
{
public:
	explicit CronWriterSec() : CronWriterBase(TEXT("초")) {}
} writerSec;

class CronWriterMin : public CronWriterBase
{
public:
	explicit CronWriterMin() : CronWriterBase(TEXT("분")) {}
} writerMin;

class CronWriterHour : public CronWriterBase
{
public:
	explicit CronWriterHour() : CronWriterBase(TEXT("시")) {}
} writerHour;

class CronWriterDay : public CronWriterBase
{
public:
	explicit CronWriterDay() : CronWriterBase(TEXT("일")) {}
} writerDay;

class CronWriterMonth : public CronWriterBase
{
public:
	explicit CronWriterMonth() : CronWriterBase(TEXT("월")) {}
} writerMonth;

class CronWriterWeekday : public CronWriterBase
{
public:
	explicit CronWriterWeekday() : CronWriterBase(TEXT("요일")) {}

protected:
	FString Conv(CronUtil::Uint8 value) const override
	{
		switch (value)
		{
		case 0:
			return TEXT("일");
		case 1:
			return TEXT("월");
		case 2:
			return TEXT("화");
		case 3:
			return TEXT("수");
		case 4:
			return TEXT("목");
		case 5:
			return TEXT("금");
		case 6:
			return TEXT("토");
		}
		return TEXT("");
	}

} writerWeekday;

CronUtil::CronDescriptor cronDescriptor(
	&writerSec,
	&writerMin,
	&writerHour,
	&writerDay,
	&writerMonth,
	&writerWeekday
);
const TCHAR* cronExpr = TEXT("0 0 5 * * 1,3,5");
FString text = cronDescriptor.Parse(cronExpr);
UE_LOG(LogTemp, Display, TEXT("cronExpression: %s"), cronExpr);
UE_LOG(LogTemp, Display, TEXT("cronDescriptor: %s"), *text);

#endif
