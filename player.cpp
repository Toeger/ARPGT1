#include <array>
#include <iostream>

#include "player.h"
#include "drawlist.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "conversions.h"
#include "sensor.h"

Player::Player(b2World &world, sf::RenderWindow *window):
    character(50),
    pc(world, {300, 300}, 50, 0.),
    camera(window)
{
    character.setFillColor({50, 100, 200});
    character.setPosition(400, 300);
    Drawlist::add(character);
    //UpdateList::add([this](){this->logicalUpdate();});
    LogicalObject::add(*this);
}

void rotate(b2Vec2 &vec, float angle){
    angle *= M_PI / 180;
    auto x = (vec.x * cos(angle)) - (vec.y * sin(angle));
    auto y = (vec.y * cos(angle)) + (vec.x * sin(angle));
    vec.x = x;
    vec.y = y;
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
        camera.rotate_left();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        velocity.y++;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x++;
        camera.rotate_right();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        //create attack cone
        b2BodyDef bodyDef;
        auto pos = pc.get_position();
        bodyDef.position = pos;

        /*
        b2BodyDef bodyDef;
        bodyDef.position.Set(center_position.x, center_position.y);
        body = world.CreateBody(&bodyDef);
        b2PolygonShape box;
        box.SetAsBox(size.x / 2, size.y / 2);
        body->CreateFixture(&box, 0.0f);
        */
        b2Body *body = get_world()->CreateBody(&bodyDef);
        b2PolygonShape box;
        std::array<b2Vec2, 3> points
        {
            pos,
            {pos.x + 100, pos.y + 100},
            {pos.x + 100, pos.y - 100},
        };
        (void)points;
        //box.Set(points.data(), points.size());
        box.SetAsBox(100, 100);
        body->CreateFixture(&box, 0.0f);

        body->SetType(b2_dynamicBody);
        body->SetActive(true);

        //check if we hit anything
        //std::cout << Sensor::get_collisions(body).size() << std::endl;
        //apply damage/knockback
        //remove attack cone
        get_world()->DestroyBody(body);
    }
    velocity.Normalize();
    velocity.x *= speed;
    velocity.y *= speed;
    rotate(velocity, camera.get_rotation());

    pc.set_velocity(velocity);
    pc.apply_position(character);
    auto box2dposition = pc.get_position();
    camera.set_position(box2dposition.x, box2dposition.y);
}

b2World *Player::get_world()
{
    return pc.get_world();
}
