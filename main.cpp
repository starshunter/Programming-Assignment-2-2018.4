#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <string>
#include <cmath>
using namespace std;

class Entity;
class Car;
class Passenger;

int minute_gap(string,string);
void to_dsetination(Car,Passenger,string,int,int,char);
void search_for_car(Passenger,bool);

class Entity
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
    string **tag;
public:
    Entity(string id,bool isOn,int isSer,int lon,int lat,string time_tag):id(id),isOn(isOn),isSer(isSer),lon(lon),lat(lat),time_tag(time_tag)
    {
        tag_cnt=0;
        tag=new string*[10000];
    }
    virtual void print()=0;
    void add_tag(string s);
};

class Car:public Entity
{
    friend void search_for_car(Passenger,bool);
    friend void to_destination(Car,Passenger,string,int,int,char);
private:
    int total_score;
    int rate_cnt;
    char direction;
    bool normal;
public:
    Car(string id,bool normal,bool isOn,int isSer,int lon,int lat,string time_tag):Entity(id, isOn, isSer, lon, lat,time_tag)
    {
        direction='H';
        this->normal=normal;
        total_score=0;
        rate_cnt=0;
    }
    void print();
    void get_online(string,int,int,char);
    void change_direction(string,char);
    void get_onboard(string,int,int);
};

class Passenger:public Entity
{
    friend void search_for_car(Passenger,bool);
    friend void to_destination(Car,Passenger,string,int,int,char);
private:
    int wait;
public:
    Passenger(string id,bool isOn,int isSer,int lon,int lat,string time_tag):Entity(id, isOn, isSer, lon, lat,time_tag)
    {
        wait=0;
    }
    void print();
    void get_online(string,string);
    void get_onboard(string);
};


void Entity::add_tag(string s)
{
    this->tag[this->tag_cnt]=&s;
    this->tag_cnt++;
}

void Car::print()
{
    cout<<this->id<<" ";
    if(this->isOn==true)
    {
        if(this->isSer==0)
            cout<<"1 ";
        else if(this->isSer==1)
            cout<<"2 ";
        else
            cout<<"3 ";
    }
    else
        cout<<"0 ";
    
    cout<<this->rate_cnt<<" ";
    cout<<this->total_score;
    if(isOn&&!isSer)
        cout<<" "<<this->lon<<" "<<this->lat;
    else if(isOn&&isSer)
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
        this->lon=(double)this->lon+duration;
    else if(this->direction=='S')
        this->lon=(double)this->lon-duration;
    else if(this->direction=='E')
        this->lon=(double)this->lat+duration;
    else if(this->direction=='W')
        this->lon=(double)this->lat-duration;
    
    this->direction=direction;
    this->time_tag=time_tag;
}
void Car::get_onboard(string time_tag,int lon,int lat)
{
    this->time_tag=time_tag;
    this->lon=lon;
    this->lat=lat;
    this->isSer=3;
}


void Passenger::print()
{
    cout<<this->id<<" ";
    if(this->isOn==true)
    {
        if(this->isSer==1)
            cout<<"1 ";
        else
            cout<<"2 ";
    }
    else
        cout<<"0 ";
    
    if(isOn&&isSer)
        cout<<" "<<this->connect_to;
    cout<<endl;
}
void Passenger::get_online(string time_tag,string id)
{
    this->isOn=true;
    this->isSer=1;
    this->time_tag=time_tag;
    this->connect_to=id;
}
void Passenger::get_onboard(string time_tag)
{
    this->wait=minute_gap(this->time_tag,time_tag);
    this->time_tag=time_tag;
    this->isSer=2;
}

template<typename T>
class EntityArray
{
protected:
    int capacity;
    int cnt;
    Entity** entityPtr;
public:
    EntityArray();
    // EntityArray(const EntityArray& ea);
    // operator=(const EntityArray& ea);
    ~EntityArray();
    void add(string id,bool isOn,bool isSer,int lon,int lat) throw(overflow_error);
    void print() const noexcept;
};

template <typename T>
EntityArray<T>::EntityArray()
{
    this->cnt=0;
    this->capacity=1;
    this->entityPtr=new Entity*[this->capacity];
}

