#pragma once
#include "Rede.h"
#include <vector>

namespace NEAT {

class Especie {
private:
    std::vector<Rede*> membros;
    float aptidaoAjustada;
    Rede representante;
    int geracoesSemMelhoria;
    float melhorAptidao;

public:
    Especie(const Rede& primeiro);
    
    void adicionarMembro(Rede* rede);
    void calcularAptidaoAjustada();
    bool verificarCompatibilidade(const Rede& rede) const;
    
    float obterAptidaoAjustada() const { return aptidaoAjustada; }
    const std::vector<Rede*>& obterMembros() const { return membros; }
    void limparMembros() { membros.clear(); }
};

} // namespace NEAT 