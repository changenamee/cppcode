#ifndef MESSAGE_H
#define MESSAGE_H

#include<string>
#include<set>

using namespace std;
class message;
class folder{
    friend class message;
public:
    void addMes(const message*);
    void rmvMes(const message*);
private:
};

class message{
    friend class folder;
public:
    explicit message(const string& s=""):contents(s){}
    message(const message&);
    message& operator=(const message&);
    message(message&&);
    message& operator=(message&&);
    ~message();
    void move_folders(message*);
    void save(folder&);
    void remove(folder&);
private:
    string contents;
    set<folder*> folders;
    void add_to_folder(const message&);
    void remove_from_folder();
};
void message::move_folders(message* m){
    folders = m->folders;   
    for(auto f:folders){
        f->rmvMes(m);
        f->addMes(this);
    }
    m->folders.clear();
}
message::message(message&& m):contents(contents){
    move_folders(&m);
}
message& message::operator=(message&& m){
    if(this!=&m){   //检测自身赋值
        remove_from_folder();
        contents = std::move(m.contents);
        move_folders(&m);
    }
    return *this;
}
void message::save(folder& f){
    folders.insert(&f);
    f.addMes(this);
}
void message::remove(folder& f){
    folders.erase(&f);
    f.rmvMes(this);

}
void message::add_to_folder(const message& m){
    for(auto it:m.folders)
        it->addMes(this);
}
message::message(const message& m):contents(m.contents),folders(m.folders){
    add_to_folder(m);
}
void message::remove_from_folder(){
    for(auto it:folders)
        it->rmvMes(this);
}
message::~message(){
    remove_from_folder();
}
message& message::operator=(const message& m){
    remove_from_folder();
    contents = m.contents;
    folders = m.folders;
    add_to_folder(m);
    return *this;
}
#endif