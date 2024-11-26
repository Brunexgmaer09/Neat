# NEAT (NeuroEvolution of Augmenting Topologies)

Uma implementação em C++ do algoritmo NEAT para evolução de redes neurais. Esta biblioteca permite que redes neurais evoluam tanto sua topologia quanto seus pesos, começando com estruturas simples e tornando-se progressivamente mais complexas.

## Características

- Implementação NEAT completa e modular
- Evolução automática da topologia da rede
- Visualização em tempo real usando SDL2
- Sistema de espécies e elitismo
- Suporte a salvamento/carregamento de redes
- Namespace próprio para evitar conflitos
- Callbacks para monitoramento da evolução

## Como Usar

### Requisitos

- C++11 ou superior
- SDL2 (opcional, apenas para visualização)
- Compilador com suporte a C++11 (g++, clang++, MSVC)

### Instalação

1. Clone o repositório:
   git clone https://github.com/seu-usuario/neat-implementation.git

2. Inclua os arquivos em seu projeto:
   #include "NEAT/include/Rede.h"
   #include "NEAT/include/Populacao.h"
   #include "NEAT/include/Visualizador.h"

### Exemplo Básico

    #include "NEAT/include/Populacao.h"
    using namespace NEAT;

    int main() {
        // Configurar a população
        Populacao::Configuracao config;
        config.tamanhoPopulacao = 50;
        config.taxaMutacao = 0.3f;
        config.taxaCruzamento = 0.8f;
        config.taxaElitismo = 0.1f;

        // Criar população (5 entradas, 1 saída)
        Populacao populacao(5, 1, config);

        // Definir callback para acompanhar evolução
        populacao.definirCallbackGeracao([](int geracao, float melhor, float media, float pior) {
            std::cout << "Geracao " << geracao << ": " << melhor << std::endl;
        });

        // Loop principal de evolução
        for (int geracao = 0; geracao < 100; geracao++) {
            // Avaliar cada rede na população
            populacao.avaliarPopulacao([](Rede& rede) {
                // Seu código de avaliação aqui
                return pontuacao;
            });

            // Evoluir para próxima geração
            populacao.evoluir();
        }
    }

### Guia Detalhado

1. Configuração da População:
   Populacao::Configuracao config;
   config.tamanhoPopulacao = 50;    // Número de indivíduos
   config.taxaMutacao = 0.3f;       // Chance de mutação (0-1)
   config.taxaCruzamento = 0.8f;    // Chance de cruzamento (0-1)
   config.taxaElitismo = 0.1f;      // Percentual de elite preservada
   config.tamanhoTorneio = 3;       // Tamanho do torneio na seleção

2. Avaliação de Redes:
   float avaliarRede(Rede& rede) {
       // Definir entradas
       std::vector<float> entradas = {0.5f, 0.3f, 0.8f, 0.2f, 0.1f};
       rede.definirEntradas(entradas);
       
       // Processar rede
       rede.avaliar();
       
       // Obter saídas
       const auto& saidas = rede.obterSaidas();
       
       // Calcular pontuação baseada no desempenho
       float pontuacao = /* seu cálculo aqui */;
       return pontuacao;
   }

3. Visualização (com SDL2):
   Visualizador::Configuracao visConfig;
   visConfig.largura = 200;
   visConfig.altura = 300;
   visConfig.raioNo = 5;
   visConfig.mostrarPesos = true;
   visConfig.corFundo = {0, 0, 0, 128};
   visConfig.corEntrada = {255, 0, 0, 255};
   visConfig.corOculta = {0, 255, 0, 255};
   visConfig.corSaida = {0, 0, 255, 255};

   Visualizador visualizador(renderer, {600, 10, 180, 200}, visConfig);
   visualizador.renderizar(rede);

4. Salvamento/Carregamento:
   // Salvar melhor rede
   populacao.salvarMelhorRede("melhor_rede.bin");

   // Carregar rede
   Rede rede(5, 1);
   rede.carregar("melhor_rede.bin");

### Dicas de Uso

1. Ajuste de Parâmetros:
   - Comece com populações pequenas (50-100)
   - Use taxas de mutação moderadas (0.2-0.4)
   - Ajuste o elitismo baseado no problema

2. Função de Avaliação:
   - Deve refletir claramente o objetivo
   - Considere múltiplos aspectos do desempenho
   - Evite platôs na função de fitness

3. Otimização:
   - Use threads para avaliação paralela
   - Implemente cache de inovações
   - Monitore a diversidade da população

### Licença

MIT License - veja LICENSE.md para detalhes

### Contribuindo

1. Fork o projeto
2. Crie sua branch (git checkout -b feature/AmazingFeature)
3. Commit suas mudanças (git commit -m 'Add some AmazingFeature')
4. Push para a branch (git push origin feature/AmazingFeature)
5. Abra um Pull Request