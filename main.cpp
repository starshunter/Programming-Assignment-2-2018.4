#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Entity
{
protected:
    string id;
    bool isOn;
    bool isSer;
    double lon;
    double lat;
public:
    Entity(string id,bool isOn,bool isSer,double lon,double lat):id(id), isOn(isOn), isSer(isSer), lon(lon), lat(lat){}
    virtual void print() = 0;
};

class Car:public Entity
{
private:
public:
    Car(string id,bool isOn,bool isSer,double lon,double lat):Entity(id, isOn, isSer, lon, lat){}
    void print();
};

void Car::print()
{
    cout<<this->id<<": ";
    if(this->isOn==true)
    {
        if(this->isSer==true)
            cout<<"in-service, (";
        else
            cout<<"empty, (";
        cout<<this->lon<<","<<this->lat<<")"<<endl;
    }
    else
        cout<<"offline"<<endl;
}

class Passenger : public Entity
{
private:
public:
    Passenger(string id,bool isOn,bool isSer,double lon,double lat):Entity(id, isOn, isSer, lon, lat){}
    void print();
};

void Passenger::print()
{
    cout << this->id << ": ";
    if(this->isOn == true)
    {
        if(this->isSer == true)
            cout << "in-service, (";
        else
            cout << "waiting, (";
        cout << this->lon << "," << this->lat << ")";
    }
    else
        cout << "offline";
    cout << endl;
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
    void add(string id,bool isOn,bool isSer,double lon,double lat) throw(overflow_error);
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
void EntityArray<T>::add(string id,bool isOn,bool isSer,double lon,double lat) throw(overflow_error)
{
    if(this->cnt<this->capacity)
    {
        this->entityPtr[this->cnt]=new T(id,isOn,isSer,lon,lat);
        this->cnt++;
    }
    else
        throw overflow_error("...\n");
}

int main()
{
    EntityArray<Car> ca;
    try
    {
        ca.add("5HE-313",true,true,0,0);
    }
    catch(overflow_error e)
    {
        cout<<e.what();
    }
    try
    {
        ca.add("LPA-039",true,false,1,1);
    }
    catch(overflow_error e)
    {
        cout<<e.what();
    }
    ca.print();
    
    EntityArray<Passenger> pa;
    try
    {
        pa.add("B90705023",true,true,0,0);
    }
    catch(overflow_error e)
    {
        cout<<e.what();
    }
    try
    {
        pa.add("R94725008",true,false,1,1);
    }
    catch(overflow_error e)
    {
        cout<<e.what();
    }
    
    pa.print();
    
    return 0;
}
