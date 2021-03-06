#include "../include/global.h"
#include "../include/Asteroids.h"
#include "../include/Draw.h"
#include "../include/Model.h"
#include "../include/Ship.h"
#include "../include/assistant.h"
#include "../include/bullet.h"
#include "../include/const.h"
#include "../include/funcs.h"
#include "../include/interface.h"
#include "../include/menu.h"
#include <SFML/Audio.hpp>

int main() {
  srand(time(NULL));
  //счет
  int count = 0;
  //изначальное количество землян.
  long long int earthlings = 10000000000;
  long long int survived = 0;
  //хранение времени перезапуска способности
  float restarting_time = 0;
  //время, за которое способность будет длиться
  float red_time = -1;
  //готова ли способность красного корабля к использованию
  bool ability_red = false;
  //используется ли в данный момент способность или нет
  bool ability_red_use = false;
  bool menu_running = false;
  int cause_of_death = 0;
  //отслеживает нажатие клавиши отв за способность
  bool ability_blue = false;
  //кд способности
  float ability_time = 0;
  //максимальное колво астероидов
  int NMAX = 3;

  Clock clock;
  sf::Draw draw_obj;

  sf::Music music;
  if (!music.openFromFile("etc/main_theme.ogg")) {
    return -1;
  }
  music.play();
  music.setLoop(true);

  Model map("background.png");
  //изначальная позиция кораблей
  Ship ship(start_x_blue, start_ship_y, "pl1.png", Keyboard::Left,
            Keyboard::Right, shipBlueW);
  Ship ship_2(start_x_red, start_ship_y, "pl2.png", Keyboard::A, Keyboard::D,
              shipRedW);

  Asteroid *asteroid[NMAX];
  for (int i = 0; i < NMAX; i++) {
    asteroid[i] = new Asteroid((float)rand() / RAND_MAX * windowWidth - 80, 0,
                               "asteroid1.png");
  }

  Assistant assist(start_x_assist, start_y_assist, "assistant.png",
                   Keyboard::Space);
  Bullet bullet(assist.x() + ship_assist_width, assist.y(),
                "bullet_assist.png");
  Bullet bullet_2(ship_2.x() + ship_red_width, ship_2.y(), "bullet_red.png");
  Bullet bullet_3(ship.x() + ship_blue_width, ship.y(), "bullet.png");

  //открытие окна
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Asteroids",
                          sf::Style::Close);
  window.setFramerateLimit(60);
  //программа работает, пока окно открыто
  while (window.isOpen()) {
    sf::Event event;

    float time = clock.getElapsedTime().asSeconds();
    clock.restart();

    while (window.pollEvent(event)) {
      //открываем меню по нажатию Esc
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Escape:
          menu_running = true;
          menu(window, menu_running, restarting_time, red_time);
          break;
        case sf::Keyboard::Space:
          if (ability_red) {
            ability_red_use = true;
            ability_red = false;
          }
          break;
        case sf::Keyboard::Return:
          if (ability_time >= 8) {
            ability_blue = true;
          }
          break;
        default:
          break;
        }
      }
    }

    window.clear();
    window.draw(map.model_sprite);
    interface(window, ship, ship_2, count, earthlings, survived, ability_red,
              ability_time);
    if (is_it_the_end(ship.destroyed, ship_2.destroyed, earthlings, survived)) {
      if (ship.destroyed && ship_2.destroyed) {
        cause_of_death = 0;
      }
      if (earthlings < min_survivors) {
        if (survived > max_survivors / 2) {
          cause_of_death = 2;
        } else {
          return 1;
        }
      }
      game_over(window, cause_of_death);
    } else {
      survived += 1000000;
      earthlings -= 1000000;
      max_asteroids(survived, NMAX);
      if (!ship.destroyed) {
        ship.update();
        for (int i = 0; i < NMAX; i++) {
          Collision(ship, *asteroid[i], count);
        }
        if (ability_time >= 8) {
          ability_blue = ship_ability(ability_blue, ship.ship_health,
                                      ship_2.ship_health, ability_time);
        } else {
          ability_time += time;
        }
      }
      draw_obj.Draw_object(window, bullet_3.destroyed, bullet_3.model_sprite);
      bullet_3.update();
      bullet_3.update();
      draw_obj.Bullet_position(ship.x() + ship_blue_width, ship.y(), bullet_3,
                               ship.destroyed);
      for (int i = 0; i < NMAX; i++) {
        Collision(bullet_3, *asteroid[i], count);
      }
      if (!ship_2.destroyed) {
        ship_2.update();
        for (int i = 0; i < NMAX; i++) {
          Collision(ship_2, *asteroid[i], count);
        }

        restarting_time += time;
        if (restarting_time >= red_restart && red_time != red_running) {
          ability_red = true;
          restarting_time = 0;
          if (red_time < red_running) {
            red_time = red_running;
          }
        }
        if (ability_red_use) {
          if (red_time >= 0) {
            assist.update();
            bullet.update();
            bullet.update();
            bullet.update();
            bullet.draw(window);
            window.draw(assist.model_sprite);
            for (int i = 0; i < NMAX; i++) {
              Collision(bullet, *asteroid[i], count);
            }
            red_time -= time;
            draw_obj.Bullet_position(assist.x() + ship_assist_width, assist.y(),
                                     bullet, assist.destroyed);
          } else
            ability_red_use = false;
        }
      }
      draw_obj.Draw_object(window, bullet_2.destroyed, bullet_2.model_sprite);
      bullet_2.update();
      bullet_2.update();
      bullet_2.update();
      draw_obj.Bullet_position(ship_2.x() + ship_red_width, ship_2.y(),
                               bullet_2, ship_2.destroyed);
      draw_obj.Draw_object(window, ship.destroyed, ship.model_sprite);
      draw_obj.Draw_object(window, ship_2.destroyed, ship_2.model_sprite);

      for (int i = 0; i < NMAX; i++) {
        Collision(bullet_2, *asteroid[i], count);
        asteroid[i]->update();
        if (asteroid[i]->y() >= windowHeight) {
          earthlings -= 1000000;
          survived -= 10000000;
        }
        if (asteroid[i]->destroyed) {
          asteroid[i]->model_sprite.setPosition(
              (float)rand() / RAND_MAX * windowWidth, 0);
          asteroid[i]->destroyed = false;
        }
        draw_obj.Draw_object(window, asteroid[i]->destroyed,
                             asteroid[i]->model_sprite);
      }
    }
    //отрисовка окна
    window.display();
  }

  return 0;
}
