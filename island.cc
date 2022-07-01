#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

float PI = M_PI;

typedef std::vector<Eigen::Vector2f> VertexList;

static VertexList initialVertices(int BORDER_SIZE, int WINDOW_SIZE) {

   Eigen::Vector2f one(0,WINDOW_SIZE - BORDER_SIZE);
   Eigen::Vector2f two = Eigen::Rotation2D<float>( 2.0f * PI / 3.0f) * one;
   Eigen::Vector2f thr = Eigen::Rotation2D<float>( 2.0f * PI / 3.0f) * two;

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
         Eigen::Rotation2D<float>( - 2.0f * PI / 6.0f) *
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
               auto t = Eigen::Transform<float,2,Eigen::Affine>::Identity();
               if (event.key.code == sf::Keyboard::Right){
                  t.translate(Eigen::Vector2f(1,0));
               } else if (event.key.code == sf::Keyboard::Left ){
                  t.translate(Eigen::Vector2f(-1,0));
               } else if (event.key.code == sf::Keyboard::Down ){
                  t.translate(Eigen::Vector2f(0,1));
               } else if (event.key.code == sf::Keyboard::Up ){
                  t.translate(Eigen::Vector2f(0,-1));
               } else if (event.key.code == sf::Keyboard::Add ){
                  t.scale(1.1);
               } else if (event.key.code == sf::Keyboard::Subtract ){
                  t.scale(1/1.1);
               } else if (event.key.code == sf::Keyboard::X ){
                  t.scale(Eigen::Vector2f(1,-1));
               } else if (event.key.code == sf::Keyboard::Y ){
                  t.scale(Eigen::Vector2f(-1,1));
               } else if (event.key.code == sf::Keyboard::R ){
                  t.rotate( 2.0f * PI / 360.0f);
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
