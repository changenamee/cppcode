#ifndef MYSTRING_H
#define MYSTRING_H
#include<memory>
#include<algorithm>
#include<iostream>
using namespace std;
class MyString{
    friend ostream& operator<<(ostream&,const MyString&);
    friend bool operator==(const MyString&,const MyString&);
    friend bool operator!=(const MyString&,const MyString&);
public:
    MyString(){}
    MyString(const char* s){
        cout<<"构造函数"<<endl;
        auto ns = const_cast<char*>(s);
        while(*ns)  ++ns;
        auto it = copy_n_alloc(s,ns);
        elements = it.first;
        first_free = it.second;
    }
    MyString(const MyString&);
    MyString& operator=(const MyString&);
    char& operator[](const size_t&);
    const char& operator[](const size_t&) const;
    ~MyString();
    MyString(MyString&&) noexcept;
    MyString& operator=(MyString&&) noexcept;
    char* begin() const{return elements;}
    char* end() const{return first_free;}
    size_t size() {return first_free-elements;}
    void free();
    void print();
private:
    char* elements;
    char* first_free;
    allocator<char> alloc;
    pair<char*,char*> copy_n_alloc(const char* p,const char* q){
        auto data = alloc.allocate(q-p);
        return {data,uninitialized_copy(p,q,data)};
    }
    void range_ini(const char* p,const char* q){
        auto it = copy_n_alloc(p,q);
        elements = it.first;
        first_free = it.second;
    }
};
char& MyString::operator[](const size_t& i){
    return elements[i];
}
const char& MyString::operator[](const size_t& i) const {
    return elements[i];
}
bool operator==(const MyString& m1,const MyString& m2){
    auto it1 = m1.elements;
    auto it2 = m2.elements;
    while(it1!=m1.first_free && it2!=m2.first_free){
        if(*it1 != *it2)    return false;
        ++it1;
        ++it2;
    }
    if(it1!=m1.first_free||it2!=m2.first_free)    return false;
    return true;
}
bool operator!=(const MyString& m1,const MyString& m2){
    return !(m1==m2);
}
MyString::MyString(MyString&& ms) noexcept:elements(ms.elements),first_free(ms.first_free){
    cout<<"移动构造"<<endl;
    elements = first_free = nullptr;
}
MyString& MyString::operator=(MyString&& ms) noexcept{
    cout<<"移动赋值"<<endl;
    if(this!=&ms){
        free();
        elements = ms.elements;
        first_free = ms.elements;
        ms.elements = ms.first_free = nullptr;
    }
    return *this;
}
void MyString::print(){
    for(auto it=elements;it!=first_free;++it)
        cout<<*it;
    cout<<endl;
}
void MyString::free(){
    if(elements){
        for_each(elements,first_free,[this](char& c){alloc.destroy(&c);});
        alloc.deallocate(elements,first_free-elements);
    }
}
MyString::~MyString(){free();cout<<"析构函数"<<endl;}
MyString::MyString(const MyString& s){
    cout<<"拷贝构造"<<endl;
    range_ini(s.begin(),s.end());
}
MyString& MyString::operator=(const MyString& s){

    cout<<"拷贝赋值"<<endl;
    auto it = copy_n_alloc(s.elements,s.first_free);
    free();
    elements = it.first;
    first_free = it.second;
    return *this;
}

ostream& operator<<(ostream& os,const MyString& ms){
    auto it = ms.elements;
    while(it!=ms.first_free)
            os<<*it++;
    return os;
}
#endif