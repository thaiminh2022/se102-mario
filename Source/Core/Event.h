#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

template <typename... Args>
class Event
{
public:
    using Handler = std::function<void(Args...)>;
    using SubscriptionId = std::size_t;

private:
    std::unordered_map<SubscriptionId, Handler> subscribers;
    SubscriptionId nextId = 0;

public:
    void Clear()
    {
        subscribers.clear();
    }

    SubscriptionId Subscribe(Handler handler)
    {
        const SubscriptionId id = ++nextId;
        subscribers.emplace(id, std::move(handler));
        return id;
    }

    void UnSubscribe(SubscriptionId id)
    {
        subscribers.erase(id);
    }

    void Emit(Args... args)
    {
        auto snapshot = subscribers;

        for (const auto& [id, handler] : snapshot)
        {
            if (subscribers.contains(id))
            {
                handler(args...);
            }
        }
    }
};