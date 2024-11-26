#pragma once

namespace NEAT {

struct ConfiguracaoNEAT {
    // Parâmetros de compatibilidade
    static float COEF_EXCESSO;
    static float COEF_DISJUNTO;
    static float COEF_PESO;
    
    // Parâmetros de mutação
    static float CHANCE_PESO_PERTURBADO;
    static float CHANCE_CONEXAO_TOGGLE;
    static float CHANCE_NOVO_NO;
    static float CHANCE_NOVA_CONEXAO;
    
    // Limites
    static int MAX_NOS;
    static int MAX_CONEXOES;
    
    static void inicializarPadrao();
};

} // namespace NEAT 