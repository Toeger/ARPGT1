#include "drawlist.h"
#include <vector>
#include <algorithm>
#include <iterator>

std::vector<sf::Shape *> shapes;
void Drawlist::add(sf::Shape &s){
    shapes.push_back(&s);
}

void Drawlist::remove(sf::Shape &s){
    using namespace std;
    shapes.erase(find(begin(shapes), end(shapes), &s));
}


void Drawlist::drawAll(sf::RenderWindow &window)
{
    for (auto &s : shapes){
        window.draw(*s);
    }
}
