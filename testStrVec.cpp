#include "StrVec.h"
#include "MyString.h"
#include<iostream>

MyString test(){
    MyString s("123");
    return s;
}
int main(void){
    MyString ns("abc");
    MyString s("ac");
    cout<<"s[1]:"<<s[1]<<endl;
    const MyString cs = s;
    s[1] = 'd';
    //cs[1] = 'd';
    cout<<s<<endl;
    return 0;
}
