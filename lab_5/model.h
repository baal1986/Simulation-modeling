#ifndef MODEL_H
#define MODEL_H

#include<QString>
#include<QQueue>
#include<QTime>
#include<QMap>
#include<QDebug>
#include<QList>
#include<QPair>

#include<algorithm>
#include<random>


namespace MDL {

  class Clients{
  private :
    QQueue< double > queue ;
    const double a ;
    const double b ;
  public :
    Clients( int &size, const double &a, const double &b )
      :a(a),b(b){ CreateClients(size); }

    double Generate() {
      std::random_device rd    ;
      std::mt19937 gen( rd() ) ;
      std::uniform_real_distribution<> dis( a, b ) ;
      return dis( gen ) ;
    }

    void CreateClients( int size ) {
      for( int i = 0 ; i < size ; i++ )
        queue.push_back( Generate() ) ;
    }
    QQueue< double >& GetQueue() { return this->queue ; }


  };

  class Operator{
  private :
    double current_task ;
    const double a ;
    const double b ;
  public :
    Operator( const double &a, const double &b )
      :current_task(0), a(a),b(b){ }


    double Generate() {
      std::random_device rd    ;
      std::mt19937 gen( rd() ) ;
      std::uniform_real_distribution<> dis( a, b ) ;
      return dis( gen ) ;
    }
    double GetCurrentTask() const { return current_task ; }
    void SetCurentTask( double current_task ) { this->current_task = current_task; }

  };

  class Computer{
  private:
    const double time_processing_task = 0 ;
    QQueue< double > queue ;
  public:
    Computer( const double time_processing_task )
      :time_processing_task(time_processing_task) {}
    QQueue< double >& GetQueue() { return this->queue ; }
    double Get_time_processing_task() const { return  time_processing_task ; }
  };

