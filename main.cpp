#include "bigHeader.h"

int func(double){
	return 10;
}

std::function<bool()> g(){
			static auto a = 5;
			static auto b = -3;
			return [](){ return a+b > 0;};
}

std::function<bool()> f(){
	auto a = 5;
	auto b = -3;
	//wont compile if a & b arent compiled because a and b are local variables
	return [a,b](){ return a+b > 0;};	
}

template<class Func>
	
	void by_const_ref(const Func &f){f();}

/*
	find out febenocci series using lambda expression
*/	

std::function<int(int)> fibonacci = [&](int n)-> int{
	if(n < 1){
		return -1;
	}else if(n == 1 || n == 2){
		return 1;
	}else {
		return fibonacci(n-1) + fibonacci (n-2);
	}
};	

int main(int argc, char *argv[]){

	auto a = 5.0,b = 10.0;
	auto i = 1.0, *ptr = &a, &ref = b;
	//auto j = 10, str = "error"; compiler error
	
	/*
		variable declaration using auto
	*/
	{
		std::map <std::string, int> index;

		auto &ref = index;
		auto *ptr = &index;
		const auto j = index; 
		index.clear();
		const auto& cref = index;
	}

	/*
		const and volatile specifiers are removed
		
	*/

	{

		/*
		example of vector
		*/

		const std::vector<int> values;
		auto a = values; // here type of a is vector<int> here const is removed
		auto &b = values; // here type of b is const vector <int>&
	
		volatile long clock = 0;
		auto c = clock; // c is not volatile anymore

		struct JetPlane{
			int a;
			std::string name;
		};

		JetPlane fleet[10];
		auto e = fleet; // type of e is JetPlane*
		auto &f = fleet; //type of f is JetPlane(&)[10] - a reference

		auto g = func; //type of g is int(*)double
		auto &h = func; // type of h is int(&)(double)


	}
	/*
		example of decltype specifier
	*/
	{
		int i = 10;
		std::cout << typeid(decltype(i+1.0)).name() << std::endl;
		std::cout << typeid(decltype(i)).name() << std::endl;
	}

	/*
		usage of declval
	*/

	{
		class A{
			private: 
				A();
		};

		//std::cout << typeid(decltype(A())).name()<< std::endl; //doesnt compile
		std::cout << typeid(decltype(std::declval<A>())).name();
	}

	/*
		lambda expression in detail
		contains three parts
		1)lambda introducer
		2) parameter list
		3) body of the lamda expression

	*/
	{

		/*
			perfect example of a lambda expression for
			avoiding unwanted boiler plates
		*/
		const auto const_val = [&]{
			auto const_val = 0;
			if(const_val == 0){
				const_val = 1000;
			}
			return const_val;
		}();//lamda is invoked immediately

	
		/*
			lambda parameters
			No defaul values for parameters
			No variable length arguments
			no unnamed parameters
		*/
		
		auto f = [](int i){return i >10; };
		bool result = f(5);
		std::cout <<"Result= "<< result << std::endl;
	}

	/*
		end of code
	*/

	/*
		use of std::function to 
		store lambda expression
	*/

	{
		class LambdaStore{
		private:	
			std::function<bool(double)> _stored_lambda;
		public:
			std::function<int(int)> get_abs() const{
				return [](int i){ return abs(i);};
			}
		
			void set_lambda(const std::function<bool(double)>& lambda){
				_stored_lambda = lambda;
			}	
		};
	
		LambdaStore ls;
		ls.set_lambda([](double d){ return d > 0.0;});

		auto abs_lambda = ls.get_abs();
		std::cout <<"abs_lambda= " <<abs_lambda(-10)<<std::endl;;
	}

	/*
		variable capturing in lambda expression
	*/

	{

		/*
			default variable capturing modes in c++11
		*/
		int a = 0 ,b = 0 ,c = 0, d = 0;
		//all valriables pass by value
		[=](){ return(a > b) && (c < d);};

		//all variables capture by reference
		[&]() {a = 20;};

		//override defaut capture by value
		[=,&b](){ b = 20; };

		//override default capture by reference
		[&,d]() {
			//d = 20; doesnt compile
			return d*20;};

	}

	/*
		mutable lambdas
		mutable allow variables captured by value to be changed
	*/

	

	by_const_ref([]{});	

	by_const_ref([]() mutable {});

	std::string s("this is a good string");
	/*
		rwsults in error
	*/
	//by_const_ref([s]() mutable {std::cout << s << std::endl;} );

	/*
		basic syntax of a lambda expresion
		[capture_block](parameter_list) mutable exception_spec -> return_type {body}
	*/

	return 0;
}