#pragma once

#include <Alpha/Core/Types.h>
#include <Alpha/Core/Logger.h>

namespace Alpha
{
    template<typename T>
    class Library
    {
    public:
        inline void Push(const std::string& name, T& obj)
        {
            if (Contains(name)) Logger::Warn("{0} is already register", name);
            else m_collection.insert(std::make_pair(name, obj));
        }

        inline T Get(const std::string& name)
        {
            auto it = m_collection.find(name);
            return (it != m_collection.end()) ? it->second : nullptr;
        }

        inline bool Contains(const std::string& name)
        {
            auto it = m_collection.find(name);
            return it != m_collection.end();
        }

        inline bool Remove(const std::string& name)
        {
            auto it = m_collection.find(name);
            bool bIsFound = it != m_collection.end();

            if (bIsFound) m_collection.erase(it);

            return bIsFound;
        }

    private:
        std::map<std::string, T> m_collection;
    };
}