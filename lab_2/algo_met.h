#ifndef ALGO_MET_H
#define ALGO_MET_H

#include <iostream>
#include <vector>

template< class Tdata >
	class GenerateRandomNumericWithAlgoritmic {
private :
//******************************************************************//
	std::vector< Tdata > m_array ;
//******************************************************************//
	const int64_t LCG_a = 16807     ;
	const int64_t LCG_c = 0         ;
	const int64_t LCG_m = 2147483647;						 
//******************************************************************//

public :
//******************************************************************//
	GenerateRandomNumericWithAlgoritmic() {}
//******************************************************************//
	GenerateRandomNumericWithAlgoritmic( 
												const GenerateRandomNumericWithAlgoritmic &_copy ) = delete ;
	GenerateRandomNumericWithAlgoritmic& operator=( 
												const GenerateRandomNumericWithAlgoritmic &other ) = delete ;
//******************************************************************//
~GenerateRandomNumericWithAlgoritmic() {}
//******************************************************************//
std::vector< Tdata > GET_ARRAY() const { return m_array ; }
void ADD_ARRAY( Tdata value ) { m_array.push_back( static_cast< Tdata >( value ) ) ; }
//******************************************************************//
	void LinearCongruentialGenerator() {
		if(!m_array.empty()) m_array.push_back((LCG_a *  m_array.back() + LCG_c ) % LCG_m) ; 
		else m_array.push_back( 1 ) ;
	}
//******************************************************************//
	void PrintVector() {
		for( const auto &itm : m_array ) std::cout << itm << " " ;
	}
//******************************************************************//
} ; // class GenerateRandomNumericWithAlgoritmic 
#endif // ALGO_MET_H