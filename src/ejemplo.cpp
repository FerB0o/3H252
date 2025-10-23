#include <iostream>
#include <string>
#include <ftxui/screen/screen.hpp>
#include <chrono>
#include <thread>

using namespace std;
using namespace ftxui;

int main ( ){

    Screen pantalla = Screen::Create(
        Dimension::Full(),
        Dimension::Fixed(10)
    );

    int x = 0;
    int y = 0;

    while (true)
    {
        /* code */
    
    Pixel& pixel = pantalla.PixelAt(x,y);
    pixel.foreground_color = Color::Red;
    pixel.background_color = Color::Green;
    pixel.character = 'X';


    pantalla.Print();
    this_thread::sleep_for(chrono::milliseconds(1000));
    pantalla.Clear();
    cout<< pantalla.ResetPosition();
    x++;
   }
return 0;


}

