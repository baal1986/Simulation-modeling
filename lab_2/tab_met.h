#ifndef TAB_MET_H
#define TAB_MET_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

template< class Tdata >
	class GenerateRandomNumericWithTables {
private :
//******************************************************************//
	std::vector< Tdata > m_array ;
//******************************************************************//
	
public :
//******************************************************************//
	GenerateRandomNumericWithTables() {}
//******************************************************************//
	GenerateRandomNumericWithTables( 
														const GenerateRandomNumericWithTables &_copy ) = delete ;
	GenerateRandomNumericWithTables& operator=( 
														const GenerateRandomNumericWithTables &other ) = delete ;
//******************************************************************//
~GenerateRandomNumericWithTables() {}
//******************************************************************//
std::vector< Tdata > GET_ARRAY() const { return m_array ; }
void ADD_ARRAY( Tdata value ) { m_array.push_back( static_cast< Tdata >( value ) ) ; }
//******************************************************************//
	void ReadFile( const std::string &file ) {
		std::ifstream input( file ) ;
		std::string line ;
		
		if( input ) {
		  while( getline( input, line, ' ') ) 
		  	ADD_ARRAY( std::stoi ( line, nullptr, 10 ) ) ;
		}
}
//******************************************************************//
	void PrintVector() {
		for( const auto &itm : m_array ) std::cout << itm << " " ;
	}
//******************************************************************//
} ; //class GenerateRandomNumericWithTables
#endif // TAB_MET_H