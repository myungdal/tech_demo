// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#include "CronUtil.h"


namespace CronUtil
{
	const TArray<FString> CronTrait::DAYS = { L"SUN", L"MON", L"TUE", L"WED", L"THU", L"FRI", L"SAT" };
	const TArray<FString> CronTrait::MONTHS = { L"NIL", L"JAN", L"FEB", L"MAR", L"APR", L"MAY", L"JUN", L"JUL", L"AUG", L"SEP", L"OCT", L"NOV", L"DEC" };

	CronDescriptor::CronDescriptor(
		const WriterBase* sec,
		const WriterBase* min,
		const WriterBase* hour,
		const WriterBase* day,
		const WriterBase* month,
		const WriterBase* weekday
	)
		:
		mWriter{ sec, min, hour, day, month, weekday }
	{
		for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
		{
			mDescriptions[i] = nullptr;
		}
	}

	CronDescriptor::~CronDescriptor()
	{
		Clear();
	}

	void CronDescriptor::Clear()
	{
		for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
		{
			if (nullptr != mDescriptions[i])
			{
				delete mDescriptions[i];
				mDescriptions[i] = nullptr;
			}
		}
	}

	bool CronDescriptor::Validate() const
	{
		for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
		{
			if (nullptr == mDescriptions[i])
				return false;

			if (DescriptionType::MAX <= mDescriptions[i]->mType)
				return false;
		}

		return true;
	}

