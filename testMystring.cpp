#include "MyString.h"
#include<iostream>

int main(void){
    MyString s("abc");
    s.print();
    MyString s1(s);
    s1.print();
    MyString s2;
    s2=s1;
    s2.print();
}