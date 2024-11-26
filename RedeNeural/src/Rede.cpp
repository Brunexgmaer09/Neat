#include "../include/Rede.h"
#include <cmath>
#include <fstream>
#include <iostream>

namespace NEAT {

int Rede::proximaInovacao = 0;

float Rede::sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

void Rede::limpar() {
    for (auto& no : nos) {
        no.valor = 0.0f;
    }
    saidas.clear();
}

void Rede::definirEntradas(const std::vector<float>& novasEntradas) {
    entradas = novasEntradas;
}

void Rede::adicionarNo(int camada) {
    No novoNo;
    novoNo.id = proximoIdNo++;
    novoNo.camada = camada;
    novoNo.valor = 0.0f;
    nos.push_back(novoNo);
}

void Rede::adicionarConexao(int deNo, int paraNo, float peso) {
    Conexao novaConexao;
    novaConexao.deNo = deNo;
    novaConexao.paraNo = paraNo;
    novaConexao.peso = peso;
    novaConexao.ativo = true;
    novaConexao.inovacao = proximaInovacao++;
    conexoes.push_back(novaConexao);
}

void Rede::mutar() {
    // Implementação básica de mutação
    for (auto& conexao : conexoes) {
        if (rand() % 100 < 10) { // 10% de chance de mutar cada conexão
            conexao.peso += (float)(rand() % 200 - 100) / 100.0f;
        }
    }
}

Rede::Rede(int numEntradas, int numSaidas) : aptidao(0), proximoIdNo(0) {
    // Adicionar nós de entrada
    for (int i = 0; i < numEntradas; i++) {
        adicionarNo(0);  // camada de entrada
    }
    
    // Adicionar nós de saída
    for (int i = 0; i < numSaidas; i++) {
        adicionarNo(2);  // camada de saída
    }
    
    // Criar conexões iniciais entre todas as entradas e saídas
    for (int i = 0; i < numEntradas; i++) {
        for (int j = 0; j < numSaidas; j++) {
            float peso = (float)(rand() % 2000 - 1000) / 1000.0f;
            adicionarConexao(i, numEntradas + j, peso);
        }
    }
}

void Rede::avaliar() {
    limpar();
    
    // Definir valores dos nós de entrada
    for (size_t i = 0; i < entradas.size(); i++) {
        nos[i].valor = entradas[i];
    }
    
    // Propagar valores pela rede
    for (const auto& conexao : conexoes) {
        if (conexao.ativo) {
            nos[conexao.paraNo].valor += nos[conexao.deNo].valor * conexao.peso;
        }
    }
    
    // Coletar saídas e aplicar função de ativação
    saidas.clear();
    for (const auto& no : nos) {
        if (no.camada == 2) {
            saidas.push_back(sigmoid(no.valor));
        }
    }
}

void Rede::salvar(const std::string& arquivo) {
    std::ofstream out(arquivo, std::ios::binary);
    
    // Salvar nós
    size_t numNos = nos.size();
    out.write(reinterpret_cast<char*>(&numNos), sizeof(numNos));
    out.write(reinterpret_cast<char*>(nos.data()), numNos * sizeof(No));
    
    // Salvar conexões
    size_t numConexoes = conexoes.size();
    out.write(reinterpret_cast<char*>(&numConexoes), sizeof(numConexoes));
    out.write(reinterpret_cast<char*>(conexoes.data()), numConexoes * sizeof(Conexao));
}

void Rede::carregar(const std::string& arquivo) {
    std::ifstream in(arquivo, std::ios::binary);
    
    // Carregar nós
    size_t numNos;
    in.read(reinterpret_cast<char*>(&numNos), sizeof(numNos));
    nos.resize(numNos);
    in.read(reinterpret_cast<char*>(nos.data()), numNos * sizeof(No));
    
    // Carregar conexões
    size_t numConexoes;
    in.read(reinterpret_cast<char*>(&numConexoes), sizeof(numConexoes));
    conexoes.resize(numConexoes);
    in.read(reinterpret_cast<char*>(conexoes.data()), numConexoes * sizeof(Conexao));
}

} // namespace NEAT 