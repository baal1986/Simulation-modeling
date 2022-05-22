#include<QMessageBox>
#include<QDebug>
#include<QTableWidget>
#include<QTableView>
#include<QVariant>
#include<QModelIndex>
#include<QString>

#include<sstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrix.h"

//***********************************************************************************************//
MainWindow::MainWindow( QWidget *parent )
  : QMainWindow( parent ), ui(new Ui::MainWindow) {
  ui->setupUi( this ) ;
}
//***********************************************************************************************//
MainWindow::~MainWindow() { delete ui ; }
//***********************************************************************************************//

//***********************************************************************************************//
void MainWindow::on_state_next_clicked() {
  int count_state = (ui->txt_count_state->text()).toInt() ;
  if( count_state <= 0 || count_state > 11 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                              tr("Количество состояний должно быть больше 0 и меньше 11") ) ;
  else {
   ui->tableWidget->clear();
   ui->tableWidget->setRowCount(count_state);
   ui->tableWidget->setColumnCount(count_state);
   ui->tableWidget->horizontalHeader();
   ui->tableWidget->show();
  }
}
//***********************************************************************************************//
void MainWindow::on_state_next_2_clicked() {
  size_t row = ui->tableWidget->rowCount() ;
  size_t col = ui->tableWidget->columnCount() ;
  if( row <=0 || col <=0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                                     tr("Сначало задайте количество состояний!") ) ;
  else {

    double **v = new double * [row] ;
    for( size_t i = 0; i < row; i++ ) v[i] = new double [col];

    double *y = new double[row];

    QVariant myData ;
    QModelIndex myIndex ;
    for( size_t i = 0 ; i < row ; ++i ) {
        if( i == (row - 1) ) y[i] = 1 ;
        else y[i] = 0 ;
      for( size_t j = 0 ; j < col ; ++j ) {
        if( i == (row - 1) ) v[i][j] = 1 ;
        else{
          myIndex = ui->tableWidget->model()->index( i, j, QModelIndex() ) ;
          myData  = ui->tableWidget->model()->data( myIndex, Qt::DisplayRole ) ;
          v[i][j] = myData.toDouble() ;
        }
      }
    }

    MTRX::Matrix< double > *matrix = new MTRX::Matrix< double >( row, col, v, y ) ;
    std::stringstream ss;

    double *x = matrix->GaussMeth();
    /*double res = 1 ;
    for( size_t i = 0 ; i < row ; i++ )  res -= x[i] ;
    x[row-1] = res;*/
       ss << "Сиcтема будет на ходится в состоянии \n" ;
      for( size_t i = 0; i < col; i++ )
        //ss << "x[" << i << "]=" << x[i] << " ; ";
        ss <<" #" << i + 1<<":  " << x[i] * 100 <<" % времени"<< " ; \n" ;

    QString s = QString::fromStdString(ss.str());
    ui->textBrowser->setText( s ) ;

    delete matrix ;
    for( size_t i = 0; i < row; i++ ) delete v[i] ;
    delete [] v ;
  }

}
//***********************************************************************************************//









