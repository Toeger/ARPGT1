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
    void rotate(float turningangle);
    void set_size(int x, int y);
    float get_rotation() const;
    ~Camera();
private:
    sf::RenderWindow *window;
    sf::View view;
};

#endif // CAMERA_H
