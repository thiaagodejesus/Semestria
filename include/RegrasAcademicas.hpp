/**
 * @file RegrasAcademicas.hpp
 * @brief Constantes com os limites regulamentares usados pelo Semestria.
 *
 * Centraliza os valores definidos pelas normas acadêmicas (frequência
 * mínima, nota de aprovação, pontuação máxima de uma disciplina etc.),
 * evitando "números mágicos" espalhados pelo código. As classes
 * @ref semestria::Disciplina e @ref semestria::AlertaAcademico consultam
 * estes valores.
 */

// O QUE É UM ARQUIVO .hpp?
// É o "contrato" do código: ele diz O QUE existe (classes, funções e o
// que cada uma recebe e devolve), mas não diz COMO funciona. O "como"
// fica no .cpp de mesmo nome, dentro de src/. Assim, quem só quer usar
// uma classe lê o .hpp e não precisa entender a implementação.
//
// Ordem sugerida de leitura dos cabeçalhos (do mais simples ao mais
// completo): RegrasAcademicas -> Excecoes -> ValidadorAcademico ->
// Horario -> Avaliacao -> Disciplina -> Semestre -> AlertaAcademico ->
// RelatorioSemestral.

// As três linhas #ifndef / #define / #endif são a "guarda de inclusão".
// Se dois arquivos derem #include neste mesmo cabeçalho, o compilador
// só lê o conteúdo uma vez. Sem isso, ele reclamaria de coisas
// declaradas em dobro. Todo .hpp do projeto começa assim.
#ifndef SEMESTRIA_REGRAS_ACADEMICAS_HPP
#define SEMESTRIA_REGRAS_ACADEMICAS_HPP

// "namespace" funciona como um sobrenome. Tudo do projeto fica dentro de
// "semestria", para não dar conflito com nomes de outras bibliotecas.
// Fora daqui, escrevemos, por exemplo: semestria::regras::NOTA_MINIMA_APROVACAO
namespace semestria {

/**
 * @brief Limites regulamentares da universidade.
 */
namespace regras {

// "constexpr" cria uma constante cujo valor é conhecido já na
// compilação e nunca muda. Se a universidade mudar uma regra, basta
// alterar o número aqui e o sistema inteiro passa a usar o novo valor.

/// Pontuação máxima que pode ser distribuída em uma disciplina.
constexpr double PONTUACAO_MAXIMA_DISCIPLINA = 100.0;

/// Nota mínima (em pontos, de 0 a 100) para aprovação em uma disciplina.
constexpr double NOTA_MINIMA_APROVACAO = 60.0;

/// Fração máxima da carga horária que o aluno pode faltar (25%).
constexpr double PERCENTUAL_MAXIMO_FALTAS = 0.25;

/**
 * @brief Fração do limite de faltas a partir da qual o aluno é alertado.
 *
 * Com o valor 0.8, o alerta de risco de infrequência é emitido quando o
 * aluno já usou 80% das faltas permitidas.
 */
// Exemplo: disciplina de 60 horas -> pode faltar 25% = 15 horas.
// O alerta aparece a partir de 80% de 15 = 12 horas de falta.
constexpr double FRACAO_ALERTA_FALTAS = 0.8;

} // namespace regras
} // namespace semestria

#endif // SEMESTRIA_REGRAS_ACADEMICAS_HPP
