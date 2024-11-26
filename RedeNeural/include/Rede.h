#pragma once
#include <vector>
#include <string>

namespace NEAT {

struct Conexao {
    int deNo;
    int paraNo;
    float peso;
    bool ativo;
    int inovacao;
};

struct No {
    int id;
    int camada;  // 0=entrada, 1=oculta, 2=saida
    float valor;
};

class Rede {
private:
    static int proximaInovacao;
    float aptidao;
    int proximoIdNo;
    std::vector<No> nos;
    std::vector<Conexao> conexoes;
    std::vector<float> entradas;
    std::vector<float> saidas;

    float sigmoid(float x);

public:
    Rede(int numEntradas = 5, int numSaidas = 1);
    
    // Métodos principais
    void definirEntradas(const std::vector<float>& novasEntradas);
    void avaliar();
    void mutar();
    void limpar();
    
    // Métodos de modificação da rede
    void adicionarNoAleatorio();
    void adicionarConexaoAleatoria();
    void adicionarNo(int camada);
    void adicionarConexao(int deNo, int paraNo, float peso);
    
    // Getters e Setters
    const std::vector<No>& obterNos() const { return nos; }
    void definirNos(const std::vector<No>& novosNos) { nos = novosNos; }
    const std::vector<Conexao>& obterConexoes() const { return conexoes; }
    void definirConexoes(const std::vector<Conexao>& novasConexoes) { conexoes = novasConexoes; }
    const std::vector<float>& obterSaidas() const { return saidas; }
    float obterAptidao() const { return aptidao; }
    void definirAptidao(float f) { aptidao = f; }
    static int obterProximaInovacao() { return proximaInovacao; }
    int obterProximoIdNo() const { return proximoIdNo; }
    
    // Serialização
    void salvar(const std::string& arquivo);
    void carregar(const std::string& arquivo);
};

} // namespace NEAT 