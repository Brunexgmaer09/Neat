#pragma once
#include "Rede.h"
#include <SDL2/SDL.h>

namespace NEAT {

class Visualizador {
public:
    struct Configuracao {
        int largura;
        int altura;
        int raioNo;
        bool mostrarPesos;
        bool mostrarBordas;
        SDL_Color corFundo;
        SDL_Color corEntrada;
        SDL_Color corOculta;
        SDL_Color corSaida;

        // Construtor com valores padrão
        Configuracao() {
            largura = 200;
            altura = 300;
            raioNo = 5;
            mostrarPesos = true;
            mostrarBordas = true;
            corFundo = {0, 0, 0, 128};
            corEntrada = {255, 0, 0, 255};
            corOculta = {0, 255, 0, 255};
            corSaida = {0, 0, 255, 255};
        }
    };

private:
    SDL_Renderer* renderer;
    SDL_Rect area;
    Configuracao config;

public:
    Visualizador(SDL_Renderer* renderer, const SDL_Rect& area, 
                 const Configuracao& config = Configuracao());
    
    void renderizar(const Rede& rede);
    void definirArea(const SDL_Rect& novaArea) { area = novaArea; }
    void definirConfiguracao(const Configuracao& novaConfig) { config = novaConfig; }
};

} // namespace NEAT 