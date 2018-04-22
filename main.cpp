#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

class Entity;
class Car;
class Passenger;
template <typename T>
class BagInterface;
template <typename T>
class Node;
template <typename T>
class LinkedBag;

int minute_gap(string,string);
void to_dsetination(Car,Passenger,string,char*,char);
int compute_score(int r,int c,int t,int d);
void search_for_car(Passenger,bool,char*,string);
void check_CP(Car,Passenger,string)throw(logic_error);
void check_AD(Car,Passenger,string)throw(logic_error);
class Entity                                                //Entity class
{
protected:
    string id;
    string connect_to;
    string time_tag;
    bool isOn;
    int isSer;
    int lon;
    int lat;
    int tag_cnt;
    int no;
    string tag[20];
public:
    Entity(string id,bool isOn,int isSer,int lon,int lat,int no,string time_tag):id(id),isOn(isOn),isSer(isSer),lon(lon),lat(lat),no(no),time_tag(time_tag)
    {
        tag_cnt=0;
    }
    virtual void print()=0;
    void add_tag(string s);
    bool operator==(const Entity&)const;
};

bool Entity::operator==(const Entity &e)const               //Entity comparison
{
    if(!this->id.compare(e.id))
        return true;
    else
        return false;
}

template <typename T>                                       //Bag interface, use template
class BagInterface
{
protected:
    virtual int current_size()const=0;
    virtual bool isEmpty()const=0;
    virtual bool add(const T&)=0;
    virtual bool remove(const T&)=0;
    virtual void clear()=0;
    virtual bool contain(const T&)const=0;
};

class Car:public Entity                                     //Car
{
    friend void search_for_car(Passenger,bool,char*,string);
    friend void to_destination(Car,Passenger,string,char*,char);
    friend void check_CP(Car,Passenger,string)throw(logic_error);
    friend void check_AD(Car,Passenger,string)throw(logic_error);
private:
    int total_score;
    int rate_cnt;
    char direction;
    bool normal;
public:
    Car(string id,bool normal,bool isOn,int isSer,int lon,int lat,int no,string time_tag):Entity(id, isOn, isSer, lon, lat,no,time_tag)
    {
        direction='H';
        this->normal=normal;
        total_score=0;
        rate_cnt=0;
    }
    Car(const Car&);
    void print();
    void get_online(string,int,int,char);
    void change_direction(string,char);
    void change_direction(string);
    void get_onboard(string,Passenger);
    void connect_to_passenger(Passenger p);
    string customer()                                       //return passeger name
    {
        return connect_to;
    }
    void leave_customer(string,int,int,int,char);
    void get_offline(string time_tag)
    {
        this->time_tag=time_tag;
        this->isOn=0;
        this->isSer=0;
    }
};

class Passenger:public Entity                               //Passenger
{
    friend void Car::get_onboard(string,Passenger);
    friend void search_for_car(Passenger,bool,char*,string);
    friend void to_destination(Car,Passenger,string,char*,char);
    friend void check_CP(Car,Passenger,string)throw(logic_error);
    friend void check_AD(Car,Passenger,string)throw(logic_error);
private:
    int wait;                                               //the time from connecting to a car to gettiing on board
public:
    Passenger(string id,bool isOn,int isSer,int lon,int lat,int no,string time_tag):Entity(id, isOn, isSer, lon, lat,no,time_tag)
    {
        wait=0;
    }
    Passenger(const Passenger&);
    void print();
    void get_online(string,int,int,string);
    void get_onboard(string);
    void connect_to_car(Car c);
    void get_offline();
};

template <typename T>                                       //Node
class Node
{
private:
    T item;
    Node<T> *next;
public:
    Node();
    Node(const T&);
    Node(const T&,Node<T>*);
    void setItem(const T&);
    void setNext(Node<T>*);
    T getItem()const;
    Node<T> *getNext()const;
};

