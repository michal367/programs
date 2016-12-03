#include <iostream>
#include <conio.h>
#include "header.h"
using namespace std;

int main()
{
    cout << "Ile bokow/wierzcholkow ma miec wielokat? (co najmniej 1)" << endl;
    int n;
    do
    {
    cin >> n;
    }while(n<1);
    cout << endl;

    Wielokat w;
    w.wczytaj(n);

    Linia *l = new Linia[n];
    for(int i=0; i<n; i++)
        l[i].tworz_linie(w, i,n);

    cout << endl;
    cout << "Jakie sa wspolrzedne punktu?" << endl;
    Punkt p;
    p.wczytaj();

    cout << endl;
    spr(l,p, n);

    delete [] l;

    getch();
    return 0;
}
