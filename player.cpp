#include <array>

#include "player.h"
#include "drawlist.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "conversions.h"
#include "sensor.h"
#include <QDebug>

Player::Player(b2World &world):
    character(50),
    pc(world, {300, 300}, 50)
{
    character.setFillColor({50, 100, 200});
    character.setPosition(400, 300);
    Drawlist::add(character);
    //UpdateList::add([this](){this->logicalUpdate();});
    LogicalObject::add(*this);
}

void Player::logicalUpdate()
{
    sf::Color c = character.getFillColor();
    c.r++;
    character.setFillColor(c);
    b2Vec2 velocity{0, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        velocity.y--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        velocity.y++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        //create attack cone
        b2BodyDef bodyDef;
        auto pos = pc.get_position();
        bodyDef.position = pos;
        b2Body *body = get_world()->CreateBody(&bodyDef);
        b2PolygonShape box;
        std::array<b2Vec2, 3> points
        {
            pos,
            {pos.x + 100, pos.y + 100},
            {pos.x + 100, pos.y - 100},
        };
        box.Set(points.data(), points.size());
        body->CreateFixture(&box, 0.0f);
        body->SetType(b2_dynamicBody);
        body->SetActive(true);

        //check if we hit anything
        qDebug() << Sensor::get_collisions(body).size();
        //apply damage/knockback
        //remove attack cone
        get_world()->DestroyBody(body);
    }
    velocity.Normalize();
    velocity.x *= speed;
    velocity.y *= speed;
    pc.set_velocity(velocity);
    pc.apply_position(character);
}

b2World *Player::get_world()
{
    return pc.get_world();
}
