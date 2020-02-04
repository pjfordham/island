#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <list>

std::uniform_int_distribution<int> randomLocationRange(0, 150);
std::random_device rd;
std::mt19937 randomNumbers(rd());

typedef std::list<sf::Vector2f> VertexList;

VertexList initialVertices(int BORDER_SIZE, int WINDOW_SIZE, int height, int base) {
   VertexList vertices;
   vertices.emplace_back((float)BORDER_SIZE, height + (float)BORDER_SIZE);
   vertices.emplace_back(base + (float)BORDER_SIZE,  height + (float)BORDER_SIZE);
   vertices.emplace_back((float)WINDOW_SIZE/2.0, height*4 + (float)BORDER_SIZE);
   vertices.emplace_back((float)BORDER_SIZE,  height + (float)BORDER_SIZE);
   return vertices;
}

VertexList divideVertices( const VertexList &vertices, bool random ) {

   VertexList new_vertices;

   auto vertex = vertices.begin();
   auto last = vertex;
   vertex++;
   while ( vertex != vertices.end() ) {
      float zero = last->x * 1.0 / 3.0 + vertex->x * 2.0 / 3.0;
      float one = last->y * 1.0 / 3.0 + vertex->y * 2.0 / 3.0;

      // Perp vector
      float deltay = (vertex->x - last->x) * 1.0 / 3.0 * sqrt(3) / 2.0;
      float deltax = (last->y - vertex->y) * 1.0 / 3.0 * sqrt(3) / 2.0;

      int i;
      if ( random ) {
         i = 75 - randomLocationRange( randomNumbers );
      } else {
         i = 100;
      }

      float two = last->x * 1.0 / 2.0 + vertex->x * 1.0 / 2.0 - deltax * ( i * 0.01 );
      float three = last->y * 1.0 / 2.0 + vertex->y * 1.0 / 2.0 - deltay * ( i * 0.01 );

      float four = last->x * 2.0 / 3.0 + vertex->x * 1.0 / 3.0;
      float five = last->y * 2.0 / 3.0 + vertex->y * 1.0 / 3.0;
      new_vertices.emplace_back( *last );
      new_vertices.emplace_back(four, five);
      new_vertices.emplace_back(two, three);
      new_vertices.emplace_back(zero, one);
      last = vertex;
      vertex++;
   }
   new_vertices.emplace_back( *last );
   return new_vertices;
}

int main()
{
   const int WINDOW_SIZE = 800;
   const int BORDER_SIZE = 20;

   float base = WINDOW_SIZE - 2 * BORDER_SIZE;
   float height = base * sqrt(3) / 2.0 / 3.0;

   VertexList vertices;

   bool random = false;

   sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, 4 * height + 2 * BORDER_SIZE), "Island");

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed)
            window.close();
         // Respond to key pressed events
         if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::Escape){
               return 0;
            }
            if (event.key.code == sf::Keyboard::Space){
               if (vertices.size() > 1000000 || vertices.empty() ) {
                  vertices = initialVertices( BORDER_SIZE, WINDOW_SIZE, height, base);
               } else {
                  vertices = divideVertices(vertices, random);
               }
               std::cout << vertices.size() << " vertexes." << std::endl;
           }
            if (event.key.code == sf::Keyboard::R){
               random = !random;
            }
         }
      }

      sf::VertexArray island(sf::LinesStrip, 0);
      for ( auto vertex : vertices ) {
         island.append(vertex);
      }

      window.clear( sf::Color::Black );
      window.draw( island );
      window.display();
   }
   return 0;
}
