#include <array>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "player.h"
#include "drawlist.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "conversions.h"
#include "sensor.h"
#include "utility.h"
#include "hittable.h"
#include "enemylist.h"

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

float degreeToRadians(float degree){
    return degree * M_PI / 180;
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
        pc.body->SetAngularVelocity(pc.body->GetAngularVelocity() - degreeToRadians(angle) * angleCorrectionFactor);
    };
    auto rotate_right = [&angle, &angleCorrectionFactor, this]{
        pc.body->SetAngularVelocity(pc.body->GetAngularVelocity() + degreeToRadians(angle) * angleCorrectionFactor);
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
        auto collisions = Sensor::get_collisions(pc.body);
        for (auto &c : collisions){
            auto user_data = c->GetUserData();
            if (user_data){
                static_cast<Hittable *>(user_data)->take_hit(weapon);
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        rotate_left();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
        rotate_right();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
        if (EnemyList::enemy_list.size()){
            const auto self_position = pc.body->GetPosition();
            b2Body *closest_body = *std::min_element(std::begin(EnemyList::enemy_list), std::end(EnemyList::enemy_list),
                [this, &self_position](const b2Body *lhs, const b2Body *rhs){
                const auto lhs_distance = (self_position - lhs->GetPosition()).Length();
                const auto rhs_distance =(self_position - rhs->GetPosition()).Length();
                return lhs_distance < rhs_distance;
            });
            const auto closest_body_position = closest_body->GetPosition();
            auto direction_angle = atan2(closest_body_position.y - self_position.y, closest_body_position.x - self_position.x);
            auto body_angle = pc.body->GetAngle();
            pc.body->SetAngularVelocity(
                (direction_angle
                        - body_angle + M_PI_2));
        }
    }
    velocity.Normalize();
    velocity.x *= speed;
    velocity.y *= speed;
    Utility::rotate(velocity, camera.get_rotation());

    pc.set_velocity(velocity);
    pc.apply_position(character);
    auto box2dposition = pc.get_position();
    camera.set_position(box2dposition.x, box2dposition.y);
    camera.set_rotation(pc.body->GetAngle() * 180 / M_PI);
}

b2World *Player::get_world()
{
    return pc.get_world();
}