template <typename T>
class LinkedBag:public BagInterface<T>                      //Linked bag
{
    friend void to_destination(Car,Passenger,string,char*,char);
    friend void search_for_car(Passenger,bool,char*,string);
    friend void check_CP(Car,Passenger,string)throw(logic_error);
    friend void check_AD(Car,Passenger,string)throw(logic_error);
private:
    Node<T> *head;
    int item_cnt;
public:
    LinkedBag();
    LinkedBag(const LinkedBag<T>&);
    virtual ~LinkedBag();
    bool isEmpty()const;
    int current_size()const;
    bool add(const T&);
    Node<T>* getPointerTo(const T&)const;
    bool remove(const T&);
    bool contain(const T&)const;
    void clear();
};
//LinkedBag instance function======================================================================================
template <typename T>
LinkedBag<T>::LinkedBag():head(nullptr),item_cnt(0){}

template <typename T>
LinkedBag<T>::LinkedBag(const LinkedBag<T> &bag)
{
    this->item_cnt=bag->item_cnt;
    Node<T> *origin=bag->head;
    if(origin==nullptr)
        this->head=nullptr;
    else
    {
        this->head=new Node<T>();
        this->head->setItem(origin->getItem());
        Node<T> *new_ptr=this->head;
        while(origin!=nullptr)
        {
            origin=origin->getNext();
            T next_item=origin->getItem();
            Node<T> *new_node_ptr=new Node<T>(next_item);
            new_ptr->setNext(new_node_ptr);
            new_ptr=new_ptr->getNext();
        }
        new_ptr->setNext(nullptr);
    }
}

template <typename T>
LinkedBag<T>::~LinkedBag()
{
    clear();
}

template <typename T>
bool LinkedBag<T>::isEmpty()const
{
    return item_cnt==0;
}

template <typename T>
int LinkedBag<T>::current_size()const
{
    return item_cnt;
}

template <typename T>
bool LinkedBag<T>::add(const T &add_new)
{
    Node<T> *new_ptr=new Node<T>(add_new);
    new_ptr->setItem(add_new);
    new_ptr->setNext(this->head);
    this->head=new_ptr;
    this->item_cnt++;
    return true;
}

template <typename T>
Node<T>* LinkedBag<T>::getPointerTo(const T &item)const
{
    bool found=false;
    Node<T> *current=this->head;
    while(!found&&(current!=nullptr))
    {
        if(item==current->getItem())
            found=true;
        else
            current=current->getNext();
    }
    return current;
}

template <typename T>
bool LinkedBag<T>::remove(const T &item)
{
    Node<T> *item_ptr=getPointerTo(item);
    bool can=!isEmpty()&&(item_ptr!=nullptr);
    if(can)
    {
        item_ptr->setItem(this->head->getItem());
        Node<T> *delete_ptr=this->head;
        this->head=this->head->getNext();
        delete delete_ptr;
        delete_ptr=nullptr;
        this->item_cnt--;
    }
    return can;
}

template <typename T>
bool LinkedBag<T>::contain(const T &item)const
{
    return (getPointerTo(item)!=nullptr);
}

template <typename T>
void LinkedBag<T>::clear()
{
    Node<T> *delete_ptr=this->head;
    while(this->head!=nullptr)
    {
        this->head=this->head->getNext();
        delete delete_ptr;
        delete_ptr=this->head;
    }
    item_cnt=0;
}
//LinkedBag instance function=======================================================================================
//Node instance function============================================================================================
template <typename T>
Node<T>::Node():next(nullptr){}
template <typename T>
Node<T>::Node(const T &n):item(n),next(nullptr){}
template <typename T>
Node<T>::Node(const T &n,Node<T> *next):item(n),next(next){}
template <typename T>
void Node<T>::setItem(const T &it)
{
    this->item=it;
}

template <typename T>
void Node<T>::setNext(Node<T> *next)
{
    this->next=next;
}

template <typename T>
T Node<T>::getItem()const
{
    return item;
}

