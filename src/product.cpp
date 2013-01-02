
#include <iostream>
#include <tuple>
#include <type_traits>

using namespace std;

template <typename T>
constexpr T product(T &&t)
{
	return t * t;
}


template <typename T, typename ...P>
constexpr T 
product(T&& t, P&&... p)
{
	return t * t * product(std::forward<P>(p)...);
}

template<typename T, typename R>
constexpr T 
product2(R&& f, T&& t)
{
	return f(std::forward<T>(t));
}

template<typename T, typename R, typename... P>
constexpr T 
product2(R&& f, T&& t, P&&... p)
{
	return f(std::forward<T>(t)) * product2(std::forward<R>(f), std::forward<P>(p)...);
} 


template<typename... T, typename R>
constexpr auto 
product3(R&& f, T&&... ps, std::tuple<T...> ts)
-> decltype(product2(std::forward<R>(f), std::forward<T>(ps)...))
{
	return product2(std::forward<R>(f), std::forward<T>(ps)...);
}


template<typename... T, typename... P, typename R>
constexpr auto
product3(R&& f, P&&... ps, std::tuple<T...> ts)
-> typename std::enable_if<sizeof...(T) != sizeof...(P), 
	decltype(product3(std::forward<R>(f), 
		std::forward<P>(ps)...,
		std::forward<decltype(std::get<sizeof...(ps)>(ts))>(std::get<sizeof...(ps)>(ts)),
		std::forward<std::tuple<T...>>(ts)))>::type
{
	return product3(std::forward<R>(f), 
		std::forward<P>(ps)...,
		std::forward<decltype(std::get<sizeof...(ps)>(ts))>(std::get<sizeof...(ps)>(ts)),
		std::forward<std::tuple<T...>>(ts));
}

template<typename... T, typename R>
constexpr auto
product3(R&& f, std::tuple<T...> ts)
-> decltype(product3(std::forward<R>(f), 
		std::forward<decltype(std::get<0>(ts))>(std::get<0>(ts)),
		std::forward<std::tuple<T...>>(ts)))
{
	return product3(std::forward<R>(f), 
		std::forward<decltype(std::get<0>(ts))>(std::get<0>(ts)),
		std::forward<std::tuple<T...>>(ts));
}

int main()
{
	// std::cout << product(1,2,3,4,5) << std::endl;
	std::cout << product2([](int x) { return x*2; }, 1, 2, 3) << std::endl;

	std::cout << product3([](int x) { return x * 2; }, std::make_tuple(1,2,3)) << std::endl;
	return 0;
}
