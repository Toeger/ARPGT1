#include "updatelist.h"

#include <vector>

std::vector<std::function<void()>> updatelist;

void UpdateList::add(std::function<void ()> f)
{
    updatelist.emplace_back(std::move(f));
}

void UpdateList::updateAll()
{
    for (auto &f : updatelist)
        f();
}
