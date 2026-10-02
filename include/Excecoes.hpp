/**
 * @file Excecoes.hpp
 * @brief Hierarquia de exceções lançadas pelo Semestria.
 *
 * Todas as exceções do sistema derivam de @ref semestria::SemestriaException,
 * o que permite tratá-las de forma genérica (por exemplo, no menu do
 * terminal) ou de forma específica quando for necessário reagir a um erro
 * em particular.
 */

// O QUE É UMA EXCEÇÃO?
// É o jeito do C++ de avisar "deu errado, não dá para continuar".
// Quem detecta o problema faz "throw" (lança) e quem chamou faz "catch"
// (captura) para mostrar uma mensagem amigável em vez de travar. Exemplo:
//
//     try {
//         avaliacao.registrarNota(120);   // vale só 100 -> lança exceção
//     } catch (const NotaInvalidaException& e) {
//         std::cout << "Erro: " << e.what() << std::endl;
//     }
//
// Árvore das exceções deste arquivo (a seta lê-se "é um tipo de"):
//
//   std::runtime_error              (já vem pronta no C++)
//    └── SemestriaException         (base de todos os erros do projeto)
//         ├── DadoInvalidoException
//         │    ├── NotaInvalidaException
//         │    └── HorarioInvalidoException
//         ├── DisciplinaDuplicadaException
//         ├── DisciplinaNaoEncontradaException
//         ├── AvaliacaoDuplicadaException
//         ├── AvaliacaoNaoEncontradaException
//         ├── LimitePontuacaoExcedidoException
//         ├── ConflitoHorarioException
//         └── ArquivoException
//
// Vantagem da árvore (herança): um "catch (const SemestriaException& e)"
// captura QUALQUER erro do projeto, enquanto um catch de uma classe mais
// específica captura só aquele caso.

#ifndef SEMESTRIA_EXCECOES_HPP
#define SEMESTRIA_EXCECOES_HPP

#include <stdexcept>
#include <string>

namespace semestria {

/**
 * @brief Classe base de todas as exceções do Semestria.
 */
// ": public std::runtime_error" é herança: SemestriaException herda tudo
// de runtime_error, inclusive o método what(), que devolve a mensagem.
class SemestriaException : public std::runtime_error {
public:
    // Esta linha reaproveita o construtor da classe mãe, que recebe a
    // mensagem de erro. Por isso dá para escrever, por exemplo:
    //     throw SemestriaException("mensagem explicando o erro");
    // As classes abaixo usam a mesma ideia, cada uma com a sua mãe.
    using std::runtime_error::runtime_error;
};

/**
 * @brief Lançada quando um dado informado pelo usuário é inválido.
 *
 * Exemplos: nome vazio, carga horária negativa ou zerada, créditos
 * inválidos, data em formato incorreto.
 */
class DadoInvalidoException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada quando uma nota é negativa ou maior que o valor da avaliação.
 */
class NotaInvalidaException : public DadoInvalidoException {
public:
    using DadoInvalidoException::DadoInvalidoException;
};

/**
 * @brief Lançada quando um horário é inválido (ex.: término antes do início).
 */
class HorarioInvalidoException : public DadoInvalidoException {
public:
    using DadoInvalidoException::DadoInvalidoException;
};

/**
 * @brief Lançada ao cadastrar uma disciplina cujo código já existe no semestre.
 */
class DisciplinaDuplicadaException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada ao buscar/remover uma disciplina que não está cadastrada.
 */
class DisciplinaNaoEncontradaException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada ao cadastrar uma avaliação com nome repetido na disciplina.
 */
class AvaliacaoDuplicadaException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada ao buscar/remover uma avaliação que não está cadastrada.
 */
class AvaliacaoNaoEncontradaException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada quando a soma dos valores das avaliações de uma disciplina
 * ultrapassaria a pontuação máxima (100 pontos).
 */
class LimitePontuacaoExcedidoException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada ao cadastrar uma aula que coincide com outra já existente.
 */
class ConflitoHorarioException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

/**
 * @brief Lançada quando não é possível ler ou escrever um arquivo.
 */
class ArquivoException : public SemestriaException {
public:
    using SemestriaException::SemestriaException;
};

} // namespace semestria

#endif // SEMESTRIA_EXCECOES_HPP
