---

# 🧩 WinAPI Modular Framework

A simple, clean C++ framework using **only the WinAPI**, with no external libraries. The project is built on a modular architecture with core components for working with files, directories, console I/O, and threads.

> ✅ C++20 Modules  
> ✅ Pure WinAPI  
> ✅ Minimalist & Readable  
> ✅ Safe Resource Management

---

## 🔧 Modules

| Module                  | Description                                                              |
|-------------------------|---------------------------------------------------------------------------|
| `Object`                | Basic CRTP template                                                       |
| `Handle`                | Safe wrapper around WinAPI handles                                        |
| `String`                | Wide‐string with UTF-8/UTF-16 conversion support                          |
| `Hresult`               | Convenient HRESULT handling                                               |
| `Console`               | Colored console input/output                                              |
| `File`                  | File operations                                                           |
| `Directory`             | Directory operations                                                      |
| `StreamReader`          | Sequential stream reading                                                 |
| `StreamWriter`          | Sequential stream writing                                                 |
| `Buffer`                | Convenient storage and transfer of binary data                            |
| `MemoryMappedFile`      | Working with memory-mapped files                                          |
| `Random`                | Pseudorandom number generation                                            |
| `Event`                 | Event object handling                                                      |
| `Signal`                | Lightweight synchronization (open/close signals)                           |
| `Mutex`                 | Classic mutex (resource-heavy)                                            |
| `CriticalSection`       | Lightweight, fast alternative to mutex                                     |
| `Semaphore`             | Semaphore for limiting thread access                                      |
| `ConditionVariable`     | Thread wait/wake by condition (e.g., task queue)                          |
| `LockGuard`             | RAII lock/unlock for `Mutex` or `CriticalSection`                         |
| `Thread`                | Thread creation and management                                            |
| `StaticThreadPool`      | Fixed-size thread pool for asynchronous tasks                             |
| `SystemThreadPool`      | Wrapper over the system thread pool                                       |
| `Socket`                | Basic socket wrapper (no protocol specialization)                         |
| `Process`               | External process management                                               |
| `Time`                  | System time retrieval                                                     |
| `PerformanceCounter`    | Performance counter utilities                                              |

---

## 📦 Usage Examples

### 📁 File Operations

```cpp
win::io::File file(L"example.txt", true);
file.GetStream(); // get the handle
```

### 📚 Reading from a File

```cpp
win::io::StreamReader reader(file.GetStream());
win::dbg::Console::Info(reader.ReadToEnd());
```

### ✍️ Writing to a File

```cpp
win::io::StreamWriter writer(file.GetStream());
writer.WriteLine(L"Hello, world!");
```

### 🧹 Automatic Cleanup

```cpp
file.Delete();
```

### 📁 Directory Operations

```cpp
win::io::Directory dir(L"MyFolder", true);
if (dir.Exists()) {
    for (auto& f : dir.GetFiles()) {
        win::dbg::Console::Info(f.Name());
    }
}
```

### 🖥️ Console I/O

```cpp
win::dbg::Console::WriteLine(win::dbg::Console::Color::Green, L"Success!");
win::dbg::Console::Error(L"An error occurred.");
```

### 🧵 Multithreading

```cpp
void PoolExample() {
    win::dbg::Console::Success("Hello From WinAPI!");
}

int main() {
    win::threading::StaticThreadPool pool(3);
    pool.enqueue(PoolExample);
    pool.enqueue(PoolExample);
    pool.enqueue(PoolExample);

    win::threading::Thread::Sleep(999999);
    return 0;
}
```

### 🌐 Networking (Raw)

```cpp
void handleClient(win::net::Socket client) {
    while (true) {
        win::Buffer msg = client.Receive();
        client.Send(msg);
    }
}

int main() {
    win::net::Socket server(win::net::ipv4, win::net::tcp);
    server.Bind("127.0.0.1", "8080");
    server.Listen(10);

    win::threading::SystemThreadPool sysPool;

    while (true) {
        auto client = server.Accept();
        sysPool.enqueue(handleClient, std::move(client));
    }
    return 0;
}
```

---

## 🚧 Roadmap

Coming in future releases:

- **v0.3 – v0.4**:  
  - 🌐 **Networking** (Winsock)

---

## 📌 Current Version

**v0.3**

---

### Changes in v0.2.5

- 🧵 **Added `Synchronization` module**, including:
  - `Signal` — wrapper for `CreateEvent`
  - `Mutex` — mutual‐exclusion primitive
  - `CriticalSection` — lightweight mutex alternative
  - `Semaphore` — concurrency limiter
  - `ConditionVariable` — wait/wake synchronization
  - `LockGuard` — RAII lock for automatic acquire/release
- 🧵 **Added `Threading` module**, including:
  - `Thread` — safe wrapper for `CreateThread`
  - `StaticThreadPool` — thread pool with task queue and `future`
  - `SystemThreadPool` — wrapper for `CreateThreadpoolWork`

---

## 📥 Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/WIN.git
   ```

2. Build with your favorite C++20 compiler:

   ```bash
   g++ -std=c++20 -o your_program main.cpp
   ```

---
