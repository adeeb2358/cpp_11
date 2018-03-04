/*
* @Author: adeeb2358
* @Date:   2018-03-04 13:52:43
* @Last Modified by:   adeeb2358
* @Last Modified time: 2018-03-04 14:25:33
*/


#include "bigHeader.h"

template<typename T>
using StrKeyMap = std::map<std::string ,T>;

template<typename Stream>
struct StreamDeleter{
	void operator()(Stream* os)const{
		os->close();
		delete os;
	}
};

template<typename Stream>
using StreamPtr = std::unique_ptr<Stream,StreamDeleter<Stream>>;


/*
 	Using using instead of typedef
 */
using PlaneID = int;
using Func = int(*)(double,double);


template<typename T>
void print(const T& t){
	t.print();
}


void template_alias_check(){
	StrKeyMap<std::string> myKeyMap;
	myKeyMap["first"] = "good";
	myKeyMap.insert(std::pair<std::string,std::string>("adeeb","is a good boy"));
	myKeyMap.clear();
	StreamPtr<std::ofstream> p_log(new std::ofstream("mylog.log"));
	*p_log << "Log Statement";
	//stream gets closed and deleted here

	/*
	 	local types as template arguments
	 */
	{
		struct A{
			std::string name() const{return "I'm A!";};
		};

		std::vector<A> v(10);
		std::cout << v[0].name() << std::endl;

	}

	/*
	 	unanamed types as template arguments
	 */

	{
		struct{
			int x = 10;
			void print() const{
				std::cout << x;
			}
		}a;

		print(a);
	}

	
	return;
}

