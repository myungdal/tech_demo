// ============================================================================
// myungdal tech-demo project
// 
// Author: 안명달 (Myungdal Ahn)
// Email: mooondal@gmail.com
// GitHub: https://github.com/myungdal/tech_demo.git
// ============================================================================

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 표준 라이브러리 헬퍼 함수
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 튜플의 각 요소에 함수를 적용하는 헬퍼 함수들.

// 재귀 종료 조건: 크기가 0인 경우.
template <typename _Tuple, typename _Function, size_t _I, size_t _Size>
inline typename std::enable_if<0 == _Size, void>::type ForEachImpl(_Tuple&, _Function)
{
}

// 재귀 종료 조건: 마지막 요소인 경우.
template <typename _Tuple, typename _Function, size_t _I, size_t _Size>
inline typename std::enable_if<_I == _Size - 1, void>::type ForEachImpl(_Tuple& tuple, _Function function)
{
	function(std::get<_I>(tuple));
}

// 재귀 단계: 현재 요소에 함수 적용 후 다음 요소로 진행.
template <typename _Tuple, typename _Function, size_t _I, size_t _Size>
inline typename std::enable_if<0 != _Size && _I < _Size - 1, void>::type ForEachImpl(_Tuple& tuple, _Function function)
{
	function(std::get<_I>(tuple));
	ForEachImpl<_Tuple, _Function, _I + 1, _Size>(tuple, function);
}

// 튜플의 각 요소에 함수를 적용한다.
// - tuple: 대상 튜플
// - function: 적용할 함수
template <typename _Tuple, typename _Function>
void ForEach(_Tuple& tuple, _Function function)
{
	ForEachImpl<_Tuple, _Function, 0, std::tuple_size<std::decay_t<_Tuple>>::value>(tuple, function);
}

//template<std::size_t I = 0, typename Func, typename... Tp>
//inline typename std::enable_if<I == sizeof...(Tp), void>::type
//ExecuteForTuple(std::tuple<Tp...>&&, Func) 
//{
//}
//
//template<std::size_t I = 0, typename Func, typename... Tp>
//inline typename std::enable_if < I < sizeof...(Tp), void>::type
//	ExecuteForTuple(std::tuple<Tp...>&& t, Func f) {
//	f(std::get<I>(t));
//	ExecuteForTuple<I + 1, Func, Tp...>(std::move(t), f);
//}

