#include "../include/Populacao.h"
#include <algorithm>
#include <iostream>
#include <random>

namespace NEAT {

Populacao::Populacao(int numEntradas, int numSaidas, const Configuracao& config)
    : config(config), geracao(0), melhorAptidao(0) {
    
    // Criar população inicial
    for (int i = 0; i < config.tamanhoPopulacao; i++) {
        individuos.emplace_back(numEntradas, numSaidas);
    }
}

void Populacao::evoluir() {
    std::wcout << L"\n=== Iniciando Evolução da População ===" << std::endl;
    std::wcout << L"Tamanho da população: " << individuos.size() << std::endl;
    
    // Ordenar por aptidão
    std::sort(individuos.begin(), individuos.end(),
        [](const Rede& a, const Rede& b) {
            return a.obterAptidao() > b.obterAptidao();
        });
    
    // Especiar a população
    especies.clear();
    for (auto& individuo : individuos) {
        bool encontrouEspecie = false;
        
        // Tentar adicionar a uma espécie existente
        for (auto& especie : especies) {
            if (especie.verificarCompatibilidade(individuo)) {
                especie.adicionarMembro(&individuo);
                encontrouEspecie = true;
                break;
            }
        }
        
        // Se não encontrou espécie compatível, criar nova espécie
        if (!encontrouEspecie && especies.size() < config.maxEspecies) {
            especies.emplace_back(individuo);
            especies.back().adicionarMembro(&individuo);
        }
    }
    
    // Ajustar aptidões
    for (auto& especie : especies) {
        especie.calcularAptidaoAjustada();
    }
    
    // Criar nova geração mantendo o tamanho original
    std::vector<Rede> novaGeracao;
    novaGeracao.reserve(config.tamanhoPopulacao);
    
    // Calcular total de aptidão ajustada de todas as espécies
    float somaAptidoesEspecies = 0;
    for (const auto& especie : especies) {
        somaAptidoesEspecies += especie.obterAptidaoAjustada();
    }
    
    // Distribuir slots proporcionalmente à aptidão de cada espécie
    int slotsRestantes = config.tamanhoPopulacao;
    
    // Garantir que cada espécie tenha pelo menos um slot (elitismo)
    for (const auto& especie : especies) {
        if (!especie.obterMembros().empty()) {
            auto melhorDaEspecie = std::max_element(
                especie.obterMembros().begin(),
                especie.obterMembros().end(),
                [](const Rede* a, const Rede* b) {
                    return a->obterAptidao() < b->obterAptidao();
                });
            novaGeracao.push_back(**melhorDaEspecie);
            slotsRestantes--;
        }
    }
    
    // Distribuir slots restantes proporcionalmente
    for (const auto& especie : especies) {
        if (slotsRestantes <= 0) break;
        
        std::wcout << L"\nProcessando espécie com " 
                   << especie.obterMembros().size() << L" membros" << std::endl;
        std::wcout << L"Aptidão média da espécie: " 
                   << especie.obterAptidaoAjustada() << std::endl;
        
        int slotsEspecie = static_cast<int>(
            (especie.obterAptidaoAjustada() / somaAptidoesEspecies) * slotsRestantes
        );
        
        std::wcout << L"Slots alocados para esta espécie: " << slotsEspecie << std::endl;
        
        for (int i = 0; i < slotsEspecie && novaGeracao.size() < config.tamanhoPopulacao; i++) {
            if (especie.obterMembros().empty()) continue;
            
            if ((float)rand() / RAND_MAX < config.taxaCruzamento) {
                // Cruzamento
                const Rede* pai1 = especie.obterMembros()[
                    rand() % especie.obterMembros().size()];
                const Rede* pai2 = especie.obterMembros()[
                    rand() % especie.obterMembros().size()];
                
                Rede filho = cruzarRedes(*pai1, *pai2);
                if ((float)rand() / RAND_MAX < config.taxaMutacao) {
                    filho.mutar();
                }
                novaGeracao.push_back(filho);
            } else {
                std::wcout << L"Criando indivíduo por mutação direta" << std::endl;
                // Mutação
                Rede filho = *especie.obterMembros()[
                    rand() % especie.obterMembros().size()];
                filho.mutar();
                novaGeracao.push_back(filho);
            }
        }
    }
    
    // Preencher slots restantes com cópias dos melhores
    while (novaGeracao.size() < config.tamanhoPopulacao) {
        novaGeracao.push_back(individuos[0]); // Copiar o melhor indivíduo
    }
    
    std::wcout << L"\n=== Evolução Concluída ===" << std::endl;
    std::wcout << L"Nova população criada com " << novaGeracao.size() << L" indivíduos" << std::endl;
    std::wcout << L"--------------------------------" << std::endl;
    
    // Atualizar população
    individuos = std::move(novaGeracao);
    geracao++;

    // Notificar callback se existir
    if (onGeracaoCallback) {
        onGeracaoCallback(geracao, 
                         melhorAptidao, 
                         aptidaoMedia, 
                         aptidaoMinima);
    }
}

void Populacao::avaliarPopulacao(std::function<float(Rede&)> funcaoAvaliacao) {
    for (auto& individuo : individuos) {
        float aptidao = funcaoAvaliacao(individuo);
        individuo.definirAptidao(aptidao);
    }
}

Rede* Populacao::selecaoTorneio(int tamanhoTorneio) {
    float melhorAptidao = -1.0f;
    Rede* melhorRede = nullptr;
    
    for (int i = 0; i < tamanhoTorneio; i++) {
        int idx = rand() % individuos.size();
        float aptidao = individuos[idx].obterAptidao();
        if (aptidao > melhorAptidao) {
            melhorAptidao = aptidao;
            melhorRede = &individuos[idx];
        }
    }
    
    return melhorRede;
}

Rede Populacao::cruzarRedes(const Rede& rede1, const Rede& rede2) {
    std::wcout << L"\n=== Iniciando Cruzamento ===" << std::endl;
    std::wcout << L"Pai 1 - Aptidão: " << rede1.obterAptidao() << std::endl;
    std::wcout << L"Pai 2 - Aptidão: " << rede2.obterAptidao() << std::endl;
    
    // Criar uma nova rede com a mesma estrutura básica
    Rede filho(0, 0); // Os números serão ignorados pois vamos copiar a estrutura
    
    // Copiar nós
    filho.definirNos(rede1.obterNos());
    
    // Cruzar conexões
    const auto& conexoes1 = rede1.obterConexoes();
    const auto& conexoes2 = rede2.obterConexoes();
    
    std::vector<Conexao> novasConexoes;
    int conexoesHerdadasPai1 = 0;
    int conexoesHerdadasPai2 = 0;
    
    for (size_t i = 0; i < conexoes1.size(); i++) {
        if (rand() % 2 == 0) {
            novasConexoes.push_back(conexoes1[i]);
            conexoesHerdadasPai1++;
        } else {
            novasConexoes.push_back(conexoes2[i]);
            conexoesHerdadasPai2++;
        }
    }
    
    filho.definirConexoes(novasConexoes);
    
    std::wcout << L"Conexões herdadas do Pai 1: " << conexoesHerdadasPai1 << std::endl;
    std::wcout << L"Conexões herdadas do Pai 2: " << conexoesHerdadasPai2 << std::endl;
    std::wcout << L"Total de conexões do filho: " << novasConexoes.size() << std::endl;
    
    // Verificar se haverá mutação
    float chanceMutacao = static_cast<float>(rand()) / RAND_MAX;
    if (chanceMutacao < config.taxaMutacao) {
        std::wcout << L"Filho será mutado (chance: " << chanceMutacao << L")" << std::endl;
    }
    
    std::wcout << L"=== Cruzamento Concluído ===" << std::endl;
    std::wcout << L"--------------------------------" << std::endl;
    
    return filho;
}

void Populacao::selecao() {
    // Ordenar indivíduos por aptidão
    std::sort(individuos.begin(), individuos.end(),
        [](const Rede& a, const Rede& b) {
            return a.obterAptidao() > b.obterAptidao();
        });
}

void Populacao::cruzamento() {
    std::vector<Rede> novaGeracao;
    
    // Preservar os melhores (elitismo)
    int numElite = static_cast<int>(config.tamanhoPopulacao * config.taxaElitismo);
    for (int i = 0; i < numElite; i++) {
        novaGeracao.push_back(individuos[i]);
    }
    
    // Preencher o resto com cruzamentos
    while (novaGeracao.size() < config.tamanhoPopulacao) {
        Rede* pai1 = selecaoTorneio(config.tamanhoTorneio);
        Rede* pai2 = selecaoTorneio(config.tamanhoTorneio);
        if (pai1 && pai2) {
            Rede filho = cruzarRedes(*pai1, *pai2);
            if ((float)rand() / RAND_MAX < config.taxaMutacao) {
                filho.mutar();
            }
            novaGeracao.push_back(filho);
        }
    }
    
    individuos = std::move(novaGeracao);
}

void Populacao::mutacao() {
    for (auto& individuo : individuos) {
        if ((float)rand() / RAND_MAX < config.taxaMutacao) {
            individuo.mutar();
        }
    }
}

void Populacao::especiar() {
    especies.clear();
    
    for (auto& individuo : individuos) {
        bool encontrouEspecie = false;
        
        for (auto& especie : especies) {
            if (especie.verificarCompatibilidade(individuo)) {
                especie.adicionarMembro(&individuo);
                encontrouEspecie = true;
                break;
            }
        }
        
        if (!encontrouEspecie && especies.size() < config.maxEspecies) {
            especies.emplace_back(individuo);
        }
    }
}

void Populacao::ajustarAptidoes() {
    for (auto& especie : especies) {
        especie.calcularAptidaoAjustada();
    }
}

void Populacao::eliminarEspeciesFracas() {
    especies.erase(
        std::remove_if(especies.begin(), especies.end(),
            [this](const Especie& especie) {
                return especie.obterAptidaoAjustada() < melhorAptidao * 0.2f;
            }),
        especies.end()
    );
}

void Populacao::salvarMelhorRede(const std::string& arquivo) {
    if (individuos.empty()) return;
    
    // Encontrar a melhor rede
    auto melhor = std::max_element(individuos.begin(), individuos.end(),
        [](const Rede& a, const Rede& b) {
            return a.obterAptidao() < b.obterAptidao();
        });
        
    melhor->salvar(arquivo);
}

void Populacao::carregarMelhorRede(const std::string& arquivo) {
    if (individuos.empty()) {
        individuos.emplace_back(0, 0);
    }
    individuos[0].carregar(arquivo);
}

} // namespace NEAT 