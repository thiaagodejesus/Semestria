/**
 * @file Disciplina.hpp
 * @brief Declaração da classe Disciplina e do enum Situacao.
 */

#ifndef SEMESTRIA_DISCIPLINA_HPP
#define SEMESTRIA_DISCIPLINA_HPP

#include <string>
#include <vector>

// Disciplina usa as classes Avaliacao e Horario, então precisa incluir
// os contratos delas. Aspas ("...") = arquivo nosso, da pasta include/;
// sinais de menor/maior (<...>) = biblioteca padrão do C++.
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
// Esta é a classe central do sistema. Uma Disciplina "TEM" várias
// avaliações e vários horários (isso se chama composição): ela guarda
// essas listas dentro de si e é a única responsável por elas.
//
// Exemplo de uso:
//     Disciplina pds2("DCC204", "PDS II", 4, 60);  // 4 créditos, 60 h
//     pds2.adicionarAvaliacao(Avaliacao("Prova 1", 30.0));
//     pds2.adicionarHorario(Horario(DiaSemana::Terca, 9, 25, 11, 5));
//     pds2.registrarFalta(2);                      // faltou 2 horas-aula
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

    // As linhas "@name ... @{ ... @}" só agrupam os métodos por assunto
    // na documentação gerada pelo Doxygen; não mudam nada no código.

    // ----------------------------------------------------------------
    /// @name Dados cadastrais
    /// @{

    // Não existe setCodigo(): o código identifica a disciplina dentro do
    // semestre, então não pode mudar depois do cadastro.

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
    // Ex.: já existem provas de 30 + 30 + 30 = 90 pontos. Adicionar um
    // trabalho de 20 daria 110, então lança exceção. Um de 10 é aceito.
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
    // Por que existem duas versões de buscarAvaliacao?
    // - A primeira devolve "Avaliacao&" (referência): é a própria avaliação
    //   guardada aqui dentro, não uma cópia. Então, por exemplo,
    //       pds2.buscarAvaliacao("Prova 1").registrarNota(25);
    //   altera de verdade a nota salva na disciplina.
    // - A segunda (com "const") é usada quando a disciplina é só para
    //   leitura, e por isso devolve uma referência que não permite mudanças.
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

    // Exemplo para entender os métodos de pontuação abaixo:
    //   Prova 1 vale 30 e o aluno tirou 20  (Realizada)
    //   Prova 2 vale 30 e ainda não aconteceu (Pendente)
    //   Os outros 40 pontos o professor ainda não distribuiu.
    //
    //   pontosDistribuidos() = 30 + 30       = 60
    //   pontosConquistados() = 20
    //   pontosEmDisputa()    = 100 - 30      = 70  (Prova 2 + os 40 restantes)
    //   calcularNotaFinal()  = 20
    //   calcularNotaMaxima() = 20 + 70       = 90  (se gabaritar o resto)
    //   Se o aluno estimar 25 na Prova 2:
    //   calcularNotaProjetada() = 20 + 25    = 45

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

    // Exemplo para entender os métodos de frequência abaixo:
    //   Disciplina de 60 horas-aula, aluno com 10 horas de falta.
    //   limiteFaltas()      = 25% de 60       = 15
    //   faltasRestantes()   = 15 - 10         = 5
    //   percentualFaltas()  = 10 / 60 * 100   = 16,7%
    //   excedeuLimiteFaltas() = false (só passa a ser true com 16 ou mais)
    //
    // "unsigned int" é um inteiro que nunca é negativo. Faz sentido aqui,
    // já que ninguém tem -3 faltas.

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

    // Atenção: aqui a Disciplina só confere choques entre as SUAS
    // próprias aulas. Choques com OUTRAS disciplinas são verificados pela
    // classe Semestre, que é quem conhece todas as disciplinas.

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
    // A ordem importa: quem passou do limite de faltas está reprovado
    // mesmo com nota alta, por isso a frequência é testada primeiro.
    // A regra 2 permite avisar a reprovação por nota antes do fim do
    // semestre: se, mesmo gabaritando tudo o que falta, o aluno não
    // chega a 60, o resultado já está definido.
    Situacao situacao() const;

private:
    std::string codigo_;                 ///< Código único no semestre.
    std::string nome_;                   ///< Nome da disciplina.
    int creditos_;                       ///< Número de créditos.
    int cargaHoraria_;                   ///< Carga horária em horas-aula.
    unsigned int faltas_;                ///< Faltas em horas-aula.
    // std::vector é uma lista que cresce sozinha conforme adicionamos
    // itens (parecido com um array, mas sem tamanho fixo).
    std::vector<Avaliacao> avaliacoes_;  ///< Avaliações vinculadas.
    std::vector<Horario> horarios_;      ///< Horários semanais de aula.
};

} // namespace semestria

#endif // SEMESTRIA_DISCIPLINA_HPP