template <typename T>
Node<T>* Node<T>::getNext()const
{
    return next;
}
//Node instance function============================================================================================
void Entity::add_tag(string s)
{
    this->tag[this->tag_cnt]=s;
    this->tag_cnt++;
}
//Car instance function=============================================================================================
Car::Car(const Car &c):Entity(c)
{
    this->total_score=c.total_score;
    this->rate_cnt=c.rate_cnt;
    this->direction=c.direction;
    this->normal=c.normal;
    for(int i=0;i<this->tag_cnt;i++)
        this->tag[i]=c.tag[i];
}
void Car::print()
{
    cout<<this->id<<" ";
    if(this->isOn==true)
    {
        cout<<this->isSer<<" ";
    }
    else
        cout<<"0 ";
    
    cout<<this->rate_cnt<<" ";
    cout<<this->total_score;
    if(isOn&&isSer==1)
        cout<<" "<<this->lon<<" "<<this->lat;
    else if(isOn&&isSer>1)
        cout<<" "<<this->connect_to;
    cout<<endl;
}
void Car::get_online(string time_tag,int lon,int lat,char direction)
{
    this->connect_to="";
    this->time_tag=time_tag;
    this->lon=lon;
    this->lat=lat;
    this->direction=direction;
    this->isOn=true;
    this->isSer=1;
}
void Car::change_direction(string time_tag,char direction)
{
    int duration=minute_gap(this->time_tag,time_tag);
    if(!this->normal)
        duration*=2;
    if(this->direction=='N')
        this->lat=(double)this->lat+duration;
    else if(this->direction=='S')
        this->lat=(double)this->lat-duration;
    else if(this->direction=='E')
        this->lon=(double)this->lon+duration;
    else if(this->direction=='W')
        this->lon=(double)this->lon-duration;
    
    this->direction=direction;
    this->time_tag=time_tag;
}
void Car::change_direction(string time_tag)
{
    int duration=minute_gap(this->time_tag,time_tag);
    if(!this->normal)
        duration*=2;
    if(this->direction=='N')
        this->lat=(double)this->lat+duration;
    else if(this->direction=='S')
        this->lat=(double)this->lat-duration;
    else if(this->direction=='E')
        this->lon=(double)this->lon+duration;
    else if(this->direction=='W')
        this->lon=(double)this->lon-duration;
    
    this->time_tag=time_tag;
}
void Car::get_onboard(string time_tag,Passenger p)
{
    this->time_tag=time_tag;
    this->lon=p.lon;
    this->lat=p.lat;
    this->isSer=3;
}

void Car::connect_to_passenger(Passenger p)
{
    this->isSer=2;
}

void Car::leave_customer(string time_tag,int lon,int lat,int score,char direction)
{
    this->isSer=1;
    this->time_tag=time_tag;
    this->connect_to="";
    this->direction=direction;
    this->lon=lon;
    this->lat=lat;
    this->total_score+=score;
    this->rate_cnt++;
}
//Car instance function=============================================================================================
//Passenger instance function=======================================================================================
Passenger::Passenger(const Passenger &p):Entity(p)
{
    this->wait=p.wait;
    for(int i=0;i<this->tag_cnt;i++)
        tag[i]=p.tag[i];
}
void Passenger::print()
{
    cout<<this->id<<" ";
    if(this->isOn==true)
    {
        if(this->isSer==1)
            cout<<"1";
        else if(this->isSer==2)
            cout<<"2";
    }
    else
        cout<<"0";
    
    if(isOn&&isSer>0)
        cout<<" "<<this->connect_to;
    cout<<endl;
}
void Passenger::get_online(string time_tag,int lon,int lat,string id)
{
    this->isOn=true;
    this->isSer=1;
    this->time_tag=time_tag;
    this->connect_to=id;
    this->lon=lon;
    this->lat=lat;
}
void Passenger::get_onboard(string time_tag)
{
    this->wait=minute_gap(this->time_tag,time_tag);
    this->time_tag=time_tag;
    this->isSer=2;
}

void Passenger::connect_to_car(Car c)
{
    this->isSer=1;
}

void Passenger::get_offline()
{
    this->isOn=0;
    this->isSer=0;
    this->connect_to="";
    this->wait=0;
}
//Passenger instance function=======================================================================================

int K,aR,bR,aL,bL,k,h,p,n,car_no=1,passenger_no=1;          //necessary variable
int profit=0,serve_cnt=0;
LinkedBag<Car>car_registered;                               //registered car
LinkedBag<Passenger>passenger_registered;                   //registered passenger
LinkedBag<Car>car_online;                                   //car online and not in service
LinkedBag<Car>car_in_service;                               //car in service
LinkedBag<Passenger>passenger_in_service;                   //passenger in service

