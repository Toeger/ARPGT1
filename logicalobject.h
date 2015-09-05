#ifndef LOGICALOBJECT_H
#define LOGICALOBJECT_H

#include <vector>
#include <memory>
#include <type_traits>

class LogicalObject
{
    static std::vector<LogicalObject> logicalObjects;
public:
    static void update(); //updates all logical objects
    template <typename T>
    static void add(T &&t){
        logicalObjects.emplace_back(std::forward<T>(t));
    }
    template <typename T>
    static void remove(const T &t){
        for (auto it = begin(logicalObjects); it != end(logicalObjects); ++it){
            if (it->isThatYou(t)){
                logicalObjects.erase(it);
                break;
            }
        }
    }

    void logicalUpdate(){
        base->logicalUpdate();
    }
    template <typename T>
    //std::enable_if<!std::is_same<T, LogicalObject>::value>
    LogicalObject(T &&t) noexcept :
        base(std::make_unique<LogicalObject_derived<std::remove_reference_t<T>>>(std::forward<T>(t)))
    {
    }
    LogicalObject(const LogicalObject &) = delete;
    LogicalObject(LogicalObject &&other) :
        base(std::move(other.base))
    {}
    LogicalObject &operator =(LogicalObject &&other){
        using namespace std;
        swap(base, other.base);
        return *this;
    }

private:
    template <typename T>
    bool isThatYou(const T &t) const{
        return base->isThatYou(&t);
    }

    struct LogicalObject_base{
        virtual void logicalUpdate() = 0;
        virtual bool isThatYou(const void *) const = 0;
    };
    template <typename T>
    struct LogicalObject_derived : LogicalObject_base{
        void logicalUpdate() override{
            m_t.logicalUpdate();
        }
        bool isThatYou(const void *p) const override{
            return &m_t == p;
        }
        LogicalObject_derived(T &t) :
        m_t(t)
        {}
        LogicalObject_derived(T &&t) :
        m_t(std::move(t))
        {}
        T &m_t;
    };

    std::unique_ptr<LogicalObject_base> base;
    LogicalObject();
};

#endif // LOGICALOBJECT_H
