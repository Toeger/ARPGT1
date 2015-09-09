#include "debugdraw.h"

#include <array>


DebugDraw::DebugDraw(sf::RenderTarget &target)
    : target(target)
{
    //SetFlags(e_shapeBit | e_jointBit | e_centerOfMassBit /*| e_aabbBit*/);
    //SetFlags(0xFFFF);
    SetFlags(b2Draw::e_shapeBit);
}

sf::Color b2ToSfColor(const b2Color &color){
    auto get_color = [](float f){ return static_cast<unsigned char>(f * 255);};

    return {get_color(color.r), get_color(color.g), get_color(color.b), get_color(color.a)};
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color)
{
    sf::ConvexShape Shape;
    Shape.setOutlineColor(b2ToSfColor(color));
    Shape.setOutlineThickness(1.f);
    Shape.setPointCount(vertexCount);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        Shape.setPoint(i, {vertices[i].x*PIXELS_METER, vertices[i].y*PIXELS_METER});
    }
    target.draw(Shape);
}
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color)
{
    sf::ConvexShape Shape;
    Shape.setFillColor(b2ToSfColor(color));
    Shape.setOutlineThickness(1.f);
    Shape.setPointCount(vertexCount);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        Shape.setPoint(i, {vertices[i].x*PIXELS_METER, vertices[i].y*PIXELS_METER});
    }
    target.draw(Shape);
}
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color &color)
{
    auto Shape = sf::CircleShape(radius*PIXELS_METER);
    Shape.setPosition({center.x*PIXELS_METER - radius, center.y*PIXELS_METER - radius});
    Shape.setOutlineColor(b2ToSfColor(color));
    target.draw(Shape);
}
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
    //TODO: what is axis for?
    (void)axis;
    auto Shape = sf::CircleShape(radius*PIXELS_METER);
    Shape.setPosition({center.x*PIXELS_METER - radius, center.y*PIXELS_METER - radius});
    Shape.setFillColor(b2ToSfColor(color));
    target.draw(Shape);
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    std::array<b2Vec2, 4> points{
        p1,
        {p1.x+1, p1.y+1},
        {p2.x+1, p2.y+1},
        p2
    };
    DrawPolygon(points.data(), points.size(), color);
}

void DebugDraw::DrawTransform(const b2Transform &xf)
{
    (void)xf;
}
