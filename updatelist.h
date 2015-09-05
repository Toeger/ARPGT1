#ifndef UPDATELIST_H
#define UPDATELIST_H

#include <functional>

namespace UpdateList {
    void add(std::function<void()> f);
    void updateAll();
}

#endif // UPDATELIST_H
