#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(sf::RenderWindow *window);
    void set_position(float x, float y);
    void set_rotation(float angle);
    void face(float x, float y);
    void rotate_left();
    void rotate_right();
    float get_rotation() const;
    ~Camera();
private:
    sf::RenderWindow *window;
    sf::View view;
};

#endif // CAMERA_H
