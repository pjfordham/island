#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <deque>


std::uniform_int_distribution<int> randomLocationRange(0, 150);
std::random_device rd;
std::mt19937 randomNumbers(rd());

typedef std::deque<sf::Vector2f> VertexList;

VertexList initialVertices(int BORDER_SIZE, int WINDOW_SIZE, int height, int base) {
   VertexList vertices;
   auto one = sf::Vector2f((float)BORDER_SIZE, height + (float)BORDER_SIZE);
   auto two = sf::Vector2f(base + (float)BORDER_SIZE,  height + (float)BORDER_SIZE);
   auto thr = sf::Vector2f((float)WINDOW_SIZE/2.0, height*4 + (float)BORDER_SIZE);
   vertices.push_back(one);
   vertices.push_back(two);
   vertices.push_back(thr);
   return vertices;
}

const float ONE_THIRD = 1.f / 3.0f;
const float TWO_THIRDS = 2.0f / 3.0f;
const float ONE_HALF = 1.0f / 2.0f;

VertexList divideVertices( const VertexList &vertices, bool random ) {

   VertexList new_vertices;

   auto vertex = vertices.begin();
   while ( vertex != vertices.end() ) {

      auto next = (vertex+1 == vertices.end() ) ? vertices.begin() : vertex+1;

      auto delta = *next - *vertex;

      sf::Transform two;
      two.translate( delta * ONE_THIRD );

      int i;
      if ( random ) {
         i = 75 - randomLocationRange( randomNumbers );
      } else {
         i = 100;
      }

      sf::Transform three;
      auto perp = sf::Transform().rotate(-90).transformPoint( delta * ONE_THIRD * ( (float)sqrt(3) / 2.0f ) );
      three.translate( delta * ONE_HALF );
      three.translate( perp );

      sf::Transform four;
      four.translate( delta * TWO_THIRDS );

      new_vertices.push_back(  *vertex );
      new_vertices.push_back( two.transformPoint( *vertex ));
      new_vertices.push_back( three.transformPoint( *vertex ));
      new_vertices.push_back( four.transformPoint( *vertex ));
      vertex++;
   }
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
            } else if (event.key.code == sf::Keyboard::Space){
               if (vertices.size() > 1000000 || vertices.empty() ) {
                  vertices = initialVertices( BORDER_SIZE, WINDOW_SIZE, height, base);
               } else {
                  vertices = divideVertices(vertices, random);
               }
               std::cout << vertices.size() << " vertices." << std::endl;
            } else {
               sf::Transform t;
               if (event.key.code == sf::Keyboard::Right){
                  t.translate(1,0);
               }
               if (event.key.code == sf::Keyboard::Left ){
                  t.translate(-1,0);
               }
               if (event.key.code == sf::Keyboard::Down ){
                  t.translate(0,1);
               }
               if (event.key.code == sf::Keyboard::Up ){
                  t.translate(0,-1);
               }
               if (event.key.code == sf::Keyboard::Add ){
                  t.scale(1.1,1.1);
               }
               if (event.key.code == sf::Keyboard::Subtract ){
                  t.scale(1/1.1,1/1.1);
               }
               if (event.key.code == sf::Keyboard::X ){
                  t.scale(1,-1);
               }
               if (event.key.code == sf::Keyboard::Y ){
                  t.scale(-1,1);
               }
               if (event.key.code == sf::Keyboard::R ){
                  t.rotate(1);
               }
               for ( auto& vertex : vertices ) {
                  vertex = t.transformPoint( vertex );
               }
               // if (event.key.code == sf::Keyboard::R){
               //    random = !random;
               // }
            }
         }
      }

      sf::VertexArray island(sf::LinesStrip, 0);
      for ( auto vertex : vertices ) {
         island.append(vertex);
      }
      island.append( *vertices.begin() );

      window.clear( sf::Color::Black );

      sf::RectangleShape origin;
      origin.setSize(sf::Vector2f(20, 20));
      origin.setPosition(-10, -10);

      window.setView( sf::View(sf::FloatRect(- WINDOW_SIZE, -4 * height + 2 * BORDER_SIZE, 2*WINDOW_SIZE, 2*4 * height + 2 * BORDER_SIZE)));
      window.draw( origin );
      window.draw( island );
      window.display();
   }
   return 0;
}
