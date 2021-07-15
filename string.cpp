#include "string.h"
#include<iostream>
using namespace std;

int main(void){
    String s1("hello");
    String s2("world");

    cout<<s1<<endl;
    s1 = s2;
    cout<<s1<<endl;
    String s3(s2);
    cout<<s3<<endl;

   
    return 0;
    
}