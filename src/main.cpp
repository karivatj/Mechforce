#include<iostream>

#include "cGame.h"

using namespace std;

int main(void)
{
    cout << "Mechforce v. 0.0" << endl;

    Game *mechforce = new Game();

    mechforce->Start();

    return 0;
}
