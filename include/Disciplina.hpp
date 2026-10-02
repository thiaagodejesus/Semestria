/**
 * @file Disciplina.hpp
 * @brief Declaração da classe Disciplina e do enum Situacao.
 */

#ifndef SEMESTRIA_DISCIPLINA_HPP
#define SEMESTRIA_DISCIPLINA_HPP

#include <string>
#include <vector>

#include "Avaliacao.hpp"
#include "Horario.hpp"

namespace semestria {

/**
 * @brief Situação do aluno em uma disciplina.
 */
enum class Situacao {
    EmAndamento,              ///< Ainda não é possível definir o resultado.
    Aprovado,                 ///< Nota mínima atingida e frequência suficiente.
    ReprovadoPorNota,         ///< Não é mais possível atingir a nota mínima.
    ReprovadoPorInfrequencia  ///< Limite de faltas ultrapassado.
};

/**
 * @brief Converte a situação para texto (ex.: "Reprovado por Nota").
 * @param situacao Situação da disciplina.
 * @return Descrição em português.
 */
std::string paraTexto(Situacao situacao);

/**
 * @brief Representa uma disciplina cursada pelo aluno no semestre.
 *
 * Conhece os dados cadastrais (código, nome, créditos e carga horária),
 * mantém as avaliações, os horários de aula e as faltas do aluno e, a
 * partir deles, calcula notas, frequência e a situação final.
 *
 * As faltas são contadas em horas-aula, na mesma unidade da carga horária.
 *
 * Colaboradores: Avaliacao, Horario.
 */
class Disciplina {
public:
    /**
     * @brief Cria uma disciplina sem avaliações, horários ou faltas.
     * @param codigo Código da disciplina (ex.: "DCC204").
     * @param nome Nome da disciplina.
     * @param creditos Número de créditos (maior que zero).
     * @param cargaHoraria Carga horária total em horas-aula (maior que zero).
     * @throws DadoInvalidoException se algum dos dados for inválido.
     */
    Disciplina(const std::string& codigo, const std::string& nome,
               int creditos, int cargaHoraria);

    // ----------------------------------------------------------------
    /// @name Dados cadastrais
    /// @{

    /// @return Código da disciplina.
    const std::string& getCodigo() const;

    /// @return Nome da disciplina.
    const std::string& getNome() const;

    /// @return Número de créditos.
    int getCreditos() const;

    /// @return Carga horária total, em horas-aula.
    int getCargaHoraria() const;

    /**
     * @brief Altera o nome da disciplina.
     * @param nome Novo nome.
     * @throws DadoInvalidoException se o nome for vazio.
     */
    void setNome(const std::string& nome);

    /**
     * @brief Altera o número de créditos.
     * @param creditos Novo número de créditos.
     * @throws DadoInvalidoException se @p creditos <= 0.
     */
    void setCreditos(int creditos);

    /**
     * @brief Altera a carga horária.
     * @param cargaHoraria Nova carga horária, em horas-aula.
     * @throws DadoInvalidoException se @p cargaHoraria <= 0.
     */
    void setCargaHoraria(int cargaHoraria);

    /// @}
    // ----------------------------------------------------------------
    /// @name Avaliações
    /// @{

    /**
     * @brief Vincula uma nova avaliação à disciplina.
     * @param avaliacao Avaliação a ser adicionada.
     * @throws AvaliacaoDuplicadaException se já existir avaliação com o
     *         mesmo nome.
     * @throws LimitePontuacaoExcedidoException se a soma dos valores das
     *         avaliações passar de 100 pontos.
     */
    void adicionarAvaliacao(const Avaliacao& avaliacao);

    /**
     * @brief Remove uma avaliação pelo nome.
     * @param nome Nome da avaliação.
     * @throws AvaliacaoNaoEncontradaException se não existir.
     */
    void removerAvaliacao(const std::string& nome);

    /**
     * @brief Busca uma avaliação pelo nome, permitindo alterá-la.
     *
     * Para alterar o valor total de uma avaliação, prefira
     * alterarValorAvaliacao(), que respeita o limite de 100 pontos.
     *
     * @param nome Nome da avaliação.
     * @return Referência para a avaliação encontrada.
     * @throws AvaliacaoNaoEncontradaException se não existir.
     */
    Avaliacao& buscarAvaliacao(const std::string& nome);

    /// @copydoc buscarAvaliacao(const std::string&)
    const Avaliacao& buscarAvaliacao(const std::string& nome) const;

    /**
     * @brief Altera o valor total de uma avaliação respeitando o limite
     * de pontuação da disciplina.
     * @param nome Nome da avaliação.
     * @param novoValor Novo valor total.
     * @throws AvaliacaoNaoEncontradaException se a avaliação não existir.
     * @throws LimitePontuacaoExcedidoException se a soma passar de 100.
     * @throws DadoInvalidoException se o valor for inválido.
     */
    void alterarValorAvaliacao(const std::string& nome, double novoValor);

