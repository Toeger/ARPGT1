#include "weapon.h"
#include "utility.h"
#include "drawlist.h"

Weapon::Weapon(b2Body *body)
{
    b2PolygonShape weaponShape;
    const int sword_size = 50;
    std::array<b2Vec2, 3> points
    {
        b2Vec2{0, 0},
        {-sword_size, -2 * sword_size},
        {sword_size, -2 * sword_size},
    };
    const auto angle = body->GetAngle();
    //const auto pos = body->GetPosition();
    drawShape.setPointCount(points.size());
    int index = 0;
    for (auto &p : points){
        //rotate by body angle
        Utility::rotate(p, angle);
        //move to body position
        //p += pos;
        drawShape.setPoint(index++, {p.x, p.y});
    }
    drawShape.setOrigin({points[0].x, points[0].y});
    drawShape.setFillColor({50, 0, 0});
    Drawlist::add(drawShape);
    weaponShape.Set(points.data(), points.size());
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &weaponShape;
    fixtureDef.isSensor = true;
    fixture = body->CreateFixture(&fixtureDef);
}

