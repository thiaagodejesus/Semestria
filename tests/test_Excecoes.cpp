#include "doctest.h"
#include "Excecoes.hpp"
#include "ValidadorAcademico.hpp"
#include "Horario.hpp"
#include "Avaliacao.hpp"
#include "Disciplina.hpp"
#include "Semestre.hpp"

using namespace semestria;

TEST_CASE("C09 - Excecoes em Horario") {
    // Horario com hora/minuto invalidos ou fim antes do inicio
    CHECK_THROWS_AS(Horario(DiaSemana::Segunda, 19, 0, 18, 0, "Auditório"), HorarioInvalidoException);
}

TEST_CASE("C09 - Excecoes em Avaliacao") {
    // Nome vazio
    CHECK_THROWS_AS(Avaliacao("", 30.0, "15/10/2026"), DadoInvalidoException);
    // Valor total invalido
    CHECK_THROWS_AS(Avaliacao("Prova 1", -10.0, "15/10/2026"), DadoInvalidoException);
    // Data invalida
    CHECK_THROWS_AS(Avaliacao("Prova 1", 30.0, "31/02/2026"), DadoInvalidoException);

    Avaliacao a("Prova 1", 30.0, "15/10/2026");
    // Nota maior que o valor total ou negativa
    CHECK_THROWS_AS(a.registrarNota(35.0), NotaInvalidaException);
    CHECK_THROWS_AS(a.registrarNota(-1.0), NotaInvalidaException);
}

TEST_CASE("C09 - Excecoes em Disciplina") {
    // Dados invalidos de criacao
    CHECK_THROWS_AS(Disciplina("", "Calculo I", 4, 60), DadoInvalidoException);
    CHECK_THROWS_AS(Disciplina("MAT001", "", 4, 60), DadoInvalidoException);
    CHECK_THROWS_AS(Disciplina("MAT001", "Calculo I", -2, 60), DadoInvalidoException);
    CHECK_THROWS_AS(Disciplina("MAT001", "Calculo I", 4, 0), DadoInvalidoException);

    Disciplina d("MAT001", "Calculo I", 4, 60);
    d.adicionarAvaliacao(Avaliacao("Prova 1", 60.0, "10/10/2026"));
    
    // Avaliacao duplicada
    CHECK_THROWS_AS(d.adicionarAvaliacao(Avaliacao("Prova 1", 20.0, "15/10/2026")), AvaliacaoDuplicadaException);
    // Soma de notas ultrapassando 100 pontos
    CHECK_THROWS_AS(d.adicionarAvaliacao(Avaliacao("Prova 2", 50.0, "20/10/2026")), LimitePontuacaoExcedidoException);
    // Avaliacao nao encontrada
    CHECK_THROWS_AS(d.removerAvaliacao("Inexistente"), AvaliacaoNaoEncontradaException);
}

TEST_CASE("C09 - Excecoes em Semestre") {
    // Semestre com nome vazio
    CHECK_THROWS_AS(Semestre(""), DadoInvalidoException);

    Semestre s("2026/2");
    Disciplina d1("MAT001", "Calculo I", 4, 60);
    s.adicionarDisciplina(d1);

    // Disciplina repetida
    CHECK_THROWS_AS(s.adicionarDisciplina(d1), DisciplinaDuplicadaException);
    // Disciplina nao encontrada
    CHECK_THROWS_AS(s.removerDisciplina("DCC999"), DisciplinaNaoEncontradaException);
}