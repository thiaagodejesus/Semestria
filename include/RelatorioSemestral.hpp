/**
 * @file RelatorioSemestral.hpp
 * @brief Declaração da classe RelatorioSemestral.
 */

#ifndef SEMESTRIA_RELATORIO_SEMESTRAL_HPP
#define SEMESTRIA_RELATORIO_SEMESTRAL_HPP

#include <ostream>
#include <string>

#include "AlertaAcademico.hpp"
#include "Semestre.hpp"

namespace semestria {

/**
 * @brief Gera os relatórios de acompanhamento do semestre.
 *
 * Coleta os dados consolidados das disciplinas de um Semestre e os
 * formata para exibição no terminal ou exportação para arquivo texto.
 * Cada seção do relatório pode ser gerada separadamente.
 *
 * O relatório guarda uma referência ao semestre: o objeto Semestre deve
 * continuar existindo enquanto o relatório for usado.
 *
 * Colaboradores: Semestre, AlertaAcademico.
 */
// Esta classe só cuida da APRESENTAÇÃO: ela não faz contas novas.
// Pede os números prontos ao Semestre, às Disciplinas e ao
// AlertaAcademico e se preocupa apenas em escrevê-los de forma organizada.
//
// Exemplo de uso:
//     RelatorioSemestral relatorio(semestre);
//     relatorio.gerarVisaoGeral(std::cout);   // mostra na tela
//     relatorio.exportar("relatorio.txt");    // salva em arquivo
class RelatorioSemestral {
public:
    /**
     * @brief Cria um relatório para o semestre informado.
     * @param semestre Semestre cujos dados serão apresentados.
     * @param alerta Monitor usado para identificar disciplinas em risco.
     */
    explicit RelatorioSemestral(const Semestre& semestre,
                                const AlertaAcademico& alerta = AlertaAcademico());

    // Por que os métodos recebem "std::ostream& saida"?
    // std::ostream é qualquer "lugar para onde se pode escrever texto".
    // std::cout (a tela) é um ostream, e um arquivo aberto com
    // std::ofstream também é. Assim, o mesmo método serve tanto para
    // mostrar no terminal quanto para gravar em arquivo, sem repetir código.

    /**
     * @brief Escreve a grade horária semanal, organizada por dia.
     * @param saida Fluxo de saída (ex.: std::cout).
     */
    void gerarGradeHoraria(std::ostream& saida) const;

    /**
     * @brief Escreve o extrato de notas por disciplina e por avaliação.
     *
     * Para cada disciplina mostra as avaliações com valor, nota e status,
     * além dos pontos distribuídos, conquistados e em disputa.
     *
     * @param saida Fluxo de saída.
     */
    void gerarExtratoNotas(std::ostream& saida) const;

    /**
     * @brief Escreve o histórico de faltas versus o limite permitido.
     * @param saida Fluxo de saída.
     */
    void gerarHistoricoFaltas(std::ostream& saida) const;

    /**
     * @brief Escreve a visão geral do semestre.
     *
     * Inclui a situação de cada disciplina, o total de créditos e de
     * horas, a NSG atual, projetada e máxima e a lista de disciplinas em
     * risco de reprovação por nota ou por faltas.
     *
     * @param saida Fluxo de saída.
     */
    void gerarVisaoGeral(std::ostream& saida) const;

    /**
     * @brief Escreve o relatório completo (todas as seções acima).
     * @param saida Fluxo de saída.
     */
    void gerarCompleto(std::ostream& saida) const;

    /**
     * @brief Exporta o relatório completo para um arquivo texto.
     * @param caminhoArquivo Caminho do arquivo a ser criado/sobrescrito.
     * @throws ArquivoException se não for possível escrever no arquivo.
     */
    void exportar(const std::string& caminhoArquivo) const;

private:
    // Guardamos uma REFERÊNCIA ao semestre (o "&"), e não uma cópia. Assim
    // o relatório sempre mostra os dados mais recentes e não duplica
    // tudo na memória. O cuidado é que o Semestre precisa continuar
    // existindo enquanto este relatório for usado.
    const Semestre& semestre_;  ///< Semestre apresentado.
    AlertaAcademico alerta_;    ///< Regras usadas para destacar riscos.
};

} // namespace semestria

#endif // SEMESTRIA_RELATORIO_SEMESTRAL_HPP
