#ifndef STRVEC_H
#define STRVEC_H
#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<initializer_list>
using namespace std;

class StrVec{
public:
    StrVec():elements(nullptr),first_free(nullptr),cap(nullptr){}
    /* StrVec(initializer_list<string> l){
        auto data = alloc_n_copy(l.begin(),l.end());
        elements = data.first;
        first_free = cap = data.second;
    } */
    StrVec(const StrVec&);
    StrVec& operator=(const StrVec&);
    ~StrVec();
    StrVec(StrVec&&) noexcept;
    StrVec& operator=(StrVec&&) noexcept;
    void resize(size_t);
    void resize(size_t,const string&);
    void reserve(size_t);
    void push_back(const string&);
    StrVec& operator=(initializer_list<string> i);
    size_t size() const {return first_free-elements;}
    size_t capacity() const {return cap-elements;}
    string* begin() const {return elements;}
    string* end() const {return first_free;}
private:
    string* elements;
    string* first_free;
    string* cap;
    static allocator<string> alloc;
    pair<string*,string*> alloc_n_copy(const string*,const string*);
    void check_n_alloc(){
        if(size()==capacity()) reallocate();
    }
    void realloc_move(size_t);
    void reallocate();
    void free();
};
StrVec& StrVec::operator=(initializer_list<string> i){
    auto data = alloc_n_copy(i.begin(),i.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}
StrVec::StrVec(StrVec&& s) noexcept:elements(s.elements),first_free(s.first_free),cap(s.cap){
    cout<<"移动拷贝"<<endl;
    s.elements = s.first_free = s.cap = nullptr;
}
StrVec& StrVec::operator=(StrVec&& s) noexcept{
    cout<<"移动赋值"<<endl;
    if(this!=&s){
        free();
        elements = s.elements;
        first_free = s.first_free;
        cap = s.cap;
        s.elements = s.first_free = s.cap = nullptr;
    }
    return *this;
}
allocator<string> StrVec::alloc;
void StrVec::push_back(const string &s){
    check_n_alloc();
    alloc.construct(first_free++,s);
}
pair<string*,string*> StrVec:: alloc_n_copy(const string* p,const string* q){
    auto data = alloc.allocate(q-p);
    return {data,uninitialized_copy(p,q,data)};
}
void StrVec::free(){
    if(elements){
        for_each(elements,first_free,[this](string &s){alloc.destroy(&s);});
        alloc.deallocate(elements,cap-elements);
    }
}
StrVec::StrVec(const StrVec& t){
    auto data = alloc_n_copy(t.begin(),t.end());
    elements = data.first;
    first_free = cap = data.second;
}
StrVec& StrVec::operator=(const StrVec& t){
    auto data = alloc_n_copy(t.begin(),t.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}
StrVec::~StrVec(){free();}
void StrVec::realloc_move(size_t n){
    auto newdata = alloc.allocate(n);

    auto dest = newdata;
    auto ele = elements;
    for(size_t i=0;i!=size();++i)
        alloc.construct(dest++,std::move(*ele++));
    free();
    elements = newdata;
    first_free = dest;
    cap = elements+n;
}
void StrVec::reallocate(){

    auto newcapacity = size()?size()*2:1;
    realloc_move(newcapacity);
}
void StrVec::resize(size_t t){
    resize(t,string());
}
void StrVec::resize(size_t t,const string& s){
    if(t>size()){
        if(t>capacity())    resize(t*2);
        for(size_t i=size();i<t;++i){
            alloc.construct(first_free++,s);
        }
    }else if(t<=size()){
        for(size_t i=0;i<t;++i)
            alloc.destroy(--first_free);
    }
}
void StrVec::reserve(size_t t){
    if(t<=capacity())   return;
    realloc_move(t);
}
#endif