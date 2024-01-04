//Dec 18 2023
#include <SFML/Graphics.hpp>
#include <iostream>;
#include <math.h>
# define M_PI 3.14159265358979323846

 float PlayerRotate=90,Speed=.055;
float MouX,MouY,PlayerXDirection,PlayerYDirection;
int GridSize = 64 ,RayX,RayY;
sf::VertexArray Fan(sf::TriangleFan, 136);
sf::CircleShape Point(9);
sf::Vector2f PlayerXY;
sf::RenderWindow window;
sf::Event event;
sf::Mouse Mouse1;
sf::Keyboard Keyboard1;
sf::Color ColorArray[]={sf::Color(0,66,0),sf::Color(155,155,155),sf::Color(0,66,0)};
sf::ConvexShape PlayerShape;

int TileArray[]={
1,1,1,1,1,1,1,1,1,1,
1,0,0,0,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,1,0,1,
1,0,0,0,2,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,1,
1,0,1,0,0,0,1,0,0,1,
1,0,1,0,0,0,0,0,0,1,
1,0,1,0,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,1
};

template <typename T>
T DegToRad(T degrees)
{
    return(T)(degrees * (M_PI/180));
}

void MoveTo(float x,float y, int x1=0,int y1=0)
{
x1= x + PlayerXY.x; x1/=GridSize;
y1= PlayerXY.y;     y1/=GridSize;
if (TileArray[(10 * y1 + x1)] != 1)
        {PlayerXY.x+=x;
        }

x1= PlayerXY.x;     x1/=GridSize;
y1= y + PlayerXY.y; y1/= GridSize;

if (TileArray[(10 * y1 + x1)] != 1)
        {PlayerXY.y+=y;
        }
};

void RayIntersect()
{
    sf::Vector2f RayLength, RayStep, RayStart = sf::Vector2f(PlayerXY.x / GridSize, PlayerXY.y / GridSize);

    sf::Vector2i MapCheck = sf::Vector2i(RayStart);
     sf::Vector2f  RayDirection=sf::Vector2f(PlayerXDirection,PlayerYDirection);
   sf::Vector2f RayUnitStep={sqrt(1+(RayDirection.y / RayDirection.x) * (RayDirection.y / RayDirection.x))
       , sqrt(1 + (RayDirection.x / RayDirection.y) * (RayDirection.x/RayDirection.y)) };

   if(RayDirection.x < 0){
   RayStep.x=  -1;
   RayLength.x = (RayStart.x - float(MapCheck.x))* RayUnitStep.x;}else
   {RayStep.x=  1;
    RayLength.x = (float(MapCheck.x + 1) - RayStart.x)* RayUnitStep.x;
   }

      if(RayDirection.y < 0){
   RayStep.y=  -1;
   RayLength.y = (RayStart.y - float(MapCheck.y))* RayUnitStep.y;} else
   {RayStep.y=  1;
   RayLength.y = (float(MapCheck.y+1) - RayStart.y) * RayUnitStep.y;
   }

   bool TileFound=false;
   float MaxDistance = 1000.f, Distance = 0.f;

   while(!TileFound && Distance < MaxDistance)

   {if (RayLength.x < RayLength.y)
   {MapCheck.x += RayStep.x;
   Distance = RayLength.x;
   RayLength.x += RayUnitStep.x;
   }else
   {MapCheck.y += RayStep.y;
   Distance = RayLength.y;
   RayLength.y += RayUnitStep.y;
   }

   if(MapCheck.x >= 0 && MapCheck.x < 10 && MapCheck.y >= 0 && MapCheck.y < 10)
   {std::cout << MapCheck.y << "\n";
       if (TileArray[int(MapCheck.y * 10 + MapCheck.x)] == 1)
   {
       TileFound =true;
   }
  }}

   sf::Vector2f RayIntersection;
   if(TileFound)
   {
       RayIntersection = 
           RayStart + RayDirection * Distance;
       Point.setPosition(RayIntersection.x*GridSize,GridSize*RayIntersection.y);
   }
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    
    bool load(  unsigned int width, unsigned int height)
    {
        bool ColorSwitch=false;
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Triangles);
        m_vertices.resize(width * height * 6);

        // populate the vertex array, with two triangles per tile
        for (unsigned int x = 0; x < width; ++x){ColorSwitch=!ColorSwitch; 
            for (unsigned int y = 0; y < height; ++y)
            {
           
                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(x + y * width) * 6];
                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(x * GridSize, y * GridSize);
                triangles[1].position = sf::Vector2f((x + 1) * GridSize, y * GridSize);
                triangles[2].position = sf::Vector2f(x * GridSize, (y + 1) * GridSize);
                triangles[3].position = sf::Vector2f(x * GridSize, (y + 1) * GridSize);
                triangles[4].position = sf::Vector2f((x + 1) * GridSize, y * GridSize);
                triangles[5].position = sf::Vector2f((x + 1) * GridSize, (y + 1) * GridSize);
            ColorSwitch=!ColorSwitch;   
         for(int i =0;i<6;i++)
         {
             triangles[i].color = ColorArray[TileArray[x + y * width]];
             if (ColorSwitch) { triangles[i].color += sf::Color(22, 22, 22); }
         }
             
        if(TileArray[x + y * width]==2)
        {PlayerXY=sf::Vector2f(x * GridSize+GridSize/2, y * GridSize+GridSize/2);}

            }}
        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();
        
        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;

};



