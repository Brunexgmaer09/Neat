#include "utils.hpp"
#include <cmath>
#include <vector>

float sigm(const float x) {
    return std::tanh(x);
}

Color interpolarCor(Color cor1, Color cor2, float fator) {
    return {
        (unsigned char)(cor1.r + (cor2.r - cor1.r) * fator),
        (unsigned char)(cor1.g + (cor2.g - cor1.g) * fator),
        (unsigned char)(cor1.b + (cor2.b - cor1.b) * fator),
        255
    };
}

void desenharConexao(Vector2 inicio, Vector2 fim, float peso) {
    DrawLineEx(inicio, fim, 1.0f, BLACK);
}

void desenharRedeNeural(const RedeNeural& rede, Rectangle area, const std::vector<double>& entradas) {
    const float raioNeuronio = 10.0f;
    const float espacamentoVertical = 30.0f;
    const float espacamentoHorizontal = 50.0f;
    
    // Cores dos neurônios
    const Color corInativa = RED;
    const Color corAtiva = MAROON;
    
    // Calcula posições dos neurônios
    std::vector<std::vector<PosicaoNeuronio>> camadas;
    
    // Camada de entrada
    camadas.push_back(std::vector<PosicaoNeuronio>(entradas.size()));
    float y = area.y + raioNeuronio;
    for(size_t i = 0; i < entradas.size(); i++) {
        camadas[0][i].posicao = {
            area.x + raioNeuronio,
            y + i * espacamentoVertical
        };
        camadas[0][i].ativacao = entradas[i];
    }
    
    // Camadas escondidas
    float x = area.x + espacamentoHorizontal;
    for(const auto& camada : rede.getCamadasEscondidas()) {
        camadas.push_back(std::vector<PosicaoNeuronio>(camada.getQuantidadeNeuronios()));
        for(int i = 0; i < camada.getQuantidadeNeuronios(); i++) {
            camadas.back()[i].posicao = {
                x + raioNeuronio,
                area.y + raioNeuronio + i * espacamentoVertical
            };
            camadas.back()[i].ativacao = camada.getNeuronio(i).getSaida();
        }
        x += espacamentoHorizontal;
    }
    
    // Camada de saída
    camadas.push_back(std::vector<PosicaoNeuronio>(rede.getCamadaSaida().getQuantidadeNeuronios()));
    for(int i = 0; i < rede.getCamadaSaida().getQuantidadeNeuronios(); i++) {
        camadas.back()[i].posicao = {
            x + raioNeuronio,
            area.y + raioNeuronio + i * espacamentoVertical
        };
        camadas.back()[i].ativacao = rede.getCamadaSaida().getNeuronio(i).getSaida();
    }
    
    // Desenha conexões
    for(size_t i = 1; i < camadas.size(); i++) {
        for(size_t j = 0; j < camadas[i].size(); j++) {
            for(size_t k = 0; k < camadas[i-1].size(); k++) {
                desenharConexao(
                    camadas[i-1][k].posicao,
                    camadas[i][j].posicao,
                    1.0f
                );
            }
        }
    }
    
    // Desenha neurônios
    for(const auto& camada : camadas) {
        for(const auto& neuronio : camada) {
            // Interpola entre vermelho e vinho baseado na ativação
            Color cor = interpolarCor(corInativa, corAtiva, 
                std::abs(neuronio.ativacao));
            
            // Desenha borda preta
            DrawCircle(neuronio.posicao.x, neuronio.posicao.y, 
                      raioNeuronio + 1, BLACK);
            // Desenha neurônio
            DrawCircle(neuronio.posicao.x, neuronio.posicao.y, 
                      raioNeuronio, cor);
        }
    }
} 