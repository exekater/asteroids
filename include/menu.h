#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <sstream>

using namespace std;
using namespace sf;

bool menu(RenderWindow &window, bool &running, float &restarting_time,
          float &red_time);

#endif
