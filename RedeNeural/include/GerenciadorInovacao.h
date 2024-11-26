#pragma once
#include <map>
#include <utility>

namespace NEAT {

class GerenciadorInovacao {
private:
    std::map<std::pair<int, int>, int> historicoInovacoes;
    int proximaInovacao;

public:
    GerenciadorInovacao() : proximaInovacao(0) {}
    
    int obterInovacao(int deNo, int paraNo);
    void limpar();
    
    static GerenciadorInovacao& instancia() {
        static GerenciadorInovacao inst;
        return inst;
    }
};

} // namespace NEAT 