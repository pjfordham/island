#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <deque>


std::uniform_int_distribution<int> randomLocationRange(0, 150);
std::random_device rd;
std::mt19937 randomNumbers(rd());

typedef std::deque<sf::Vector2f> VertexList;

template <int a, int b, int c, int d>
sf::Vector2f transform( sf::Vector2f in) {
   return sf::Vector2f( a * in.x + b * in.y , c*in.x + d*in.y);
}
sf::Vector2f translate( sf::Vector2f in, sf::Vector2f in2) {
   return sf::Vector2f( in.x + in2.x , in.y + in2.y);
}
sf::Vector2f scale( sf::Vector2f in, float z) {
   return sf::Vector2f( in.x * z , in.y *z);
}

auto I(sf::Vector2f x) { return transform < 1, 0, 0, 1>(x); }
auto Scale2(sf::Vector2f x) { return transform < 2, 0, 0, 2>(x); }
auto FlipY(sf::Vector2f x) { return transform < -1, 0, 0, 1>(x); }
auto FlipX(sf::Vector2f x) { return transform < 1, 0, 0, -1>(x); }



VertexList initialVertices(int BORDER_SIZE, int WINDOW_SIZE, int height, int base) {
   VertexList vertices;
   auto one = I(sf::Vector2f((float)BORDER_SIZE, height + (float)BORDER_SIZE));
   auto two = I(sf::Vector2f(base + (float)BORDER_SIZE,  height + (float)BORDER_SIZE));
   auto thr = I(sf::Vector2f((float)WINDOW_SIZE/2.0, height*4 + (float)BORDER_SIZE));
   vertices.push_back(one);
   vertices.push_back(two);
   vertices.push_back(thr);
   return vertices;
}

VertexList divideVertices( const VertexList &vertices, bool random ) {

   VertexList new_vertices;

   auto vertex = vertices.begin();
   auto last = vertices.end() - 1;
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
            if (event.key.code == sf::Keyboard::X ){
               for ( auto& vertex : vertices ) {
                  vertex = FlipX( vertex );
               }
            }
            if (event.key.code == sf::Keyboard::A ){
               sf::Transform t;
               t.scale(1.1,1.1);
               for ( auto& vertex : vertices ) {
                  vertex = t.transformPoint( vertex );
               }
            }
              if (event.key.code == sf::Keyboard::Z ){
               sf::Transform t;
               t.scale(1/1.1,1/1.1);
               for ( auto& vertex : vertices ) {
                  vertex = t.transformPoint( vertex );
               }
            }
            if (event.key.code == sf::Keyboard::R ){
               sf::Transform t;
               t.rotate(1);
               for ( auto& vertex : vertices ) {
                  vertex = t.transformPoint( vertex );
               }
            }
          if (event.key.code == sf::Keyboard::Y ){
               for ( auto& vertex : vertices ) {
                  vertex = FlipY( vertex );
               }
            }
            // if (event.key.code == sf::Keyboard::R){
            //    random = !random;
            // }
         }
      }

      sf::VertexArray island(sf::LinesStrip, 0);
      for ( auto vertex : vertices ) {
         island.append(vertex);
      }
      island.append( *vertices.begin() );

      window.clear( sf::Color::Black );

      window.setView(   sf::View(sf::FloatRect(- WINDOW_SIZE, -4 * height + 2 * BORDER_SIZE, 2*WINDOW_SIZE, 2*4 * height + 2 * BORDER_SIZE)));
      window.draw( sf::RectangleShape(  sf::Vector2f(20, 20)));
      window.draw( island );
      window.display();
   }
   return 0;
}
