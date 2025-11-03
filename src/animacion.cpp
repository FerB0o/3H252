#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <ftxui/screen/screen.hpp>

using namespace std;
using namespace ftxui;

// ----------------------
// Clase base genérica
// ----------------------
class ObjetoAnimado {
protected:
    int x, y;
public:
    ObjetoAnimado(int posX, int posY) : x(posX), y(posY) {}
    virtual void Dibujar(Screen& pantalla) = 0;
    virtual void Mover() = 0;
    bool FueraDePantalla(int altura) const { return y < 0 || y > altura; }
    virtual ~ObjetoAnimado() {}
};
// ----------------------
// Clase Enemigo
// ----------------------
class Enemigo : public ObjetoAnimado {
private:
    vector<string> forma;
    bool vivo = true;
    int frame = 0;
public:
    Enemigo(int posX, int posY) : ObjetoAnimado(posX, posY) {
        forma = {
        "  /\\  ",
        " (  ) ",
        "/ -- \\",
        "| () |",
        " \\__/ "
        };
    }

    void Dibujar(Screen& pantalla) override {
        // Dibujar la nave
        for (size_t i = 0; i < forma.size(); i++) {
            for (size_t j = 0; j < forma[i].size(); j++) {
                if (forma[i][j] != ' ') {
                    Pixel& p = pantalla.PixelAt(x + j, y + i);
                    p.character = forma[i][j];
                    p.foreground_color = Color::Blue1;
                }
            }
        }
    }


    void Mover() override {
        // Movimiento lateral automático (zig-zag)
        frame++;
        if ((frame / 50) % 2 == 0)
            x++;
        else
            x--;

        if (x < 0) x = 0;
        if (x > 60) x = 60;
    }
    void Morir() { vivo = false; }
    bool EstaVivo() const { return vivo; }

};


// ----------------------
// Clase Misil (disparo)
// ----------------------
class Misil : public ObjetoAnimado {
public:
    Misil(int posX, int posY) : ObjetoAnimado(posX, posY) {}

    void Dibujar(Screen& pantalla) override {
        Pixel& p = pantalla.PixelAt(x, y);
        p.character = '|';
        p.foreground_color = Color::Yellow;
    }

    void Mover() override {
        y -= 1; // el misil sube
    }
};

// ----------------------
// Clase Nave (que dispara)
// ----------------------
class Nave : public ObjetoAnimado {
private:
    vector<string> forma;
    vector<Misil> misiles;
    int frame = 0;
public:
    Nave(int posX, int posY) : ObjetoAnimado(posX, posY) {
        forma = {
            "   /\\   ",
            "  /==\\  ",
            " |====| ",
            " |====| ",
          "<<\\__/>>"
        };
    }

    void Dibujar(Screen& pantalla) override {
        // Dibujar la nave
        for (size_t i = 0; i < forma.size(); i++) {
            for (size_t j = 0; j < forma[i].size(); j++) {
                if (forma[i][j] != ' ') {
                    Pixel& p = pantalla.PixelAt(x + j, y + i);
                    p.character = forma[i][j];
                    p.foreground_color = Color::Red;
                }
            }
        }

        // Dibujar los misiles
        for (auto& m : misiles)
            m.Dibujar(pantalla);
    }

    void Mover() override {
        // Movimiento lateral automático (zig-zag)
        frame++;
        if ((frame / 50) % 2 == 0)
            x++;
        else
            x--;

        if (x < 0) x = 0;
        if (x > 60) x = 60;

        // Mover los misiles
        for (auto& m : misiles)
            m.Mover();

        // Eliminar los misiles que salen de pantalla
        misiles.erase(
            remove_if(misiles.begin(), misiles.end(),
                      [](Misil& m) { return m.FueraDePantalla(80); }),
            misiles.end()
        );

        // Disparar cada cierto número de frames
        if (frame % 5 == 0)
            Disparar();
    }

    void Disparar() {
        // El misil sale del centro de la nave
        int posX = x + 4;
        int posY = y - 1;
        misiles.push_back(Misil(posX, posY));
    }
};

// ----------------------
// Clase que controla toda la animación
// ----------------------
class Animacion {
private:
    Screen pantalla;
    Nave nave;
   
public:
    Animacion() :
        pantalla(Screen::Create(Dimension::Full(), Dimension::Fixed(30))),
        nave(20, 20) {}

    void Iniciar() {
        while (true) {
            pantalla.Clear();
            nave.Dibujar(pantalla);
            pantalla.Print();
            cout << pantalla.ResetPosition();

            this_thread::sleep_for(chrono::milliseconds(90));
            nave.Mover();
        }
    }
};

// ----------------------
// Programa principal
// ----------------------
int main() {
    Animacion anim;
    anim.Iniciar();
    return 0;
}