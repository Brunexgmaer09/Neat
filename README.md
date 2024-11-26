# Biblioteca NEAT (NeuroEvolution of Augmenting Topologies)

**NEAT** é uma implementação robusta e eficiente do algoritmo NEAT em C++, permitindo a evolução de redes neurais através de neuroevolução.

## 🧠 Sobre o Projeto

Esta biblioteca implementa o **algoritmo NEAT**, que permite a evolução automática de redes neurais. O sistema começa com redes simples e aumenta gradualmente sua complexidade através de mutações e evolução natural.

## ✨ Características Principais

- **Evolução automática da topologia da rede**
- **Sistema de espécies aprimorado**
  - Distribuição proporcional de slots por espécie
  - Elitismo por espécie
  - Cálculo de compatibilidade otimizado
- **Crescimento gradual da complexidade**
- **Sistema de logs detalhados**
  - Monitoramento de cruzamentos
  - Estatísticas por espécie
  - Acompanhamento de evolução
- **Visualização em tempo real** (usando SDL2)
- **Salvamento e carregamento de redes**
- **Interface simples e intuitiva**
- **Totalmente configurável**

## 🚀 Começando

### Pré-requisitos

- **C++17** ou superior
- **SDL2** (opcional, para visualização)
- **Compilador compatível**: g++, clang++, MSVC

### 📥 Instalação

1. **Clone o repositório:**

    ```bash
    git clone https://github.com/seu-usuario/rede-neural-neat.git
    ```

2. **Inclua os arquivos em seu projeto:**

    ```cpp
    #include "RedeNeural/include/Rede.h"
    #include "RedeNeural/include/Populacao.h"
    #include "RedeNeural/include/Visualizador.h"
    ```

## 💻 Exemplo de Uso

```cpp
#include "RedeNeural/include/Populacao.h"

// Configurar população
NEAT::Populacao::Configuracao config;
config.tamanhoPopulacao = 200;
config.taxaMutacao = 0.4f;
config.taxaCruzamento = 0.8f;
config.limiarCompatibilidade = 1.0f;
config.maxEspecies = 15;

// Configurar parâmetros NEAT
NEAT::ConfiguracaoNEAT::COEF_EXCESSO = 1.0f;
NEAT::ConfiguracaoNEAT::COEF_DISJUNTO = 1.0f;
NEAT::ConfiguracaoNEAT::COEF_PESO = 0.3f;

// Criar população
NEAT::Populacao populacao(numEntradas, numSaidas, config);

// Callback para monitoramento (opcional)
populacao.definirCallbackGeracao([](int geracao, float melhor, float media, float pior) {
    std::cout << "Geração " << geracao << ": " << melhor << std::endl;
});

// Evoluir
populacao.avaliarPopulacao(avaliarRede);
populacao.evoluir();
```

## ⚙️ Configuração

### Parâmetros Principais

```cpp
// Configuração da População
Populacao::Configuracao config;
config.tamanhoPopulacao = 200;    // Tamanho da população
config.taxaMutacao = 0.4f;        // Chance de mutação
config.taxaCruzamento = 0.8f;     // Chance de cruzamento
config.taxaElitismo = 0.1f;       // Percentual de elite
config.limiarCompatibilidade = 1.0f; // Limiar para formar espécies
config.maxEspecies = 15;          // Máximo de espécies

// Configuração NEAT
ConfiguracaoNEAT::COEF_EXCESSO = 1.0f;
ConfiguracaoNEAT::COEF_DISJUNTO = 1.0f;
ConfiguracaoNEAT::COEF_PESO = 0.3f;
ConfiguracaoNEAT::CHANCE_NOVO_NO = 0.05f;
ConfiguracaoNEAT::CHANCE_NOVA_CONEXAO = 0.08f;
```

## 📁 Estrutura do Projeto

```
RedeNeural/
├── include/
│   ├── Rede.h
│   ├── Populacao.h
│   ├── Especie.h
│   ├── Configuracao.h
│   └── Visualizador.h
├── src/
│   ├── Rede.cpp
│   ├── Populacao.cpp
│   ├── Especie.cpp
│   └── Configuracao.cpp
└── docs/
```

## 🆕 Novidades na Versão Atual

- Sistema de espécies aprimorado
- Logs detalhados do processo evolutivo
- Melhor distribuição de slots por espécie
- Parâmetros otimizados para evolução mais eficiente
- Suporte a callbacks para monitoramento
- Codificação UTF-8 para logs

## 🤝 Contribuindo

1. **Faça um Fork do projeto**
2. **Crie sua Feature Branch:**

    ```bash
    git checkout -b feature/NovaFeature
    ```

3. **Commit suas mudanças:**

    ```bash
    git commit -m 'Adiciona nova feature'
    ```

4. **Push para a Branch:**

    ```bash
    git push origin feature/NovaFeature
    ```

5. **Abra um Pull Request**

## 📝 Licença

Este projeto está sob a licença **MIT**. Veja o arquivo LICENSE para mais detalhes.

**Link do Projeto:** [https://github.com/seu-usuario/rede-neural-neat](https://github.com/seu-usuario/rede-neural-neat)

⭐️ **From BrunexCoder**
