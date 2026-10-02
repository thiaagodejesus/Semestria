/**
 * @file Horario.hpp
 * @brief Declaração da classe Horario e do enum DiaSemana.
 */

#ifndef SEMESTRIA_HORARIO_HPP
#define SEMESTRIA_HORARIO_HPP

#include <string>

namespace semestria {

/**
 * @brief Dias da semana em que uma aula pode ocorrer.
 */
enum class DiaSemana {
    Domingo,
    Segunda,
    Terca,
    Quarta,
    Quinta,
    Sexta,
    Sabado
};

/**
 * @brief Converte um dia da semana para texto (ex.: "Segunda").
 * @param dia Dia da semana.
 * @return Nome do dia em português.
 */
std::string paraTexto(DiaSemana dia);

/**
 * @brief Representa um intervalo semanal de aula de uma disciplina.
 *
 * Guarda o dia da semana, o horário de início e de término e o local da
 * aula. Sabe verificar se colide com outro horário, o que é usado por
 * @ref Disciplina e @ref Semestre para impedir choques na grade.
 *
 * Colaboradores: Disciplina, Semestre.
 */
class Horario {
public:
    /**
     * @brief Cria um horário de aula.
     * @param dia Dia da semana da aula.
     * @param horaInicio Hora de início (0 a 23).
     * @param minutoInicio Minuto de início (0 a 59).
     * @param horaFim Hora de término (0 a 23).
     * @param minutoFim Minuto de término (0 a 59).
     * @param sala Sala ou prédio onde a aula ocorre (opcional).
     * @throws HorarioInvalidoException se alguma hora/minuto estiver fora do
     *         intervalo ou se o término não for posterior ao início.
     */
    Horario(DiaSemana dia, int horaInicio, int minutoInicio,
            int horaFim, int minutoFim, const std::string& sala = "");

    /// @return Dia da semana da aula.
    DiaSemana getDia() const;

    /// @return Hora de início da aula.
    int getHoraInicio() const;

    /// @return Minuto de início da aula.
    int getMinutoInicio() const;

    /// @return Hora de término da aula.
    int getHoraFim() const;

    /// @return Minuto de término da aula.
    int getMinutoFim() const;

    /// @return Sala ou prédio da aula (pode ser vazio).
    const std::string& getSala() const;

    /**
     * @brief Altera a sala ou prédio da aula.
     * @param sala Novo local.
     */
    void setSala(const std::string& sala);

    /// @return Início da aula em minutos desde 00:00.
    int inicioEmMinutos() const;

    /// @return Término da aula em minutos desde 00:00.
    int fimEmMinutos() const;

    /// @return Duração da aula, em minutos.
    int duracaoEmMinutos() const;

    /**
     * @brief Verifica se este horário se sobrepõe a outro.
     *
     * Dois horários colidem quando ocorrem no mesmo dia e seus intervalos
     * se interceptam. Aulas "encostadas" (uma termina exatamente quando a
     * outra começa) não colidem.
     *
     * @param outro Horário a ser comparado.
     * @return true se houver sobreposição.
     */
    bool colideCom(const Horario& outro) const;

    /**
     * @brief Formata o horário para exibição.
     * @return Texto no formato "Segunda 07:30-09:10 (sala)".
     */
    std::string toString() const;

    /**
     * @brief Ordena horários por dia da semana e, depois, por início.
     * @param outro Horário a ser comparado.
     * @return true se este horário vier antes de @p outro na grade.
     */
    bool operator<(const Horario& outro) const;

    /**
     * @brief Compara dois horários (dia, início e término; a sala é ignorada).
     * @param outro Horário a ser comparado.
     * @return true se representarem o mesmo intervalo.
     */
    bool operator==(const Horario& outro) const;

private:
    DiaSemana dia_;       ///< Dia da semana da aula.
    int horaInicio_;      ///< Hora de início.
    int minutoInicio_;    ///< Minuto de início.
    int horaFim_;         ///< Hora de término.
    int minutoFim_;       ///< Minuto de término.
    std::string sala_;    ///< Sala ou prédio.
};

} // namespace semestria

#endif // SEMESTRIA_HORARIO_HPP
