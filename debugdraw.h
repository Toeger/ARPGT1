#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>
//#include <Box2D/Dynamics/
#include <SFML/Graphics.hpp>

#define PIXELS_METER /*15.f*/1.f

struct b2AABB;

class DebugDraw : public b2Draw
{
    sf::RenderTarget &target;

public:
    DebugDraw(sf::RenderTarget &target);
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color) override;
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color &color) override;
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color &color) override;
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color &color) override;
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
    void DrawTransform(const b2Transform& xf) override;
};

#endif // DEBUGDRAW_H
