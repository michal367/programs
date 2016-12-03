#include "header.h"

Punkt::Punkt(int x, int y)
{
    this->x = x;
    this->y = y;
}
void Punkt::wczytaj()
{
    cout << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;
}


void Wielokat::wczytaj(int n)
{
    x = new float[n];
    y = new float[n];
    for(int i=0; i<n; i++)
    {
        cout << i+1 << "." << endl;
        cout << "x: ";
        cin >> x[i];
        cout << "y: ";
        cin >> y[i];
    }
}
Wielokat::~Wielokat()
{
    delete [] x;
    delete [] y;
}

void Linia::tworz_linie(Wielokat & w, int i, int n)
{
        x1 = w.x[i];
        y1 = w.y[i];
    if(i==n-1)
    {
        x2 = w.x[0];
        y2 = w.y[0];
    }
    else
    {
        x2 = w.x[i+1];
        y2 = w.y[i+1];
    }
}


void spr(Linia l[], Punkt & p, int n)
{
    int licznik=0;
    for(int i=0; i<n; i++)
    {
        //spr czy P nale¿y do ktoregos boku W
        if(((l[i].y1>=p.y && l[i].y2<=p.y) || (l[i].y2>=p.y && l[i].y1<=p.y)) &&
           ((l[i].x1>=p.x && l[i].x2<=p.x) || (l[i].x2>=p.x && l[i].x1<=p.x)))
        {
            if((l[i].x1==p.x && l[i].x2==p.x) || (l[i].y1==p.y && l[i].y2==p.y))
            {
                licznik=1;
                break;
            }
            else if(p.x == (((p.y-l[i].y1)/(l[i].y2-l[i].y1))*(l[i].x2-l[i].x1)+l[i].x1))
            {
                licznik=1;
                break;
            }
        }
        //spr ile polprosta od P przecina bokow W
        if((l[i].y1>p.y && l[i].y2<p.y) || (l[i].y2>p.y && l[i].y1<p.y) ||
           (l[i].y1>p.y && l[i].y2==p.y)|| (l[i].y2>p.y && l[i].y1==p.y))
        {
            if(l[i].x1>p.x && l[i].x2>p.x)
                licznik++;
            else if(l[i].x1>p.x || l[i].x2>p.x)
                if(p.x < (((p.y-l[i].y1)/(l[i].y2-l[i].y1))*(l[i].x2-l[i].x1)+l[i].x1))
                    licznik++;
        }
    }
    if(licznik%2 == 0)
        cout << "Punkt NIE nalezy do wielokata" << endl;
    else
        cout << "Punkt nalezy do wielokata" << endl;
}
