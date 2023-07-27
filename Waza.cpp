#include <SFML/Graphics.hpp>
#include <array>
#include <time.h>
#define size 16
#define MaxX 30
#define MaxY 20
using namespace sf;

int w = size*MaxX; //szerokosc
int h = size*MaxY; //wysokosc

bool DeadEnd = false;
int num=4;

enum class kierunek {L, R, U, D} dir; //left, right, up, down

struct punkt
{
    float x, y;
    punkt(float a = 0, float b = 0) : x(a), y(b) {};
};

punkt fruit;

class Waz
{
private:

    std::vector<punkt> s = {punkt(), punkt(), punkt(), punkt()};
  
public:
    void move();
    void hapsf(); //haps fruit
    void wall();
    void hapsu(); //haps yourself
    void drawSnake(Sprite& sprite, RenderWindow& window);
} S;

void Waz::move()
{
    //przesuwamy weza
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    if (dir == kierunek::U) s[0].y += 1;
    if (dir == kierunek::L) s[0].x -= 1;
    if (dir == kierunek::R) s[0].x += 1;
    if (dir == kierunek::D) s[0].y -= 1;
}

void Waz::hapsf()
{
    srand(time(0));
    if ((s[0].x == fruit.x) && (s[0].y == fruit.y))
    {
        num++;
        s.push_back(punkt());
         
        bool again = false;
        do
        {
            fruit.x = float(rand() % MaxX);
            fruit.y = float(rand() % MaxY);

            for (std::vector<punkt>::iterator i = s.begin(); i != s.end()+1; i++)
                if (i->x == fruit.x && i->y == fruit.y)
                again = true;
        } while (again);
    }
}

void Waz::wall()
{
    if (s[0].x > MaxX) s[0].x = 0;
    if (s[0].x < 0) s[0].x = MaxX;
    if (s[0].y > MaxY) s[0].y = 0;
    if (s[0].y < 0) s[0].y = MaxY;
}

void Waz::hapsu()
{
    for (int i = 1; i < num; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y)
            DeadEnd = true;
}

void Waz::drawSnake(Sprite& sprite, RenderWindow& window)
{
    for (int i = 0; i < num; i++)
    {
        sprite.setPosition(s[i].x * size, s[i].y * size);  
        window.draw(sprite);
    }
}

void Tick()
 {
    S.hapsf();
    S.move();
    S.wall();
    S.hapsu();
 }

int main()
{  

    RenderWindow window(VideoMode(w, h), "Waz");

    Texture t1,t2, t3;
    t1.loadFromFile("images/tlo.png");
    t2.loadFromFile("images/waz.png");
    t3.loadFromFile("images/owoc.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);

    Clock clock;
    float timer=0, delay=0.1;

    fruit.x=10;
    fruit.y=10; 
    
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time; 

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) dir = kierunek::L;   
        if (Keyboard::isKeyPressed(Keyboard::Right)) dir = kierunek::R;    
        if (Keyboard::isKeyPressed(Keyboard::Up)) dir = kierunek::D;
        if (Keyboard::isKeyPressed(Keyboard::Down)) dir = kierunek::U;

        if (timer>delay && !DeadEnd) {timer=0; Tick();}

        ////// draw  ///////
        window.clear();

        for (int i=0; i<MaxX; i++) 
            for (int j=0; j<MaxY; j++) 
                { sprite1.setPosition(i*size, j*size);  window.draw(sprite1); }
   
        S.drawSnake(sprite2, window);
        sprite3.setPosition(fruit.x*size, fruit.y*size);  window.draw(sprite3);    

        window.display();
    }

    return 0;
}
