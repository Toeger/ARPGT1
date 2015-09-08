#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <Box2D/Box2D.h>
//#include <Box2D/Dynamics/
#include <SFML/Graphics.hpp>

#define PIXELS_METER /*15.f*/35.f

struct b2AABB;

class DebugDraw : public b2Draw
{
    sf::RenderTarget &Target;

public:
    DebugDraw(sf::RenderTarget &target);
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
};

#endif // DEBUGDRAW_H
