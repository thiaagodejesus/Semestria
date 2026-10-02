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

#ifndef SEMESTRIA_REGRAS_ACADEMICAS_HPP
#define SEMESTRIA_REGRAS_ACADEMICAS_HPP

namespace semestria {

/**
 * @brief Limites regulamentares da universidade.
 */
namespace regras {

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
constexpr double FRACAO_ALERTA_FALTAS = 0.8;

} // namespace regras
} // namespace semestria

#endif // SEMESTRIA_REGRAS_ACADEMICAS_HPP
