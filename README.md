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
## 📚 Чтение из файла
```cpp
win::io::StreamReader reader(file.GetStream());
win::dbg::Console::Info(reader.ReadToEnd());
```
## ✍️ Запись в файл
```cpp
win::io::StreamWriter writer(file.GetStream());
writer.WriteLine(L"Привет, мир!");
```
## 🧹 Автоматическое удаление
```cpp
file.Delete();
```
## 📁 Работа с директориями
```cpp
win::io::Directory dir(L"MyFolder", true);
if (dir.Exists()) {
    for (auto& file : dir.GetFiles()) {
        win::dbg::Console::Info(file.Name());
    }
}
```
## 🖥️ Работа с консолью
```cpp
win::dbg::Console::WriteLine(win::dbg::Console::Color::Green, L"Успешно!");
win::dbg::Console::Error(L"Произошла ошибка.");
```

## 🚧 Планы на будущее
Следующая версия будет включать:

> 🌐 Сетевое программирование (Winsock)

> 🧵 Многопоточность (CreateThread, критические секции)

> 📁 Улучшенная работа с путями и файловой системой

## 📌 Текущая версия
``
v0.2
``
