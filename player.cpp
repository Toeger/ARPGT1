#include "player.h"
#include "drawlist.h"
#include "updatelist.h"
#include "logicalobject.h"


Player::Player():
    character(50)
{
    character.setFillColor(sf::Color(50, 100, 200));
    Drawlist::add(character);
    //UpdateList::add([this](){this->logicalUpdate();});
    LogicalObject::add(*this);
}

void Player::logicalUpdate()
{
    sf::Color c = character.getFillColor();
    c.r++;
    character.setFillColor(c);
    if (c.r == 0)
        LogicalObject::remove(*this);
}
