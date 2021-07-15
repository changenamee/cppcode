#ifndef TESTMYLIB_THREADPOOL_H
#define TESTMYLIB_THREADPOOL_H
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <assert.h>
#include <memory>
class ThreadPool{
public:
    ThreadPool(int threadNum=4):pool_(std::make_shared<Pool>()){
        assert(threadNum>0);
        for(size_t i=0;i<threadNum;++i){
            std::thread([pool = pool_]{
                        std::unique_lock<std::mutex> locker(pool->mtx);
                        if(pool->tasks.empty()){
                            pool->cond.wait(locker);    //任务队列中没有任务，等待加入任务
                        }else if(pool->close)   break;  //线程结束
                        else{   //执行任务
                            auto task = pool->tasks.front();
                            pool->tasks.pop();
                            locker.unlock();
                            task();
                            locker.lock();
                        }
                    }).detach();
        }
    }

    ThreadPool() = default;

    ~ThreadPool(){
        if(static_cast<bool>(pool_)){   //有任务未结束
            {
                std::unique_lock<std::mutex> locker(pool_->mtx);
                pool_->close = true;
            }
            pool_->cond.notify_all();
        }
    }

    template<class T>
    void AddTask(T&& task){ //添加任务
        {
            std::unique_lock<std::mutex> locker(pool_->mtx);
            pool_->tasks.emplace(std::forward<T>(task));
        }
        pool_->cond.notify_one();
    }
private:
    struct Pool{
        std::mutex mtx;
        std::condition_variable cond;
        bool close;
        std::queue<std::function<void()>>   tasks;  //任务列表
    };
    std::shared_ptr<Pool> pool_;
};
#endif //TESTMYLIB_THREADPOOL_H
