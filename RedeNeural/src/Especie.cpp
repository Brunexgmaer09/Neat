#include "../include/Especie.h"
#include "../include/Configuracao.h"
#include <cmath>

namespace NEAT {

Especie::Especie(const Rede& primeiro) 
    : representante(primeiro), 
      aptidaoAjustada(0), 
      geracoesSemMelhoria(0), 
      melhorAptidao(0) {
}

void Especie::adicionarMembro(Rede* rede) {
    membros.push_back(rede);
}

void Especie::calcularAptidaoAjustada() {
    if (membros.empty()) return;
    
    aptidaoAjustada = 0;
    for (auto membro : membros) {
        aptidaoAjustada += membro->obterAptidao();
    }
    aptidaoAjustada /= membros.size();
    
    // Atualizar melhor aptidão
    float melhorAtual = membros[0]->obterAptidao();
    for (size_t i = 1; i < membros.size(); i++) {
        melhorAtual = std::max(melhorAtual, membros[i]->obterAptidao());
    }
    
    if (melhorAtual > melhorAptidao) {
        melhorAptidao = melhorAtual;
        geracoesSemMelhoria = 0;
    } else {
        geracoesSemMelhoria++;
    }
}

bool Especie::verificarCompatibilidade(const Rede& rede) const {
    const auto& conexoesA = representante.obterConexoes();
    const auto& conexoesB = rede.obterConexoes();
    
    int disjuntos = 0;
    int excessos = 0;
    float somaDiferencasPesos = 0.0f;
    int coincidentes = 0;
    
    size_t i = 0, j = 0;
    while (i < conexoesA.size() && j < conexoesB.size()) {
        if (conexoesA[i].inovacao == conexoesB[j].inovacao) {
            somaDiferencasPesos += std::abs(conexoesA[i].peso - conexoesB[j].peso);
            coincidentes++;
            i++;
            j++;
        } else if (conexoesA[i].inovacao < conexoesB[j].inovacao) {
            disjuntos++;
            i++;
        } else {
            disjuntos++;
            j++;
        }
    }
    
    // Genes excedentes
    excessos = (conexoesA.size() - i) + (conexoesB.size() - j);
    
    // Normalização
    float N = std::max(conexoesA.size(), conexoesB.size());
    if (N < 20) N = 1;
    
    float diferencaMedia = coincidentes > 0 ? somaDiferencasPesos / coincidentes : 0;
    
    // Cálculo de compatibilidade com pesos ajustados
    float compatibilidade = (
        NEAT::ConfiguracaoNEAT::COEF_EXCESSO * excessos / N +
        NEAT::ConfiguracaoNEAT::COEF_DISJUNTO * disjuntos / N +
        NEAT::ConfiguracaoNEAT::COEF_PESO * diferencaMedia
    );
    
    // Usar o limiar de compatibilidade da configuração
    return compatibilidade < 1.0f;
}

} // namespace NEAT 