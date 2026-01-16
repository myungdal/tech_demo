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
#include <sstream>
#include <bitset>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <string_view>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cron
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace cron
{
	inline constexpr std::time_t INVALID_TIME = static_cast<std::time_t>(-1);
	inline constexpr size_t INVALID_INDEX = static_cast<size_t>(-1);

	class cronexpr;

	namespace detail
	{
		enum class cron_field
		{
			second,
			minute,
			hour_of_day,
			day_of_week,
			day_of_month,
			month,
			year
		};

		template <typename Traits>
		static bool find_next(const cronexpr& cex, std::tm& date, size_t dot);
	}

	struct bad_cronexpr : public std::runtime_error
	{
	public:
		explicit bad_cronexpr(std::string message)
			:
			std::runtime_error(message)
		{
		}
	};


	struct cron_standard_traits
	{
		static constexpr uint8_t CRON_MIN_SECONDS = 0;
		static constexpr uint8_t CRON_MAX_SECONDS = 59;

		static constexpr uint8_t CRON_MIN_MINUTES = 0;
		static constexpr uint8_t CRON_MAX_MINUTES = 59;

		static constexpr uint8_t CRON_MIN_HOURS = 0;
		static constexpr uint8_t CRON_MAX_HOURS = 23;

		static constexpr uint8_t CRON_MIN_DAYS_OF_WEEK = 0;
		static constexpr uint8_t CRON_MAX_DAYS_OF_WEEK = 6;

		static constexpr uint8_t CRON_MIN_DAYS_OF_MONTH = 1;
		static constexpr uint8_t CRON_MAX_DAYS_OF_MONTH = 31;

		static constexpr uint8_t CRON_MIN_MONTHS = 1;
		static constexpr uint8_t CRON_MAX_MONTHS = 12;

		static constexpr uint8_t CRON_MAX_YEARS_DIFF = 4;

		static constexpr const wchar_t* DAYS[] = { L"SUN", L"MON", L"TUE", L"WED", L"THU", L"FRI", L"SAT" };
		static constexpr const wchar_t* MONTHS[] = { L"NIL", L"JAN", L"FEB", L"MAR", L"APR", L"MAY", L"JUN", L"JUL", L"AUG", L"SEP", L"OCT", L"NOV", L"DEC" };
	};

	struct cron_oracle_traits
	{
		static constexpr uint8_t CRON_MIN_SECONDS = 0;
		static constexpr uint8_t CRON_MAX_SECONDS = 59;

		static constexpr uint8_t CRON_MIN_MINUTES = 0;
		static constexpr uint8_t CRON_MAX_MINUTES = 59;

		static constexpr uint8_t CRON_MIN_HOURS = 0;
		static constexpr uint8_t CRON_MAX_HOURS = 23;

		static constexpr uint8_t CRON_MIN_DAYS_OF_WEEK = 1;
		static constexpr uint8_t CRON_MAX_DAYS_OF_WEEK = 7;

		static constexpr uint8_t CRON_MIN_DAYS_OF_MONTH = 1;
		static constexpr uint8_t CRON_MAX_DAYS_OF_MONTH = 31;

		static constexpr uint8_t CRON_MIN_MONTHS = 0;
		static constexpr uint8_t CRON_MAX_MONTHS = 11;

		static constexpr uint8_t CRON_MAX_YEARS_DIFF = 4;

		static constexpr const wchar_t* DAYS[] = { L"NIL", L"SUN", L"MON", L"TUE", L"WED", L"THU", L"FRI", L"SAT" };
		static constexpr const wchar_t* MONTHS[] = { L"JAN", L"FEB", L"MAR", L"APR", L"MAY", L"JUN", L"JUL", L"AUG", L"SEP", L"OCT", L"NOV", L"DEC" };
	};

	struct cron_quartz_traits
	{
		static constexpr uint8_t CRON_MIN_SECONDS = 0;
		static constexpr uint8_t CRON_MAX_SECONDS = 59;

		static constexpr uint8_t CRON_MIN_MINUTES = 0;
		static constexpr uint8_t CRON_MAX_MINUTES = 59;

		static constexpr uint8_t CRON_MIN_HOURS = 0;
		static constexpr uint8_t CRON_MAX_HOURS = 23;

		static constexpr uint8_t CRON_MIN_DAYS_OF_WEEK = 1;
		static constexpr uint8_t CRON_MAX_DAYS_OF_WEEK = 7;

		static constexpr uint8_t CRON_MIN_DAYS_OF_MONTH = 1;
		static constexpr uint8_t CRON_MAX_DAYS_OF_MONTH = 31;

		static constexpr uint8_t CRON_MIN_MONTHS = 1;
		static constexpr uint8_t CRON_MAX_MONTHS = 12;

		static constexpr uint8_t CRON_MAX_YEARS_DIFF = 4;

		static constexpr const wchar_t* DAYS[] = { L"NIL", L"SUN", L"MON", L"TUE", L"WED", L"THU", L"FRI", L"SAT" };
		static constexpr const wchar_t* MONTHS[] = { L"NIL", L"JAN", L"FEB", L"MAR", L"APR", L"MAY", L"JUN", L"JUL", L"AUG", L"SEP", L"OCT", L"NOV", L"DEC" };
	};

	class cronexpr;

	template <typename Traits = cron_standard_traits>
	inline cronexpr make_cron(std::wstring_view expr);

	class cronexpr
	{
		std::bitset<60> seconds;
		std::bitset<60> minutes;
		std::bitset<24> hours;
		std::bitset<7>  days_of_week;
		std::bitset<31> days_of_month;
		std::bitset<12> months;

		friend bool operator==(const cronexpr& e1, const cronexpr& e2);
		friend bool operator!=(const cronexpr& e1, const cronexpr& e2);

		template <typename Traits>
		friend bool detail::find_next(
			const cronexpr& cex,
			std::tm& date,
			const size_t dot
		);

		friend std::wstring to_string(const cronexpr& cex);

		template <typename Traits>
		friend cronexpr make_cron(std::wstring_view expr);
	};

	inline bool operator==(const cronexpr& e1, const cronexpr& e2)
	{
		return
			e1.seconds == e2.seconds &&
			e1.minutes == e2.minutes &&
			e1.hours == e2.hours &&
			e1.days_of_week == e2.days_of_week &&
			e1.days_of_month == e2.days_of_month &&
			e1.months == e2.months;
	}

	inline bool operator!=(const cronexpr& e1, const cronexpr& e2)
	{
		return !(e1 == e2);
	}

	inline std::wstring to_string(const cronexpr& cex)
	{
		std::string result =
			cex.seconds.to_string() + " " +
			cex.minutes.to_string() + " " +
			cex.hours.to_string() + " " +
			cex.days_of_month.to_string() + " " +
			cex.months.to_string() + " " +
			cex.days_of_week.to_string();

		return std::wstring().assign(result.begin(), result.end());
	}

	namespace utils
	{
		inline std::time_t tm_to_time(std::tm& date)
		{
			return _mkgmtime(&date);
		}

		inline std::tm* time_to_tm(const std::time_t* date, OUT std::tm* const out)
		{
			const errno_t err = _gmtime64_s(out, date);
			return 0 == err ? out : nullptr;
		}

		inline std::tm to_tm(std::wstring_view time)
		{
			std::wistringstream str(time.data());
			str.imbue(std::locale(setlocale(LC_ALL, nullptr)));

			std::tm result;
			str >> std::get_time(&result, L"%Y-%m-%d %H:%M:%S");
			if (str.fail()) throw std::runtime_error("Parsing date failed!");

			result.tm_isdst = -1; // DST info not available

			return result;
		}

		inline std::wstring to_string(const std::tm& tm)
		{
			std::wostringstream str;
			str.imbue(std::locale(setlocale(LC_ALL, nullptr)));
			str << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
			if (str.fail()) throw std::runtime_error("Writing date failed!");

			return str.str();
		}

		inline std::wstring to_upper(std::wstring text)
		{
			std::transform(std::begin(text), std::end(text), std::begin(text), [](wchar_t c)
				{
					return static_cast<wchar_t>(std::toupper(c));
				}
			);

			return text;
		}

		static std::vector<std::wstring> split(std::wstring_view text, const wchar_t delimiter)
		{
			std::vector<std::wstring> tokens;
			std::wstring token;
			std::wistringstream tokenStream(text.data());
			while (std::getline(tokenStream, token, delimiter))
			{
				tokens.push_back(token);
			}
			return tokens;
		}

		static constexpr inline bool contains(std::wstring_view text, const wchar_t ch)
		{
			return std::wstring_view::npos != text.find_first_of(ch);
		}
	}

	namespace detail
	{

		inline uint8_t to_cron_int(std::wstring_view text)
		{
			try
			{
				return static_cast<uint8_t>(std::stoul(text.data()));
			}
			catch (const std::exception& ex)
			{
				throw bad_cronexpr(ex.what());
			}
		}

		inline std::wstring replace_ordinals(
			std::wstring text,
			const std::vector<std::wstring>& replacement)
		{
			for (size_t i = 0; i < replacement.size(); ++i)
			{
				const auto pos = text.find(replacement.at(i));
				if (std::wstring::npos != pos)
					text.replace(pos, 3, std::to_wstring(i));
			}

			return text;
		}

		inline std::pair<uint8_t, uint8_t> make_range(
			std::wstring_view field,
			const uint8_t minval,
			const uint8_t maxval)
		{
			uint8_t first = 0;
			uint8_t end = 0;
			if (field.size() == 1 && field.at(0) == '*')
			{
				first = minval;
				end = maxval;
			}
			else if (!utils::contains(field, '-'))
			{
				first = to_cron_int(field);
				end = first;
			}
			else
			{
				auto parts = utils::split(field, '-');
				if (parts.size() != 2)
					throw bad_cronexpr("Specified range requires two fields");

				first = to_cron_int(parts.at(0));
				end = to_cron_int(parts.at(1));
			}

			if (first > maxval || end > maxval)
			{
				throw bad_cronexpr("Specified range exceeds maximum");
			}
			if (first < minval || end < minval)
			{
				throw bad_cronexpr("Specified range is less than minimum");
			}
			if (first > end)
			{
				throw bad_cronexpr("Specified range start exceeds range end");
			}

			return { first, end };
		}

		template <size_t N>
		static void set_cron_field(
			std::wstring_view value,
			std::bitset<N>& target,
			const uint8_t minval,
			const uint8_t maxval)
		{
			if (value.length() > 0 && value[value.length() - 1] == ',')
				throw bad_cronexpr("Value cannot end with comma");

			auto fields = utils::split(value, ',');
			if (fields.empty())
				throw bad_cronexpr("Expression parsing error");

			for (const auto& field : fields)
			{
				if (!utils::contains(field, '/'))
				{
					auto [first, end] = detail::make_range(field, minval, maxval);

					for (uint8_t i = first - minval; i <= end - minval; ++i)
					{
						target.set(i);
					}
				}
				else
				{
					auto parts = utils::split(field, '/');
					if (parts.size() != 2)
						throw bad_cronexpr("Increment must have two fields");

					auto [first, end] = detail::make_range(parts[0], minval, maxval);

					if (!utils::contains(parts[0], '-'))
					{
						end = maxval;
					}

					auto delta = detail::to_cron_int(parts[1]);
					if (delta <= 0)
						throw bad_cronexpr("Increment must be a positive value");

					for (uint8_t i = first - minval; i <= end - minval; i += delta)
					{
						target.set(i);
					}
				}
			}
		}

		template <typename Traits>
		static void set_cron_days_of_week(
			std::wstring value,
			std::bitset<7>& target)
		{
			auto days = utils::to_upper(value);
			auto days_replaced = detail::replace_ordinals(
				days,
				Traits::DAYS
			);

			if (days_replaced.size() == 1 && days_replaced[0] == '?')
				days_replaced[0] = '*';

			set_cron_field(
				days_replaced,
				target,
				Traits::CRON_MIN_DAYS_OF_WEEK,
				Traits::CRON_MAX_DAYS_OF_WEEK);
		}

		template <typename Traits>
		static void set_cron_days_of_month(
			std::wstring value,
			std::bitset<31>& target)
		{
			if (value.size() == 1 && value[0] == '?')
				value[0] = '*';

			set_cron_field(
				value,
				target,
				Traits::CRON_MIN_DAYS_OF_MONTH,
				Traits::CRON_MAX_DAYS_OF_MONTH);
		}

		template <typename Traits>
		static void set_cron_month(
			std::wstring value,
			std::bitset<12>& target)
		{
			auto month = utils::to_upper(value);
			auto month_replaced = replace_ordinals(
				month,
				Traits::MONTHS
			);

			set_cron_field(
				month_replaced,
				target,
				Traits::CRON_MIN_MONTHS,
				Traits::CRON_MAX_MONTHS);
		}

		template <size_t N>
		inline size_t next_set_bit(
			const std::bitset<N>& target,
			size_t /*minimum*/,
			size_t /*maximum*/,
			size_t offset)
		{
			for (auto i = offset; i < N; ++i)
			{
				if (target.test(i)) return i;
			}

			return INVALID_INDEX;
		}

		inline void add_to_field(
			std::tm& date,
			const cron_field field,
			const int val)
		{
			switch (field)
			{
			case cron_field::second:
				date.tm_sec += val;
				break;
			case cron_field::minute:
				date.tm_min += val;
				break;
			case cron_field::hour_of_day:
				date.tm_hour += val;
				break;
			case cron_field::day_of_week:
			case cron_field::day_of_month:
				date.tm_mday += val;
				break;
			case cron_field::month:
				date.tm_mon += val;
				break;
			case cron_field::year:
				date.tm_year += val;
				break;
			}

			if (INVALID_TIME == utils::tm_to_time(date))
				throw bad_cronexpr("Invalid time expression");
		}

		inline void set_field(
			std::tm& date,
			const cron_field field,
			const int val)
		{
			switch (field)
			{
			case cron_field::second:
				date.tm_sec = val;
				break;
			case cron_field::minute:
				date.tm_min = val;
				break;
			case cron_field::hour_of_day:
				date.tm_hour = val;
				break;
			case cron_field::day_of_week:
				date.tm_wday = val;
				break;
			case cron_field::day_of_month:
				date.tm_mday = val;
				break;
			case cron_field::month:
				date.tm_mon = val;
				break;
			case cron_field::year:
				date.tm_year = val;
				break;
			}

			if (INVALID_TIME == utils::tm_to_time(date))
				throw bad_cronexpr("Invalid time expression");
		}

		inline void reset_field(
			std::tm& date,
			const cron_field field)
		{
			switch (field)
			{
			case cron_field::second:
				date.tm_sec = 0;
				break;
			case cron_field::minute:
				date.tm_min = 0;
				break;
			case cron_field::hour_of_day:
				date.tm_hour = 0;
				break;
			case cron_field::day_of_week:
				date.tm_wday = 0;
				break;
			case cron_field::day_of_month:
				date.tm_mday = 1;
				break;
			case cron_field::month:
				date.tm_mon = 0;
				break;
			case cron_field::year:
				date.tm_year = 0;
				break;
			}

			if (INVALID_TIME == utils::tm_to_time(date))
				throw bad_cronexpr("Invalid time expression");
		}

		inline void reset_all_fields(
			std::tm& date,
			const std::bitset<7>& marked_fields)
		{
			for (size_t i = 0; i < marked_fields.size(); ++i)
			{
				if (marked_fields.test(i))
					reset_field(date, static_cast<cron_field>(i));
			}
		}

		inline void mark_field(
			std::bitset<7>& orders,
			const cron_field field)
		{
			if (!orders.test(static_cast<size_t>(field)))
				orders.set(static_cast<size_t>(field));
		}

		template <size_t N>
		static size_t find_next(
			const std::bitset<N>& target,
			std::tm& date,
			const unsigned int minimum,
			const unsigned int maximum,
			const unsigned int value,
			const cron_field field,
			const cron_field next_field,
			const std::bitset<7>& marked_fields)
		{
			auto next_value = next_set_bit(target, minimum, maximum, value);
			if (INVALID_INDEX == next_value)
			{
				add_to_field(date, next_field, 1);
				reset_field(date, field);
				next_value = next_set_bit(target, minimum, maximum, 0);
			}

			if (INVALID_INDEX == next_value || next_value != value)
			{
				set_field(date, field, static_cast<int>(next_value));
				reset_all_fields(date, marked_fields);
			}

			return next_value;
		}

		template <typename Traits>
		static size_t find_next_day(
			std::tm& date,
			const std::bitset<31>& days_of_month,
			size_t day_of_month,
			const std::bitset<7>& days_of_week,
			size_t day_of_week,
			const std::bitset<7>& marked_fields)
		{
			unsigned int count = 0;
			unsigned int maximum = 366;
			while (
				(!days_of_month.test(day_of_month - Traits::CRON_MIN_DAYS_OF_MONTH) ||
					!days_of_week.test(day_of_week - Traits::CRON_MIN_DAYS_OF_WEEK))
				&& count++ < maximum)
			{
				add_to_field(date, cron_field::day_of_month, 1);

				day_of_month = date.tm_mday;
				day_of_week = date.tm_wday;

				reset_all_fields(date, marked_fields);
			}

			return day_of_month;
		}

		template <typename Traits>
		static bool find_next(const cronexpr& cex,
			std::tm& date,
			const size_t dot)
		{
			bool res = true;

			std::bitset<7> marked_fields;
			std::bitset<7> empty_list;

			unsigned int second = date.tm_sec;
			auto updated_second = find_next(
				cex.seconds,
				date,
				Traits::CRON_MIN_SECONDS,
				Traits::CRON_MAX_SECONDS,
				second,
				cron_field::second,
				cron_field::minute,
				empty_list);

			if (second == updated_second)
			{
				mark_field(marked_fields, cron_field::second);
			}

			unsigned int minute = date.tm_min;
			auto update_minute = find_next(
				cex.minutes,
				date,
				Traits::CRON_MIN_MINUTES,
				Traits::CRON_MAX_MINUTES,
				minute,
				cron_field::minute,
				cron_field::hour_of_day,
				marked_fields);
			if (minute == update_minute)
			{
				mark_field(marked_fields, cron_field::minute);
			}
			else
			{
				res = find_next<Traits>(cex, date, dot);
				if (!res) return res;
			}

			unsigned int hour = date.tm_hour;
			auto updated_hour = find_next(
				cex.hours,
				date,
				Traits::CRON_MIN_HOURS,
				Traits::CRON_MAX_HOURS,
				hour,
				cron_field::hour_of_day,
				cron_field::day_of_week,
				marked_fields);
			if (hour == updated_hour)
			{
				mark_field(marked_fields, cron_field::hour_of_day);
			}
			else
			{
				res = find_next<Traits>(cex, date, dot);
				if (!res) return res;
			}

			unsigned int day_of_week = date.tm_wday;
			unsigned int day_of_month = date.tm_mday;
			auto updated_day_of_month = find_next_day<Traits>(
				date,
				cex.days_of_month,
				day_of_month,
				cex.days_of_week,
				day_of_week,
				marked_fields);
			if (day_of_month == updated_day_of_month)
			{
				mark_field(marked_fields, cron_field::day_of_month);
			}
			else
			{
				res = find_next<Traits>(cex, date, dot);
				if (!res) return res;
			}

			unsigned int month = date.tm_mon;
			auto updated_month = find_next(
				cex.months,
				date,
				Traits::CRON_MIN_MONTHS,
				Traits::CRON_MAX_MONTHS,
				month,
				cron_field::month,
				cron_field::year,
				marked_fields);
			if (month != updated_month)
			{
				if (date.tm_year - dot > Traits::CRON_MAX_YEARS_DIFF)
					return false;

				res = find_next<Traits>(cex, date, dot);
				if (!res) return res;
			}

			return res;
		}
	}

	template <typename Traits>
	inline cronexpr make_cron(std::wstring_view expr)
	{
		cronexpr cex;

		if (expr.empty())
			throw bad_cronexpr("Invalid empty cron expression");

		auto fields = utils::split(expr, ' ');
		fields.erase(
			std::remove_if(std::begin(fields), std::end(fields),
				[](std::wstring_view s) {return s.empty(); }),
			std::end(fields));
		if (fields.size() != 6)
			throw bad_cronexpr("cron expression must have six fields");

		detail::set_cron_field(fields[0], cex.seconds, Traits::CRON_MIN_SECONDS, Traits::CRON_MAX_SECONDS);
		detail::set_cron_field(fields[1], cex.minutes, Traits::CRON_MIN_MINUTES, Traits::CRON_MAX_MINUTES);
		detail::set_cron_field(fields[2], cex.hours, Traits::CRON_MIN_HOURS, Traits::CRON_MAX_HOURS);

		detail::set_cron_days_of_week<Traits>(fields[5], cex.days_of_week);

		detail::set_cron_days_of_month<Traits>(fields[3], cex.days_of_month);

		detail::set_cron_month<Traits>(fields[4], cex.months);

		return cex;
	}

	template <typename Traits = cron_standard_traits>
	inline std::tm cron_next(const cronexpr& cex, std::tm date)
	{
		time_t original = utils::tm_to_time(date);
		if (INVALID_TIME == original) return {};

		if (!detail::find_next<Traits>(cex, date, date.tm_year))
			return {};

		time_t calculated = utils::tm_to_time(date);
		if (INVALID_TIME == calculated) return {};

		if (calculated == original)
		{
			add_to_field(date, detail::cron_field::second, 1);
			if (!detail::find_next<Traits>(cex, date, date.tm_year))
				return {};
		}

		return date;
	}

	template <typename Traits = cron_standard_traits>
	inline std::time_t cron_next(const cronexpr& cex, const std::time_t& date)
	{
		std::tm val;
		std::tm* dt = utils::time_to_tm(&date, &val);
		if (dt == nullptr) return INVALID_TIME;

		time_t original = utils::tm_to_time(*dt);
		if (INVALID_TIME == original) return INVALID_TIME;

		if (!detail::find_next<Traits>(cex, *dt, dt->tm_year))
			return INVALID_TIME;

		time_t calculated = utils::tm_to_time(*dt);
		if (INVALID_TIME == calculated) return calculated;

		if (calculated == original)
		{
			add_to_field(*dt, detail::cron_field::second, 1);
			if (!detail::find_next<Traits>(cex, *dt, dt->tm_year))
				return INVALID_TIME;
		}

		return utils::tm_to_time(*dt);
	}

	inline int cron_cnt(std::time_t prev, const cronexpr& cex, std::time_t next, int maxCnt)
	{
		int cnt = 0;
		for (int i = 0; i < maxCnt; ++i)
		{
			prev = cron::cron_next(cex, prev);
			if (prev <= next)
				++cnt;
			else
				break;
		}
		return cnt;
	}
}

