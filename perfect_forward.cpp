/*
* @Author: adeeb2358
* @Date:   2018-03-18 17:57:20
* @Last Modified by:   adeeb2358
* @Last Modified time: 2018-03-18 22:22:50
*/

#include "bigHeader.h"
#include "perfect_forward.h"

/*
 	perfect forwarding problem is solved this way
*/

template<typename T,typename T1, typename T2>
std::unique_ptr<T> make_unique(T1&& arg1,T2&& arg2){
	return std::unique_ptr<T>(new T(
		std::forward<T>(arg1),
		std::forward<T>(arg2)
		));
}

template<typename T,typename... Args>
std::unique_ptr<T> make_unique(Args&&... args){
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/*
 	reference collapsing and rvalues in templates

 	A& & becomes A&
 	A& && becomes A&
 	A&& & becomes A&
 	A&& && becomes A&&

 	template<typename T> void f(T&&);
 	f(lvalue A) => T is a& => F(a& &&) => F(a&)
 	F(RVALUE a)=> T is A => f(A&&)
*/

/*
 	constant expression
 */

constexpr long fibonacci(int n){
	return n < 1 ? -1:((n == 1 || n == 2)?1:fibonacci(n-1)+fibonacci(n-2));
}


void check_perfect_forward(){
	auto a = 10;
	const auto b = a;
	//constexpr auto d = b; //wont compile

	const auto abc = 145;
	constexpr auto c = abc;

}