int main()
{
    string useless;
    char statement[1000]={0};
    cin>>K>>aR>>bR>>aL>>bL>>k>>h>>p>>n;
    cin.get();                                              //'\n'
    getline(cin,useless);
    while(cin.getline(statement,sizeof(statement)))
    {
        if(strlen(statement)==0)
            break;
        char *statement_time=strtok(statement," ");
        char *operation=strtok(NULL,":");
        
        if(!strcmp(operation,"NP"))                         //new passenger
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *tags=p;
            Passenger new_p(id,0,0,0,0,passenger_no,statement_time);
            char *add_tag=strtok(tags,",");
            while(add_tag!=nullptr)
            {
                new_p.add_tag(add_tag);
                add_tag=strtok(NULL,",");
            }
            if(!passenger_registered.contain(new_p))        //if the passenger hasn't registered
            {
                passenger_registered.add(new_p);
                passenger_no++;
            }
        }
        else if(!strcmp(operation,"NC"))                    //new car
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,"");
            char *tags;
            if(p[0]==')')                                   //there is no tag
            {
                tags=nullptr;
                p=p+1;
            }
            else
            {
                tags=strtok(p,")");
                p=strtok(NULL," ");
            }
            bool normal=true;
            if(!strcmp(p,"L"))                              //if the car is not normal
                normal=false;
            Car new_c(id,normal,0,0,0,0,car_no,statement_time);
            char *add_tag=strtok(tags,",");
            while(add_tag!=nullptr)
            {
                new_c.add_tag(add_tag);
                add_tag=strtok(NULL,",");
            }
            if(!car_registered.contain(new_c))              //if the car hasn't registered
            {
                car_registered.add(new_c);
                car_no++;
            }
        }
        else if(!strcmp(operation,"OC"))                    //car get online
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *coordinate=p;
            p=strtok(NULL," ");
            char direction=*p;
            int lon=atoi(strtok(coordinate,","));
            int lat=atoi(strtok(NULL,","));
            Car search_test(id,false,0,0,lon,lat,0,statement_time);
            if(car_registered.contain(search_test)&&(!car_online.contain(search_test)&&!car_in_service.contain(search_test)))       //if the car is registered and is not online
            {
                Car c_get_online=car_registered.getPointerTo(search_test)->getItem();
                c_get_online.get_online(statement_time,lon,lat,direction);
                car_online.add(c_get_online);
            }
        }
        else if(!strcmp(operation,"EC"))                    //car changes direction
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char direction=*p;
            Car search_test(id,false,0,0,0,0,0,statement_time);
            if(car_online.contain(search_test))             //if the car is online
            {
                Car c_change_direction=car_online.getPointerTo(search_test)->getItem();
                c_change_direction.change_direction(statement_time,direction);
                car_online.remove(c_change_direction);
                car_online.add(c_change_direction);
            }
        }
        else if(!strcmp(operation,"OP"))                    //passenger get online
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *coordinate=p;
            p=strtok(NULL," ");
            bool normal=true;
            if(!strcmp(p,"L"))
                normal=false;
            Passenger search_test(id,0,0,0,0,0,statement_time);
            if(passenger_registered.contain(search_test)&&!passenger_in_service.contain(search_test))//if the passenger is registered and is not in service
            {
                Passenger p_online=passenger_registered.getPointerTo(search_test)->getItem();
                search_for_car(p_online,normal,coordinate,statement_time);
            }
        }
        else if(!strcmp(operation,"CP"))                    //the car get the passenger
        {
            char *p=strtok(NULL," ");
            string id=p;
            Car search_test(id,false,0,0,0,0,0,statement_time);
            if(car_in_service.contain(search_test))         //if the car is online
            {
                Node<Car>*catch_c=car_in_service.getPointerTo(search_test);
                Passenger search_test(catch_c->getItem().customer(),0,0,0,0,0,statement_time);
                Node<Passenger>*catch_p=passenger_in_service.getPointerTo(search_test);
                Car temp_c=catch_c->getItem();
                Passenger temp_p=catch_p->getItem();
                try
                {
                    check_CP(temp_c,temp_p,statement_time);  //if the statement is correct
                    car_in_service.remove(temp_c);
                    passenger_in_service.remove(temp_p);
                    temp_c.get_onboard(statement_time,temp_p);
                    temp_p.get_onboard(statement_time);
                    car_in_service.add(temp_c);
                    passenger_in_service.add(temp_p);
                }
                catch(logic_error)
                {
                }
            }
        }
        else if(!strcmp(operation,"AD"))                    //get to the destination
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *coordinate=p;
            p=strtok(NULL," ");
            char direction=*p;
            Car search_test(id,false,0,0,0,0,0,statement_time);
            if(car_in_service.contain(search_test))         //if the car is in service
            {
                Car destination_c=car_in_service.getPointerTo(search_test)->getItem();
                Passenger search_test_p(destination_c.customer(),0,0,0,0,0,statement_time);
                Passenger destination_p=passenger_in_service.getPointerTo(search_test_p)->getItem();
                try
                {
                    check_AD(destination_c,destination_p,statement_time);
                    to_destination(destination_c,destination_p,statement_time,coordinate,direction);
                }
                catch(logic_error e)
                {
                }
            }
        }
        else if(!strcmp(operation,"LC"))                    //car get offline
        {
            char *p=strtok(NULL," ");
            string id=p;
            Car search_test(id,false,0,0,0,0,0,statement_time);
            if(car_online.contain(search_test))
            {
                Car off_c=car_online.getPointerTo(search_test)->getItem();
                off_c.get_offline(statement_time);
                car_online.remove(off_c);
                car_registered.remove(off_c);
                car_registered.add(off_c);
            }
        }
        else if(!strcmp(operation,"SC"))                    //print the statement of car
        {
            char *p=strtok(NULL," ");
            string id=p;
            Car search_test(id,false,0,0,0,0,0,statement_time);
            if(car_in_service.contain(search_test))
            {
                Car temp=car_in_service.getPointerTo(search_test)->getItem();
                temp.print();
            }
            else if(car_online.contain(search_test)&&!car_in_service.contain(search_test))
            {
                Car temp=car_online.getPointerTo(search_test)->getItem();
                temp.change_direction(statement_time);
                temp.print();
            }
            else if(car_registered.contain(search_test)&&!car_online.contain(search_test)&&!car_in_service.contain(search_test))
            {
                Car temp=car_registered.getPointerTo(search_test)->getItem();
                temp.print();
            }
            else
                cout<<id<<": no registration!"<<endl;
        }
        else if(!strcmp(operation,"SP"))                    //print the statement of passenger
        {
            char *p=strtok(NULL," ");
            string id=p;
            Passenger search_test(id,false,0,0,0,0,statement_time);
            if(passenger_in_service.contain(search_test))
                passenger_in_service.getPointerTo(search_test)->getItem().print();
            else if(passenger_registered.contain(search_test)&&!passenger_in_service.contain(search_test))
                passenger_registered.getPointerTo(search_test)->getItem().print();
            else
                cout<<id<<": no registration!"<<endl;
        }
        else if(!strcmp(operation,"SR"))
        {
            cout<<profit<<endl;
        }
        else if(!strcmp(operation,"ZZ"))                    //system crash
        {
            car_in_service.clear();
            car_online.clear();
            passenger_in_service.clear();
        }
        memset(statement,0,sizeof(statement));
    }
    return 0;
}

