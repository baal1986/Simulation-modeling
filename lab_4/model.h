#ifndef MODEL_H
#define MODEL_H

#include<QString>
#include<QQueue>
#include<QTime>
#include<QMap>
#include<QDebug>

#include<algorithm>
#include<random>


namespace MDL {

//--------------------------------------------------------------------------------------------------------//
struct Task {
private:
//****************************************************************************************************//
  double id_task = 0 ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  Task()                              = delete ;
  Task( const Task &_copy )           = delete ;
  Task& operator=( const Task &task ) = delete ;
//****************************************************************************************************//
  Task( double &id_task ) : id_task(id_task) {}
//****************************************************************************************************//
  double Get_id_task() { return id_task ; }
//****************************************************************************************************//
} ;
//--------------------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------------------//
struct Queue {
private :
//****************************************************************************************************//
  QList< Task * > queue ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  void  Push( Task *task ) { this->queue.append( task ) ; }
  void Pop() { this->queue.removeFirst() ; }
//****************************************************************************************************//
  bool     isEmpty()  const { return this->queue.isEmpty() ? true : false ; }
  size_t   GetSize()  const { return this->queue.size() ; }
  double   Get_head() const { return this->queue.first()->Get_id_task() ; }
  double   Get_tail() const { return this->queue.front()->Get_id_task() ; }
  void     Print() { for( const auto &itm : this->queue ) qDebug() << itm->Get_id_task() << "\n" ; }
//****************************************************************************************************//
} ;
//--------------------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------------------//
class DistributionLaw {
private:
//****************************************************************************************************//
  struct AbsLaw {
  private :
    const double _a = 0 ;
    const double _b = 0 ;
    const int    _i = 0 ;
  public :
    double Get_a() { return _a ; }
    double Get_b() { return _b ; }
    int    Get_i() { return _i ; }

    AbsLaw( const double &a, const double &b ) : _a(a), _b(b) {}
    AbsLaw( const int &i ) : _i( i ) {}
    virtual ~AbsLaw() {}