void ButtonLive()
{
    if (Keyboard1.isKeyPressed(Keyboard1.A))
    {
        PlayerRotate = fmod(PlayerRotate + Speed * .5, 360);
    }
    if (Keyboard1.isKeyPressed(Keyboard1.D))
 { PlayerRotate= fmod(PlayerRotate - Speed*.5, 360);
    if(PlayerRotate < 0){PlayerRotate+=360;}}
       if (Keyboard1.isKeyPressed(Keyboard1.W))
       {
           MoveTo(PlayerXDirection*Speed,PlayerYDirection*Speed);
       }
          if (Keyboard1.isKeyPressed(Keyboard1.S))
 {   MoveTo(-PlayerXDirection*Speed,-PlayerYDirection*Speed);
}

			  
}

void PlayerLive()
{
    PlayerXDirection=cos(DegToRad(PlayerRotate));
    PlayerYDirection=sin(DegToRad(PlayerRotate + 180));

//    for(int FOV=1; FOV<136;FOV++){
//    float CosDir=cos(DegToRad(PlayerRotate + FOV - 135/2));
//     float SinDir=sin(DegToRad(PlayerRotate + 180+ FOV - 135/2));
//for(int i=0;i<133;i++)
//{
//    RayX = PlayerXY.x + CosDir * i;
//    RayY = PlayerXY.y + SinDir * i;
//   if (TileArray[(RayY / GridSize * 10 + RayX / GridSize)] == 1||i==132)
//   {
//       RayPoints[FOV].position = sf::Vector2f(RayX, RayY);
//   break;}}}
   RayIntersect();
    ButtonLive();

};

int main()
{
	window.create(sf::VideoMode(777, 777), "SFML tutorial", sf::Style::Close | sf::Style::Resize);
    // create an array of 3 vertices that define a triangle primitive
Point.setOrigin(4.5,4.5);
    	PlayerShape.setPointCount(3);
	PlayerShape.setPoint(0, sf::Vector2f(-33, 30));
	PlayerShape.setPoint(1, sf::Vector2f(0, 0));
	PlayerShape.setPoint(2, sf::Vector2f(33, 30));
	
    PlayerShape.setOutlineThickness(5);
	
	  PlayerShape.setOutlineColor(sf::Color(0,0,0));
	PlayerShape.setFillColor(sf::Color(0,77,0));

      TileMap map;
   map.load(10, 10);

	while (window.isOpen())
	{
	
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:

				window.close();


			}}

                PlayerLive();
			PlayerShape.setPosition(PlayerXY);PlayerShape.setRotation(450-PlayerRotate);
		
		window.clear(sf::Color(0,66,0));
        window.draw(map);
        window.draw(PlayerShape);
        //window.draw(Fan);
        window.draw(Point);
		window.display();
	}
	
	return 0;
}