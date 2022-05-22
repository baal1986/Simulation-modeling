#include<iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <numeric>
#include <map>
#include <cmath>

#include "tab_met.h"
#include "algo_met.h"
#include "random_analysis.h"


int main() {


RandomAnalysis< int16_t > *RA_1 = new RandomAnalysis< int16_t >() ;
RandomAnalysis< int32_t > *RA_2 = new RandomAnalysis< int32_t >() ;


std::cout << "*********************\n" ;
//Табличный ГСЧ
std::cout << "Табличный ГСЧ" << "\n\n" ;
GenerateRandomNumericWithTables< int16_t > *GRNWT = new GenerateRandomNumericWithTables< int16_t >()     ;
GRNWT->ReadFile( "500_random_numeric" ) ;
//GRNWT->PrintVector() ;
RA_1->ConvertToBinary( GRNWT->GET_ARRAY() )  ;

std::cout << "Тест № 1 : " << "\n" ;
std::pair< int16_t, int64_t >res_test1  = RA_1->Test_1(); 
std::cout << "[0]: "<< res_test1.first << " | " << "[1]: " << res_test1.second << "\n\n";

std::cout << "Тест № 2 : " << "\n" ;
RA_1->Test_2(); std::cout << "\n" ;
std::cout << "Тест № 3 : " << "\n" ;
RA_1->Test_3(); std::cout << "\n" ;
std::cout << "*********************\n" ;


std::cout << "\n\n\n" ;

std::cout << "*********************\n" ;
//Алгоритмический ГСЧ
std::cout << "Алгоритмический ГСЧ" << "\n\n" ;
GenerateRandomNumericWithAlgoritmic< int32_t > *GRNWA = new GenerateRandomNumericWithAlgoritmic< int32_t >() ;
for( size_t i = 1 ; i < 50000 ; i++ ) GRNWA->LinearCongruentialGenerator() ;
RA_2->ConvertToBinary( GRNWA->GET_ARRAY() )  ;

std::cout << "Тест № 1 : " << "\n" ;
std::pair< int32_t, int64_t >res_test2  = RA_2->Test_1(); 
std::cout << "[0]: "<< res_test2.first << " | " << "[1]: " << res_test2.second << "\n\n";

std::cout << "Тест № 2 : " << "\n" ;
RA_2->Test_2(); std::cout << "\n" ;
std::cout << "Тест № 3 : " << "\n" ;
RA_2->Test_3(); std::cout << "\n" ;
std::cout << "*********************\n" ;



delete  RA_2, GRNWA ;


return 0 ;
}