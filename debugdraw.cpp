#include "debugdraw.h"

DebugDraw::DebugDraw(sf::RenderTarget &target)
    : Target(target)
{
    SetFlags(e_shapeBit | e_jointBit | e_centerOfMassBit /*| e_aabbBit*/);
    //SetFlags(0xFFFF);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount)
{
    sf::ConvexShape Shape;
    Shape.setOutlineThickness(1.f);
    Shape.setPointCount(vertexCount);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        Shape.setPoint(i, {vertices[i].x*PIXELS_METER, vertices[i].y*PIXELS_METER});
    }
    target.Draw(Shape);
}
void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::Shape Shape;
    Shape.SetOutlineWidth(1.f);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        Shape.AddPoint(vertices[i].x*PIXELS_METER, vertices[i].y*PIXELS_METER, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), sf::Color(color.r*255, color.g*255, color.b*255));
    }
    target.Draw(Shape);
}
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    sf::Shape Shape = sf::Shape::Circle(center.x*PIXELS_METER, center.y*PIXELS_METER, radius*PIXELS_METER, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), 1.f, sf::Color(color.r*255, color.g*255, color.b*255));
    target.Draw(Shape);
}
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    sf::Shape Shape = sf::Shape::Circle(0.f, 0.f, radius*PIXELS_METER, sf::Color(128*color.r, 128*color.g, 128*color.b, 128), 1.f, sf::Color(color.r*255, color.g*255, color.b*255));
    Shape.SetPosition(center.x*PIXELS_METER, center.y*PIXELS_METER);
    target.Draw(Shape);
}
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    sf::Shape Line = sf::Shape::Line(p1.x*PIXELS_METER, p1.y*PIXELS_METER, p2.x*PIXELS_METER, p2.y*PIXELS_METER, 1.f, sf::Color(color.r*255, color.g*255, color.b*255));
    target.Draw(Line);
}
void DebugDraw::DrawTransform(const b2Transform& xf)
{
    //Log("Debug draw: Transform");
    //std::cout << "Drawing transform" << std::endl;
    /*b2Vec2 p1 = xf.position, p2;
    const float32 k_axisScale = 0.4f;
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(p1.x, p1.y);
    p2 = p1 + k_axisScale * xf.R.col1;
    glVertex2f(p2.x, p2.y);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(p1.x, p1.y);
    p2 = p1 + k_axisScale * xf.R.col2;
    glVertex2f(p2.x, p2.y);

    glEnd();*/
}
void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    target.Draw(sf::Shape::Circle(p.x*PIXELS_METER, p.y*PIXELS_METER, size, sf::Color(color.r*255, color.g*255, color.b*255)));
}
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
    target.Draw(sf::Shape::Rectangle(aabb->lowerBound.x*PIXELS_METER, aabb->lowerBound.y*PIXELS_METER, (aabb->lowerBound.x+aabb->upperBound.x)*PIXELS_METER, (aabb->lowerBound.y+aabb->upperBound.y)*PIXELS_METER, sf::Color(color.r*255, color.g*255, color.b*255)));
}