	FString CronDescriptor::Parse(const FString& cronExpr)
	{
		FString result;

		if (cronExpr.IsEmpty())
			return result;

		Clear();

		TArray<FString> fields;
		cronExpr.ParseIntoArray(fields, TEXT(" "));

		if (6 != fields.Num())
			return result;

		mDescriptions[static_cast<size_t>(DescriptionCategory::SEC)] = SetField(fields[0], CronTrait::CRON_MIN_SECONDS, CronTrait::CRON_MAX_SECONDS);
		mDescriptions[static_cast<size_t>(DescriptionCategory::MIN)] = SetField(fields[1], CronTrait::CRON_MIN_MINUTES, CronTrait::CRON_MAX_MINUTES);
		mDescriptions[static_cast<size_t>(DescriptionCategory::HOUR)] = SetField(fields[2], CronTrait::CRON_MIN_HOURS, CronTrait::CRON_MAX_HOURS);
		mDescriptions[static_cast<size_t>(DescriptionCategory::DAY)] = SetDaysOfMonth(fields[3]);
		mDescriptions[static_cast<size_t>(DescriptionCategory::MONTH)] = SetMonth(fields[4]);
		mDescriptions[static_cast<size_t>(DescriptionCategory::WEEKDAY)] = SetDaysOfWeek(fields[5]);

		if (false == Validate())
			return result;

		bool meaningful[static_cast<size_t>(DescriptionCategory::MAX)];
		for (size_t i = 0; i < static_cast<size_t>(DescriptionCategory::MAX); ++i)
		{
			meaningful[i] = IsMeaningfulForWriter(static_cast<DescriptionCategory>(i));
		}

		if (meaningful[static_cast<size_t>(DescriptionCategory::MONTH)] ||			
			meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
			meaningful[static_cast<size_t>(DescriptionCategory::DAY)])
		{
			Write(DescriptionCategory::MONTH, result);
		}

		if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)] ||
			meaningful[static_cast<size_t>(DescriptionCategory::DAY)] ||
			meaningful[static_cast<size_t>(DescriptionCategory::HOUR)])
		{
			if (meaningful[static_cast<size_t>(DescriptionCategory::WEEKDAY)])
				Write(DescriptionCategory::WEEKDAY, result);
			else
				Write(DescriptionCategory::DAY, result);
		}

		if (meaningful[static_cast<size_t>(DescriptionCategory::HOUR)] || meaningful[static_cast<size_t>(DescriptionCategory::MIN)])
		{
			Write(DescriptionCategory::HOUR, result);
		}

		if (meaningful[static_cast<size_t>(DescriptionCategory::MIN)] || meaningful[static_cast<size_t>(DescriptionCategory::SEC)])
		{
			Write(DescriptionCategory::MIN, result);
		}

		Write(DescriptionCategory::SEC, result);

		return result;
	}

	bool CronDescriptor::IsMeaningfulForWriter(DescriptionCategory category) const
	{
		const DescriptionType descriptionType = mDescriptions[static_cast<size_t>(category)]->mType;
		return ((DescriptionType::ONE == descriptionType) ||
			(DescriptionType::RANGE == descriptionType) ||
			(DescriptionType::LIST == descriptionType) ||
			(DescriptionType::REPEAT == descriptionType));
	}

	void CronDescriptor::Write(DescriptionCategory category, FString& result) const
	{
		size_t i = static_cast<size_t>(category);
		auto writer = mWriter[i];
		if (!writer) return;

		const CronUtil::DescriptionOne* one = GetDescription<CronUtil::DescriptionOne>(category);
		const CronUtil::DescriptionAll* all = GetDescription<CronUtil::DescriptionAll>(category);
		const CronUtil::DescriptionRange* range = GetDescription<CronUtil::DescriptionRange>(category);
		const CronUtil::DescriptionList* list = GetDescription<CronUtil::DescriptionList>(category);
		const CronUtil::DescriptionRepeat* repeat = GetDescription<CronUtil::DescriptionRepeat>(category);

		if (one) writer->Write(*one, result);
		else if (all) writer->Write(*all, result);
		else if (range) writer->Write(*range, result);
		else if (list) writer->Write(*list, result);
		else if (repeat) writer->Write(*repeat, result);
		else return;

		result += TEXT(" ");
	}

	const Description* CronDescriptor::SetField(const FString& value, const Uint8 minval, const Uint8 maxval) const
	{
		if (value.Len() > 0 && value[value.Len() - 1] == ',')
			return nullptr;

		TArray<FString> fields;
		value.ParseIntoArray(fields, TEXT(","));
		if (0 == fields.Num())
		{
			return nullptr;
		}
		else if (1 == fields.Num())
		{
			FString& field = fields[0];

			if (field.Contains(TEXT("*")))
			{
				return new DescriptionAll();
			}
			else if (field.Contains(TEXT("/")))
			{
				TArray<FString> parts;
				field.ParseIntoArray(parts, TEXT("/"));
				if (parts.Num() != 2)
					return nullptr;

				auto start = FCString::Atoi(*parts[0]);
				auto interval = FCString::Atoi(*parts[1]);
				if ((start <= 0) || (interval <= 0))
					return nullptr;
				else
					return new DescriptionRepeat(start, interval);
			}
			else
			{
				auto range = MakeRange(field, minval, maxval);
				auto from = range.Key;
				auto to = range.Value;
				if (from == to)
					return new DescriptionOne(from);
				else
					return new DescriptionRange(from, to);
			}
		}
		else
		{
			TArray<Uint8> list;
			for (const auto& field : fields)
			{
				auto number = FCString::Atoi(*field);
				if (number <= 0)
					return nullptr;
				else
					list.Add(static_cast<Uint8>(number));
			}
			return new DescriptionList(MoveTemp(list));
		}

		return nullptr;
	}

	const Description* CronDescriptor::SetDaysOfWeek(FString value) const
	{
		auto days = value.ToUpper();
		auto days_replaced = ReplaceOrdinals(days, CronTrait::DAYS);

		if (days_replaced.Len() == 1 && days_replaced[0] == '?')
			days_replaced[0] = '*';

		return SetField(days_replaced, CronTrait::CRON_MIN_DAYS_OF_WEEK, CronTrait::CRON_MAX_DAYS_OF_WEEK);
	}

	const Description* CronDescriptor::SetDaysOfMonth(FString value) const
	{
		if (value.Len() == 1 && value[0] == '?')
			value[0] = '*';

		return SetField(value, CronTrait::CRON_MIN_DAYS_OF_MONTH, CronTrait::CRON_MAX_DAYS_OF_MONTH);
	}

	const Description* CronDescriptor::SetMonth(FString value) const
	{
		auto month = value.ToUpper();
		auto month_replaced = ReplaceOrdinals(month, CronTrait::MONTHS);

		return SetField(month_replaced, CronTrait::CRON_MIN_MONTHS, CronTrait::CRON_MAX_MONTHS);
	}


	FString CronDescriptor::ReplaceOrdinals(FString text, const TArray<FString>& replacement) const
	{
		for (size_t i = 0; i < replacement.Num(); ++i)
		{
			const int32 pos = text.Find(replacement[i]);
			if (INDEX_NONE != pos)
			{
				text.RemoveAt(pos, 3);
				text.InsertAt(pos, FString::FromInt(i));
			}
		}

		return text;
	}

	TPair<Uint8, Uint8> CronDescriptor::MakeRange(const FString& field, const Uint8 minval, const Uint8 maxval) const
	{
		Uint8 _first = 0;
		Uint8 _last = 0;
		if (field.Len() == 1 && field[0] == TEXT('*'))
		{
			_first = minval;
			_last = maxval;
		}
		else if (!field.Contains(TEXT("-")))
		{
			_first = static_cast<Uint8>(FCString::Atoi(*field));
			_last = _first;
		}
		else
		{
			TArray<FString> parts;
			field.ParseIntoArray(parts, TEXT("-"));
			if (parts.Num() != 2)
				throw CronException(TEXT("Specified range requires two fields"));

			_first = static_cast<Uint8>(FCString::Atoi(*parts[0]));
			_last = static_cast<Uint8>(FCString::Atoi(*parts[1]));
		}

		if (_first > maxval || _last > maxval)
		{
			throw CronException(TEXT("Specified range exceeds maximum"));
		}
		if (_first < minval || _last < minval)
		{
			throw CronException(TEXT("Specified range is less than minimum"));
		}
		if (_first > _last)
		{
			throw CronException(TEXT("Specified range start exceeds range end"));
		}

		return TPair<Uint8, Uint8>(_first, _last);
	}

	FString DescriptChar(const FString& cronExpr)
	{
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
				result += FString::Printf(TEXT("留%s"), *mUnitStr);
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
			explicit CronWriterSec() : CronWriterBase(TEXT("珥")) {}
		} writerSec;

		class CronWriterMin : public CronWriterBase
		{
		public:
			explicit CronWriterMin() : CronWriterBase(TEXT("遺")) {}
		} writerMin;

		class CronWriterHour : public CronWriterBase
		{
		public:
			explicit CronWriterHour() : CronWriterBase(TEXT("떆")) {}
		} writerHour;

		class CronWriterDay : public CronWriterBase
		{
		public:
			explicit CronWriterDay() : CronWriterBase(TEXT("씪")) {}
		} writerDay;

		class CronWriterMonth : public CronWriterBase
		{
		public:
			explicit CronWriterMonth() : CronWriterBase(TEXT("썡")) {}
		} writerMonth;

		class CronWriterWeekday : public CronWriterBase
		{
		public:
			explicit CronWriterWeekday() : CronWriterBase(TEXT("슂씪")) {}

		protected:
			FString Conv(CronUtil::Uint8 value) const override
			{
				switch (value)
				{
				case 0:
					return TEXT("留ㅼ＜ 씪");
				case 1:
					return TEXT("留ㅼ＜ 썡");
				case 2:
					return TEXT("留ㅼ＜ 솕");
				case 3:
					return TEXT("留ㅼ＜ 닔");
				case 4:
					return TEXT("留ㅼ＜ 紐");
				case 5:
					return TEXT("留ㅼ＜ 湲");
				case 6:
					return TEXT("留ㅼ＜ 넗");
				default:
					break;
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
		
		FString description = cronDescriptor.Parse(cronExpr);
		
		return description;
	}
}
