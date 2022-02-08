/* Week 06   08 Feb 2022
   Factory Method or Object Factory
   * Define an interface for creating an object , but let subclasses decide which class to instantiate
   * Defining a virtual constructor 
*/

#include <fstream>
#include <iostream>
#include <map>
using namespace std;

enum class VehicleType
{
    twoWheeler = 2,
    threeWheeler = 3,
    fourWheeler = 4
};

class Vehicle
{
public:
    virtual void printVehicle() = 0;
};

namespace Before
{
    class TwoWheeler : public Vehicle
    {
    public:
        TwoWheeler() { cout << "Two wheeler ctor\n";}
        void printVehicle() { cout << "Iam a two wheeler\n";}
    };
    class ThreeWheeler : public Vehicle
    {
    public:
        ThreeWheeler() { cout << "Three wheeler ctor\n";}
        void printVehicle() { cout << "Iam a three wheeler\n";}
    };

    class FourWheeler : public Vehicle
    {
    public:
        FourWheeler() { cout << "Four wheeler ctor\n";}
        void printVehicle() { cout << "Iam a four wheeler\n";}
    };

    class Client
    {
    public:
        Client(VehicleType type);
        ~Client();
        Vehicle* getVehicle() const { return m_vehicle;}
    private:
        Vehicle* m_vehicle{nullptr};
    };

    Client::Client(VehicleType type)
    {
        switch(type)
        {
            case VehicleType::twoWheeler:
                m_vehicle = new TwoWheeler;
                break;
            case VehicleType::threeWheeler:
                m_vehicle = new ThreeWheeler;
                break;
            case VehicleType::fourWheeler:
                m_vehicle = new FourWheeler;
                break;
            default:
                cerr << "Unknown vehicle type\n";
        }
        // the switch case will increase if we add one more type 
    }

    Client::~Client()
    {
        delete m_vehicle;
    }
}

// singleton VehicleFactory
class VehicleFactory
{
public:
    typedef Vehicle* (*CreateVehicleCallBack)();
    Vehicle* CreateVehicle(VehicleType type);
    void RegisterVehicle(VehicleType type, CreateVehicleCallBack createFn);
    void UnregisterVehicle(VehicleType type);
    static VehicleFactory* instance() { 
        if(!_instance)
            _instance = new VehicleFactory;
        return _instance;
    }
private:
    VehicleFactory() {}; 
    VehicleFactory(const VehicleFactory&);
    static VehicleFactory *_instance; // one and only instance
    std::map<VehicleType, CreateVehicleCallBack> _vehicleCallBackMap;
};

VehicleFactory* VehicleFactory::_instance = nullptr;

Vehicle* VehicleFactory::CreateVehicle(VehicleType type)
{
    auto it = _vehicleCallBackMap.find(type);
    if( it == _vehicleCallBackMap.end())
    {
        std::cerr << "Unknown vechicle type\n";
        return nullptr;
    }
    return it->second();
}

void VehicleFactory::RegisterVehicle(VehicleType type, CreateVehicleCallBack createFn)
{
    auto it = _vehicleCallBackMap.find(type);
    if( it == _vehicleCallBackMap.end())
    {
        _vehicleCallBackMap.insert(std::make_pair(type, createFn));
    }
}

void VehicleFactory::UnregisterVehicle(VehicleType type)
{
    _vehicleCallBackMap.erase(type);
}

class TwoWheeler : public Vehicle
{ 
public:
    TwoWheeler() { cout << "Two wheeler ctor\n";}
    static Vehicle* create() {return new TwoWheeler;}
    void printVehicle() { cout << "Iam a two wheeler\n";}  
};

class ThreeWheeler : public Vehicle
{
public:
    ThreeWheeler() { cout << "Three wheeler ctor\n";}
    static Vehicle* create() {return new ThreeWheeler;}
    void printVehicle() { cout << "Iam a three wheeler\n";}
};

class FourWheeler : public Vehicle
{
public:
    FourWheeler() { cout << "Four wheeler ctor\n";}
    static Vehicle* create() {return new FourWheeler;}
    void printVehicle() { cout << "Iam a four wheeler\n";}
};

class Client
{
public:
    Client(VehicleType type);
    ~Client();
    Vehicle* getVehicle() const { return m_vehicle;}
private:
    Vehicle* m_vehicle{nullptr};
};

Client::Client(VehicleType type)
{
    m_vehicle = VehicleFactory::instance()->CreateVehicle(type);
}

Client::~Client()
{
    delete m_vehicle;
}

int main()
{
    VehicleFactory::instance()->RegisterVehicle(VehicleType::twoWheeler, TwoWheeler::create);
    VehicleFactory::instance()->RegisterVehicle(VehicleType::threeWheeler, ThreeWheeler::create);
    VehicleFactory::instance()->RegisterVehicle(VehicleType::fourWheeler, FourWheeler::create);

    Client *cl = new Client(VehicleType::twoWheeler);
    Vehicle *vl = cl->getVehicle();
    if(vl)
        vl->printVehicle();

    Client *cl2 = new Client(VehicleType::threeWheeler);
    Vehicle *vl2 = cl2->getVehicle();
    if(vl2)
        vl2->printVehicle();    
    return 0; 
}

