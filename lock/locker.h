#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem
{
public:
    sem()
    {
        if (sem_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();
        }
    }
    sem(int num)
    {
        if (sem_init(&m_sem, 0, num) != 0)
        {
            throw std::exception();
        }
    }
    ~sem()
    {
        sem_destroy(&m_sem);
    }
    bool wait()
    {
        return sem_wait(&m_sem) == 0;
    }
    bool post()
    {
        return sem_post(&m_sem) == 0;
    }

private:
    sem_t m_sem;
}; // RAII
class locker
{
public:
    locker()
    {
        if (pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0;
    }
    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0;
    }
    pthread_mutex_t *get()
    {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
};
class cond
{
public:
    cond()
    {
        if (pthread_cond_init(&m_cond, NULL) != 0)
        {
            // pthread_mutex_destroy(&m_mutex);
            throw std::exception();
        }
    }
    ~cond()
    {
        pthread_cond_destroy(&m_cond);
    }
    bool wait(pthread_mutex_t *m_mutex)
    {
        int ret = 0;
        // pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_wait(&m_cond, m_mutex);
        // pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }
    bool timewait(pthread_mutex_t *m_mutex, struct timespec t)
    {
        int ret = 0;
        // pthread_mutex_lock(&m_mutex);
        ret = pthread_cond_timedwait(&m_cond, m_mutex, &t);
        // pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }
    bool signal()
    {
        return pthread_cond_signal(&m_cond) == 0;
    }
    bool broadcast()
    {
        return pthread_cond_broadcast(&m_cond) == 0;
    }

private:
    // static pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};
class LockerGuard
{
public:
    explicit LockerGuard(locker &mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }
    ~LockerGuard()
    {
        m_mutex.unlock();
    }
private:
    locker m_mutex;
};
#endif

// 一些函数解释
//
// sem_init(&m_sem, 0, 0)
//    参数1:   指向要初始化的信号量的指针
//    参数2:   指定信号量的共享性，通常为0表示信号量是线程内部共享的
//    参数3:   表示信号量的初始值
//    返回值:  0代表成功，非0代表失败
//
// sem_wait(&m_sem)
//    功能：尝试从信号量m_sem中减去1。
//         如果当前信号量的值大于等于1，sem_wait将立即返回，并将信号量的值减1；
//         如果当前信号量的值为0，sem_wait会阻塞当前线程，直到信号量的值大于等于1；
//    返回值：0--成功获取到了资源  非0---没有获取到资源
//
// sem_post(&m_sem)
//    功能：增加信号量m_sem的值。
//         将信号量的值增加1，用于释放资源或者通知等待在该信号量上的线程；
//         当信号量的值增加后，等待该信号量的线程中的一个（如果有的话）被唤醒，以便继续执行
//    返回值：0--成功  非0--失败
//
//
// pthread_mutex_init(&m_mutex, NULL)
//    &m_mutex：指向要初始化的互斥锁的指针
//    NULL：表示使用默认的互斥锁属性
//    初始化成功后，互斥锁 m_mutex 就可以用于对临界区（共享资源）进行保护，防止多个线程同时访问造成数据混乱
//
// pthread_mutex_lock(&m_mutex)
//    功能：请求互斥锁并阻塞直到获得该锁。
//         调用 pthread_mutex_lock 函数会尝试对互斥锁 m_mutex 进行加锁操作，
//         如果当前互斥锁没有被其他线程锁定，那么这个调用会立即成功，当前线程获得了互斥锁；
//         加锁操作的目的是确保在互斥锁范围内的临界区只能被一个线程访问，其他线程需要等待当前线程释放锁后才能继续执行；
//         当线程成功获得互斥锁时，它可以进入临界区执行关键代码。其他线程若尝试访问同一互斥锁，将被阻塞直到当前线程释放锁；
//
//
// pthread_cond_wait(&m_cond, m_mutex)
//    功能：调用 pthread_cond_wait 函数将会使当前线程在条件变量 m_cond 上等待。
//         在等待过程中，该函数会自动释放相应的互斥锁 m_mutex，当前线程会被阻塞直到有其他线程
//         通过条件变量 m_cond 发出信号通知并且成功获取互斥锁。
//         pthread_cond_wait 函数在等待条件满足时会阻塞当前线程，将线程挂起直到条件满足并且接收到信号。
//         在收到信号后，pthread_cond_wait 函数会重新获取互斥锁并返回，让线程可以继续执行。
//         这种机制通常用于线程之间的同步，其中一个线程等待某个条件成立，而其他线程在某些条件下发送信号来通知等待的线程继续执行。
//
// pthread_cond_timedwait(&m_cond, m_mutex, &t)
//   功能：相比于 pthread_cond_wait，pthread_cond_timedwait 允许设置一个超时时间 &t。
//        函数会在指定的超时时间之后自动返回，不管条件是否满足。
//
// pthread_cond_signal(&m_cond)
//    pthread_cond_signal 只会唤醒一个等待线程，如果有多个线程在条件变量上等待，则只有其中的一个会被唤醒。
//    通过调用 pthread_cond_signal 可以实现线程之间的协作，使得一个线程可以通知其他线程某个条件已经满足，从而引导线程的执行流程。
//    在使用条件变量时，通常会先修改共享条件，然后通过 pthread_cond_signal 或者 pthread_cond_broadcast 来通知等待线程条件的改变。
//
// pthread_cond_broadcast(&m_cond)
//    调用 pthread_cond_broadcast 函数会向等待在条件变量 m_cond 上的所有线程发送信号，
//    通知它们条件已经满足，可以继续执行。所有等待在这个条件变量上的线程都将被唤醒。