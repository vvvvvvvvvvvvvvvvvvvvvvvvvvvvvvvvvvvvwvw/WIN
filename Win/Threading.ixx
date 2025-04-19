#include  "pch.h"

export module Threading;

import Object;
import Handle;
import TypeDef;
import Synchronization;

using namespace win;


template<typename F, typename... ARGS>
struct TaskContext {
    F func;
    std::tuple<ARGS...> args;

    TaskContext(F&& f, ARGS&&... a)
        : func(std::forward<F>(f))
        , args(std::forward<ARGS>(a)...)
    {
    }
};

template<typename F, typename... ARGS>
ULONG WINAPI ThreadBody(LPVOID lp)
{
    using Ctx = TaskContext<F, ARGS...>;
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
        Thread(const Handle& hThread) : m_hThread(hThread) {}

    public:
        template<typename F, typename... ARGS, std::enable_if_t<std::is_invocable_v<F, ARGS...>, int> = 0>
        Thread(F&& task, ARGS&&... args)
        {

            static_assert(std::is_invocable_v<F, ARGS...>, "IS NOT INVOCABLE");
            using DecayF = std::decay_t<F>;
            using Ctx = TaskContext<DecayF, std::decay_t<ARGS>...>;
            auto* ctx = new Ctx(std::forward<F>(task), std::forward<ARGS>(args)...);
            m_hThread = CreateThread(
                nullptr,
                0,
                ThreadBody<DecayF, std::decay_t<ARGS>...>,
                ctx,
                0,
                &m_id
            );
            GetThreadContext(m_hThread, m_context);
        }
        Thread(const Thread& other) : m_hThread(other.m_hThread) {}
        Thread(Thread&& other) noexcept : m_hThread(std::move(other.m_hThread)) {}

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

//
// --------------------------------------------------
//

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
                    m_cv.Wait(m_cs, [&]() { return !m_tasks.empty() || !m_running; });
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
                m_workers.emplace_back(workerTask, this);
            }
        }

        
        template<typename F, typename... ARGS, std::enable_if_t<std::is_invocable_v<F, ARGS...>, int> = 0>
        void enqueue(F&& func, ARGS&&... args)
        {
            LockGuard<CriticalSection> lock(m_cs);
            m_tasks.emplace([f = std::make_shared<std::decay_t<F>>(std::forward<F>(func)),
                tuple_args = std::make_shared<std::tuple<std::decay_t<ARGS>...>>(std::forward<ARGS>(args)...)]() mutable {
                    std::apply(*f, std::move(*tuple_args));
                });
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

//
//-----------------------------------------------
//

template<typename F, typename... ARGS>
VOID CALLBACK SystemPoolBody(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
    using Ctx = TaskContext<F, ARGS...>;
    std::unique_ptr<Ctx> context(static_cast<Ctx*>(Context));
    using ReturnType = std::invoke_result_t<F, ARGS...>;
    auto caller = [&]() -> ReturnType {
        return std::apply(
            [](auto&& f, auto&&... args) -> ReturnType {
                return std::invoke(std::forward<decltype(f)>(f), std::forward<decltype(args)>(args)...);
            },
            std::tuple_cat(std::make_tuple(std::move(context->func)), std::move(context->args))
        );
        };
    caller();
}

export namespace win::threading
{
    class SystemThreadPool : public Object<SystemThreadPool>
    {
    public:
        SystemThreadPool() = default;

      
        template<typename F, typename... ARGS>
        void enqueue(F&& func, ARGS&&... args)
        {
            using DecayF = std::decay_t<F>;
            auto* ctx = new TaskContext<DecayF, std::decay_t<ARGS>...>(
                std::forward<F>(func),
                std::forward<ARGS>(args)...
            );
            PTP_WORK work = CreateThreadpoolWork(
                &SystemPoolBody<DecayF, std::decay_t<ARGS>...>,
                ctx,
                nullptr
            );
            SubmitThreadpoolWork(work);
        }
    };
}