template <typename T>
EntityArray<T>::~EntityArray()
{
    for(int i=0;i<this->cnt;i++)
        delete this->entityPtr[i];
    delete [] this->entityPtr;
}

template <typename T>
void EntityArray<T>::print() const noexcept
{
    for(int i=0;i<this->cnt;i++)
        this->entityPtr[i]->print();
}

template <typename T>
void EntityArray<T>::add(string id,bool isOn,bool isSer,int lon,int lat) throw(overflow_error)
{
    if(this->cnt<this->capacity)
    {
        this->entityPtr[this->cnt]=new T(id,isOn,isSer,lon,lat);
        this->cnt++;
    }
    else
        throw overflow_error("...\n");
}
int K,aR,bR,aL,bL,k,h,p,n;                                                                    //necessary parameter
int profit;
int main()
{
    string useless;
    char statement[1000]={0};
    //cin>>K>>aR>>bR>>aL>>bL>>k>>h>>p>>n;
    //cin.get();                                                                              //"\n"
    getline(cin,useless);
    while(cin.getline(statement,sizeof(statement)))
    {
        if(strlen(statement)==0)
            break;
        char *statement_time=strtok(statement," ");
        char *operation=strtok(NULL,":");
        if(!strcmp(operation,"NP"))
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *tags=p;
            //add tags
        }
        else if(!strcmp(operation,"NC"))
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *tags=p;
            //add tags
            p=strtok(NULL," ");
            string direction=p;
        }
        else if(!strcmp(operation,"OC"))
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *coordinate=p;
            p=strtok(NULL," ");
            string direction=p;
        }
        else if(!strcmp(operation,"EC"))
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            string direction=p;
        }
        else if(!strcmp(operation,"OP"))
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *coordinate=p;
            p=strtok(NULL," ");
            bool normal=false;
            if(!strcmp(p,"N"))
                normal=true;
        }
        else if(!strcmp(operation,"CP"))
        {
            char *p=strtok(NULL," ");
            string id=p;
        }
        else if(!strcmp(operation,"AD"))
        {
            char *p=strtok(NULL,"(");
            string id=p;
            p=strtok(NULL,")");
            char *coordinate=p;
            p=strtok(NULL," ");
            string direction=p;
        }
        else if(!strcmp(operation,"LC"))
        {
            char *p=strtok(NULL," ");
            string id=p;
        }
        else if(!strcmp(operation,"SC"))
        {
            char *p=strtok(NULL," ");
            string id=p;
        }
        else if(!strcmp(operation,"SP"))
        {
            char *p=strtok(NULL," ");
            string id=p;
        }
        else if(!strcmp(operation,"SR"))
        {
            cout<<profit<<endl;
        }
        else if(!strcmp(operation,"ZZ"))
        {
            
        }
    }
    return 0;
}

int minute_gap(string was,string now)
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

void to_destination(Car c,Passenger p,string time_tag,int lon,int lat,char direction)
{
    int duration=minute_gap(c.time_tag,time_tag);
    int distance=abs(lon-c.lon)+abs(lat-c.lat);
    int score=4;
    if(!c.normal)
    {
        if(distance%1)
            distance=(distance+1)/2;
        else
            distance/=2;
    }
    bool free=false;
    if(p.wait>20)
        free=true;
    if(duration>3*distance)
        free=true;
    
    if(p.wait>20)
        score-=2;
    else if(10<p.wait&&p.wait<20)
        score-=1;
    if(duration>3*distance)
        score-=2;
    else if(duration>2*distance&&duration<3*distance)
        score-=1;
    
    for(int i=0;i<c.tag_cnt;i++)
        for(int j=0;j<p.tag_cnt;j++)
            if(c.tag[i]==c.tag[j])
                score++;
    
    if(score>5)
        score=5;
    else if(score<1)
        score=1;
    
    if(c.normal)
        profit+=aR+bR*distance;
    else
        profit+=aL+bL*distance;
    
    c.total_score+=score;
    c.rate_cnt++;
}

void search_for_car(Passenger p,bool normal)
{
    
}
