#include <tuple>
#include <memory>
#include <Windows.h>
#include <deque>
#include <vector>
#include <future>
#include <queue>

export module Threading;

import Object;
import Handle;
import TypeDef;
import Synchronization;

using namespace win;
template<typename RT, typename... ARGS>
using Task = Func<RT, ARGS...>;


template<typename RT, typename... ARGS>
struct TaskContext {
    Task<RT, ARGS...> func;
    std::tuple<ARGS...> args;

    TaskContext(Task<RT, ARGS...> f, ARGS&& ...a)
        : func(f), args(std::forward<ARGS>(a)...)
    {
    }
};

template<typename RT, typename... ARGS>
ULONG WINAPI ThreadBody(LPVOID lp)
{
    using Ctx = TaskContext<RT, ARGS...>;
    std::unique_ptr<Ctx> context(static_cast<Ctx*>(lp));
    std::apply(context->func, context->args);
    return 0;
}

export namespace win::threading
{
    class Thread : public Object<Thread>
    {
    private:
        Handle m_hThread;
        CONTEXT* m_context;
        d_word m_id;
    private:

        Thread(const Handle& hTherad) : m_hThread(hTherad) {}

    public:
        template<typename RT, typename... ARGS>
        Thread(Task<RT, ARGS...> task, ARGS... args)
        {
            auto* ctx = new TaskContext<RT, ARGS...>(task, std::forward<ARGS>(args)...);
            m_hThread = CreateThread(0, 0, ThreadBody<RT, ARGS...>, ctx, 0, &m_id);
            GetThreadContext(m_hThread, m_context);
        }
        Thread(const Thread& other) : m_hThread(other.m_hThread) {};
        Thread(Thread&& other) noexcept : m_hThread(std::move(other.m_hThread)) {};

        void Join()
        {
            if (m_hThread.IsValid())
                WaitForSingleObject(m_hThread, INFINITE);
        }
        bool Joinable() const
        {
            return m_hThread.IsValid();
        }
        static void Sleep(unsigned long ms)
        {
            ::Sleep(ms);
        }
        static d_word Id()
        {
            return GetCurrentThreadId();
        }
        Thread GetCurrentThread()
        {
            return Thread(::GetCurrentThread());
        }
        ~Thread() = default;
    };
}

export namespace win::threading {

    class StaticThreadPool : public Object<StaticThreadPool> {
    private:
        std::vector<Thread> m_workers;
        std::queue<std::function<void()>> m_tasks;
        CriticalSection m_cs;
        ConditionVariable m_cv;
        bool m_running;

    private:

        void workerThread() {
            while (true) {
                std::function<void()> task;
                {
                    LockGuard<CriticalSection> lock(m_cs);
                    m_cv.Wait(m_cs, m_tasks.empty());

                    if (!m_running && m_tasks.empty()) {
                        return; 
                    }

                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                task();
            }
        }

        static void __stdcall workerTask(StaticThreadPool* pool)
        {
            pool->workerThread();
        }

    public:
        StaticThreadPool(size_t thread_count) : m_running(true)
        {
            m_workers.reserve(thread_count);
            for (size_t i = 0; i < thread_count; ++i) {
                m_workers.emplace_back(Task<void, StaticThreadPool*>{workerTask}, this);
            }
        }

        template<typename F, typename... Args>
        void enqueue(F&& func, Args&&... args)
        {
            {
                LockGuard<CriticalSection> lock(m_cs);
                m_tasks.emplace([f = std::decay_t<F>(std::forward<F>(func)),
                    tuple_args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                        std::apply(f, std::move(tuple_args));
                    });
            }
            m_cv.Notify();
        }


        template<typename F, typename... Args>
        auto enqueueWithResult(F&& func, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> 
        {
            using ReturnType = std::invoke_result_t<F, Args...>;
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                [f = std::forward<F>(func),
                tuple_args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                    return std::apply(f, std::move(tuple_args));
                }
            );
            std::future<ReturnType> future = task->get_future();
            {
                LockGuard<CriticalSection> lock(m_cs);
                m_tasks.emplace([task]() { (*task)(); });
            }
            m_cv.Notify();
            return future;
        }

        void stop() {
            {
                LockGuard<CriticalSection> lock(m_cs);
                m_running = false;
            }
            m_cv.NotifyAll();
            for (auto& worker : m_workers) {
                if (worker.Joinable()) {
                    worker.Join();
                }
            }
        }

        ~StaticThreadPool() {
            stop();
        }
    };

}

struct WORK
{
    bool initalized;
    bool working;
    PTP_WORK work;
};

template<typename RT, typename... ARGS>
struct SystemThreadPoolContext : public TaskContext<RT, ARGS...>
{
   WORK* owner;
};

template<typename RT, typename... ARGS>
VOID CALLBACK SystemPoolBody(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
    using Ctx = SystemThreadPoolContext<RT, ARGS...>;
    std::unique_ptr<Ctx> context(static_cast<Ctx*>(Context));
    std::apply(context->func, context->args);
    if (context->owner)
        context->owner->working = false;
}

export namespace win::threading
{

    class SystemThreadPool : public Object<SystemThreadPool>
    {
    private:
        std::vector<WORK> m_works;
    public:

        SystemThreadPool(size_t thread_count) : m_works(thread_count) {}

        template<typename RT, typename... ARGS>
        void enqueue(Task<RT, ARGS...> task, ARGS... args)
        {
            for (auto& w : m_works)
            {
                if (!w.working)
                {
                    auto* ctx = new SystemThreadPoolContext<RT, ARGS...>(task, std::forward<ARGS>(args)...);
                    ctx->owner = &w;

                    if (w.initalized)
                    {
                        CloseThreadpoolWork(w.work);
                        w.initalized = false;
                    }

                 
                    w.work = CreateThreadpoolWork(SystemPoolBody<RT, ARGS...>, ctx, NULL);
                    w.initalized = true;
                 

                    SubmitThreadpoolWork(w.work);
                    w.working = true;
                    return;
                }
            }
        }
    };

}