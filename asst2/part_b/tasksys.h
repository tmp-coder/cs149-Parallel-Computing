#ifndef _TASKSYS_H
#define _TASKSYS_H

#include<thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <unordered_set>
#include <condition_variable>
#include <unordered_map>
#include "itasksys.h"

/*
 * TaskSystemParallelThreadPoolSpinning: This class is the student's
 * implementation of a parallel task execution engine that uses a
 * thread pool. See definition of ITaskSystem in itasksys.h for
 * documentation of the ITaskSystem interface.
 */
class TaskSystemParallelThreadPoolSpinning: public ITaskSystem {

    private:
        std::atomic<int> done;
        std::vector<std::thread> workers;
        std::vector<std::mutex *> lks;
        std::vector<std::queue<std::function<void()>>> queues;
        int NUM_THREADS;
        void enqueue(int taskId,const std::function<void()> & task);
        void init();
        std::mutex finish,lk_done;
        std::condition_variable cv_finish,cv_main;
        bool close;
        // for async schduler
        
        std::queue<TaskID> running;
        std::unordered_map<TaskID,std::vector<TaskID>* > dependency_graph;
        std::unordered_map<TaskID,int> deg;
        std::unordered_map<TaskID,std::pair<IRunnable*,int>> task_groups;
        int task_id=0;
        void try_steal(int cur_thread,int steal_thread);
    public:
        TaskSystemParallelThreadPoolSpinning(int num_threads);
        ~TaskSystemParallelThreadPoolSpinning();
        virtual const char* name();
        virtual void run(IRunnable* runnable, int num_total_tasks);
        virtual TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps);
        virtual void sync();
};

/*
 * TaskSystemSerial: This class is the student's implementation of a
 * serial task execution engine.  See definition of ITaskSystem in
 * itasksys.h for documentation of the ITaskSystem interface.
 */
class TaskSystemSerial: public TaskSystemParallelThreadPoolSpinning {
    public:
        TaskSystemSerial(int num_threads);
        ~TaskSystemSerial();
        const char* name();
        void run(IRunnable* runnable, int num_total_tasks);
        TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                const std::vector<TaskID>& deps){
                                    run(runnable,num_total_tasks);
                                }
        void sync(){}

        
};

/*
 * TaskSystemParallelSpawn: This class is the student's implementation of a
 * parallel task execution engine that spawns threads in every run()
 * call.  See definition of ITaskSystem in itasksys.h for documentation
 * of the ITaskSystem interface.
 */
class TaskSystemParallelSpawn: public TaskSystemParallelThreadPoolSpinning {
    public:
        std::thread* ths;
        int num_threads;
        TaskSystemParallelSpawn(int num_threads);
        ~TaskSystemParallelSpawn();
        const char* name();
        // void staticAssign(IRunnable * runnable,int thread_idx,int num_total_tasks);
        void run(IRunnable* runnable, int num_total_tasks);
        // TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
        //                         const std::vector<TaskID>& deps);
        // void sync();
        static void staticAssign(IRunnable * runnable,int num_total_tasks,int taks_start,int num_tasks);
};


/*
 * TaskSystemParallelThreadPoolSleeping: This class is the student's
 * optimized implementation of a parallel task execution engine that uses
 * a thread pool. See definition of ITaskSystem in
 * itasksys.h for documentation of the ITaskSystem interface.
 */
class TaskSystemParallelThreadPoolSleeping:public TaskSystemParallelThreadPoolSpinning {
    public:
        TaskSystemParallelThreadPoolSleeping(int num_threads);
        ~TaskSystemParallelThreadPoolSleeping();
        const char* name();
        // void run(IRunnable* runnable, int num_total_tasks);
        // TaskID runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
        //                         const std::vector<TaskID>& deps);
        // void sync();
};

#endif
