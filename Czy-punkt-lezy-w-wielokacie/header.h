#include <iostream>
using namespace std;

class Linia;
class Punkt;

void spr(Linia l[], Punkt & p, int n);

class Punkt
{
    float x,y;
public:
    Punkt(int=0, int=0);
    void wczytaj();
    friend void spr(Linia l[], Punkt & p, int n);
};

class Wielokat
{
    float *x,*y;
public:
    //Wielokat();
    void wczytaj(int);
    friend class Linia;
    friend void tworz_linie(Wielokat & w, int, int);
    ~Wielokat();
};

class Linia
{
    float x1,y1, x2,y2;
public:
    void tworz_linie(Wielokat & w, int, int);
    friend void spr(Linia l[], Punkt & p, int n);
};
