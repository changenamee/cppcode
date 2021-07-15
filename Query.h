#include<map>
#include<set>
#include<vector>
#include<memory>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;

class ResultQuery;
class TextQuery{
public:
    using line_no = vector<string>::size_type;
    TextQuery(ifstream&);
    ResultQuery query(const string&) const;
private:
    shared_ptr<vector<string> > file;   //文件内容
    map<string,shared_ptr<set<line_no> > > wm;    //单词对应的行号
};
TextQuery::TextQuery(ifstream& input):file(new vector<string>){
    string s;
    while(getline(input,s)){
        istringstream is(s);
        string word;
        file->push_back(s); //将本行插入文件内容中
        int n = file->size()-1;  //行号
        while(is>>word){
            auto &lines = wm[word];
            if(!lines)
                lines.reset(new set<line_no>);
            lines->insert(n);
        }
    }
}
class ResultQuery{
    friend ostream& print(ostream&,const ResultQuery&);
public:
    using line_no = vector<string>::size_type;
    ResultQuery(const string& s,shared_ptr<vector<string> > f,shared_ptr<set<line_no> > m):
                word(s),file(f),wm(m){}
    set<line_no>::iterator begin() const {return wm->begin();}
    set<line_no>::iterator end() const {return wm->end();}

    shared_ptr<vector<string> > get_file(){return file;}
private:
    string word;    //要查找的单词
    shared_ptr<vector<string> > file;   //文件内容
    shared_ptr<set<line_no> > wm;   //出现的行号
};
ostream& print(ostream& os,const ResultQuery& q){
    cout<<q.word<<" occurs "<<q.wm->size()<<" times"<<endl;
    for(auto& num:*q.wm){
        cout<<"(line "<<num+1<<"):"<<*(q.file->begin()+num)<<endl;
    }
    return os;
}
//非引用返回函数返回一个右值，引用参数需要传入一个左值
//参数声明为const，就可以接受函数返回的右值了
ResultQuery TextQuery::query(const string& s) const{
    static shared_ptr<set<line_no> > nodata(new set<line_no>);
    auto it = wm.find(s);
    if(it==wm.end()){
        return ResultQuery(s,file,nodata);
    }else{
        return ResultQuery(s,file,it->second);
    }
}

class Query_base{
    friend class Query;
protected:
    using line_no = TextQuery::line_no;
    ~Query_base() = default;
private:
    virtual ResultQuery eval(const TextQuery&) const = 0;
    virtual string rep() const = 0;
};

class Query{
    friend Query operator&(const Query&,const Query&);
    friend Query operator|(const Query&,const Query&);
    friend Query operator~(const Query&);
public:
    Query(const string&);
    ResultQuery eval(const TextQuery& t) const{
        cout<<"Query eval()"<<endl;
        return q->eval(t);
    }
    string rep() const {
        return q->rep();
    }
private:
    Query(shared_ptr<Query_base> query):q(query){}
    shared_ptr<Query_base> q;
};
ostream& operator<<(ostream& os,const Query& query){
    os<<query.rep();
    return os;
}

class WordQuery:public Query_base{
    friend class Query;
    WordQuery(const string& s):word(s){}
    ResultQuery eval(const TextQuery& q) const {
        cout<<"WordQuery eval()"<<endl;
        return q.query(word);
    }
    string rep() const {
        return word;
    }
    string word;
};

inline
Query::Query(const string& s):q(new WordQuery(s)){}

class NotQuery:public Query_base{
    friend Query operator~(const Query&);
    NotQuery(const Query& query):q(query){}
    ResultQuery eval(const TextQuery& t) const;
    string rep() const{return "~("+q.rep()+")";}
    Query q;
};
inline
Query operator~(const Query& q){
    return shared_ptr<Query_base>(new NotQuery(q));
}

class BinaryQuery:public Query_base{
protected:
    BinaryQuery(const Query& q1,const Query& q2,const string& s):
                lhs(q1),rhs(q2),opeName(s){}
    string rep() const{
        return "("+lhs.rep()+" "+opeName+" "+rhs.rep()+")";
    }
    Query lhs,rhs;
    string opeName; //操作符名字
};

class AndQuery:public BinaryQuery{
    friend Query operator&(const Query&,const Query&);
    AndQuery(const Query& q1,const Query& q2):BinaryQuery(q1,q2,"&"){}
    ResultQuery eval(const TextQuery& t) const;
};
inline Query operator&(const Query& q1,const Query& q2){
    return shared_ptr<Query_base>(new AndQuery(q1,q2));
}

class OrQuery:public BinaryQuery{
    friend Query operator|(const Query&,const Query&);
    OrQuery(const Query& p,const Query& q):BinaryQuery(p,q,"|"){}
    ResultQuery eval(const TextQuery&) const;
};
inline Query operator|(const Query& p,const Query& q){
    return shared_ptr<Query_base>(new OrQuery(p,q));
}

ResultQuery OrQuery::eval(const TextQuery& t) const{
    cout<<"OrQuery eval()"<<endl;
    auto left = lhs.eval(t),right = rhs.eval(t);
    auto line_set = make_shared<set<line_no> >(left.begin(),left.end());
    line_set->insert(right.begin(),right.end());
    return ResultQuery(rep(),left.get_file(),line_set);
}

ResultQuery AndQuery::eval(const TextQuery& t) const{
    cout<<"AndQuery eval()"<<endl;
    auto left = lhs.eval(t),right = rhs.eval(t);
    auto line_set = make_shared<set<line_no> >();
    set_intersection(left.begin(),left.end(),right.begin(),right.end(),inserter(*line_set,line_set->begin()));
    return ResultQuery(rep(),left.get_file(),line_set);
}
ResultQuery NotQuery::eval(const TextQuery& t) const{
    cout<<"NotQuery eval()"<<endl;
    auto res = q.eval(t);
    auto line_set = make_shared<set<line_no> >();
    auto beg = res.begin(),en = res.end();
    size_t sz = res.get_file()->size();
    for(size_t i=0;i!=sz;++i){
        if(beg==en || *beg!=i){
            line_set->insert(i);
        }else if(beg!=en) ++beg;
    }
    return ResultQuery(rep(),res.get_file(),line_set);
}
