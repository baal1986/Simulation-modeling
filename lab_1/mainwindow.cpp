#include<QtMath>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this) ;
}

MainWindow:: ~MainWindow() {
  delete ui ;
}


void MainWindow::on_pushButton_clicked() {
  double a = (ui->ledt_a->text()).toDouble() ;
  double b = (ui->ledt_b->text()).toDouble() ;
  double h =  1 ;
  {

    int N = ( b - a ) / h + 2 ;
    QVector<double> x(N), y(N) ;


    int i = 0 ;
    for ( double X = a ; X <= b ; X += h ) {
      x[i] = X ;
      y[i] = ( 1 / ( b - a ) ) ;
      i++ ;
    }

    ui->widget->clearGraphs() ;
    ui->widget->addGraph() ;
    ui->widget->graph(0)->setData(x, y) ;
    ui->widget->xAxis->setLabel("x") ;
    ui->widget->yAxis->setLabel("y") ;
    ui->widget->xAxis->setRange( a, b ) ;//Для оси Ox

    double minY = y[0], maxY = y[0] ;
    for ( int i = 1 ; i < N ; i++ ) {
      if ( y[i] < minY ) minY = y[i] ;
      if ( y[i] > maxY ) maxY = y[i] ;
    }

    ui->widget->yAxis->setRange(minY, maxY ) ;//Для оси Oy
    ui->widget->replot() ;

  }
/*************************************************************************************************************/
  {
    int i = 0 ;
    int N = ( b - a ) / h + 2 ;
    QVector<double> x(N), y(N) ;

    for ( double X = a ; X <= b ; X += h ) {
        x[i] = X ;
        y[i] = ( ( X - a ) / ( b - a ) ) ;
        i++ ;
    }

    ui->widget_2->clearGraphs() ;
    ui->widget_2->addGraph() ;
    ui->widget_2->graph(0)->setData(x, y) ;
    ui->widget_2->xAxis->setLabel("x") ;
    ui->widget_2->yAxis->setLabel("y") ;
    ui->widget_2->xAxis->setRange( a, b ) ;//Для оси Ox

    double minY = y[0], maxY = y[0] ;
    for ( int i = 1 ; i < N ; i++ ) {
      if ( y[i] < minY ) minY = y[i] ;
      if ( y[i] > maxY ) maxY = y[i] ;
    }

    ui->widget_2->yAxis->setRange(minY, maxY ) ;//Для оси Oy
    ui->widget_2->replot();

  }
}



void MainWindow::on_pushButton_2_clicked() {
  double a = (ui->ledt_a_2->text()).toDouble() ;
  double b = (ui->ledt_b_2->text()).toDouble() ;
  double h =  1 ;
  {

    int N = ( b - a ) / h + 2 ;
    QVector<double> x(N), y(N) ;


    int i = 0 ;
    for ( double X = a ; X <= b ; X += h ) {
      x[i] = X ;
      y[i] = ( (1 / (qPow( (2*M_PI),0.5 ))) * qExp( -( qPow(X, 2) / 2 ) ) ) ;
      i++ ;
    }

    ui->widget_3->clearGraphs() ;
    ui->widget_3->addGraph() ;
    ui->widget_3->graph(0)->setData(x, y) ;
    ui->widget_3->xAxis->setLabel("x") ;
    ui->widget_3->yAxis->setLabel("y") ;
    ui->widget_3->xAxis->setRange( a-10, b+10 ) ;//Для оси Ox

    double minY = y[0], maxY = y[0] ;
    for ( int i = 1 ; i < N ; i++ ) {
      if ( y[i] < minY ) minY = y[i] ;
      if ( y[i] > maxY ) maxY = y[i] ;
    }

    ui->widget_3->yAxis->setRange(minY, maxY ) ;//Для оси Oy
    ui->widget_3->replot() ;

  }
/*************************************************************************************************************/
  {
    int i = 0 ;
    int N = ( b - a ) / h + 2 ;
    QVector<double> x(N), y(N) ;

    for ( double X = a ; X <= b ; X += h ) {
        x[i] = X ;
        y[i] = ( 0.5 * ( 1+ qExp(X/(qPow(X, 0.5)) )) ) ;
        i++ ;
    }

    ui->widget_4->clearGraphs() ;
    ui->widget_4->addGraph() ;
    ui->widget_4->graph(0)->setData(x, y) ;
    ui->widget_4->xAxis->setLabel("x") ;
    ui->widget_4->yAxis->setLabel("y") ;
    ui->widget_4->xAxis->setRange( a, b ) ;//Для оси Ox

    double minY = y[0], maxY = y[0] ;
    for ( int i = 1 ; i < N ; i++ ) {
      if ( y[i] < minY ) minY = y[i] ;
      if ( y[i] > maxY ) maxY = y[i] ;
    }

    ui->widget_4->yAxis->setRange(minY, maxY ) ;//Для оси Oy
    ui->widget_4->replot();

  }

}
