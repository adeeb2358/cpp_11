/*
* @Author: adeeb2358
* @Date:   2018-03-04 22:41:10
* @Last Modified by:   adeeb2358
* @Last Modified time: 2018-03-18 22:33:23
*/

#include "bigHeader.h"
#include "move_semantics.h"

/*
 	Move semantics in c++11
 	L value reference and R Value reference
	
	lvalue
	has a name
	can have address taken

	eg: var
	*ptr
	arr[n]
	++x

	r value
	doesn't have a name
	Cant have address taken
	this pointer is an r value expression by defenition
	
	the below statements has no meaning and is invalid
	eg:- &(a*b)
	&x++
	&string("abc")

	std::vector<int> v;
	v[0];// lvalue because vector<int>::operator[] retiurns int&
	v.size(); //r value because vector<int>::size returns size_T
	string s;
	s+"abc" // rvalue because string::operaetor+ returns string
 */
class HighPlane{

	
};



class A1{
	public:
		A1(){
		std::cout <<"A default constructor called" <<std::endl;

	}

	A1(const A1& rhs){
		std::cout <<"A Copy constructor called" <<std::endl;

	}
};

class A{
public:
	A(){
		std::cout <<"A default constructor called" <<std::endl;
	}

	A(const A& rhs){
		//copy constructor => lvalues and const rvalues are passwed to the copy constructor
		std::cout <<"A Copy constructor called" <<std::endl;
	}
	
	A(A&& rhs){
		//move constructor =>non const rvalues are passed here
		std::cout <<"A move constructor called" <<std::endl;
	}
};

std::string f(){
	//here f returns an r value
	return std::string("return rvalue");
}

const std::string g(){
	return std::string("returns const rvalue");
}

void f1(HighPlane& high){
	std::cout << "HighPlane& called" <<std::endl;
}

void f1(const  HighPlane&& high){
	std::cout << "HighPlane&& called" << std::endl;
}

void f1(const HighPlane& high){
	std::cout << "const HighPlane& called" << std::endl;
}
auto make_const_jet = []() -> const HighPlane{
	return HighPlane();
};

class CustomMove{
	double _d;
	int *_p;
	std::string _str;

	
public:
	
	//copy constructor
	CustomMove(CustomMove&& rhs):_d(rhs._d),_p(rhs._p),_str(std::move(rhs._str)){
		rhs._p = nullptr;
		rhs._str.clear();
	}
	
	//copy assignment operator
	CustomMove& operator=(CustomMove&& rhs){
		delete _p;
		_d = rhs._d;
		_p = rhs._p;
		_str = std::move(rhs._str);
		rhs._p = nullptr;
		rhs._str.clear();

		return *this;
	}


};

class OtherPlane{
	std::string _model = "new model";
public:
	void set_model(const std::string& model){
		std::cout <<"copy constructor called"<<std::endl;;	
		_model = model;
	}

	void set_model(std::string&& model){
		std::cout <<"move constructor called"<<std::endl;;
		_model = std::move(model);
		model.clear();
	}
};

//Reference qualifiers for member function

struct Aa{
	bool run() const &{ std::cout << "executed lvalue "<<std::endl;return true;}
	bool run() && {std::cout <<"executed rvalue reference"<<std::endl;return false;};		
};


class MyOperator{
	private:
		int count = 0;
	public:
		MyOperator(int count):count(count){

		}	

		MyOperator& operator+=(const MyOperator& otherOperator){
			//take a copy of this operator
			count += otherOperator.count;
			return *this;
		}

		MyOperator operator+(const MyOperator& otherOperator) const &{
			MyOperator m(*this);
			m += otherOperator;
			return m;
		}

		MyOperator operator+(const MyOperator& otherOperator) &&{
			std::cout << "Calling MyOperat &&"<<std::endl;
			*this+= otherOperator;
			return std::move(*this);
		}

};

struct Curious{
	int count = 10;
	Curious& operator ++() &{
		++count;
		return *this;
	}

	Curious* operator &() &{
		return this;
	}
};

/*
 Making move only tupes for better peformance

*/

