Вот пример кода для раздела README:

---

# 🧩 WinAPI Modular Framework

Простой и чистый фреймворк на C++ с использованием **только WinAPI**, без сторонних библиотек. Проект построен на модульной архитектуре с базовыми компонентами для работы с файлами, директориями, консолью и потоками.

> ✅ C++20 Modules  
> ✅ Только WinAPI  
> ✅ Минимализм и читаемость  
> ✅ Безопасное управление ресурсами

---

## 🔧 Модули

| Модуль                | Описание                                                                 |
|------------------------|--------------------------------------------------------------------------|
| `Object`               | Базовый CRTP-шаблон                                                     |
| `Handle`               | Безопасная обёртка над дескрипторами WinAPI                             |
| `String`               | Широкая строка с поддержкой конверсии UTF-8 / UTF-16                    |
| `Hresult`              | Удобная работа с HRESULT                                                |
| `Console`              | Цветной вывод и ввод из консоли                                         |
| `File`                 | Работа с файлами                                                        |
| `Directory`            | Работа с директориями                                                   |
| `StreamReader`         | Последовательное чтение потока                                          |
| `StreamWriter`         | Последовательная запись в поток                                         |
| `Buffer`               | Удобное хранение и передача бинарных данных                             |
| `MemoryMappedFile`     | Работа с файлами, отображёнными в память                                |
| `Random`               | Генерация псевдослучайных чисел                                         |
| `Event`                | Синхронизация с помощью событий                                         |
| `Signal`               | Упрощённый механизм синхронизации (сигналы открытия/закрытия потока)    |
| `Mutex`                | Классический мьютекс (дорогой по ресурсам)                              |
| `CriticalSection`      | Лёгкий и быстрый аналог мьютекса                                        |
| `Semaphore`            | Семафор для ограничения доступа потоков                                 |
| `ConditionVariable`    | Остановка и пробуждение потока по условию (например, очередь задач)      |
| `LockGuard`            | Автоматическая блокировка/разблокировка `Mutex` или `CriticalSection`   |
| `Thread`               | Создание и управление потоками                                          |
| `StaticThreadPool`     | Пул фиксированного числа потоков для асинхронных задач                  |
| `SystemThreadPool`     | Использование системного пула потоков                                   |
| `Socket`               | Обёртка над сокетами (без специализации)                               |
| `Process`              | Управление внешними процессами                                          |
| `Time`                 | Получение системного времени                                            |
| `PerformanceCounter`   | Работа со счётчиками производительности                                 |

---


## 📦 Примеры использования

### 📁 Работа с файлами

```cpp
win::io::File file(L"example.txt", true);
file.GetStream(); // получить дескриптор
```

### 📚 Чтение из файла
```cpp
win::io::StreamReader reader(file.GetStream());
win::dbg::Console::Info(reader.ReadToEnd());
```

### ✍️ Запись в файл
```cpp
win::io::StreamWriter writer(file.GetStream());
writer.WriteLine(L"Привет, мир!");
```

### 🧹 Автоматическое удаление
```cpp
file.Delete();
```

### 📁 Работа с директориями
```cpp
win::io::Directory dir(L"MyFolder", true);
if (dir.Exists()) {
    for (auto& file : dir.GetFiles()) {
        win::dbg::Console::Info(file.Name());
    }
}
```

### 🖥️ Работа с консолью
```cpp
win::dbg::Console::WriteLine(win::dbg::Console::Color::Green, L"Успешно!");
win::dbg::Console::Error(L"Произошла ошибка.");
```

### 🧵 Многопоточность
```cpp
void Pool_Example()
{
	win::dbg::Console::Success("Hello From WINAPI!");
}

int main()
{
	win::threading::StaticThreadPool sth(3);
	sth.enqueue(Pool_Example);
	sth.enqueue(Pool_Example);
	sth.enqueue(Pool_Example);

	win::threading::Thread::Sleep(999999);

	//   :(

	return 0;
}
```
## 🌐 Сетевое программирование (сырое)
```
void client_handling(win::net::Socket client)
{
    // exo server
    while (true)
    {
        win::Buffer message = client.Receive();
        client.Send(message);
    }
}

int main()
{
   
    win::net::Socket server_socket(win::net::ipv4, win::net::tcp);
    server_socket.Bind("127.0.0.1", "8080");
    server_socket.Listen(10);

    win::threading::SystemThreadPool sys_pool;
    
    while (true)
    {
        auto client = server_socket.Accept();
        sys_pool.enqueue(client_handling, std::move(client));
    }
    return 0;
}
```
---

## 🚧 Планы на будущее

В следующих версиях будет добавлено:

- **v0.3 - v0.4**:
  - 🌐 **Сетевое программирование** (Winsock)
---

## 📌 Текущая версия


**v0.3**

---

### Изменения в версии v0.2.5:


- 🧵 **Добавлен модуль `Synchronization`**, включающий:
  - `Signal` — обёртка над `CreateEvent`
  - `Mutex` — класс для защиты от одновременного доступа
  - `CriticalSection` — lightweight-альтернатива мьютексу
  - `Semaphore` — управление количеством одновременно работающих потоков
  - `ConditionVariable` — синхронизация ожидания с возможностью пробуждения
  - `LockGuard` — RAII-обёртка для автоматического захвата/освобождения
- 🧵 **Добавлен модуль `Threading`**, включающий:
  - `Thread` — безопасный обёрточный класс для `CreateThread`
  - `StaticThreadPool` — пул потоков с поддержкой очереди задач и `future`
  - `SystemThreadPool` — обёртка над `CreateThreadpoolWork`, нативный thread pool Windows
  
--- 

## 📥 Установка

1. Клонируйте репозиторий:

   ```bash
   git clone https://github.com/yourusername/WIN.git
   ```

2. Скомпилируйте проект с использованием вашего любимого компилятора C++20:

   ```bash
   g++ -std=c++20 -o your_program main.cpp
   ```

---

