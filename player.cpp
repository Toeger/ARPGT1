#include <array>
#include <iostream>

#include "player.h"
#include "drawlist.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "conversions.h"
#include "sensor.h"
#include "utility.h"

Player::Player(b2World &world, sf::RenderWindow *window):
    character(50),
    pc(world, {300, 300}, 50, 0.),
    camera(window),
    weapon(pc.body)
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
    const float angle = 5;
    pc.body->SetAngularVelocity(0);
    const float angleCorrectionFactor = 3;
    auto rotate_left = [&angle, &angleCorrectionFactor, this]{
        camera.rotate(-angle);
        pc.body->SetAngularVelocity(pc.body->GetAngularVelocity() - angle * angleCorrectionFactor);
    };
    auto rotate_right = [&angle, &angleCorrectionFactor, this]{
        camera.rotate(angle);
        pc.body->SetAngularVelocity(pc.body->GetAngularVelocity() + angle * angleCorrectionFactor);
    };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        velocity.y--;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x--;
        rotate_left();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        velocity.y++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x++;
        rotate_right();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        std::cout << Sensor::get_collisions(pc.body).size() << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        rotate_left();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
        rotate_right();
    }
    velocity.Normalize();
    velocity.x *= speed;
    velocity.y *= speed;
    Utility::rotate(velocity, camera.get_rotation());

    pc.set_velocity(velocity);
    pc.apply_position(character);
    auto box2dposition = pc.get_position();
    camera.set_position(box2dposition.x, box2dposition.y);
}

b2World *Player::get_world()
{
    return pc.get_world();
}
