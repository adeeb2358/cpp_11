/*
* @Author: adeeb2358
* @Date:   2018-03-04 16:15:24
* @Last Modified by:   adeeb2358
* @Last Modified time: 2018-03-04 21:12:48
*/

#include "bigHeader.h"
#include "class_init.h"

/*
 1)	In class intitializer for non static data memebers
 2) Delegating constructors
 3) Inheriting constructors
 4) Default methods
 5) Deleted methods
 6) override and final specifiers
 7) Extented friend delcarations
 8) Nested access rights
 */ 


class Engine{};

class JetPlane {
	private: 
		
		std::string model           = "unknown";
		std::string manufacturer    = "unknown";
		//std::vector<Engine>	engines = {Engine(),Engine()};
		std::vector<Engine> engines {getEngineCount(manufacturer,model)};
	public:
		JetPlane(const std::string& manufacturer,const std::string& model)
		:manufacturer(manufacturer),model(model){

		}
		
		static size_t getEngineCount(
			const std::string& manufacturer,
			const std::string& model
			){
			
			if (manufacturer == "Unknown" || model == "Unknown")
                return 0;
				
			if (manufacturer == "Airbus"){
                if (model == "A380-500")
                    return 2;
            }
		}

};

/*
 	Inhertiing constructors
*/
class Plane{
	private:
		std::vector<Engine> engines;
		std::string manufacturer;
		std::string model;
	public:
		Plane(const std::string& manufacturer)
		:manufacturer(manufacturer){

		}
};
	
/*
 	end of code
*/

class Boat{
	private:
		std::string boat_manufacturer;
	public:
		Boat(const std::string& manufacturer)
		: boat_manufacturer(manufacturer){

		}
};

class SmallPlane : public Plane{
	using Plane::Plane;
};

class PropPlane : public Plane{
	public:
		using Plane::Plane;
		//overrides the plane constructor with same parameters
		PropPlane(const std::string& manufacturer)
		:Plane(manufacturer){
			std::cout << "In PropPlane()" << std::endl;
		}
};

class FloatPlane : public Plane,public Boat{
	
	private:
		
	public:
		using Plane::Plane;
		using Boat::Boat;
		FloatPlane(const std::string& manufacturer)
		:Plane(manufacturer),Boat("n/a"){

		}
};

/*
 	delegating constrcutor
*/

class BigPlane{
	private:	
		std::vector<Engine> engines;
		std::string manufacturer;
		std::string model;
	
	public:
		BigPlane(): BigPlane(2,"Unknown","Unknown"){

		}


	private:
		BigPlane(
			size_t engine_count,
			const std::string& manufacturer,
			const std::string& model
			):
			engines(engine_count),
			manufacturer(manufacturer),
			model(model){
				//func1 shall be called
		}
};

/*
 	Default methods inside a class
 	a class consist of 
 	defualt constructor 
 	default assignment operator
 	default copy etc
 	if we override any of the method
 	c++ will stops from generating the default
 	version associated with
 	so we can reintiate this with the following syntax
*/

class MediumPlane{
	public:
		MediumPlane() = default;
		MediumPlane(const MediumPlane& other); //here the copy constructor is not defualt
		MediumPlane(MediumPlane&&) = default;

};

class LargePlane{
public:
	LargePlane() = default;
	virtual ~LargePlane() = default;
protected:
	LargePlane(const LargePlane& other) = default;
	LargePlane& operator=(const LargePlane&) = default;
};

/*
 	deleted methods means that its uncallable
 	or unusable just opposite of default
 	it can be used to any functions
 	1) Dsiable some instantiations of a template
 */

class DeletePlane{
public:
	DeletePlane()                   = default;
	DeletePlane(const DeletePlane&) = delete;
	DeletePlane& operator           =(const DeletePlane&) = delete;
	DeletePlane(DeletePlane&&)      = default;
	DeletePlane& operator           =(DeletePlane&&) = default;
};

/*
 	override and final
 	override allows one method to override from
 	its base class implementation
 	final tells that it cant be overrided any more
 	when final is applioed to a class
 	it tells that it cant be inherited
 */

/*
 	Extented friend declaration
 	friend delcaration without the class keyword
 */

class A;
class B;

class Friend{
	friend class A; //old declaration its still ok
	friend B; //we can also do this now
};
typedef B B2;

class Amigo{
	friend B2;
	friend class D; //declares new class D
	//friend D; //error undeclared class found
};	

template <typename T, typename U>
class Ami{
	friend T; //OK
/*	friend class U; 
	//still and error, cant use an eloborate specifier in a template
	
*/
};

/*
 	Nested class access rights
 	can access other classes
 */

class NestedPlane{
private:
	int flap_angle;
	class GPSNavigator {};
	class AutoPilot{
		GPSNavigator gps_navigator;
		void adjustFlaps(NestedPlane& plane ,int flap_angle){
			plane.flap_angle = flap_angle;
		}
	};
};

/*
  	Uniformity in initialization
	we can use braces for all kind of initialization
*/

int x{5};
class Point{
public:
	int x,y;
	Point(int x,int y)
	:x(x),y(y){

	}
};

class Hexagon{
	int points[6];
	Hexagon(): points{1,2,3,4,5,6}{

	}
};

std::vector<int> myVector{1,2,3,4};

std::vector<int> extract_core_points(const std::vector<int>& v){
	return {v.front(),v[v.size()/2],v.back()};
}

void check_class_init(){
	JetPlane myJetPlane("Airbus","A380-500");
	SmallPlane mySmallPlane("Boeing");
	PropPlane prop_plane("ATR");	
	FloatPlane("Boeing FloatPlane");
	Ami<std::string,std::string> rc; //OK
	Ami<char,std::string> f;// OK friend char has no effect in the template

	Point p1{10,20};
	Point p2 = {10,20};

	std::vector<int> core_points = extract_core_points({1,2,3,4,5,6});
	return;
}