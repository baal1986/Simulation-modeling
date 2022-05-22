#ifndef RANDOM_ANALYSIS_H
#define RANDOM_ANALYSIS_H

#include <iostream>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <map>
#include <cmath>
#include <sstream> 


template< class Tdata>
	class RandomAnalysis {
private :
//******************************************************************//
	std::vector< Tdata > m_array ; 
//******************************************************************//
	std::map< Tdata, int64_t > m_map ;
//******************************************************************//

public :
//******************************************************************//
	RandomAnalysis() {}	
//******************************************************************//								
	RandomAnalysis( const RandomAnalysis &_copy ) 					 = delete ;
	RandomAnalysis& operator=( const RandomAnalysis &other ) = delete ;
//******************************************************************//
	~RandomAnalysis() {}
//******************************************************************//
	void ADD_ARRAY( Tdata value ) { 
		m_array.push_back( static_cast< Tdata >( value ) ) ; 
	}
//******************************************************************//
	void ADD_MAP() { 
		for( const  auto &itm : m_array ) ++m_map[itm] ; 
	}
//******************************************************************//
	std::vector<Tdata> ConvertToBinary( const std::vector< Tdata > &array ) {
		Tdata x ;
		for( const auto &itm : array ) {
			bool flag = false ;
			for ( int i = (sizeof( Tdata ) << 3) ; i >= 0 ; i-- ) {
				x= ((itm >> i) & 1) ;
				if( x == 1 ) flag = true ;
				if( flag ) ADD_ARRAY( x ) ;
      }
		}
  return ( m_array ) ;
	}
//******************************************************************//
	std::pair<Tdata, int64_t> Test_1() {
		ADD_MAP() ;
		
		if( double(std::max(m_map[0], m_map[1]) / (double)(m_map[0] + m_map[1])) < 0.01 ) 
			std::cout << "Последовательность НЕ является истинно случайной\n" ;
		else std::cout << "Последовательность является истинно случайной\n" ;

	return std::make_pair( m_map[0], m_map[1] ) ;
	}
//******************************************************************//
	void Test_2() {
		
		int res  = abs(m_map[1] - m_map[0]) / (pow( 2*(m_map[0]+m_map[1])  ,0.5 ) ) ;
		if( erfc( res ) > 0.001 ) std::cout << "Последовательность является истинно случайной\n" ;
		else std::cout << "Последовательность НЕ является истинно случайной\n" ;
	}
//******************************************************************//
	void Test_3() {

		int pi = m_map[1] / (m_map[0] + m_map[1]) ;

		int64_t count_ones = 0 ;
		if( abs(pi-0.5) <= 2/ (pow(m_map[0] + m_map[1], 0.5)) ) {

			auto begin = m_array.begin() ;
			auto end   = m_array.end  () ;

			while( begin != end ) {
        int64_t c = 0 ;
	        while( begin != end && *begin == 1 ) {
	            if( begin != end ) begin++ ; c++ ;
	        }
	        if( c > 1 ) count_ones++ ;
	        if( begin != end ) begin++ ;
			}

			if( erfc( (count_ones-2*pi*(m_map[0]+m_map[1])*(1-pi)) / 
								(2*(pow((2*(m_map[0]+m_map[1])),0.5))*pi*(1-pi) ) ) > 0.001 )
				std::cout << "Последовательность является истинно случайной\n" ;
			else std::cout << "Последовательность НЕ является истинно случайной\n" ;
		} else std::cout << "Последовательность НЕ является истинно случайной\n" ;
	}
//******************************************************************//
	void PrintVector() {
		for( const auto &itm : m_array ) std::cout << itm << " " ;
	}
//******************************************************************//
	void PrintMap() {
		for( const auto &[ k, v ] : m_map ) std::cout << k << " : " << v << "\n" ;
	}
//******************************************************************//
} ; // class RandomAnalysis
#endif // RANDOM_ANALYSIS_H