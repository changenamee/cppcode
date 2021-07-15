#ifndef __STRING__
#define __STRING__

#include<string>
#include<iostream>
using namespace std;
class String{
    public:
        String(const char* str=0);
        String(const String&);
        String& operator=(const String&);
        ~String();
        //没有改变东西，函数声明为const
        char* get_data() const {return data;}
    private:
        char *data;
};

inline 
String::String(const String& str){
    //new-->heap，生命到整个程序结束；local variable-->stack，只在函数内有效；
    //new要记得delete，否则内存泄漏
    data = new char[strlen(str.data)+1];
    strcpy(data,str.data);
}

inline
String::String(const char* str){
    if(str){
        data = new char[strlen(str)+1];
        strcpy(data,str);
    }else{
        data = new char[1];
        *data = '\0';
    }
}

inline
String& String::operator=(const String& str){
    if(&str == this){
        return *this;
    }
    //拷贝赋值，1.释放原来的内存空间；2.分配新空间；3.完成赋值操作
    delete[] data;
    data = new char[strlen(str.data)+1];
    strcpy(data,str.data);
    return *this;
}
inline
String::~String(){
    //arr new --> arr delete 否则会造成内存泄漏（内存还在，指针没有了）
    delete[] data;  
}
inline
ostream& operator<<(ostream& os,const String& str){
    os<<str.get_data();
    return os;
}
#endif