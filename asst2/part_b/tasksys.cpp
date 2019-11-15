#include<thread>
#include <vector>
#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include <cassert>
#include "tasksys.h"


IRunnable::~IRunnable() {}

ITaskSystem::ITaskSystem(int num_threads) {}
ITaskSystem::~ITaskSystem() {}

const char* TaskSystemSerial::name() {
    return "Serial";
}

TaskSystemSerial::TaskSystemSerial(int num_threads): TaskSystemParallelThreadPoolSpinning(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
}

TaskSystemSerial::~TaskSystemSerial() {}

void TaskSystemSerial::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    // std::cout << "run serial\n";
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}



const char* TaskSystemParallelSpawn::name() {
    return "Parallel + Always Spawn";
}

TaskSystemParallelSpawn::TaskSystemParallelSpawn(int num_threads): TaskSystemParallelThreadPoolSpinning(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    this->num_threads = num_threads;
    this->ths = new std::thread[num_threads];
}

TaskSystemParallelSpawn::~TaskSystemParallelSpawn() {delete []ths;}


void TaskSystemParallelSpawn::staticAssign(IRunnable * runnable,int num_total_tasks,int taks_start,int num_tasks){
    
    int task_end = std::min(num_tasks + taks_start,num_total_tasks);
    // std::cout << "task_start: "<<taks_start << " "<<"task_end "<<task_end << "\n";
    for(int i=taks_start ; i<task_end ; ++i)
        runnable->runTask(i,num_total_tasks);
}



void TaskSystemParallelSpawn::run(IRunnable* runnable, int num_total_tasks) {

    int num_work_thread = this->num_threads -1;
    int per_thread = (num_total_tasks + num_work_thread)/ (num_work_thread +1);
    for(int i=0 ; i<num_work_thread; ++i)
        this->ths[i] = std::thread(TaskSystemParallelSpawn::staticAssign,runnable,num_total_tasks,i * per_thread,per_thread);
    // main thread also work
    TaskSystemParallelSpawn::staticAssign(runnable,num_total_tasks,num_work_thread * per_thread,per_thread);
    for(int i=0 ; i<num_work_thread ; ++i)
        this->ths[i].join();
    
}


const char* TaskSystemParallelThreadPoolSpinning::name() {
    return "Parallel + Thread Pool + Spin";
}

TaskSystemParallelThreadPoolSpinning::TaskSystemParallelThreadPoolSpinning(int num_threads): ITaskSystem(num_threads),close(false) {
    
    this->NUM_THREADS = num_threads;
}

void TaskSystemParallelThreadPoolSpinning::init(){
    if(!workers.empty())
        return;
    done =0;
    std::srand((unsigned)(std::time(NULL) + rand()));
    auto work_fun = [this](int thread_idx){
        for(;;){
            int no_tasks = 0;

            while (!queues[thread_idx].empty())
            {
                auto task = queues[thread_idx].front();
                queues[thread_idx].pop();
                task();
                no_tasks ++;
            }
            if(no_tasks>0)
            {
                // std::cout << "thread "<<thread_idx << "finish "<< no_tasks << "tasks\n";
                done-= no_tasks;
                if(done==0){
                    std::lock_guard<std::mutex> lk(lk_done);
                    cv_main.notify_one();
                }
            }
            if(close)
                return;
            if(queues[thread_idx].empty())
            {
                
                std::unique_lock<std::mutex> lk(finish);
                // cv_finish.notify_all();
                // std::cout << "i'sleep "<< thread_idx << "left tasks " << queues[thread_idx].size() << "\n";
                cv_finish.wait(lk,[this,thread_idx]{return !queues[thread_idx].empty() || close;});
            }
        }
    };
    for(int i=0 ; i<this->NUM_THREADS ; ++i){
        queues.emplace_back(std::queue<std::function<void()>>());
        // std::mutex m;
        lks.emplace_back(new std::mutex);
        workers.emplace_back(
            std::thread(work_fun,i)
        );
    }
    queues.emplace_back(std::queue<std::function<void()>>());
}


TaskSystemParallelThreadPoolSpinning::~TaskSystemParallelThreadPoolSpinning() {
    if(!workers.empty()){// lazy init bug
        close = true;// change statu,then notifyall,
        cv_finish.notify_all();
        cv_main.notify_all();
        for(int i=0 ; i<NUM_THREADS ; ++i)
            if(workers[i].joinable()){
                // std::cout << "i'm join id:" << i << "\n";
                workers[i].join();
            }
    }
}

void TaskSystemParallelThreadPoolSpinning::enqueue(int taskId,const std::function<void()>& task){
    // twice of choice
    // assume NUM_THREADS is power of 2
    int q1 = std::rand() &(NUM_THREADS -1);
    int q2 = std::rand() &(NUM_THREADS -1);
    if(queues[q1].size() > queues[q2].size())
        std::swap(q1,q2);
    queues[q1].push(task);
}

void TaskSystemParallelThreadPoolSpinning::run(IRunnable* runnable, int num_total_tasks) {

    init();
    
    done = num_total_tasks;
    for(int i=0; i< num_total_tasks ; ++i){
        auto task = [runnable,i,num_total_tasks]{runnable->runTask(i,num_total_tasks);};
        enqueue(i,task);
    }
    
    {
        std::lock_guard<std::mutex> lk(finish);
        cv_finish.notify_all(); 
    }
    
    {
        std::unique_lock<std::mutex> lk(lk_done);
        cv_main.wait(lk,[this]{return done <=0 || close;});
    }

}

TaskID TaskSystemParallelThreadPoolSpinning::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                              const std::vector<TaskID>& deps) {
    
    deg[task_id] = deps.size();
    task_groups[task_id] = std::make_pair(runnable,num_total_tasks);
    for(const auto &e : deps){
        if(! dependency_graph.count(e))
            dependency_graph[e] = new std::vector<int>();
        dependency_graph[e]->push_back(task_id);
    }

    return task_id++;
}

void TaskSystemParallelThreadPoolSpinning::sync() {

    init();
    for(const auto & e : deg)
        if(e.second ==0){
            running.push(e.first);
            const auto & task = task_groups[e.first];
            
            done += task.second;
            for(int i=0 ; i< task.second ; ++i)
            {
                enqueue(i,[task,i]{task.first->runTask(i,task.second);});
            }
        }
    cv_finish.notify_all();
    while(!running.empty()){
        while(done !=0){
            
            {
                cv_finish.notify_all();
                std::unique_lock<std::mutex> lk(lk_done);
                cv_main.wait(lk,[this]{return close || done==0;});
            }
        }
        int sz =running.size();
        while( sz !=0){
            auto end = running.front();
            running.pop();
            sz --;
            if(dependency_graph.count(end)){
                for(const auto &e : *dependency_graph[end]){
                    if(-- deg[e] == 0){
                        running.push(e);
                        const auto & task = task_groups[e];
                        
                        done += task.second;
                        for(int i=0 ; i<task.second ; ++i)
                            enqueue(i,[task,i]{task.first->runTask(i,task.second);});
                        
                    }
                }
            }
        }
    }
    dependency_graph.clear();
    deg.clear();
    task_groups.clear();
    task_id =0;
    return;
}

const char* TaskSystemParallelThreadPoolSleeping::name() {
    return "Parallel + Thread Pool + Sleep";
}

TaskSystemParallelThreadPoolSleeping::TaskSystemParallelThreadPoolSleeping(int num_threads): TaskSystemParallelThreadPoolSpinning(num_threads) {
    
}

TaskSystemParallelThreadPoolSleeping::~TaskSystemParallelThreadPoolSleeping() {
    
}

