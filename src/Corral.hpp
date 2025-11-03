#pragma once
#include <vector>
#include "Dibujo.hpp"

class Corral {
public:
    int x, y, ancho, alto;
    Dibujo dibujo_corral;
    std::vector<Dibujo> animales;

    Corral(int x, int y, int ancho, int alto, ftxui::Color color = ftxui::Color::White)
        : x(x), y(y), ancho(ancho), alto(alto),
          dibujo_corral(x, y, GenerarFiguraCorral(ancho, alto), color) {}

    static std::vector<std::u32string> GenerarFiguraCorral(int ancho, int alto) {
        std::vector<std::u32string> figura;
        std::u32string top = U"+" + std::u32string(ancho-2, U'-') + U"+";
        std::u32string mid = U"|" + std::u32string(ancho-2, U' ') + U"|";
        figura.push_back(top);
        for (int i = 0; i < alto-2; ++i) figura.push_back(mid);
        figura.push_back(top);
        return figura;
    }

    void AgregarAnimal(const Dibujo& animal) {
        // Asegura que el animal esté dentro del corral
        int ax = animal.x;
        int ay = animal.y;
        if (ax < x+1) ax = x+1;
        if (ax > x+ancho-3) ax = x+ancho-3;
        if (ay < y+1) ay = y+1;
        if (ay > y+alto-3) ay = y+alto-3;
        Dibujo animal_dentro(ax, ay, animal.figura, animal.color);
        animales.push_back(animal_dentro);
    }

    void Dibujar(ftxui::Screen& screen) const {
        dibujo_corral.Dibujar(screen);
        for (const auto& animal : animales) {
            animal.Dibujar(screen);
        }
    }
};