    virtual double GetRandom() = 0 ;
  };
//****************************************************************************************************//
  struct Uniform : public AbsLaw {
    Uniform( const double &a, const double &b ) : AbsLaw( a, b ) {}
    double GetRandom() override {
      std::random_device rd    ;
      std::mt19937 gen( rd() ) ;
      std::uniform_real_distribution<> dis( Get_a(), Get_b() ) ;
    return dis( gen ) ;
    }
  } ;
//****************************************************************************************************//
  struct Poisson : public AbsLaw {
    Poisson( const int &l ) : AbsLaw( l ) {}
    double GetRandom() override {
      std::random_device rd    ;
      std::mt19937 gen( rd() ) ;
      std::poisson_distribution<> dis( Get_i() ) ;
    return dis( gen ) ;
    }
  } ;
//****************************************************************************************************//
  struct Exponential : public AbsLaw {
    Exponential( const int &l ): AbsLaw( l ) {}
    double GetRandom() override {
      std::random_device rd    ;
      std::mt19937 gen( rd() ) ;
      std::exponential_distribution<> dis ( Get_i() ) ;
    return dis( gen ) ;
    }
 } ;
//****************************************************************************************************//
  struct Normal : public AbsLaw {
    Normal( const double &m = 0, const double &s = 1 ) : AbsLaw( m, s ) {}
    double GetRandom() override {
      std::random_device rd    ;
      std::mt19937 gen( rd() ) ;
      std::normal_distribution<> dis( Get_a(), Get_b() ) ;
    return std::round( dis(gen) );
    }
 } ;
//****************************************************************************************************//
private :
//****************************************************************************************************//
  AbsLaw *pLaw = nullptr ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  enum Law { uniform = 0, poisson = 1, exponential = 2, normal = 3 } law ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  DistributionLaw( DistributionLaw &_copy )               = delete ;
  DistributionLaw& operator=( const DistributionLaw &DL ) = delete ;
//****************************************************************************************************//
  DistributionLaw()  {}
//****************************************************************************************************//
  DistributionLaw( const Law &law, const int &i ) : law( law ) {
    if( law == poisson )        pLaw = new Poisson( i )     ;
    else if(law == exponential) pLaw = new Exponential( i ) ;
  }
//****************************************************************************************************//
  DistributionLaw( const Law &law, const double &d1, const double &d2 ) : law( law ) {
    if( law == uniform )        pLaw = new Uniform( d1, d2 ) ;
    else if( law == normal )    pLaw = new Normal( d1,  d2 ) ;
  }
//****************************************************************************************************//
 ~DistributionLaw() { if( pLaw != nullptr ) delete pLaw ; }
//****************************************************************************************************//
  double GetRandom() const { return pLaw->GetRandom() ; }
//****************************************************************************************************//
} ;
//--------------------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------------------//
class Generator {
private :
//****************************************************************************************************//
  DistributionLaw *DL = nullptr ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  Generator()                                        = delete ;
  Generator( const Generator &_copy )                = delete ;
  Generator& operator=( const Generator &generator ) = delete ;
//****************************************************************************************************//
  Generator( const DistributionLaw::Law &l, const int &i )
   : DL( new DistributionLaw( l , i ) ) {}
//****************************************************************************************************//
  Generator( const DistributionLaw::Law &l, const double &d1, const double &d2 )
   : DL( new DistributionLaw( l , d1, d2 ) ) {}
//****************************************************************************************************//
  virtual ~Generator() { if( DL != nullptr ) delete DL ; }
//****************************************************************************************************//
  double Generate() const { return DL->GetRandom() ; }
//****************************************************************************************************//
} ;
//--------------------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------------------//
class Processing {
private :
//****************************************************************************************************//
  DistributionLaw *DL = nullptr ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  Processing()                                          = delete ;
  Processing( const Processing &_copy )                 = delete ;
  Processing& operator=( const Processing &processing ) = delete ;
//****************************************************************************************************//
  Processing( const DistributionLaw::Law &l, const int &i )
   : DL( new DistributionLaw( l , i ) ) {}
//****************************************************************************************************//
  Processing( const DistributionLaw::Law &l, const double &d1, const double &d2 )
   : DL( new DistributionLaw( l , d1, d2 ) ) {}
//****************************************************************************************************//
  virtual ~Processing() { if( DL != nullptr ) delete DL ; }
//****************************************************************************************************//
  double Generate() const { return DL->GetRandom() ; }
//****************************************************************************************************//
} ;
//--------------------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------------------//
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
  Generator       *GENERATOR  = nullptr ;
  Processing      *PROCESSING = nullptr ;
//****************************************************************************************************//
public :
//****************************************************************************************************//
  Statistics& GetStat() { return STAT ; }
//****************************************************************************************************//
public :
//****************************************************************************************************//
  Modeling( const DistributionLaw::Law &l_g, const double &d1_g, const double &d2_g,
            const DistributionLaw::Law &l_p, const int &i_p )
     : GENERATOR( new Generator  ( l_g, d1_g, d2_g ) ),
       PROCESSING( new Processing( l_p, i_p ) ) {}
//****************************************************************************************************//
  Modeling( const DistributionLaw::Law &l_g, const int &i_g,
            const DistributionLaw::Law &l_p, const int &i_p )
     : GENERATOR( new Generator  ( l_g, i_g ) ),
       PROCESSING( new Processing( l_p, i_p ) ) {}
//****************************************************************************************************//
  Modeling( const DistributionLaw::Law &l_g, const double &d1_g, const double &d2_g,
            const DistributionLaw::Law &l_p, const double &d1_p, const double &d2_p )
     : GENERATOR( new Generator  ( l_g, d1_g, d2_g ) ),
       PROCESSING( new Processing( l_p, d1_p, d2_p ) ) {}
//****************************************************************************************************//
  Modeling()                                            = delete ;
  Modeling( const Modeling &_copy )                     = delete ;
  const Modeling& operator=( const Modeling &modeling ) = delete ;
//****************************************************************************************************//
  virtual ~Modeling() {
    if( GENERATOR  != nullptr ) delete GENERATOR  ;
    if( PROCESSING != nullptr ) delete PROCESSING ;
  }
//****************************************************************************************************//

//****************************************************************************************************//
  void Run( const uint64_t &_max_time ) {
    Queue *QUEUE = new Queue() ;

    const double max_time = static_cast<double>(_max_time) ;
    const double step_time = 0.1 ;

    // моделируем процесс работы генератора заявок
    double time = 0 ;

    while( time < max_time ) {
     time += step_time ;
     double res = GENERATOR->Generate() ;


     if( res < time ) {
      if( time < max_time ) { if(time+res < max_time) QUEUE->Push( new Task(res) ) ; time += res ; }
     } else {
      while( res > time ) { time += step_time ; }
      if( time < max_time ) { if(time+res < max_time) QUEUE->Push( new Task(res) ) ; time += res ; }
     }

    }
    STAT.Add( "genereted_tasks", QUEUE->GetSize() ) ;
    // моделируем процесс работы генератора заявок


    // моделируем процесс работы обработчика заявок
    time = 0 ;

    uint64_t completed_task   = 0 ;
    double   time_in_queue    = QUEUE->Get_head() ;

    double time_in_processing = 0 ;

    while( time < max_time && (!QUEUE->isEmpty()) ) {
      time_in_processing += PROCESSING->Generate() ;

      if( time > time_in_processing ) {
        if( time_in_processing >= time_in_queue ) {
         completed_task++ ;
         QUEUE->Pop() ;
         if( !QUEUE->isEmpty() ) time_in_queue += QUEUE->Get_head() ;
        }
        else continue ;
      }else if( time < time_in_processing ) {
        while( time < time_in_processing ) time += step_time ;
        if( time_in_processing >= time_in_queue ) {
         completed_task++ ;
         QUEUE->Pop() ;
         if( !QUEUE->isEmpty() ) time_in_queue += QUEUE->Get_head() ;
        }
        else continue ;
      }
      if(time > time_in_processing) time += step_time ;
    }

    STAT.Add( "completed_task", completed_task ) ;
    STAT.Add( "time", time ) ;
    // моделируем процесс работы обработчика заявок

    delete QUEUE ;
  }
//****************************************************************************************************//
} ;
//--------------------------------------------------------------------------------------------------------//
}// namespace MDL

#endif // MODEL_H
