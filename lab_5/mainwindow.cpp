#include<QMessageBox>

#include<sstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_pushButton_clicked() {

  QString str_l_e = ui->lineEdit_2->text();
  if( str_l_e.isEmpty() ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                                tr("Вы не ввели время моделирования, будет использован 0( ноль )") ) ;

  int count_task = ui->lineEdit->text().toInt() ;
  if( count_task <= 0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                                tr("Количество клиентов это положительное число!") ) ;
  double time_modeling = ui->lineEdit_2->text().toDouble() ;
  if( time_modeling < 0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                                tr("Время должно быть положительное число или 0 если время в модели не учитывается") ) ;

  MDL::Modeling * MODEL = new MDL::Modeling();
  MDL::Clients *CLIENTS = new MDL::Clients(count_task, 8, 12);




  QVector< MDL::Operator * > OPERATORS ;
  OPERATORS.append( new MDL::Operator( 15, 25 ) ) ;
  OPERATORS.append( new MDL::Operator( 30, 50 ) ) ;
  OPERATORS.append( new MDL::Operator( 20, 60 ) ) ;


  QVector< MDL::Computer * > COMPUTERS ;
  COMPUTERS.append( new MDL::Computer( 15 ) ) ;
  COMPUTERS.append( new MDL::Computer( 30 ) ) ;


  QMap< int, int > capacity_operators = QMap< int, int >();
  capacity_operators[0] = 0 ;
  capacity_operators[1] = 0 ;
  capacity_operators[2] = 0 ;

  QMap< int, double > task_in_processing = QMap< int, double >();
  task_in_processing[0] = 0 ;
  task_in_processing[1] = 0 ;
  task_in_processing[2] = 0 ;



  MODEL->Run( time_modeling, CLIENTS, OPERATORS, COMPUTERS, task_in_processing, capacity_operators ) ;

  std::stringstream ss;
  QString str0 = "time_modeling";
  ss << "Время моделирования СМО : " << MODEL->GetStat().GetInfoWithKey(str0) << "\n" ;
  QString str = "all_tasks_modeling_stage_#_1" ;
  ss << "Всего пришло клиентов : " << MODEL->GetStat().GetInfoWithKey(str) << "\n" ;

  QString str3 = "count_rejection_tasks_modeling_stage_#_1" ;
  ss << "Отказов в обслуживании : " << MODEL->GetStat().GetInfoWithKey(str3) << "\n" ;
  QString str4 = "count_acception_tasks_modeling_stage_#_1" ;
  ss << "Обслуженно клиентов : " << MODEL->GetStat().GetInfoWithKey(str4) << "\n" ;

  QString str5 = "count_acception_tasks_modeling_stage_#_2_with_computer_1" ;
  ss << "Обработано заявок компьютером # 1 : " << MODEL->GetStat().GetInfoWithKey(str5) << "\n" ;
  QString str6 = "count_acception_tasks_modeling_stage_#_2_with_computer_2" ;
  ss << "Обработано заявок компьютером # 2 : " << MODEL->GetStat().GetInfoWithKey(str6) << "\n" ;


  QString s = QString::fromStdString(ss.str());
  ui->textBrowser->setText( s ) ;

  delete MODEL ;
  delete CLIENTS ;


}
