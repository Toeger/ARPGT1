#include "weapon.h"
#include "utility.h"
#include "drawlist.h"
#include "logicalobject.h"

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
    LogicalObject::add(*this);
    weaponShape.Set(points.data(), points.size());
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &weaponShape;
    fixtureDef.isSensor = true;
    fixture = body->CreateFixture(&fixtureDef);
}

void Weapon::logicalUpdate()
{
    auto body = fixture->GetBody();
    //auto pos = body->GetPosition();
    auto polyShape = dynamic_cast<b2PolygonShape *>(fixture->GetShape());
    assert(polyShape);
    for (int i = 0; i < polyShape->GetVertexCount(); ++i){
        const auto &pos = polyShape->GetVertex(i);
        drawShape.setPoint(i, {pos.x, pos.y});

    }
    //drawShape.setPosition({pos.x, pos.y});
    drawShape.setRotation(body->GetAngle());
}
