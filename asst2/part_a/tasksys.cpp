#include<thread>
#include <vector>
#include <iostream>
#include <queue>
#include <functional>
#include <mutex>
#include "tasksys.h"


IRunnable::~IRunnable() {}

ITaskSystem::ITaskSystem(int num_threads) {}
ITaskSystem::~ITaskSystem() {}

const char* TaskSystemSerial::name() {
    return "Serial";
}

TaskSystemSerial::TaskSystemSerial(int num_threads): ITaskSystem(num_threads) {
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

    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}

TaskID TaskSystemSerial::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                          const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //
    
    return 0;
}

void TaskSystemSerial::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //
    
    return;
}

const char* TaskSystemParallelSpawn::name() {
    return "Parallel + Always Spawn";
}

TaskSystemParallelSpawn::TaskSystemParallelSpawn(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    this->num_threads = num_threads;
    this->ths = new std::thread[num_threads];
}

TaskSystemParallelSpawn::~TaskSystemParallelSpawn() {delete []this->ths;}


void TaskSystemParallelSpawn::staticAssign(IRunnable * runnable,int num_total_tasks,int taks_start,int num_tasks){
    
    int task_end = std::min(num_tasks + taks_start,num_total_tasks);
    // std::cout << "task_start: "<<taks_start << " "<<"task_end "<<task_end << "\n";
    for(int i=taks_start ; i<task_end ; ++i)
        runnable->runTask(i,num_total_tasks);
}



void TaskSystemParallelSpawn::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    // std::cout << "total_tasks : " << num_total_tasks << " "<<"num_threads : "<<this->num_threads << "\n";
    int num_work_thread = this->num_threads -1;
    int per_thread = (num_total_tasks + num_work_thread)/ (num_work_thread +1);
    for(int i=0 ; i<num_work_thread; ++i)
        this->ths[i] = std::thread(TaskSystemParallelSpawn::staticAssign,runnable,num_total_tasks,i * per_thread,per_thread);
    // main thread also work
    TaskSystemParallelSpawn::staticAssign(runnable,num_total_tasks,num_work_thread * per_thread,per_thread);
    for(int i=0 ; i<num_work_thread ; ++i)
        this->ths[i].join();
    
}


TaskID TaskSystemParallelSpawn::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                 const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //

    return 0;
}

void TaskSystemParallelSpawn::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //

    return;
}

const char* TaskSystemParallelThreadPoolSpinning::name() {
    return "Parallel + Thread Pool + Spin";
}

TaskSystemParallelThreadPoolSpinning::TaskSystemParallelThreadPoolSpinning(int num_threads): ITaskSystem(num_threads),close(false) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    this->NUM_THREADS = num_threads;
}

void TaskSystemParallelThreadPoolSpinning::init(){
    if(!workers.empty())
        return;
    
    auto work_fun = [this](int thread_idx){
        for(;;){
            int no_tasks = 0;

            while (!queues[thread_idx].empty())
            {
                auto task = std::move(queues[thread_idx].front());
                queues[thread_idx].pop();
                task();
                no_tasks ++;
            }
            if(no_tasks>0)
            {
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
    close = true;// change statu,then notifyall,
    cv_finish.notify_all();
    cv_main.notify_all();
    for(int i=0 ; i<NUM_THREADS ; ++i)
        if(workers[i].joinable()){
            // std::cout << "i'm join id:" << i << "\n";
            workers[i].join();
        }
}

void TaskSystemParallelThreadPoolSpinning::enqueue(int taskId,const std::function<void()>& task){
    // power of 2 trick
    int q1 = std::rand() % (NUM_THREADS -1);
    int q2 = std::rand() % (NUM_THREADS - q1 ) +q1;
    if(q1 != q2 && queues[q1].size() > queues[q2].size())
        std::swap(q1,q2);
    // int q1 = taskId % NUM_THREADS;
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
        // std::cout << "main sleep\n";
        std::unique_lock<std::mutex> lk(lk_done);
        cv_main.wait(lk,[this]{return done <=0 || close;});
    }

}

TaskID TaskSystemParallelThreadPoolSpinning::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                              const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //

    return 0;
}

void TaskSystemParallelThreadPoolSpinning::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //

    return;
}

const char* TaskSystemParallelThreadPoolSleeping::name() {
    return "Parallel + Thread Pool + Sleep";
}

TaskSystemParallelThreadPoolSleeping::TaskSystemParallelThreadPoolSleeping(int num_threads): TaskSystemParallelThreadPoolSpinning(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    
}

TaskSystemParallelThreadPoolSleeping::~TaskSystemParallelThreadPoolSleeping() {
    
}

// void TaskSystemParallelThreadPoolSleeping::run(IRunnable* runnable, int num_total_tasks) {


//     //
//     // TODO: CS149 students will modify the implementation of this
//     // method in Part A.  The implementation provided below runs all
//     // tasks sequentially on the calling thread.
//     //

//     for (int i = 0; i < num_total_tasks; i++) {
//         runnable->runTask(i, num_total_tasks);
//     }
// }

TaskID TaskSystemParallelThreadPoolSleeping::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                    const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //

    return 0;
}

void TaskSystemParallelThreadPoolSleeping::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //

    return;
}
