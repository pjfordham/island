#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

float PI = M_PI;
float fcos(float x) { return cos(x); }
float fsin(float x) { return sin(x); }

typedef std::vector<Eigen::Vector2f> VertexList;

float d120 = 2.0f * PI / 3.0f;
float d60 = -PI / 3.0f;
float d1 = 2.0f * PI / 3.0f / 360.0f;

Eigen::Matrix2f rotate120{
   { fcos(d120),-fsin(d120) },
   { fsin(d120), fcos(d120) } };

Eigen::Matrix2f rotate60{
   { fcos(d60),-fsin(d60) },
   { fsin(d60), fcos(d60) } };

Eigen::Matrix2f rotate1{
   { fcos(d1),-fsin(d1) },
   { fsin(d1), fcos(d1) } };

Eigen::Matrix2f flipX{
   { -1,0 },
   { 0,1 } };

Eigen::Matrix2f flipY{
   { 1, 0 },
   { 0, -1  } };

Eigen::Matrix2f zoomIn{
   { 1.1, 0 },
   { 0, 1.1  } };

Eigen::Matrix2f zoomOut{
   { 1/1.1, 0 },
   { 0, 1/1.1  } };

static VertexList initialVertices(int BORDER_SIZE, int WINDOW_SIZE) {

   Eigen::Vector2f one(0,WINDOW_SIZE - BORDER_SIZE);
   Eigen::Vector2f two = rotate120 * one;
   Eigen::Vector2f thr = rotate120 * two;

   return {one, two, thr};
}

static VertexList divideVertices( const VertexList &vertices ) {

   VertexList new_vertices;

   for (auto vertex = vertices.begin();
        vertex != vertices.end();
        vertex++) {

      auto next = (vertex+1 == vertices.end() ) ? vertices.begin() : vertex+1;
      auto delta = (*next - *vertex) * 1.0f / 3.0f;

      auto top =
         Eigen::Translation<float,2>( delta ) *
         rotate60 *
         delta;

      new_vertices.push_back( *vertex );
      new_vertices.push_back( *vertex + delta );
      new_vertices.push_back( *vertex + top );
      new_vertices.push_back( *vertex + delta * 2.0f );
   }
   return new_vertices;
}

int main()
{
   const int WINDOW_SIZE = 800;
   const int BORDER_SIZE = 50;

   VertexList vertices;

   sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Island");

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
                  vertices = initialVertices( BORDER_SIZE, WINDOW_SIZE );
               } else {
                  vertices = divideVertices(vertices);
               }
               std::cout << vertices.size() << " vertices." << std::endl;
            } else {
               auto t = Eigen::Matrix2f();
               // if (event.key.code == sf::Keyboard::Right){
               //    t.translate(Eigen::Vector2f(1,0));
               // } else if (event.key.code == sf::Keyboard::Left ){
               //    t.translate(Eigen::Vector2f(-1,0));
               // } else if (event.key.code == sf::Keyboard::Down ){
               //    t.translate(Eigen::Vector2f(0,1));
               // } else if (event.key.code == sf::Keyboard::Up ){
               //    t.translate(Eigen::Vector2f(0,-1));
               // } else
               if (event.key.code == sf::Keyboard::Add ){
                  t = zoomIn;
               } else if (event.key.code == sf::Keyboard::Subtract ){
                  t = zoomOut;
               } else if (event.key.code == sf::Keyboard::X ){
                  t = flipX;
               } else if (event.key.code == sf::Keyboard::Y ){
                  t = flipY;
               } else if (event.key.code == sf::Keyboard::R ){
                  t = rotate1;
               }
               for ( auto& vertex : vertices ) {
                  vertex = t * vertex;
               }
            }
         }
      }

      sf::VertexArray island(sf::LinesStrip, 0);
      for ( auto vertex : vertices ) {
         island.append(sf::Vector2f(vertex(0),vertex(1)));
      }
      if ( vertices.begin() != vertices.end() ) {
         auto &v =*vertices.begin();
         island.append( sf::Vector2f( v(0), v(1)) );
      }

      window.clear( sf::Color::Black );

      sf::RectangleShape origin;
      origin.setSize(sf::Vector2f(20, 20));
      origin.setPosition(-10, -10);

      window.setView( sf::View(sf::FloatRect(-WINDOW_SIZE, -WINDOW_SIZE,
					     2*WINDOW_SIZE, 2*WINDOW_SIZE)));
      window.draw( origin );
      window.draw( island );
      window.display();
   }
   return 0;
}
