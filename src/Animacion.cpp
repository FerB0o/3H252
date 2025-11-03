#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
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
    virtual ~ObjetoAnimado() {}
    int GetX() const { return x; }
    int GetY() const { return y; }
};

// ----------------------
// Clase Misil (disparo)
// ----------------------
class Misil : public ObjetoAnimado {
public:
    Misil(int posX, int posY) : ObjetoAnimado(posX, posY) {}
    void Dibujar(Screen& pantalla) override {
        Pixel& p = pantalla.PixelAt(x, y);
        p.character = '^';
        p.foreground_color = Color::Blue1;
    }
    void Mover() override { y -= 1; } // sube
};

// ----------------------
// Clase Alien (enemigo)
// ----------------------
class Alien : public ObjetoAnimado {
private:
    vector<string> forma;
    bool vivo = true;
    int frame = 0;
public:
    Alien(int posX, int posY) : ObjetoAnimado(posX, posY) {
        forma = { " /M\\ ", "<-O->", " \\_/ " };
    }

    void Dibujar(Screen& pantalla) override {
        if (!vivo) return;
        for (size_t i = 0; i < forma.size(); i++) {
            for (size_t j = 0; j < forma[i].size(); j++) {
                if (forma[i][j] != ' ') {
                    Pixel& p = pantalla.PixelAt(x + j, y + i);
                    p.character = forma[i][j];
                    p.foreground_color = Color::Red1;
                }
            }
        }
    }

    void Mover() override {
        if (!vivo) return;
        frame++;
        if ((frame / 15) % 2 == 0) x++;
        else x--;
        if (frame % 80 == 0) y++; // bajan lentamente
    }

    void Morir() { vivo = false; }
    bool EstaVivo() const { return vivo; }
};

// ----------------------
// Clase Nave (jugador)
// ----------------------
class Nave : public ObjetoAnimado {
private:
    vector<string> forma;
    vector<Misil> misiles;
    int frame = 0;
public:
    Nave(int posX, int posY) : ObjetoAnimado(posX, posY) {
        forma = {
            "    /\\  ",
            "\\  |==|  /",
            " ||====||",
            "   \\__/  "
        };
    }

    void Dibujar(Screen& pantalla) override {
        for (size_t i = 0; i < forma.size(); i++) {
            for (size_t j = 0; j < forma[i].size(); j++) {
                if (forma[i][j] != ' ') {
                    Pixel& p = pantalla.PixelAt(x + j, y + i);
                    p.character = forma[i][j];
                    p.foreground_color = Color::White;
                }
            }
        }
        for (auto& m : misiles) m.Dibujar(pantalla);
    }

    void Mover() override {
        frame++;
        // Movimiento automático zig-zag
        if ((frame / 45) % 2 == 0) x++;
        else x--;
        if (x < 0) x = 0;
        if (x > 60) x = 60;

        //Disparo 
        if (frame % 10 == 0)
            Disparar();

        // Mover misiles
        for (auto& m : misiles)
            m.Mover();

        // Eliminar los que salgan de pantalla
        misiles.erase(
            remove_if(misiles.begin(), misiles.end(),
                [](Misil& m) { return m.GetY() < 0; }),
            misiles.end()
        );
    }

    void Disparar() {
        misiles.push_back(Misil(x + 3, y - 1));
    }

    vector<Misil>& GetMisiles() { return misiles; }
};


// Clase Animacion
class Animacion {
private:
    Screen pantalla;
    Nave nave;
    vector<Alien> aliens;
    int frame = 0;

public:
    Animacion()
        : pantalla(Screen::Create(Dimension::Full(), Dimension::Fixed(30))),
          nave(20, 24) {

        // Crear una formación de aliens
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 3; j++)
                aliens.emplace_back(10 + i * 10, 3 + j * 5);
    }

    void Iniciar() {
        while (true) {
            pantalla.Clear();

            // Dibujar nave y aliens
            nave.Dibujar(pantalla);
            for (auto& a : aliens)
                a.Dibujar(pantalla);

            // Mostrar pantalla
            pantalla.Print();
            cout << pantalla.ResetPosition();

            this_thread::sleep_for(chrono::milliseconds(100));

            // Mover todo
            nave.Mover();
            for (auto& a : aliens)
                a.Mover();

            // Detectar colisiones
            DetectarColisiones();
        }
    }

    void DetectarColisiones() {
        for (auto& misil : nave.GetMisiles()) {
            for (auto& alien : aliens) {
                if (!alien.EstaVivo()) continue;

                int dx = abs((misil.GetX()) - (alien.GetX() + 2));
                int dy = abs((misil.GetY()) - (alien.GetY() + 1));

                if (dx < 3 && dy < 2) {
                    alien.Morir();
                }
            }
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
