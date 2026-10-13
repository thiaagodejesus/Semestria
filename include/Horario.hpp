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
// "enum class" é uma lista fechada de opções. Em vez de guardar o dia
// como texto ("segunda", "Segunda", "seg"...), o que abre espaço para
// erro de digitação, usamos DiaSemana::Segunda, DiaSemana::Terca etc.
// O compilador recusa qualquer valor que não esteja na lista.
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
// Função "solta", fora de qualquer classe. Outros arquivos têm funções
// paraTexto() para outros enums; o compilador escolhe a certa pelo tipo
// do parâmetro (isso se chama sobrecarga).
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
// Exemplo de uso:
//     Horario aula(DiaSemana::Segunda, 7, 30, 9, 10, "ICEx 2004");
//     // representa: segunda-feira, das 07:30 às 09:10, na sala ICEx 2004
class Horario {
// "public" = o que qualquer parte do programa pode usar.
// "private" (lá embaixo) = os dados internos, que só a própria classe
// mexe. Isso é o ENCAPSULAMENTO: ninguém consegue, por exemplo, criar
// uma aula que termina antes de começar, porque todo dado passa pelos
// métodos, que validam antes de aceitar.
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
    // Este é o CONSTRUTOR: roda automaticamente quando um Horario é
    // criado. O trecho  = ""  no último parâmetro o torna opcional: se a
    // sala não for informada, ela fica vazia.
    Horario(DiaSemana dia, int horaInicio, int minutoInicio,
            int horaFim, int minutoFim, const std::string& sala = "");

    // Os métodos "get" só devolvem um dado guardado. O "const" depois dos
    // parênteses é uma promessa: este método não altera o objeto.

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

    // Converter para "minutos desde a meia-noite" facilita as contas:
    // 07:30 vira 7*60 + 30 = 450 e 09:10 vira 550. Comparar dois números
    // é bem mais simples do que comparar horas e minutos separados.

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
    // Exemplos (todos na segunda-feira):
    //   07:30-09:10 e 08:00-09:40 -> colidem (uma começa antes da outra acabar)
    //   07:30-09:10 e 09:10-10:50 -> NÃO colidem (só encostam)
    //   07:30-09:10 na segunda e 07:30-09:10 na terça -> NÃO colidem
    bool colideCom(const Horario& outro) const;

    /**
     * @brief Formata o horário para exibição.
     * @return Texto no formato "Segunda 07:30-09:10 (sala)".
     */
    std::string toString() const;

    // Os dois métodos "operator" abaixo ensinam o C++ a usar < e == com
    // objetos Horario. O "<" permite ordenar a grade da semana com
    // std::sort; o "==" permite achar um horário específico numa lista.

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
    // Atributos (os dados que cada objeto guarda). Usamos o "_" no final
    // do nome para diferenciar atributos de parâmetros e variáveis locais.
    DiaSemana dia_;       ///< Dia da semana da aula.
    int horaInicio_;      ///< Hora de início.
    int minutoInicio_;    ///< Minuto de início.
    int horaFim_;         ///< Hora de término.
    int minutoFim_;       ///< Minuto de término.
    std::string sala_;    ///< Sala ou prédio.
};

} // namespace semestria

#endif // SEMESTRIA_HORARIO_HPP
