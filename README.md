# Semestria

O Semestria é um sistema de acompanhamento acadêmico que permite cadastrar disciplinas, horários, créditos, faltas e avaliações. Com base na carga horária, calcula o limite de ausências e alerta o aluno sobre sua frequência. Também registra notas, estima o desempenho em cada matéria e calcula uma projeção da NSG do semestre de modo simples e claro.

Projeto final da disciplina **Programação e Desenvolvimento de Software II (PDS II)**, DCC/ICEx/UFMG, 2º semestre de 2026.

## Integrantes

| Nome | Matrícula |
| --- | --- |
| Eduarda Lanna | 2024110945 |
| Flávio Jach Maia | 2026020340 |
| Leonardo Matheus Rodrigues Pereira | 2026019872 |
| Lucas Netto Brando Coutinho | 2022059950 |
| Thiago Abreu de Jesus | 2025019909 |

## O problema e a motivação

Ao longo do semestre, o estudante de graduação precisa acompanhar ao mesmo tempo várias disciplinas, cada uma com sua carga horária, seus horários de aula e um conjunto próprio de provas, trabalhos e VPLs. Essas informações costumam ficar espalhadas entre o sistema acadêmico, o Moodle, planilhas e anotações, e o aluno raramente sabe com precisão quantas faltas ainda pode ter em cada matéria, quantos pontos já conquistou, quantos ainda estão em disputa ou como o desempenho atual afeta sua Nota Semestral Global (NSG). Com isso, muitos só percebem que estão perto da reprovação por infrequência ou por nota quando já não há como reverter a situação.

Escolhemos esse tema porque é um problema que vivemos todo semestre. O Semestria reúne esses dados em um único lugar, aplica automaticamente as regras acadêmicas (limite de 25% de faltas e 60 pontos para aprovação) e transforma os números em avisos e projeções claras. Assim o aluno consegue planejar o esforço até o fim do período. O domínio também se presta bem à aplicação dos conceitos da disciplina: modelagem orientada a objetos, encapsulamento, tratamento de exceções, persistência em arquivos texto e testes de unidade.

## Objetivos

- Cadastrar e gerenciar as disciplinas do semestre (código, nome, créditos e carga horária).
- Registrar os horários semanais de aula e impedir choques de horário na grade.
- Controlar as faltas, calcular o limite permitido e alertar quando o aluno chegar a 80% desse limite.
- Lançar avaliações e notas, mostrando os pontos distribuídos, os conquistados e os ainda em disputa.
- Simular notas hipotéticas e projetar a NSG atual, a projetada e a máxima possível.
- Gerar e exportar relatórios com a situação de cada disciplina e os riscos de reprovação.

Os requisitos detalhados estão nas [User Stories](design/user_stories.md) e a modelagem inicial está nos [cartões CRC](design/crc_cards.json).

## Arquitetura

As classes abaixo foram definidas a partir dos cartões CRC. Os contratos (arquivos `.hpp`) ficam em [`include/`](include/) e todo o código está no namespace `semestria`.

| Arquivo | Responsabilidade |
| --- | --- |
| `Horario.hpp` | Intervalo semanal de aula (dia, início, fim, sala) e detecção de colisões. |
| `Avaliacao.hpp` | Prova, trabalho ou VPL: valor, nota obtida ou estimada, data limite e status. |
| `Disciplina.hpp` | Dados da disciplina, avaliações, faltas, horários, notas e situação final. |
| `Semestre.hpp` | Grade do período, conflitos de horário, créditos, carga horária e NSG. |
| `AlertaAcademico.hpp` | Limites regulamentares e avisos de risco por faltas ou por nota. |
| `RelatorioSemestral.hpp` | Grade horária, extrato de notas, histórico de faltas e visão geral, no terminal ou em arquivo. |
| `ValidadorAcademico.hpp` | Validação dos dados de entrada (textos, códigos, notas, horas e datas). |
| `RegrasAcademicas.hpp` | Constantes regulamentares (25% de faltas, 60 pontos, 100 pontos por disciplina). |
| `Excecoes.hpp` | Hierarquia de exceções do sistema, derivada de `SemestriaException`. |

## Estrutura do repositório

```
Semestria/
├── build/      # arquivos gerados na compilação
├── design/     # artefatos de modelagem (User Stories e cartões CRC)
├── include/    # cabeçalhos (.hpp) com os contratos das classes
├── src/        # implementações (.cpp)
├── tests/      # testes de unidade (doctest)
├── Doxyfile    # configuração da documentação
└── README.md
```

## Documentação

A documentação do código é escrita em comentários Doxygen nos cabeçalhos. Para gerá-la, com o [Doxygen](https://www.doxygen.nl/) instalado, execute na raiz do repositório:

```bash
doxygen Doxyfile
```

Depois, abra `docs/html/index.html` no navegador. A pasta `docs/` é gerada automaticamente e não é versionada.

## Tecnologias

- C++11
- Doxygen (documentação)
- doctest (testes de unidade)
- Make (compilação)
