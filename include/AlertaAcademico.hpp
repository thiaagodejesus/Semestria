/**
 * @file AlertaAcademico.hpp
 * @brief Declaração da classe AlertaAcademico e da estrutura Aviso.
 */

#ifndef SEMESTRIA_ALERTA_ACADEMICO_HPP
#define SEMESTRIA_ALERTA_ACADEMICO_HPP

#include <string>
#include <vector>

#include "Disciplina.hpp"
#include "RegrasAcademicas.hpp"
#include "Semestre.hpp"

namespace semestria {

/**
 * @brief Motivo de um aviso emitido ao aluno.
 */
enum class TipoAviso {
    RiscoInfrequencia,       ///< Faltas atingiram a fração de alerta do limite.
    ReprovacaoInfrequencia,  ///< Faltas ultrapassaram o limite.
    RiscoNota,               ///< Aproveitamento abaixo do mínimo de aprovação.
    ReprovacaoNota           ///< Não é mais possível atingir a nota mínima.
};

/**
 * @brief Converte o tipo de aviso para texto.
 * @param tipo Tipo do aviso.
 * @return Descrição em português (ex.: "Risco de infrequência").
 */
std::string paraTexto(TipoAviso tipo);

/**
 * @brief Aviso gerado pelo AlertaAcademico para uma disciplina.
 */
// Exemplo de aviso:
//   tipo             = TipoAviso::RiscoInfrequencia
//   codigoDisciplina = "DCC204"
//   mensagem         = "Você já usou 12 das 15 horas de falta permitidas."
struct Aviso {
    TipoAviso tipo;                ///< Motivo do aviso.
    std::string codigoDisciplina;  ///< Disciplina a que o aviso se refere.
    std::string mensagem;          ///< Texto explicativo para o aluno.
};

/**
 * @brief Monitora frequência e notas e gera avisos de risco de reprovação.
 *
 * Conhece os limites regulamentares (percentual máximo de faltas, nota
 * mínima de aprovação e a fração do limite de faltas que dispara o alerta).
 * Por padrão usa os valores de RegrasAcademicas.hpp, mas eles podem ser
 * ajustados no construtor.
 *
 * Colaboradores: Disciplina, Avaliacao.
 */
// Pense nesta classe como um "fiscal": ela não altera nada, só olha as
// disciplinas e diz se há algo preocupando. Por isso quase todos os
// métodos recebem uma disciplina como "const Disciplina&" (só leitura).
class AlertaAcademico {
public:
    /**
     * @brief Cria um monitor com os limites informados.
     * @param percentualMaximoFaltas Fração máxima de faltas (0 a 1).
     * @param notaMinimaAprovacao Nota mínima para aprovação (0 a 100).
     * @param fracaoAlertaFaltas Fração do limite de faltas que dispara o
     *        aviso de risco (0 a 1).
     * @throws DadoInvalidoException se algum limite estiver fora do intervalo.
     */
    // Os três parâmetros têm valor padrão (depois do "="). Então:
    //   AlertaAcademico alerta;               // usa as regras padrão
    //   AlertaAcademico rigido(0.25, 70.0);   // exige 70 pontos para aprovar
    explicit AlertaAcademico(
        double percentualMaximoFaltas = regras::PERCENTUAL_MAXIMO_FALTAS,
        double notaMinimaAprovacao = regras::NOTA_MINIMA_APROVACAO,
        double fracaoAlertaFaltas = regras::FRACAO_ALERTA_FALTAS);

    /// @return Fração máxima de faltas permitida.
    double getPercentualMaximoFaltas() const;

    /// @return Nota mínima de aprovação.
    double getNotaMinimaAprovacao() const;

    /// @return Fração do limite de faltas que dispara o alerta.
    double getFracaoAlertaFaltas() const;

    /**
     * @brief Calcula o limite de faltas de uma disciplina com estes limites.
     * @param disciplina Disciplina analisada.
     * @return Limite de faltas em horas-aula.
     */
    unsigned int limiteFaltas(const Disciplina& disciplina) const;

    /**
     * @brief Verifica se o aluno está próximo do teto de faltas.
     * @param disciplina Disciplina analisada.
     * @return true se as faltas atingiram a fração de alerta do limite,
     *         sem ainda ultrapassá-lo.
     */
    // Ex.: limite de 15 horas -> alerta a partir de 12 horas de falta
    // (80% de 15), até 15. Com 16 ou mais, já é reprovação.
    bool emRiscoDeInfrequencia(const Disciplina& disciplina) const;

    /**
     * @brief Verifica se o aluno já ultrapassou o limite de faltas.
     * @param disciplina Disciplina analisada.
     * @return true se as faltas passaram do limite.
     */
    bool reprovadoPorInfrequencia(const Disciplina& disciplina) const;

    /**
     * @brief Calcula quantos pontos o aluno ainda precisa para ser aprovado.
     * @param disciplina Disciplina analisada.
     * @return Pontos que faltam para atingir a nota mínima (0 se já atingiu).
     *         Se o valor for maior que Disciplina::pontosEmDisputa(), a
     *         aprovação não é mais possível.
     */
    // Ex.: o aluno já tem 35 pontos -> precisa de mais 60 - 35 = 25.
    // Se só restam 20 pontos em disputa, não há mais como passar.
    double pontuacaoNecessaria(const Disciplina& disciplina) const;

    /**
     * @brief Verifica se há risco iminente de reprovação por nota.
     *
     * Há risco quando o aproveitamento nas avaliações já realizadas
     * (pontos conquistados / valor das avaliações realizadas) está abaixo
     * da nota mínima de aprovação em termos percentuais.
     *
     * @param disciplina Disciplina analisada.
     * @return true se houver risco.
     */
    // Ex.: o aluno fez provas que somavam 40 pontos e tirou 20 nelas.
    // Aproveitamento = 20 / 40 = 50%, abaixo de 60%, então há risco:
    // mantendo esse ritmo, ele termina o semestre com 50 pontos.
    bool emRiscoPorNota(const Disciplina& disciplina) const;

    /**
     * @brief Verifica se a aprovação por nota já não é possível.
     * @param disciplina Disciplina analisada.
     * @return true se a nota máxima possível for menor que a nota mínima.
     */
    bool reprovadoPorNota(const Disciplina& disciplina) const;

    // Os dois métodos "verificar" têm o mesmo nome, mas recebem tipos
    // diferentes (sobrecarga). O C++ escolhe qual chamar pelo argumento:
    //   alerta.verificar(pds2);      // avisos de uma disciplina
    //   alerta.verificar(semestre);  // avisos de todas as disciplinas

    /**
     * @brief Gera todos os avisos aplicáveis a uma disciplina.
     * @param disciplina Disciplina analisada.
     * @return Lista de avisos (vazia se não houver riscos).
     */
    std::vector<Aviso> verificar(const Disciplina& disciplina) const;

    /**
     * @brief Gera os avisos de todas as disciplinas do semestre.
     * @param semestre Semestre analisado.
     * @return Lista de avisos de todas as disciplinas.
     */
    std::vector<Aviso> verificar(const Semestre& semestre) const;

private:
    double percentualMaximoFaltas_;  ///< Fração máxima de faltas.
    double notaMinimaAprovacao_;     ///< Nota mínima de aprovação.
    double fracaoAlertaFaltas_;      ///< Fração do limite que dispara alerta.
};

} // namespace semestria

#endif // SEMESTRIA_ALERTA_ACADEMICO_HPP
