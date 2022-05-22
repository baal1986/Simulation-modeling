#ifndef MATRIX_H
#define MATRIX_H

#include<QVector>
#include<QString>

#include<algorithm>


namespace MTRX {
//---------------------------------------------------------------------------------------//
template< class Tdata >
  class Matrix {
private :
//************************************************************************************//
  Tdata **matrix ;
  Tdata *y ;
  size_t row  ;
  size_t col ;
//************************************************************************************//
public :
//************************************************************************************//
  Matrix() = delete ;
  Matrix( Matrix< Tdata > &_copy ) = delete ;
  Matrix< Tdata >& operator=( Matrix< Tdata > &matrix ) = delete ;
//************************************************************************************//
  Matrix( const size_t &row, const size_t &col, Tdata **v, Tdata *y )
    : matrix( v ), y(y), row( row ), col( col ) {}
//************************************************************************************//
  virtual ~Matrix() {  }
//************************************************************************************//
  double * GaussMeth() {

    double **a = this->matrix ;
    double *y = this->y ;

    int n = col ;
    double *x, max ;
    int k, index ;
    const double eps = 0.01 ;  // точность
    x = new double[n] ;
    k = 0 ;

    while( k < n ) {
      max = abs( a[k][k] ) ;
      index = k ;
      for( int i = k + 1 ; i < n ; i++ ) {
        if( abs(a[i][k]) > max ) {
          max = abs( a[i][k] ) ;
          index = i ;
        }
      }

      for( int j = 0 ; j < n ; j++ ) {
        double temp = a[k][j] ;
        a[k][j] = a[index][j] ;
        a[index][j] = temp ;
      }
      double temp = y[k] ;
      y[k] = y[index] ;
      y[index] = temp ;

      for( int i = k ; i < n ; i++ ) {
        double temp = a[i][k] ;
        if( abs(temp) < eps ) continue ;
        for( int j = 0 ; j < n ; j++ ) a[i][j] = a[i][j] / temp ;
        y[i] = y[i] / temp ;
        if( i == k ) continue ;
        for(int j = 0; j < n; j++) a[i][j] = a[i][j] - a[k][j] ;
        y[i] = y[i] - y[k] ;
      }
      k++ ;
    } // while

    for( k = n - 1 ; k >= 0 ; k-- ) {
      x[k] = y[k] ;
      for( int i = 0 ; i < k ; i++) y[i] = y[i] - a[i][k] * x[k] ;
    }

   return x;
  }
//************************************************************************************//
} ;
//---------------------------------------------------------------------------------------//
} // namespace MTRX


#endif // MATRIX_H
