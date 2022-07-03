#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

float PI = M_PI;
float fcos(float x) { return cos(x); }
float fsin(float x) { return sin(x); }

typedef std::vector<Eigen::Vector3f> VertexList;

Eigen::Matrix3f rotate(float v) {
   const float d1 = 2.0f * PI / 360.0f;
   float rads = v * d1;
   Eigen::Matrix3f x{
      { fcos(rads),-fsin(rads),0 },
      { fsin(rads), fcos(rads),0 },
      {0,0,1}
   };
   return x;
}

Eigen::Matrix3f flipX{
   { -1,0,0 },
   { 0,1,0 },
   {0,0,1} };

Eigen::Matrix3f flipY{
   { 1, 0,0 },
   { 0, -1,0  },
   {0,0,1} };

Eigen::Matrix3f translate(Eigen::Vector3f v) {
   Eigen::Matrix3f x{
      { 1,0,v(0) },
      { 0,1,v(1) },
      { 0,0,1 } };
   return x;
}

Eigen::Matrix3f scale(float v) {
   Eigen::Matrix3f x{
      { v,0,0 } ,
      { 0,v,0 },
      { 0,0,1 } };
   return x;
}

static VertexList initialVertices(int BORDER_SIZE, int WINDOW_SIZE) {

   Eigen::Vector3f one(0,WINDOW_SIZE - BORDER_SIZE, 1);
   Eigen::Vector3f two = rotate(120.0) * one;
   Eigen::Vector3f thr = rotate(120.0) * two;

   return {one, two, thr};
}

static VertexList divideVertices( const VertexList &vertices ) {

   VertexList new_vertices;

   for (auto vertex = vertices.begin();
	vertex != vertices.end();
	vertex++) {

      auto next = (vertex+1 == vertices.end() ) ? vertices.begin() : vertex+1;
      Eigen::Vector3f delta = (*next - *vertex) * 1.0f / 3.0f;

      // We need this to be 1 for translations to work
      delta[2] = 1;

      // dont' use auto here since the lazy evaluator does something weird.
      Eigen::Vector3f top =
	 translate( delta ) *
	 rotate(-60.0) *
	 delta;

      new_vertices.push_back( *vertex );
      new_vertices.push_back( translate( delta ) * *vertex );
      new_vertices.push_back( translate( top ) * *vertex );
      new_vertices.push_back( translate( scale(2.0) * delta ) * *vertex );
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
	       Eigen::Matrix3f t = Eigen::Matrix3f::Identity();
	       if (event.key.code == sf::Keyboard::Right){
		  t = translate(Eigen::Vector3f(1,0,1));
	       } else if (event.key.code == sf::Keyboard::Left ){
		  t = translate(Eigen::Vector3f(-1,0,1));
	       } else if (event.key.code == sf::Keyboard::Down ){
		  t = translate(Eigen::Vector3f(0,1,1));
	       } else if (event.key.code == sf::Keyboard::Up ){
		  t = translate(Eigen::Vector3f(0,-1,1));
	       } else if (event.key.code == sf::Keyboard::Add ){
		  t = scale(1.1);
	       } else if (event.key.code == sf::Keyboard::Subtract ){
		  t = scale(1.0/1.1);
	       } else if (event.key.code == sf::Keyboard::X ){
		  t = flipX;
	       } else if (event.key.code == sf::Keyboard::Y ){
		  t = flipY;
	       } else if (event.key.code == sf::Keyboard::R ){
		  t = rotate(1.0);
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
