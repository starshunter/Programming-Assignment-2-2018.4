#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
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
    vector<T*>entities;
public:
    EntityArray();
    // EntityArray(const EntityArray& ea);
    // operator=(const EntityArray& ea);
    ~EntityArray();
    void add(string id,bool isOn,bool isSer,double lon,double lat) throw(overflow_error);
    void print() noexcept;
};

template <typename T>
EntityArray<T>::EntityArray()
{
    entities.clear();
}

template <typename T>
EntityArray<T>::~EntityArray()
{
    entities.clear();
}

template <typename T>
void EntityArray<T>::print() noexcept
{
    for(int i=0;i<entities.size();i++)
        this->entities[i]->print();
}

template <typename T>
void EntityArray<T>::add(string id,bool isOn,bool isSer,double lon,double lat) throw(overflow_error)
{
    T *p=new T(id,isOn,isSer,lon,lat);
    this->entities.push_back(p);
}

int main()
{
    EntityArray<Car> ca;
    ca.add("5HE-313",true,true,0,0);
    ca.add("LPA-039",true,false,1,1);
    ca.print();
    
    EntityArray<Passenger> pa;
    pa.add("B90705023",true,true,0,0);
    pa.add("R94725008",true,false,1,1);
    pa.print();
    
    return 0;
}
