/*
* @Author: adeeb2358
* @Date:   2018-03-04 11:32:57
* @Last Modified by:   adeeb2358
* @Last Modified time: 2018-03-04 12:59:02
*/


#include "bigHeader.h"
#include "var_temp.h"
/*
 Expansion of template parameter pack

 @tparam     Stream   { description }
 @tparam     Columns  { description }
*/
template<typename Stream, typename... Columns>
class CSVPrinter{
	public:	
		/*
		 constaining parameter packs to one type
		
		 @param[in]  s        { parameter_description }
		 @param[in]  strings  The strings
		
		 @tparam     Strings  { description }
		*/
		template<typename... Strings>
		void outputStrings(const std::string& s,const Strings&... strings) const{
			writeColumn(s,word_delimeter);
			outputStrings(strings...);
		}

		/**
		 * @brief      { function_description }
		 *
		 * @param[in]  s     { parameter_description }
		 */
		void outputStrings(const std::string& s){
			writeColumn(s,"\n");
		}

		/**
		 * @brief      { function_description }
		 *
		 * @param[in]  columns  The columns
		 */
		void outputLine(const Columns&... columns) const{
			writeLine(validateColoumn(columns)...);
		}
		/**
		 * @brief      { function_description }
		 *
		 * @param      _stream  The stream
		 * @param[in]  headers  The headers
		 *
		 * @tparam     Headers  { description }
		 */
		template<typename... Headers>
		CSVPrinter(Stream& _stream,const Headers&... headers)
			:_stream(_stream),headers({std::string(headers)...}){
		 	static_assert(sizeof...(Headers) == sizeof...(Columns), 
            "Number of headers must match number of columns");
		}

		/**
		 * @brief      { function_description }
		 */
		void outputHeaders(){
			std::for_each(headers.begin(),headers.end()-1,
					[=](const std::string& header){
						writeColumn(header,word_delimeter);
					}

				);
			writeColumn(headers.back(),"\n");
		}
		
	private:
		
		Stream& _stream;
		std::array<std::string,sizeof...(Columns)> headers;
		std::string word_delimeter = ",";
		std::string line_delimeter = "\n";

		/**
		 * @brief      Writes a line.
		 *
		 * @param[in]  value   The value
		 * @param[in]  values  The values
		 *
		 * @tparam     Value   { description }
		 * @tparam     Values  { description }
		 */
		template <typename Value, typename... Values>
		void writeLine(const Value& value, const Values&... values) const{
			writeColumn(value,word_delimeter);
			writeLine(values...);
		}

		/**
		 * @brief      Writes a line.
		 *
		 * @param[in]  value  The value
		 *
		 * @tparam     Value  { description }
		 */
		template<typename Value>
		void writeLine(const Value& value) const{
			writeColumn(value,line_delimeter);
		}

		/**
		 * @brief      Writes a column.
		 *
		 * @param[in]  value      The value
		 * @param[in]  delimeter  The delimeter
		 *
		 * @tparam     Value      { description }
		 */
		template<typename Value>
		void writeColumn(const Value& value,const std::string& delimeter) const{
			_stream << value << delimeter;
		}

		/**
		 * @brief      { function_description }
		 *
		 * @param[in]  value  The value
		 *
		 * @tparam     Value  { description }
		 *
		 * @return     { description_of_the_return_value }
		 */
		template<typename Value>
		const Value& validateColoumn(const Value& value) const{
			return value;
		}

};


/*
	traversing template parameter pack 
*/


/**
 * @brief      { struct_description }
 *
 * @tparam     Types  { description }
 */
template<typename... Types> 
struct TupleSize;

template<typename Head, typename... Tail> // traverse types
                                          //
struct TupleSize<Head, Tail...> 
{
    static const size_t value = sizeof(Head) + TupleSize<Tail...>::value;
};

/**
 * @brief      { struct_description }
 */
template<> struct TupleSize<>   // end recursion
{ 
    static const size_t value = 0;
};

template<typename Head, typename... Tail> 
const size_t TupleSize<Head, Tail...>::value;

const size_t TupleSize<>::value;

/*
 	end of traversing parameter packs for size requitements
*/

// example of expanding a template parameter pack into a set of base classes
//
// @tparam     Bases  { description }
//
template<typename... Bases>
class Derived : public Bases...
{};

/*
 	Mested variadic templates
*/

/**
 * @brief      { struct_description }
 *
 * @tparam     Args1  { description }
 */
template<typename... Args1>
struct zip{
	template<typename... Args2>
	struct with{
		typedef std::tuple<std::pair<Args1,Args2>...> type;
	};
};

/*
 	template with two  parameter pack

 */
template <size_t... Ns>
struct Indexes 
{};
template<typename... Ts, size_t... Ns>
auto cherry_pick(const std::tuple<Ts...>& t, Indexes<Ns...>) ->
    decltype(std::make_tuple(std::get<Ns>(t)...)) 
{
    return std::make_tuple(std::get<Ns>(t)...);
}

/**
 * @brief      { function_description }
 */
void check_var_temp(){
	std::ofstream csvStream("csv.txt");
	CSVPrinter<decltype(csvStream),
		std::string,
		std::string,
		std::string,
		std::string,
		std::string > printer(csvStream,"RollNo","Name","Sem","Course","Place");
	
	printer.outputHeaders();
	for(auto i = 0; i < 20; i++){
		printer.outputLine(
			std::to_string(i),
			"Name"+std::to_string(i),
			"Sem"+std::to_string(i),
			"Course"+std::to_string(i),
			"Place"+std::to_string(i)
		);
	}	
	
	csvStream.close();
	std::cout << TupleSize<std::string,int ,double,long>::value;

	/*
	 	nested variadic templates
	 	T1 is a tuple<pair<short,unsigned short>,pair<int,unsigned>>
	 
	typedef zip<short>::with<unsigned short,unsigned ::type T2>;
	this statement will go for error, different number of arguments
	specified for Args1 and Args2
	 */
	typedef zip<short,int>::with<unsigned short,unsigned>::type T1;

	/*
	 	two parameter packs with single function template
	 */
	   auto data = std::make_tuple(10, 12012013, "B737", "Boeing 737", 2, 125000000);
       auto cherry_picked = cherry_pick(data,Indexes<0,2,4>()); 
}