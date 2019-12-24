#include <iostream>
using namespace std;

class test
{
private:
    int a;
public:
    test(){a=0;};
    ~test(){};
    void inc_a(){++this->a;};
    void print(){cout<<a<<endl;};
};

class test2
{
private:
    test *t;
public:
    test2(){};
    ~test2(){};
    void inc(test &arg)
    {

        t = &arg;
        (*t).inc_a(); // или t->inc_a();
    }
};



int main(int argc, char const *argv[])
{
    test T;
    test2 T2;
    T.print();
    T.inc_a();
    T.print();
    T2.inc(T);
    T.print();
    return 0;
}
