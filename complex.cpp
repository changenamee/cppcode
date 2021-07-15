#include"complex.h"
#include<iostream>
using namespace std;

ostream&
operator <<(ostream&os,const Complex& c){
    return os<<"("<<real(c)<<","<<imag(c)<<")";
    //os<<"("<<c.real()<<","<<c.imagin()<<")";
}
int main(void){
    Complex c1(2,1);
    Complex c2(3,4);
    cout<<c1<<endl;
    cout<<c1+c2<<endl;
    cout<<9+c1<<endl;
    cout<<c2+3<<endl;
    c1 += c2;
    cout<<c1<<endl;
}