int minute_gap(string was,string now)                       //the time gap between two time tags in minute
{
    int was_h,was_m,now_h,now_m;
    string a,b;
    a=was.substr(0,2);
    b=now.substr(0,2);
    was_h=stoi(a);
    now_h=stoi(b);
    a=was.substr(3,2);
    b=now.substr(3,2);
    was_m=stoi(a);
    now_m=stoi(b);
    int sum=(now_h-was_h)*60+now_m-was_m;
    return sum;
}
                                                            //when car reach destination
void to_destination(Car c,Passenger p,string time_tag,char *coordinate,char direction)
{
    int lon=atoi(strtok(coordinate,","));
    int lat=atoi(strtok(NULL,","));
    int duration=minute_gap(c.time_tag,time_tag);
    int distance=abs(lon-c.lon)+abs(lat-c.lat);
    int t_duration=distance;
    int score=4;
    if(!c.normal)                                           //estimate duration if the car is not a normal car
    {
        if(t_duration%2==1)
            t_duration=(t_duration+1)/2;
        else
            t_duration/=2;
    }
    bool free=false;
    if(p.wait>20)                                           //if the passenger wait longer than 20 minute before the car pick him up
        free=true;
    if(duration>3*t_duration)                               //duration > 3*estimate duration
        free=true;
    
    if(p.wait>20)                                           //wait > 20 min
        score-=2;
    else if(10<p.wait&&p.wait<=20)                          //10 < wait <=20
        score-=1;
    if(duration>3*t_duration)                               //duration > 3*actual duration
        score-=2;
    else if(duration>2*t_duration&&duration<=3*t_duration)  //estimate duration*2 < duration <= 2*estimate duration
        score-=1;
    
    for(int i=0;i<c.tag_cnt;i++)                            //how many tags are the same
        for(int j=0;j<p.tag_cnt;j++)
            if(!c.tag[i].compare(p.tag[j]))
                score++;
    
    if(score>5)
        score=5;
    else if(score<1)
        score=1;
    
    if(c.normal&&!free)                                     //normal car
        profit+=(aR+bR*distance);
    else if(!c.normal&&!free)                               //not normal car
        profit+=(aL+bL*distance);
    
    c.leave_customer(time_tag,lon,lat,score,direction);     //let car drop the customer and set new direction
    car_registered.remove(c);
    car_in_service.remove(c);
    car_online.add(c);
    c.get_offline(time_tag);
    car_registered.add(c);
    p.get_offline();                                        //passenger offline
    passenger_in_service.remove(p);
    passenger_registered.remove(p);
    passenger_registered.add(p);
    serve_cnt++;
}