  class Modeling {
  private :
  //****************************************************************************************************//
    struct Statistics {
    private:
      QMap< QString , uint64_t > stats ;
    public :
      void Add( const QString &s, const uint64_t &i ) { stats[s] = i ; }
      uint64_t GetInfoWithKey( const QString &s ) { return stats[s] ; }
    } STAT ;
  //****************************************************************************************************//
  public :
  //****************************************************************************************************//
    Statistics& GetStat() { return STAT ; }
  //****************************************************************************************************//
  public :
  //****************************************************************************************************//
    Modeling()                                            {}
    Modeling( const Modeling &_copy )                     = delete ;
    const Modeling& operator=( const Modeling &modeling ) = delete ;
  //****************************************************************************************************//
    virtual ~Modeling() {}
  //****************************************************************************************************//
    bool isFree( const QMap< int, double > &task_in_processing ) {
      for( auto it = task_in_processing.begin() ; it != task_in_processing.end() ; ++it ){
        if( it.value() == 0 ) return true ;
      }
      return false ;
    }
  //****************************************************************************************************//
    int BestCapacity( const QMap< int, int > &capacity_operators,
                      const QMap< int, double > &task_in_processing ) {

      QSet< QPair< int, double > > list_task_in_processing ;
      for( auto it = task_in_processing.begin() ; it != task_in_processing.end() ; ++it )
        if( it.value() == 0 ) list_task_in_processing.insert( qMakePair( it.key(), it.value() ) ) ;

      QSet< QPair< int, int > > list_capacity_operators ;
      for( auto it = capacity_operators.begin() ; it != capacity_operators.end() ; ++it )
        list_capacity_operators.insert(qMakePair( it.value(), it.key() ) ) ;


      for( auto it_list_capacity_operators = list_capacity_operators.rbegin() ;
                it_list_capacity_operators != list_capacity_operators.rend() ;
                 ++ it_list_capacity_operators )
      for( auto it_list_task_in_processing = list_task_in_processing.begin() ;
                it_list_task_in_processing != list_task_in_processing.end() ;
                ++ it_list_task_in_processing )

      if( it_list_capacity_operators->second == it_list_task_in_processing->first )
        return  it_list_task_in_processing->first ;

    return -1;
    }
  //****************************************************************************************************//
    void Run( double &time_modeling,
              MDL::Clients *CLIENTS, QVector< MDL::Operator * > &OPERATORS,  QVector< MDL::Computer * > &COMPUTERS,
              QMap< int, double > &task_in_processing, QMap< int, int > &capacity_operators ) {

      const double step_time = 0.01 ;

       STAT.Add( "time_modeling", time_modeling ) ;
       double time_modeling_for_computer = time_modeling ;
      //МОДЕЛИРУЕМ РАБОТУ ОПЕРАТОРОВ
      STAT.Add( "all_tasks_modeling_stage_#_1", static_cast<int>(CLIENTS->GetQueue().size() ) ) ;
      double current_time_stage_2  = 0 ;
      int    count_rejection_tasks = 0 ;
      int    count_acception_tasks = 0 ;

      double time_start = 0 ;
      double time_for_computers = 0 ;

      QVector< bool > is_add_first_task = QVector< bool >()={ false,false,false } ;

    if( time_modeling > 0 ){
        while( CLIENTS->GetQueue().size() > 0 && time_modeling > time_start ) {
          current_time_stage_2 += step_time ;
          time_start+= step_time;
          time_modeling -= step_time ;
          for( auto it = task_in_processing.begin() ; it != task_in_processing.end() ; it++ ){
           if( it.value() - step_time > 0 && is_add_first_task[it.key()] ) it.value() -= step_time ;
           else if( it.value() - step_time <= 0 && is_add_first_task[it.key()] ) {
            if( time_for_computers == 0 ) { time_for_computers =  time_start; }
            is_add_first_task[it.key()] = false ;
            it.value() = 0 ;
            count_acception_tasks++ ;
            if( it.key() == 0 || it.key() == 1 ) {
             COMPUTERS[0]->GetQueue().push_back( OPERATORS[it.key()]->GetCurrentTask()) ;
             OPERATORS[it.key()]->SetCurentTask(0);
            }
            else if( it.key() == 2 ){
             COMPUTERS[1]->GetQueue().push_back( OPERATORS[it.key()]->GetCurrentTask()) ;
             OPERATORS[it.key()]->SetCurentTask(0);
            }
          }
         }

         if(  (CLIENTS->GetQueue().size() > 0 ) && (CLIENTS->GetQueue().head() <= current_time_stage_2 ) ){

           if( isFree(task_in_processing) ) {
            if(CLIENTS->GetQueue().size() > 0 ) CLIENTS->GetQueue().pop_front() ;

            current_time_stage_2 = 0 ;
            int best_capacity = BestCapacity( capacity_operators, task_in_processing ) ;
            if( best_capacity != -1 ) {
              double new_task = OPERATORS[best_capacity]->Generate();
              OPERATORS[best_capacity]->SetCurentTask( new_task ) ;
              task_in_processing[best_capacity] = new_task ;

              is_add_first_task[best_capacity] = true ;
            } else continue ;

           }else { count_rejection_tasks++ ;  if( CLIENTS->GetQueue().size() > 0) CLIENTS->GetQueue().pop_front() ; current_time_stage_2 = 0 ; }
         }


       }//while

        STAT.Add( "count_rejection_tasks_modeling_stage_#_1", count_rejection_tasks ) ;
        STAT.Add( "count_acception_tasks_modeling_stage_#_1", count_acception_tasks ) ;
        //МОДЕЛИРУЕМ РАБОТУ ОПЕРАТОРОВ



       //МОДЕЛИРУЕМ РАБОТУ КОМПЬЮТЕРОВ
        // Время когда была обработана первая заявка оператором - начальное время для обработки компьютером
        //time_for_computers;
        time_modeling_for_computer -= time_for_computers ;

        double cur_task_comp_1 = 0 ;
        int64_t count_acception_tasks_with_computer_1 = 0 ;
        double cur_task_comp_2 = 0 ;
        int64_t count_acception_tasks_with_computer_2 = 0 ;

        while( COMPUTERS[0]->GetQueue().size() != 0 && (time_modeling_for_computer) > cur_task_comp_1 ) {
         if( COMPUTERS[0]->GetQueue().size() > 0 ) { cur_task_comp_1 += COMPUTERS[0]->GetQueue().head() ;
          COMPUTERS[0]->GetQueue().pop_front() ;
          count_acception_tasks_with_computer_1++ ;
         }
        }
        while( COMPUTERS[1]->GetQueue().size() != 0 && (time_modeling_for_computer) > cur_task_comp_2 ) {
         if( COMPUTERS[1]->GetQueue().size() > 0 ) { cur_task_comp_2 += COMPUTERS[1]->GetQueue().head() ;
          COMPUTERS[1]->GetQueue().pop_front() ;
          count_acception_tasks_with_computer_2++ ;
         }
        }

         STAT.Add( "count_acception_tasks_modeling_stage_#_2_with_computer_1", count_acception_tasks_with_computer_1 ) ;
         STAT.Add( "count_acception_tasks_modeling_stage_#_2_with_computer_2", count_acception_tasks_with_computer_2 ) ;


       //МОДЕЛИРУЕМ РАБОТУ КОМПЬЮТЕРОВ
    }

    else if( time_modeling == 0 ) {
          while( CLIENTS->GetQueue().size() > 0 ) {
qDebug()<< CLIENTS->GetQueue().size() << "\n";
            current_time_stage_2 += step_time ;
            time_start+= step_time;
            for( auto it = task_in_processing.begin() ; it != task_in_processing.end() ; it++ ){
             if( it.value() - step_time > 0 && is_add_first_task[it.key()] ) it.value() -= step_time ;
             else if( it.value() - step_time <= 0 && is_add_first_task[it.key()] ) {
              if( time_for_computers == 0 ) { time_for_computers =  time_start; }
              is_add_first_task[it.key()] = false ;
              it.value() = 0 ;
              if( it.key() == 0 || it.key() == 1 ) {
               COMPUTERS[0]->GetQueue().push_back( OPERATORS[it.key()]->GetCurrentTask()) ;
               OPERATORS[it.key()]->SetCurentTask(0);
               count_acception_tasks++ ;
              }
              else if( it.key() == 2 ){
               COMPUTERS[1]->GetQueue().push_back( OPERATORS[it.key()]->GetCurrentTask()) ;
               OPERATORS[it.key()]->SetCurentTask(0);
               count_acception_tasks++ ;
              }
            }
           }

           if(  (CLIENTS->GetQueue().size() != 0 ) && (CLIENTS->GetQueue().head() <= current_time_stage_2 ) ){

             if( isFree(task_in_processing) ) {
              if(CLIENTS->GetQueue().size() != 0 ) CLIENTS->GetQueue().pop_front() ;

              current_time_stage_2 = 0 ;
              int best_capacity = BestCapacity( capacity_operators, task_in_processing ) ;
              if( best_capacity != -1 ) {
                double new_task = OPERATORS[best_capacity]->Generate();
                OPERATORS[best_capacity]->SetCurentTask( new_task ) ;
                task_in_processing[best_capacity] = new_task ;

                is_add_first_task[best_capacity] = true ;
              } else continue ;

             }else { count_rejection_tasks++ ;  if( CLIENTS->GetQueue().size() != 0) CLIENTS->GetQueue().pop_front() ; current_time_stage_2 = 0 ; }
           }
           if( CLIENTS->GetQueue().size() == 0 ) {
            for( auto it = task_in_processing.begin() ; it != task_in_processing.end() ; it++ ){
              if( it.key() == 0 || it.key() == 1 ) {
               COMPUTERS[0]->GetQueue().push_back( OPERATORS[it.key()]->GetCurrentTask()) ;
               OPERATORS[it.key()]->SetCurentTask(0);
               count_acception_tasks++ ;
              }
              else if( it.key() == 2 ){
               COMPUTERS[1]->GetQueue().push_back( OPERATORS[it.key()]->GetCurrentTask()) ;
               OPERATORS[it.key()]->SetCurentTask(0);
               count_acception_tasks++ ;
              }
            }
           }

         }//while

          STAT.Add( "count_rejection_tasks_modeling_stage_#_1", count_rejection_tasks ) ;
          STAT.Add( "count_acception_tasks_modeling_stage_#_1", count_acception_tasks ) ;
          //МОДЕЛИРУЕМ РАБОТУ ОПЕРАТОРОВ



         //МОДЕЛИРУЕМ РАБОТУ КОМПЬЮТЕРОВ
         STAT.Add( "count_acception_tasks_modeling_stage_#_2_with_computer_1", COMPUTERS[0]->GetQueue().size() ) ;
         STAT.Add( "count_acception_tasks_modeling_stage_#_2_with_computer_2", COMPUTERS[1]->GetQueue().size() ) ;
         //МОДЕЛИРУЕМ РАБОТУ КОМПЬЮТЕРОВ
  }


 }

  //****************************************************************************************************//

  } ;
  //--------------------------------------------------------------------------------------------------------//


}


#endif // MODEL_H
