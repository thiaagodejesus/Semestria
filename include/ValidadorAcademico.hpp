/**
 * @file ValidadorAcademico.hpp
 * @brief Funções de validação dos dados de entrada do Semestria.
 */

#ifndef SEMESTRIA_VALIDADOR_ACADEMICO_HPP
#define SEMESTRIA_VALIDADOR_ACADEMICO_HPP

#include <string>

namespace semestria {

/**
 * @brief Reúne as regras de validação dos dados acadêmicos.
 *
 * Classe utilitária (somente métodos estáticos, não instanciável) usada
 * pelas demais classes para verificar entradas antes de alterar seu
 * estado. Os métodos apenas respondem se o dado é válido; cabe a quem
 * chama decidir qual exceção lançar.
 */
class ValidadorAcademico {
public:
    ValidadorAcademico() = delete;

    /**
     * @brief Verifica se um texto (nome, sala etc.) não é vazio.
     * @param texto Texto a ser verificado.
     * @return true se houver ao menos um caractere diferente de espaço.
     */
    static bool textoValido(const std::string& texto);

    /**
     * @brief Verifica se o código de uma disciplina é válido.
     *
     * Um código válido não é vazio e contém apenas letras e dígitos
     * (ex.: "DCC204", "MAT001").
     *
     * @param codigo Código a ser verificado.
     * @return true se o código for válido.
     */
    static bool codigoDisciplinaValido(const std::string& codigo);

    /**
     * @brief Verifica se a quantidade de créditos é válida.
     * @param creditos Número de créditos.
     * @return true se @p creditos for maior que zero.
     */
    static bool creditosValidos(int creditos);

    /**
     * @brief Verifica se a carga horária é válida.
     * @param cargaHoraria Carga horária total, em horas-aula.
     * @return true se @p cargaHoraria for maior que zero.
     */
    static bool cargaHorariaValida(int cargaHoraria);

    /**
     * @brief Verifica se o valor (pontuação máxima) de uma avaliação é válido.
     * @param valor Valor da avaliação.
     * @return true se 0 < @p valor <= regras::PONTUACAO_MAXIMA_DISCIPLINA.
     */
    static bool valorAvaliacaoValido(double valor);

    /**
     * @brief Verifica se uma nota é compatível com o valor da avaliação.
     * @param nota Nota obtida (ou estimada).
     * @param valorMaximo Valor total da avaliação.
     * @return true se 0 <= @p nota <= @p valorMaximo.
     */
    static bool notaValida(double nota, double valorMaximo);

    /**
     * @brief Verifica se uma hora do dia é válida.
     * @param hora Hora (0 a 23).
     * @param minuto Minuto (0 a 59).
     * @return true se ambos estiverem dentro dos intervalos.
     */
    static bool horaValida(int hora, int minuto);

    /**
     * @brief Verifica se uma data está no formato "DD/MM/AAAA" e existe no
     * calendário (considerando anos bissextos).
     * @param data Data a ser verificada.
     * @return true se a data for válida.
     */
    static bool dataValida(const std::string& data);
};

} // namespace semestria

#endif // SEMESTRIA_VALIDADOR_ACADEMICO_HPP
