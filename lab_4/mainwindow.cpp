#include<QStringList>
#include<QPushButton>
#include<QDebug>
#include<QVariant>
#include<QMessageBox>

#include<sstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) , ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}


void MainWindow::on_pushButton_clicked() {

    MDL::DistributionLaw *DL_g = new MDL::DistributionLaw() ;
    MDL::DistributionLaw::Law  law_g =  DL_g->uniform ;

    MDL::DistributionLaw *DL_p = new MDL::DistributionLaw() ;
    MDL::DistributionLaw::Law law_p ;

    int index_of_tab = ui->tabWidget->currentIndex() ;

    int time_modeling = (ui->lineEdit_time->text()).toInt() ;

    double a_g = (ui->lineEdit_a->text()).toDouble() ;
    double b_g = (ui->lineEdit_b->text()).toDouble() ;

    double a_p, b_p, i_p ;
    bool isErr = true ;

    if( a_g < 0 || b_g <= 0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                           tr("Введите интервал для генератора заявок") ) ;
    else {
      switch( index_of_tab ) {
        case 0 :
          a_p = (ui->lineEdit_a_u->text()).toDouble() ;
          b_p = (ui->lineEdit_b_u->text()).toDouble() ;
          if( a_p < 0 || b_p <= 0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                 tr("Введите интервал равномерного распределения для обработчика заявок") ) ;
          else { law_p = DL_p->uniform ;  isErr = false ; }
        break ;
        case 1 :
          i_p = (ui->lineEdit_i_p->text()).toDouble() ;
          if( i_p == 0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                 tr("Введите Мат.ожидание для обработчика заявок") ) ;
          else { law_p = DL_p->poisson ;  isErr = false ; }
        break ;
        case 2 :
          i_p = (ui->lineEdit_i_e->text()).toDouble() ;
          if( i_p == 0 ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                 tr("Введите Мат.ожидание для обработчика заявок") ) ;
          else { law_p = DL_p->exponential ;  isErr = false ; }
        break ;
        case 3 :
          a_p = (ui->lineEdit_a_n->text()).toDouble() ;
          b_p = (ui->lineEdit_b_n->text()).toDouble() ;
          if( a_p < 0 || b_p < 0) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                                 tr("Введите Мат.ожидание и дисперсию для обработчика заявок") ) ;
          else { law_p = DL_p->normal ;  isErr = false ; }
        break ;
      }

      if( isErr ) QMessageBox::information( NULL, QObject::tr( "Информация" ),
                    tr("Вы ввели неверные параметры!") ) ;
      else {
        MDL::Modeling *MODEL = nullptr ;

        if( law_p == DL_p->uniform || law_p == DL_p->normal ) {
          MODEL = new MDL::Modeling( law_g, a_g, b_g, law_p, a_p, b_p ) ;
        }
        else if( law_p == DL_p->poisson || law_p == DL_p->exponential ) {
          MODEL = new MDL::Modeling( law_g, a_g, b_g, law_p, i_p ) ;
        }

        MODEL->Run( time_modeling ) ;

        std::stringstream ss;
        QString str = "genereted_tasks" ;
          ss << "Сгенерировано заявок : " << MODEL->GetStat().GetInfoWithKey(str) << "\n" ;
        QString str2 = "completed_task" ;
        ss << "Обработанно заявок : " << MODEL->GetStat().GetInfoWithKey(str2) << "\n" ;
        QString str3 = "time" ;
        ss << "Время обработки заявок : " << MODEL->GetStat().GetInfoWithKey(str3) << "\n\n\n" ;
        ss << "Оптимальная длина очереди сообщений равна: " <<
              static_cast<double>(MODEL->GetStat().GetInfoWithKey(str2) * time_modeling ) /
                                static_cast<double>(MODEL->GetStat().GetInfoWithKey(str3)) ;


        QString s = QString::fromStdString(ss.str());
        ui->textBrowser->setText( s ) ;

        delete MODEL ;
      }

    }
    delete DL_g, DL_p ;
}
