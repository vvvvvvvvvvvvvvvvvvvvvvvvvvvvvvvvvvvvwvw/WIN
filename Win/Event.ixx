#include "pch.h"

export module Event;


template<typename... Args>
class Event {
public:
    using HandlerFunc = std::function<void(Args...)>;
    using HandlerId = size_t;

    Event() : _nextId(1) {}

    
    HandlerId subscribe(HandlerFunc&& handler) {
        std::lock_guard<std::mutex> lock(_mutex);
        HandlerId id = _nextId++;
        _handlers.emplace(id, std::move(handler));
        return id;
    }

    void unsubscribe(HandlerId id) {
        std::lock_guard<std::mutex> lock(_mutex);
        _handlers.erase(id);
    }
    
    void operator()(Args... args) const {
        std::unordered_map<HandlerId, HandlerFunc> handlersCopy;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            handlersCopy = _handlers;
        }
        for (auto& [id, func] : handlersCopy) {
            func(std::forward<Args>(args)...);
        }
    }

private:
    mutable std::mutex _mutex;
    std::unordered_map<HandlerId, HandlerFunc> _handlers;
    HandlerId _nextId;
};



template<typename... Args>
class ScopedConnection {
public:
    using EventType = Event<Args...>;
    ScopedConnection(EventType& evt, typename EventType::HandlerId id)
        : _event(&evt), _id(id) {
    }
    ~ScopedConnection() {
        if (_event) _event->unsubscribe(_id);
    }
  
    ScopedConnection(const ScopedConnection&) = delete;
    ScopedConnection& operator=(const ScopedConnection&) = delete;
    
    ScopedConnection(ScopedConnection&& o) noexcept
        : _event(o._event), _id(o._id) {
        o._event = nullptr;
    }
private:
    EventType* _event;
    typename EventType::HandlerId _id;
};