class MyMoveOnlyType{

private:
	int *p;
public:
	
	MyMoveOnlyType():p(new int[10]){

	}

	~MyMoveOnlyType(){
		delete p;
	}
	
	MyMoveOnlyType(const MyMoveOnlyType& rhs) = delete;
	MyMoveOnlyType& operator=(const MyMoveOnlyType& rhs) = delete;

	//move constructor
	MyMoveOnlyType(MyMoveOnlyType&& rhs){
		*this = std::move(rhs);
	}
	//move assignment
	MyMoveOnlyType& operator=( MyMoveOnlyType&& rhs){
		if(this == &rhs){
			return *this;
		}

		p = rhs.p;
		rhs.p = nullptr;
		return *this;
	}


};

void check_move_semant(){
	std::string adeeb_lvalue = "adeeb mohammed"; //string adeeb mohammed is rvalue
	std::string adeeb_next_val = adeeb_lvalue + "good boy"; // adeeb_lvalue + " good boy" is rvalue because + operator returns a string
	int a = 5 ;
	int b = 9 ;
	int c = (a*b);

	int d[5] = {0};

	//std::cout << &(a+b);
	// std::cout << &(d++) ;
	std::vector<int> v;
	v.push_back(123);
	//v[0] = 123;
	std::cout<<"size=" << v.size() << std::endl;
	//05 01 1991
	const int max_power_level = 100; // its const l value
	HighPlane highPlane;//non const lvalue
	//both lvalue and rvalue can have a const attribute

	//overload resolution
	f1(make_const_jet());
	HighPlane high;

	//speed variation in lvalue reference and rvalue reference
	
	std::vector<A> vA;
	vA.push_back(A());
	std::cout <<"==>One A insertion ends"<<std::endl;
	vA.push_back(A());

	std::vector<A1> vA1;
	vA1.push_back(A1());
	std::cout <<"==>One A1 insertion ends"<<std::endl;
	vA1.push_back(A1());

	/*
	 	compiler generated move semantics
	 	no user declared copy constructor or copy assignment operator
	 	No user declared move assignment operator
	 	no user declared destructor
	 	the move constructor wouldnt be implicilty marked as deleted
	
		move has to be done by member by member

		read of the object in its current state if its initialized 
		transfer the ownership
		leacve the source object in a valid state
	 */

	std::string model("Airbus 320");
	OtherPlane otherPlane;

	otherPlane.set_model(model);
	otherPlane.set_model(std::string("Airbus 320"));

	/*
	 	rules for move semantics

	 	dont return const from our functions
	 	overload resolution copy constructor will be invoked rather than move constructor
		const T f() and const f(const T&&) no use

		for derived class , if we have a base class with move constructor,
		for derived class we must be careful

		Derived (Derived&& rhs): Base(rhs){} // copy constructor of the base class will be called
		Derived (Derived&& rhs): BAse(std::move(rhs)){} -> move constructor will be called

		B(B&& rhs){
			*this = rhs // invokes the copy assignement operator
			*this = move(rhs) ==> solution
		}


		always check for self assignment 
		otherwise udefined behavior

		B& operator=(B&& rhs){

			if(this == &rhs){
				return *this;
			}
			_p \ rhs._p;
			rhs._p = nullptr;
			return this;

		}

		dont use explicit std::move



	 */
	Aa aa;
	aa.run();// ;value called
	Aa().run(); // rba;ie refernce called

	//addomh temporary objects
	MyOperator o1(11);
	MyOperator o2(11);
	MyOperator o3(0);

	o3 = o1 + o2;
	o3 = MyOperator(25) + o2;

	Curious() = Curious(); //assign to rvalue
	//Curious& c_val = ++Curious(); // c is a dangling reference
	//&Curious(); // address of rvalue
	//
	MyMoveOnlyType a_value;
	//MyMoveOnlyType b(a); wont compile because copy assignemnt is deleted
	MyMoveOnlyType c_value(std::move(a_value));

	MyMoveOnlyType d_value;
	d_value = std::move(a_value);

}