    /// @return Todas as avaliações vinculadas, na ordem de cadastro.
    const std::vector<Avaliacao>& getAvaliacoes() const;

    /// @return Soma dos valores totais de todas as avaliações cadastradas.
    double pontosDistribuidos() const;

    /// @return Soma das notas oficiais das avaliações já realizadas.
    double pontosConquistados() const;

    /**
     * @brief Pontos que ainda estão em disputa na disciplina.
     *
     * Corresponde a 100 menos o valor das avaliações já realizadas, ou
     * seja, inclui as avaliações pendentes e os pontos ainda não
     * distribuídos pelo professor.
     *
     * @return Pontos ainda em disputa.
     */
    double pontosEmDisputa() const;

    /**
     * @brief Calcula a nota final consolidada.
     * @return Soma das notas das avaliações realizadas (igual a
     *         pontosConquistados()).
     */
    double calcularNotaFinal() const;

    /**
     * @brief Calcula a nota projetada, usada nas simulações.
     * @return Soma das notas realizadas e das notas estimadas.
     */
    double calcularNotaProjetada() const;

    /**
     * @brief Calcula a maior nota ainda possível.
     * @return pontosConquistados() + pontosEmDisputa().
     */
    double calcularNotaMaxima() const;

    /// @}
    // ----------------------------------------------------------------
    /// @name Frequência
    /// @{

    /**
     * @brief Registra faltas na disciplina.
     * @param horas Quantidade de horas-aula de falta (padrão: 1).
     */
    void registrarFalta(unsigned int horas = 1);

    /**
     * @brief Remove faltas registradas (correção de lançamento).
     * @param horas Quantidade de horas-aula a remover (padrão: 1).
     * @throws DadoInvalidoException se @p horas for maior que o total de
     *         faltas registradas.
     */
    void removerFalta(unsigned int horas = 1);

    /// @return Total de faltas registradas, em horas-aula.
    unsigned int getFaltas() const;

    /**
     * @brief Calcula o limite de faltas permitido.
     * @return 25% da carga horária, arredondado para baixo.
     */
    unsigned int limiteFaltas() const;

    /// @return Faltas que ainda podem ser cometidas sem reprovar (mínimo 0).
    unsigned int faltasRestantes() const;

    /**
     * @brief Calcula o percentual de faltas em relação à carga horária.
     * @return Valor entre 0 e 100 (pode passar de 100 em casos extremos).
     */
    double percentualFaltas() const;

    /// @return true se as faltas ultrapassaram o limite permitido.
    bool excedeuLimiteFaltas() const;

    /// @}
    // ----------------------------------------------------------------
    /// @name Horários
    /// @{

    /**
     * @brief Adiciona um horário semanal de aula.
     * @param horario Horário a ser adicionado.
     * @throws ConflitoHorarioException se colidir com outro horário
     *         desta mesma disciplina.
     */
    void adicionarHorario(const Horario& horario);

    /**
     * @brief Remove um horário de aula.
     * @param horario Horário a ser removido (comparado por dia e intervalo).
     * @throws DadoInvalidoException se o horário não estiver cadastrado.
     */
    void removerHorario(const Horario& horario);

    /// @return Horários semanais de aula da disciplina.
    const std::vector<Horario>& getHorarios() const;

    /**
     * @brief Verifica se algum horário desta disciplina colide com algum
     * horário de outra.
     * @param outra Disciplina a ser comparada.
     * @return true se houver choque de horário.
     */
    bool conflitaCom(const Disciplina& outra) const;

    /// @}

    /**
     * @brief Determina a situação do aluno na disciplina.
     *
     * Regras, aplicadas nesta ordem:
     * 1. faltas acima do limite: ReprovadoPorInfrequencia;
     * 2. nota máxima possível abaixo de 60: ReprovadoPorNota;
     * 3. todas as avaliações realizadas e 100 pontos distribuídos:
     *    Aprovado (nota >= 60);
     * 4. caso contrário: EmAndamento.
     *
     * @return Situação atual.
     */
    Situacao situacao() const;

private:
    std::string codigo_;                 ///< Código único no semestre.
    std::string nome_;                   ///< Nome da disciplina.
    int creditos_;                       ///< Número de créditos.
    int cargaHoraria_;                   ///< Carga horária em horas-aula.
    unsigned int faltas_;                ///< Faltas em horas-aula.
    std::vector<Avaliacao> avaliacoes_;  ///< Avaliações vinculadas.
    std::vector<Horario> horarios_;      ///< Horários semanais de aula.
};

} // namespace semestria

#endif // SEMESTRIA_DISCIPLINA_HPP
