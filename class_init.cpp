/*
* @Author: adeeb2358
* @Date:   2018-03-04 16:15:24
* @Last Modified by:   adeeb2358
* @Last Modified time: 2018-03-04 20:07:28
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

void check_class_init(){
	JetPlane myJetPlane("Airbus","A380-500");
	SmallPlane mySmallPlane("Boeing");
	PropPlane prop_plane("ATR");	
	FloatPlane("Boeing FloatPlane");
	return;
}