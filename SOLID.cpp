/* 31 Jan 2022
   SOLID Design Principles
    The Single Responsibility Principle
    The Open-Closed Principle
    The Liskov Substitution Principle
    The Interface Segregation Principle
    The Dependency Inversion Principle

    http://www.vishalchovatiya.com/start-here/
    http://www.vishalchovatiya.com/open-closed-principle-in-cpp-solid-as-a-rock/

*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/*Single responsibility principle
        Benefits:
        Expressiveness, Maintainability, Reusability
        Less buggy and easier to maintain
*/
namespace SRP
{    
    // Violating SRP
    class Journal
    {
        string m_title;
        vector<string> m_entries;
    public:
        void add_entries(const string &entry) {
            m_entries.emplace_back(entry);
        }
        vector<string> get_entries() const {return m_entries;}
        void save(const string &filename)
        {
            fstream fs(filename, ios::out);
            for(auto &s: m_entries) fs << s << endl;
        }
    };
    /* 
    Above C++ example seems fine as long as you have a single domain object i.e. Journal. 
    but this is not usually the case in a real-world application.
    As we start adding domain objects like Book, File, etc. you have to implement save method 
    for everyone separately which is not the actual problem.
    The real problem arises when you have to change or maintain save functionality. 
    For instance, some other day you will no longer save data on files & adopted database. 
    In this case, you have to go through every domain object implementation & need to change 
    code all over which is not good.
    Here, we have violated the Single Responsibility Principle by providing 
    Journal class two reason to change i.e.
        Things related to Journal
        Saving the Journal
    Moreover, code will also become repetitive, bloated & hard to maintain.
    */
   // Solution - Separation of Concerns
    class SRPJournal
    {
        string m_title;
        vector<string> m_entries;
    public:
        void add_entries(const string &entry) {
            m_entries.emplace_back(entry);
        }
        vector<string> get_entries() const {return m_entries;}
    };

    class SavingManager {
    public:
        static void save(const SRPJournal &j, const string &filename)
        {
            fstream fs(filename, ios::out);
            for(auto &s : j.get_entries())
                fs << s << endl;
        }
    };

}

/*  Open-Closed Principle
    classes should be open for extension, closed for modification
    This literally means you should be able to extend a classes behaviour, without modifying it. 
    This might seems weird to you & may raise a question 
    that how can you change the behaviour of a class without modifying it?
    But there are many answers to this in object-oriented design like 
    dynamic polymorphism, static polymorphism, C++ template, etc.
*/
namespace OCP
{
    // Violating the Open Closed Principle
    enum class COLOR { RED, GREEN, BLUE, YELLOW };
    enum class SIZE { SMALL, MEDIUM, LARGE };
    struct Product {
        string  m_name;
        COLOR   m_color;
        SIZE    m_size;
        //float   m_weight;
    };

    using Items = vector<Product*>;
    #define ALL(C)  begin(C), end(C)
    struct ProductFilter {
        static Items by_color(Items items, const COLOR e_color) {
            Items result;
            for (auto &i : items)
                if (i->m_color == e_color)
                    result.push_back(i);
            return result;
        }
        static Items by_size(Items items, const SIZE e_size) {
            Items result;
            for (auto &i : items)
                if (i->m_size == e_size)
                    result.push_back(i);
            return result;
        }
        static Items by_size_and_color(Items items, const SIZE e_size, const COLOR e_color) {
            Items result;
            for (auto &i : items)
                if (i->m_size == e_size && i->m_color == e_color)
                    result.push_back(i);
            return result;
        }
    };

    /*
        Now what if we add 1 more atrribute i.e weight
        Then we have add functions for filtering by weight, by weight and name,
        by weight and size , by weight and color , by weight , size and color.
        Now if we add 1 more then again the same ....
        So this is not a scalable option
    */

   /* Solution 
   Achieving OCP via Interface design or abstraction level
   Here is the scalable solution
   */

    template <typename T>
    struct Specification {
       virtual ~Specification() = default;
       virtual bool is_satisfied(T *item) const  = 0;
    };

    struct ColorSpecification : Specification<Product> {
       COLOR e_color;
       ColorSpecification(COLOR e_color) : e_color(e_color){}
       bool is_satisfied(Product *item) const {return item->m_color == e_color;}
    };

    struct SizeSpecification : Specification<Product> {
        SIZE e_size;
        SizeSpecification(SIZE e_size) : e_size(e_size) {}
        bool is_satisfied(Product *item) const { return item->m_size == e_size; }
    };

    // struct WeightSpecification : Specification<Product> {
    //     int e_weight;
    //     WeightSpecification(int e_weight) : e_weight(e_weight) {}
    //     bool is_satisfied(Product *item) const { return item->m_weight == e_weight; }
    // };

    template <typename T>
    struct Filter {
        virtual vector<T *> filter(vector<T *> items, const Specification<T> &spec) = 0;    
    };

    // for combined filter
    template <typename T>
    struct AndSpecification : Specification<T> {
        const Specification<T> &first;
        const Specification<T> &second;
        AndSpecification(const Specification<T> &first, const Specification<T> &second)
        : first(first), second(second) {}
        
        bool is_satisfied(T *item) const 
        { 
            return first.is_satisfied(item) && second.is_satisfied(item); 
        }
    };

    template <typename T>
    AndSpecification<T> operator&&(const Specification<T> &first, const Specification<T> &second) 
    {
        return {first, second};
    }

    struct BetterFilter : Filter<Product> {
        vector<Product *> filter(vector<Product *> items, const Specification<Product> &spec) 
        {
            vector<Product *> result;
            for (auto &p : items)
            {
                if (spec.is_satisfied(p))
                    result.push_back(p);
            }
            return result;
        }
    };

}

int main()
{
    // SRP
    {
        SRP::SRPJournal journal;
        journal.add_entries("Dear XYZ");
        journal.add_entries("I ate a bug");
        journal.add_entries("I cried today");
        //journal.save("diary.txt");
        SRP::SavingManager::save(journal, "diary.txt"); 
        // if any changes required related to saving then we only 
        // need to modify SavingManager
    }

    // OCP
    {
        using namespace OCP;  // now no need to do OCP::Product
        const Items all{
            new Product{"Apple",    COLOR::GREEN,  SIZE::SMALL},
            new Product{"Tree",     COLOR::GREEN,  SIZE::LARGE},
            new Product{"House",    COLOR::BLUE,   SIZE::LARGE},
            new Product{"Banana",   COLOR::YELLOW, SIZE::SMALL},
            new Product{"Mountain", COLOR::GREEN,  SIZE::LARGE},
        };
        // for (auto &p : ProductFilter::by_color(all, COLOR::GREEN))
        //     cout << p->m_name << " is green\n";
        // for (auto &p : ProductFilter::by_size_and_color(all, SIZE::LARGE, COLOR::GREEN))
        //     cout << p->m_name << " is green & large\n";

        BetterFilter bf;
        for(auto &x: bf.filter(all, ColorSpecification(COLOR::GREEN)))
            cout << x->m_name << " is green\n"; 

        for(auto &x: bf.filter(all, SizeSpecification(SIZE::LARGE)))
            cout << x->m_name << " is large\n";    
    
        auto green_things = ColorSpecification{COLOR::GREEN};
        auto large_things = SizeSpecification{SIZE::LARGE};

        // use variadic templates to filter by more than 2 combinations
        for (auto &x : bf.filter(all, green_things && large_things))
            cout << x->m_name << " is green and large\n";
    }

    return EXIT_SUCCESS;
}