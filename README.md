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

| Модуль         | Описание                                      |
|----------------|-----------------------------------------------|
| `Object`       | Базовый CRTP-шаблон                           |
| `Handle`       | Безопасная обёртка над дескрипторами WinAPI  |
| `String`       | Широкая строка с конвертацией UTF-8 / UTF-16 |
| `Hresult`      | Удобная работа с HRESULT                      |
| `Console`      | Цветной вывод и чтение из консоли            |
| `File`         | Работа с файлами                              |
| `Directory`    | Работа с директориями                         |
| `StreamReader` | Чтение потока                                 |
| `StreamWriter` | Запись в поток                                |

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

---

## 🚧 Планы на будущее

В следующих версиях будет добавлено:

- **v0.3 - v0.4**:
  - 🌐 **Сетевое программирование** (Winsock)
  - 🧵 **Многопоточность** (Thread, ThreadPool, Signal, критические секции, ...)

---

## 📌 Текущая версия


**v0.2.5**

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

