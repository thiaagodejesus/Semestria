/**
 * @file Avaliacao.hpp
 * @brief Declaração da classe Avaliacao e do enum StatusAvaliacao.
 */

#ifndef SEMESTRIA_AVALIACAO_HPP
#define SEMESTRIA_AVALIACAO_HPP

#include <string>

namespace semestria {

/**
 * @brief Situação de uma avaliação em relação à nota.
 */
// Ciclo de vida de uma avaliação:
//
//   Pendente --definirEstimativa()--> Estimada --registrarNota()--> Realizada
//      ^                                 |                              |
//      +-------removerEstimativa()-------+                              |
//      +-------------------------cancelarRegistro()---------------------+
//
// (registrarNota() também pode ser chamado direto de Pendente.)
//
// "Estimada" serve para simulações: o aluno chuta quanto acha que vai
// tirar e o sistema mostra como ficariam a nota final e a NSG.
enum class StatusAvaliacao {
    Pendente,  ///< Ainda não realizada e sem nota estimada.
    Estimada,  ///< Ainda não realizada, mas com nota hipotética (simulação).
    Realizada  ///< Já realizada, com nota oficial lançada.
};

/**
 * @brief Converte o status de uma avaliação para texto.
 * @param status Status da avaliação.
 * @return "Pendente", "Estimada" ou "Realizada".
 */
std::string paraTexto(StatusAvaliacao status);

/**
 * @brief Representa uma avaliação (prova, trabalho, VPL etc.) de uma disciplina.
 *
 * Guarda o nome, o valor total, a data limite e a nota do aluno. A nota
 * pode ser oficial (avaliação realizada) ou uma estimativa usada nas
 * simulações de projeção de nota e de NSG.
 *
 * Colaboradores: Disciplina, ValidadorAcademico.
 */
// Exemplo de uso:
//     Avaliacao p1("Prova 1", 30.0, "20/10/2026");  // vale 30 pontos
//     p1.definirEstimativa(25.0);  // "acho que vou tirar 25"
//     p1.registrarNota(22.5);      // saiu a nota oficial: 22,5
class Avaliacao {
public:
    /**
     * @brief Cria uma avaliação ainda pendente.
     * @param nome Nome descritivo (ex.: "Prova 1", "Trabalho Final").
     * @param valorTotal Pontuação máxima da avaliação.
     * @param dataLimite Data de entrega/realização no formato "DD/MM/AAAA"
     *        (opcional; vazio significa sem data definida).
     * @throws DadoInvalidoException se o nome for vazio, se o valor não
     *         estiver entre 0 (exclusivo) e 100 ou se a data for inválida.
     */
    Avaliacao(const std::string& nome, double valorTotal,
              const std::string& dataLimite = "");

    /// @return Nome descritivo da avaliação.
    const std::string& getNome() const;

    /// @return Valor total (pontuação máxima) da avaliação.
    double getValorTotal() const;

    /// @return Data limite no formato "DD/MM/AAAA" (vazia se não definida).
    const std::string& getDataLimite() const;

    /**
     * @brief Retorna a nota registrada na avaliação.
     * @return Nota oficial se realizada, nota estimada se houver
     *         estimativa ou 0 se estiver pendente.
     */
    double getNotaObtida() const;

    /// @return Status atual da avaliação.
    StatusAvaliacao getStatus() const;

    /// @return true se a avaliação já foi realizada (nota oficial lançada).
    bool isRealizada() const;

    /// @return true se a nota atual for apenas uma estimativa.
    bool isEstimativa() const;

    // Os métodos "set" alteram um dado, mas sempre validam antes. Se o
    // valor for inválido, lançam uma exceção (ver Excecoes.hpp) e o
    // objeto continua como estava.

    /**
     * @brief Altera o nome da avaliação.
     * @param nome Novo nome.
     * @throws DadoInvalidoException se o nome for vazio.
     */
    void setNome(const std::string& nome);

    /**
     * @brief Altera o valor total da avaliação.
     * @param valorTotal Novo valor total.
     * @throws DadoInvalidoException se o valor for inválido.
     * @throws NotaInvalidaException se a nota já registrada passar a
     *         exceder o novo valor.
     */
    void setValorTotal(double valorTotal);

    /**
     * @brief Altera a data limite da avaliação.
     * @param dataLimite Nova data no formato "DD/MM/AAAA" (ou vazia).
     * @throws DadoInvalidoException se a data for inválida.
     */
    void setDataLimite(const std::string& dataLimite);

    /**
     * @brief Lança a nota oficial e marca a avaliação como realizada.
     * @param nota Nota obtida pelo aluno.
     * @throws NotaInvalidaException se a nota for negativa ou maior que o
     *         valor total.
     */
    // Ex.: numa prova de 30 pontos, registrarNota(35) lança exceção.
    void registrarNota(double nota);

    /**
     * @brief Define uma nota hipotética para simulação.
     *
     * Só altera avaliações que ainda não foram realizadas.
     *
     * @param nota Nota estimada.
     * @throws NotaInvalidaException se a nota for negativa ou maior que o
     *         valor total.
     * @throws SemestriaException se a avaliação já tiver sido realizada.
     */
    void definirEstimativa(double nota);

    /**
     * @brief Remove a nota estimada, voltando a avaliação para pendente.
     *
     * Não tem efeito sobre avaliações realizadas.
     */
    void removerEstimativa();

    /**
     * @brief Desfaz o lançamento da nota oficial, voltando a avaliação
     * para pendente (útil para corrigir lançamentos errados).
     */
    void cancelarRegistro();

private:
    std::string nome_;        ///< Nome descritivo.
    double valorTotal_;       ///< Pontuação máxima.
    // Um único campo guarda a nota, seja ela oficial ou estimada; quem
    // diz qual dos dois casos vale é o status_ logo abaixo.
    double nota_;             ///< Nota oficial ou estimada.
    std::string dataLimite_;  ///< Data limite ("DD/MM/AAAA").
    StatusAvaliacao status_;  ///< Situação da nota.
};

} // namespace semestria

#endif // SEMESTRIA_AVALIACAO_HPP