int compute_score(int r,int c,int t,int d)                  //the score formula
{
    int ans=r-k*c+h*t-p*d;
    return ans;
}
                                                            //passenger search for car that is empty
void search_for_car(Passenger p,bool normal,char *coordinate,string time_tag)
{
    Node<Car> *current=car_online.head;
    Node<Car> *best_car=nullptr;
    int best=-10000000;
    p.lon=atoi(strtok(coordinate,","));
    p.lat=atoi(strtok(NULL,","));
    
    while(current!=nullptr)                                 //go through every car online and is empty
    {
        if(current->getItem().normal!=normal)               //the car's type is correct
        {
            current=current->getNext();
            continue;
        }
        Car car_current=current->getItem();
        car_current.change_direction(time_tag,car_current.direction);
        
        int distance=abs(p.lon-car_current.lon)+abs(p.lat-car_current.lat);
        if(distance>K)                                      //excceed K
        {
            current=current->getNext();
            continue;
        }
        int same_tag=0;
        for(int i=0;i<p.tag_cnt;i++)                        //how many tags are the same
            for(int j=0;j<car_current.tag_cnt;j++)
                if(!(p.tag[i]).compare(car_current.tag[j]))
                    same_tag++;
        int a=car_current.total_score;
        int b=car_current.rate_cnt;
        int the_score=compute_score(a,b,same_tag,distance); //compute the score
        
        if(the_score>best)
        {
            best=the_score;
            best_car=current;
        }
        else if(the_score==best)                            //if the score ties the best score
        {
            int distance_bset=abs(p.lon-best_car->getItem().lon)+abs(p.lat-best_car->getItem().lat);
            if(distance<distance_bset)
            {
                best=the_score;
                best_car=current;
            }
            else if(distance_bset==distance)
            {
                if(car_current.no<best_car->getItem().no)
                {
                    best=the_score;
                    best_car=current;
                }
            }
        }
        current=current->getNext();
    }
    if(best_car==nullptr)                                   //if there is no car qualify
        return;
    
    Car to_service=best_car->getItem();
    to_service.change_direction(time_tag,to_service.direction);
    to_service.isOn=true;
    to_service.isSer=2;
    to_service.connect_to=p.id;
    to_service.time_tag=time_tag;
    p.get_online(time_tag,p.lon,p.lat,to_service.id);
    car_in_service.add(to_service);
    passenger_in_service.add(p);
    car_online.remove(to_service);
    //============================================================================================
}

void check_CP(Car c,Passenger p,string time_tag)throw(logic_error)//check the statement of CP
{
    int duration=minute_gap(c.time_tag,time_tag);
    if(c.isSer==2&&p.isSer==1&&duration>=0)
        return;
    throw logic_error("...");
}

void check_AD(Car c,Passenger p,string time_tag)throw(logic_error)//check the statement of AD
{
    int duration=minute_gap(c.time_tag,time_tag);
    if(c.isSer==3&&p.isSer==2&&duration>=0)
        return;
    throw logic_error("...");
}
