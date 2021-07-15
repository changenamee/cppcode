#include<iostream>
#include<memory>
using namespace std;

template<class T>
class SmartPointer{
private:
    T* _ptr;
    size_t* count;  //引用计数
public:
    SmartPointer(T* ptr = nullptr):_ptr(ptr){
        cout<<"SmartPointer Constructr"<<endl;
        if(ptr){
            count = new size_t(1);
        }else{
            count = new size_t(0);
        }
    }

    SmartPointer(const SmartPointer& sp){
        cout<<"SmartPointer copy Constructr"<<endl;
        if(this != &sp){
            this->_ptr = sp._ptr;
            this->count = sp.count;
            (*this->count)++;
        }
    }

    SmartPointer& operator=(const SmartPointer& sp){
        cout<<"SmartPointer operator= Constructr"<<endl;
        if(this->_ptr==sp._ptr)   return *this;
        if(this->_ptr){
            (*this->count)--;
            if(*this->count==0){
                delete this->_ptr;
                delete this->count;
            }
        }

        this->_ptr = sp._ptr;
        this->count = sp.count;
        (*this->count)++;
        return *this;
    }

    T& operator*(){
        assert(this->_ptr!=nullptr);
        return *(this->_ptr);
    }
    T* operator ->(){
        assert(this->_ptr!=nullptr);
        return this->_ptr;
    }
    ~SmartPointer(){
        (*this->count)--;
        if(*this->count==0){
            delete _ptr;
            delete count;
        }
    }
    size_t use_count(){
        return *this->count;
    }
};

int main(void){
    SmartPointer<int> sp(new int(10));
    SmartPointer<int> sp1(sp);
    cout<<"sp count:"<<sp.use_count()<<endl;
    cout<<"sp1 count:"<<sp1.use_count()<<endl;
    SmartPointer<int> sp2(new int(20));
    sp1 = sp2;
    cout<<"sp count:"<<sp.use_count()<<endl;
    cout<<"sp2 count:"<<sp2.use_count()<<endl;

}