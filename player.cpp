#include "player.h"
#include "drawlist.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "conversions.h"

Player::Player(b2World &world):
    character(50)
{
    character.setFillColor({50, 100, 200});
    character.setPosition(400, 300);
    Drawlist::add(character);
    //UpdateList::add([this](){this->logicalUpdate();});
    LogicalObject::add(*this);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(400, 300);
    body = world.CreateBody(&bodyDef);
    b2CircleShape characterShape;
    characterShape.m_radius = 50;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &characterShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
}

void Player::logicalUpdate()
{
    sf::Color c = character.getFillColor();
    c.r++;
    character.setFillColor(c);
    b2Vec2 velocity{0, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        velocity.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        velocity.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x += speed;
    }
    body->SetLinearVelocity(velocity);
    character.setPosition(
                Conversions::boxPosToSfCirclePos(
                    body->GetPosition(), character.getRadius()));
}
