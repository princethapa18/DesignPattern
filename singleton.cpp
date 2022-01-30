/* 29 Mar 2019 Singleton Pattern

   What is the singleton pattern?
   When can the singleton pattern be used, and when should it be avoided?
   When u need only 1 object of some type in the entire program.


   What is lazy initialization, and what problems does it solve?
   In Singleton we are doing lazy initialization. We are initializing when it is called by function Instance().

   How can we make singleton initialization thread safe?
   Either use Meyers' Singleton or do double checked locking when instantiating the singleton object

   What is the deletion order problem, and what are the solutions?

   one instance
   Ex: message logger

   thread safety issue  [double checked locking or Meyers' Singleton]
   destroying singleton issue
   dead reference problem
*/

#include <iostream>
using namespace std;

class Singleton
{
public:
    /* Meyers' Singleton pattern exploits 3 important properties:
       1) Static function objects are initialized when control flow hits the function
          for the first time.
       2) The life time of function static variable begins the first time the program flow
          encounters the declaration & ends at program termination.
       3) If control enters the declaration concurrently while the variable is being
          initialized , the concurrent execution shall wait for completion of the initialization.
    */
    static Singleton& Instance()
    {
        static Singleton obj; //thread safe
        return obj;
    }
    /* this is not thread safe
       for making it thread safe double lock checking should be done
    static Singleton* Instance()
    {
        if(instance == NULL)
            instance = new Singleton();
        return instance;
    }

    Do not delete singleton obj via destructor as it will lead to infinite.
    You should know when the singleton object is not used anymore and then only delete by using this function.
    static void deleteInstance()
    {
        if(instance != NULL)
            delete instance;
    }
    */
    void setData(int x);
    int  getData();
private:
    Singleton() { data = 0; };
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);
    ~Singleton() { };
    int data;
    //static Singleton *instance;
};

void Singleton::setData(int x){
    data = x;
}

int Singleton::getData(){
    return data;
}

//Singleton* Singleton::instance = NULL;

int main()
{
    /*
        Singleton *test = Singleton::Instance();
        test->setData(5);
        cout << "data: " << test->getData() << endl;
        Singleton *sec = Singleton::Instance();
        cout << "data: " << sec->getData() << endl;
    */
        Singleton &test = Singleton::Instance();
        test.setData(5);
        cout << "data: " << test.getData() << endl;
        Singleton &sec = Singleton::Instance();
        cout << "data: " << sec.getData() << endl;

        return 0;
}
