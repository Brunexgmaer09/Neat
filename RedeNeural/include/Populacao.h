#pragma once
#include "Rede.h"
#include "Especie.h"
#include <vector>
#include <functional>

namespace NEAT {

class Populacao {
public:
    struct Configuracao {
        int tamanhoPopulacao;
        float taxaElitismo;
        float taxaMutacao;
        float taxaCruzamento;
        float limiarCompatibilidade;
        int tamanhoTorneio;
        int maxEspecies;
        int geracoesSemMelhoria;

        Configuracao() {
            tamanhoPopulacao = 50;
            taxaElitismo = 0.1f;
            taxaMutacao = 0.3f;
            taxaCruzamento = 0.8f;
            limiarCompatibilidade = 3.0f;
            tamanhoTorneio = 3;
            maxEspecies = 10;
            geracoesSemMelhoria = 15;
        }
    };

private:
    Configuracao config;
    std::vector<Rede> individuos;
    std::vector<Especie> especies;
    int geracao;
    float melhorAptidao;
    
    std::function<void(int, float, float, float)> onGeracaoCallback;

public:
    Populacao(int numEntradas, int numSaidas, const Configuracao& config = Configuracao());
    
    void evoluir();
    void avaliarPopulacao(std::function<float(Rede&)> funcaoAvaliacao);
    void selecao();
    void cruzamento();
    void mutacao();
    void especiar();
    void ajustarAptidoes();
    void eliminarEspeciesFracas();
    
    int obterGeracao() const { return geracao; }
    float obterMelhorAptidao() const { return melhorAptidao; }
    std::vector<Rede>& obterIndividuos() { return individuos; }
    const std::vector<Rede>& obterIndividuos() const { return individuos; }
    const std::vector<Especie>& obterEspecies() const { return especies; }
    
    void definirCallbackGeracao(std::function<void(int, float, float, float)> callback) {
        onGeracaoCallback = callback;
    }
    
    void salvarMelhorRede(const std::string& arquivo);
    void carregarMelhorRede(const std::string& arquivo);
    
    void definirConfiguracao(const Configuracao& novaConfig) {
        config = novaConfig;
    }

protected:
    Rede* selecaoTorneio(int tamanhoTorneio);
    Rede cruzarRedes(const Rede& rede1, const Rede& rede2);
};

} // namespace NEAT 