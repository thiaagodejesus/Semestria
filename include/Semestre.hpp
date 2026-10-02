/**
 * @file Semestre.hpp
 * @brief Declaração da classe Semestre e da estrutura ConflitoHorario.
 */

#ifndef SEMESTRIA_SEMESTRE_HPP
#define SEMESTRIA_SEMESTRE_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "Disciplina.hpp"
#include "Horario.hpp"

namespace semestria {

/**
 * @brief Descreve um choque de horário entre duas disciplinas.
 */
struct ConflitoHorario {
    std::string codigoDisciplinaA;  ///< Código da primeira disciplina.
    std::string codigoDisciplinaB;  ///< Código da segunda disciplina.
    Horario horarioA;               ///< Aula da primeira disciplina.
    Horario horarioB;               ///< Aula da segunda disciplina.
};

/**
 * @brief Representa um período letivo (ex.: "2026/2") e sua grade.
 *
 * Armazena as disciplinas cursadas, garante que não haja códigos
 * repetidos nem choques de horário e calcula os indicadores do período:
 * total de créditos, carga horária e projeções da NSG.
 *
 * A NSG (Nota Semestral Global) é a média das notas das disciplinas
 * ponderada pelos créditos:
 *
 *     NSG = soma(nota_i * creditos_i) / soma(creditos_i)
 *
 * Colaboradores: Disciplina, Horario.
 */
class Semestre {
public:
    /**
     * @brief Cria um semestre sem disciplinas.
     * @param identificador Identificador do período (ex.: "2026/2").
     * @throws DadoInvalidoException se o identificador for vazio.
     */
    explicit Semestre(const std::string& identificador);

    /// @return Identificador do período.
    const std::string& getIdentificador() const;

    // ----------------------------------------------------------------
    /// @name Grade de disciplinas
    /// @{

    /**
     * @brief Adiciona uma disciplina à grade do semestre.
     * @param disciplina Disciplina a ser adicionada.
     * @throws DisciplinaDuplicadaException se já houver disciplina com o
     *         mesmo código.
     * @throws ConflitoHorarioException se algum horário da disciplina
     *         colidir com outra disciplina já cadastrada.
     */
    void adicionarDisciplina(const Disciplina& disciplina);

    /**
     * @brief Remove uma disciplina da grade.
     * @param codigo Código da disciplina.
     * @throws DisciplinaNaoEncontradaException se não existir.
     */
    void removerDisciplina(const std::string& codigo);

    /**
     * @brief Busca uma disciplina pelo código, permitindo alterá-la.
     *
     * Para incluir horários, prefira adicionarHorario(), que verifica
     * choques com as demais disciplinas do semestre.
     *
     * @param codigo Código da disciplina.
     * @return Referência para a disciplina encontrada.
     * @throws DisciplinaNaoEncontradaException se não existir.
     */
    Disciplina& buscarDisciplina(const std::string& codigo);

    /// @copydoc buscarDisciplina(const std::string&)
    const Disciplina& buscarDisciplina(const std::string& codigo) const;

    /**
     * @brief Verifica se uma disciplina está cadastrada.
     * @param codigo Código da disciplina.
     * @return true se existir disciplina com esse código.
     */
    bool possuiDisciplina(const std::string& codigo) const;

    /// @return Disciplinas do semestre, na ordem de cadastro.
    const std::vector<Disciplina>& getDisciplinas() const;

    /// @return Quantidade de disciplinas cadastradas.
    std::size_t quantidadeDisciplinas() const;

    /**
     * @brief Adiciona um horário de aula a uma disciplina do semestre.
     * @param codigo Código da disciplina.
     * @param horario Horário a ser adicionado.
     * @throws DisciplinaNaoEncontradaException se a disciplina não existir.
     * @throws ConflitoHorarioException se o horário colidir com qualquer
     *         aula já cadastrada no semestre.
     */
    void adicionarHorario(const std::string& codigo, const Horario& horario);

    /**
     * @brief Lista todos os choques de horário existentes na grade.
     * @return Conflitos encontrados (vazio se a grade estiver consistente).
     */
    std::vector<ConflitoHorario> identificarConflitos() const;

    /**
     * @brief Monta a grade semanal com todas as aulas do semestre.
     * @return Pares (código da disciplina, horário) ordenados por dia e
     *         horário de início.
     */
    std::vector<std::pair<std::string, Horario>> gradeSemanal() const;

    /// @}
    // ----------------------------------------------------------------
    /// @name Indicadores do semestre
    /// @{

    /// @return Soma dos créditos de todas as disciplinas.
    int totalCreditos() const;

    /// @return Soma das cargas horárias de todas as disciplinas (horas-aula).
    int cargaHorariaTotal() const;

    /**
     * @brief NSG considerando apenas os pontos já consolidados.
     * @return NSG calculada com Disciplina::calcularNotaFinal(); 0 se não
     *         houver disciplinas.
     */
    double calcularNSGAtual() const;

    /**
     * @brief NSG simulada com as notas realizadas e estimadas.
     * @return NSG calculada com Disciplina::calcularNotaProjetada(); 0 se
     *         não houver disciplinas.
     */
    double calcularNSGProjetada() const;

    /**
     * @brief Maior NSG ainda possível no semestre.
     * @return NSG calculada com Disciplina::calcularNotaMaxima(); 0 se não
     *         houver disciplinas.
     */
    double calcularNSGMaxima() const;

    /// @}

private:
    std::string identificador_;            ///< Identificador do período.
    std::vector<Disciplina> disciplinas_;  ///< Grade do semestre.
};

} // namespace semestria

#endif // SEMESTRIA_SEMESTRE_HPP
