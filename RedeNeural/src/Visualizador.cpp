#include "../include/Visualizador.h"
#include <map>
#include <algorithm>

namespace NEAT {

Visualizador::Visualizador(SDL_Renderer* renderer, const SDL_Rect& area, 
                         const Configuracao& config)
    : renderer(renderer), area(area), config(config) {
}

void Visualizador::renderizar(const Rede& rede) {
    // Desenhar fundo
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 
        config.corFundo.r, config.corFundo.g, 
        config.corFundo.b, config.corFundo.a);
    SDL_RenderFillRect(renderer, &area);
    
    const auto& nos = rede.obterNos();
    const auto& conexoes = rede.obterConexoes();
    
    // Calcular posições dos nós
    std::map<int, SDL_Point> posicoes;
    std::vector<std::vector<const No*>> camadasNos(3);
    
    // Organizar nós por camada
    for (const auto& no : nos) {
        camadasNos[no.camada].push_back(&no);
    }
    
    // Posicionar nós
    for (int camada = 0; camada < 3; camada++) {
        float x = area.x + (camada * area.w / 2);
        float espacoY = area.h / (camadasNos[camada].size() + 1);
        
        for (size_t i = 0; i < camadasNos[camada].size(); i++) {
            float y = area.y + ((i + 1) * espacoY);
            posicoes[camadasNos[camada][i]->id] = {
                static_cast<int>(x),
                static_cast<int>(y)
            };
        }
    }
    
    // Desenhar conexões
    for (const auto& conexao : conexoes) {
        if (!conexao.ativo) continue;
        
        auto itInicio = posicoes.find(conexao.deNo);
        auto itFim = posicoes.find(conexao.paraNo);
        
        if (itInicio != posicoes.end() && itFim != posicoes.end()) {
            // Cor baseada no peso
            Uint8 r = conexao.peso < 0 ? 255 : 0;
            Uint8 g = conexao.peso > 0 ? 255 : 0;
            SDL_SetRenderDrawColor(renderer, r, g, 0, 255);
            
            SDL_RenderDrawLine(renderer,
                itInicio->second.x, itInicio->second.y,
                itFim->second.x, itFim->second.y);
        }
    }
    
    // Desenhar nós
    for (const auto& [id, pos] : posicoes) {
        auto it = std::find_if(nos.begin(), nos.end(),
            [id](const No& n) { return n.id == id; });
            
        if (it != nos.end()) {
            SDL_Color cor;
            switch (it->camada) {
                case 0: cor = config.corEntrada; break;
                case 1: cor = config.corOculta; break;
                case 2: cor = config.corSaida; break;
            }
            
            SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);
            
            SDL_Rect noRect = {
                pos.x - config.raioNo,
                pos.y - config.raioNo,
                config.raioNo * 2,
                config.raioNo * 2
            };
            SDL_RenderFillRect(renderer, &noRect);
            
            if (config.mostrarBordas) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &noRect);
            }
        }
    }
}

} // namespace NEAT 