#include "logicalobject.h"

std::vector<LogicalObject> LogicalObject::logicalObjects;

void LogicalObject::update()
{
    for (auto &lo : logicalObjects)
        lo.logicalUpdate();
}
