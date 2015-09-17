#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <array>
#include <typeinfo>
#include <Box2D/Box2D.h>

#include "player.h"
#include "practicedummy.h"
#include "entity.h"
#include "components.h"
#include "utility.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
    auto &now =  std::chrono::high_resolution_clock::now;
    auto last_update_timepoint = now();
    const auto lfps = 30;
    const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
    using namespace std::chrono_literals;

    window.setVerticalSyncEnabled(true);

    b2World world({0, 0});

    Player p(&window);
    {

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(10, 10);
        auto body = world.CreateBody(&bodyDef);
        b2CircleShape characterShape;
        characterShape.m_radius = 50 / 100.f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &characterShape;
        fixtureDef.density = 1.f;
        fixtureDef.friction = 0;
        body->CreateFixture(&fixtureDef);
        body->SetLinearDamping(10.f);

        p.add(body);
    }
    p.camera.rotate(0);
    PracticeDummy pd;
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(11, 10);
        auto body = world.CreateBody(&bodyDef);
        b2CircleShape characterShape;
        characterShape.m_radius = 40 / 100.f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &characterShape;
        fixtureDef.density = 1.f;
        fixtureDef.friction = 0;
        body->CreateFixture(&fixtureDef);
        body->SetLinearDamping(5.0f);

        pd.add(body);
        sf::CircleShape cs(characterShape.m_radius * 100);
        cs.setOutlineColor({100, 200, 200});
        pd.add(cs);
    }
    Entity background_picture;
    {
        sf::Sprite sprite;
        background_picture.add(sf::Texture());
        auto &image = *background_picture.get<sf::Texture>();
        sprite.setPosition(-1000000, -1000000);
        image.loadFromFile("grass3.png");
        sprite.setTexture(image);
        image.setRepeated(true);
        image.setSmooth(true);
        sprite.setTextureRect({0, 0, 2000000, 2000000});
        background_picture.add(sprite);
    }

    while (window.isOpen()){
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                //auto zoom = p.camera.get_zoom();
                p.camera.set_size(event.size.width, event.size.height);
                //p.camera.set_zoom(zoom);
                //window.setSize({event.size.width, event.size.height});
            if (event.type == sf::Event::MouseWheelMoved){
                const auto zoom_factor = 0.1f;
                p.camera.set_zoom(pow(1 - zoom_factor, event.mouseWheel.delta));
            }
        }
        //UpdateList::updateAll();
        while (now() - last_update_timepoint > logical_frame_duration)
        {
            last_update_timepoint += logical_frame_duration;
            auto &player_body = *p.get<Components::Physical_shape>();
            //setting player direction and velocity
            {
                b2Vec2 vel{0, 0};
                if (window.hasFocus()){
                    const auto camera_turning_speed = 5.f; //in degree
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                        vel.y--;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                        vel.x--;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                        vel.y++;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                        vel.x++;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                        p.camera.rotate(-camera_turning_speed);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
                        p.camera.rotate(camera_turning_speed);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
                        const auto &dummy_body = *pd.get<Components::Physical_shape>();
                        const auto &dummy_pos = dummy_body->GetPosition();
                        const auto &player_pos = player_body->GetPosition();
                        p.camera.face(dummy_pos.x - player_pos.x, dummy_pos.y - player_pos.y);
                    }
                    vel.Normalize();
                    vel *= 5;
                    Utility::rotate(vel.x, vel.y, p.camera.get_rotation() * M_PI / 180);
                    //player_body->ApplyForce(vel, player_body->GetPosition(), true);
                }
                player_body->SetLinearVelocity(vel);
            }
            //resolve physics
            world.Step(1/30.f, 6, 2);
        }
        window.clear(sf::Color::Black);
        //rendering system
        for (auto sit = System::begin<sf::Sprite>(); sit; sit.advance()){
            window.draw(sit.get<sf::Sprite>());
        }
        auto &player_body = *p.get<Components::Physical_shape>();
        const auto &pos = player_body->GetPosition();
        p.camera.set_position(pos.x * 100, pos.y * 100);
        for (auto sit = System::begin<Components::CircleShape, Components::Physical_shape>(); sit; sit.advance()){
            const auto &pos = sit.get<Components::Physical_shape>()->GetPosition();
            auto &cs = sit.get<Components::CircleShape>();
            auto sfmlpos = Utility::b2s_coords(pos);
            sfmlpos.x -= cs.getRadius();
            sfmlpos.y -= cs.getRadius();
            cs.setPosition(sfmlpos);
            window.draw(cs);
        }
        window.display();
    }
}
