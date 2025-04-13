#include <Windows.h>
#include <stdexcept>

export module Synchronization;

import Object;
import Handle;
import TypeDef;

export namespace win::threading
{
        class Signal : public Object<Signal> {
        private:
            Handle m_hEvent;

        public:
            Signal() = delete;

            Signal(bool manual, bool state) {
                m_hEvent = CreateEvent(nullptr, manual, state, nullptr);
                if (!m_hEvent.IsValid()) {
                    throw 404;
                }
            }

            void Set() {
                if (!m_hEvent.IsValid()) {
                    throw std::runtime_error("Invalid event handle");
                }
                SetEvent(m_hEvent);
            }

            void ReSet() {
                if (!m_hEvent.IsValid()) {
                    throw std::runtime_error("Invalid event handle");
                }
                ResetEvent(m_hEvent);
            }

            unsigned long Wait(unsigned long timeout = INFINITE) {
                if (!m_hEvent.IsValid()) {
                    throw std::runtime_error("Invalid event handle");
                }
                return WaitForSingleObject(m_hEvent, timeout);
            }

            Handle* native_handle() {
                return &m_hEvent;
            }

            ~Signal() {
                if (m_hEvent.IsValid()) {
                    CloseHandle(m_hEvent);
                }
            }
        };
        class Mutex : public Object<Mutex> {
        private:
            Handle m_hMutex;

        public:
            Mutex() {
                m_hMutex = CreateMutex(nullptr, false, nullptr);
                if (!m_hMutex.IsValid()) {
                    throw 404;
                }
            }

            Mutex(const Mutex& other) = delete;
            Mutex(Mutex&& other) = delete;

            void Lock() {
                if (!m_hMutex.IsValid()) {
                    throw std::runtime_error("Invalid mutex handle");
                }
                WaitForSingleObject(m_hMutex, INFINITE);
            }

            void Unlock() {
                if (!m_hMutex.IsValid()) {
                    throw std::runtime_error("Invalid mutex handle");
                }
                ReleaseMutex(m_hMutex);
            }

            Handle* native_handle() {
                return &m_hMutex;
            }

            ~Mutex() {
                if (m_hMutex.IsValid()) {
                    CloseHandle(m_hMutex);
                }
            }
        };
        class CriticalSection {
        private:
            CRITICAL_SECTION m_cs;

        public:
            CriticalSection() {
                if (!InitializeCriticalSectionEx(&m_cs, 0, 0)) {
                    throw 404;
                }
            }

            ~CriticalSection() {
                DeleteCriticalSection(&m_cs);
            }

            CriticalSection(const CriticalSection&) = delete;
            CriticalSection& operator=(const CriticalSection&) = delete;

            void Lock() {
                EnterCriticalSection(&m_cs);
            }

            void Unlock() {
                LeaveCriticalSection(&m_cs);
            }

            operator CRITICAL_SECTION* () {
                return &m_cs;
            }
        };
        class Semaphore : public Object<Semaphore> {
        private:
            Handle m_hSemaphore;

        public:
            Semaphore() = delete;

            Semaphore(int threads_count) {
                if (threads_count <= 0) {
                    throw std::invalid_argument("Semaphore count must be positive");
                }
                m_hSemaphore = CreateSemaphore(nullptr, threads_count, threads_count, nullptr);
                if (!m_hSemaphore.IsValid()) {
                    throw 404;
                }
            }

            void Wait() const {
                if (!m_hSemaphore.IsValid()) {
                    throw std::runtime_error("Invalid semaphore handle");
                }
                WaitForSingleObject(m_hSemaphore, INFINITE);
            }

            void Release() {
                if (!m_hSemaphore.IsValid()) {
                    throw std::runtime_error("Invalid semaphore handle");
                }
                ReleaseSemaphore(m_hSemaphore, 1, nullptr);
            }

            ~Semaphore() {
                if (m_hSemaphore.IsValid()) {
                    CloseHandle(m_hSemaphore);
                }
            }
        };
        class ConditionVariable : public Object<ConditionVariable> {
        private:
            CONDITION_VARIABLE cv;

        public:
            ConditionVariable() {
                InitializeConditionVariable(&cv);
            }

            template<typename Predicate>
            bool Wait(CriticalSection& cs, Predicate&& func) {
                static_assert(std::is_invocable_r_v<bool, Predicate>, "Predicate must be callable and return bool");
                while (!func()) {
                    if (!SleepConditionVariableCS(&cv, cs, INFINITE)) {
                        return false;
                    }
                }
                return true;
            }

            void Notify() {
                WakeConditionVariable(&cv);
            }

            void NotifyAll() {
                WakeAllConditionVariable(&cv);
            }

            ~ConditionVariable() = default;
        };
        template<typename _Mutex>
        class LockGuard {
        public:
            using mutex_type = _Mutex;

            explicit LockGuard(_Mutex& mtx) : _MyMutex(mtx) {
                _MyMutex.Lock();
            }

            ~LockGuard() noexcept {
                _MyMutex.Unlock();
            }

            LockGuard(const LockGuard&) = delete;
            LockGuard& operator=(const LockGuard&) = delete;

        private:
            _Mutex& _MyMutex;
        };
}