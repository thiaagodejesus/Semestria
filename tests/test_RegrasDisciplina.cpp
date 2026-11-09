#include "doctest.h"
#include "Disciplina.hpp"
#include "Avaliacao.hpp"
#include "Excecoes.hpp"
using namespace semestria;

TEST_CASE("Regras de pontuacao, faltas e situacao da Disciplina") {
    Disciplina d("DCC004", "PDS II", 4, 60);

    SUBCASE("Setters validam antes de alterar") {
        d.setNome("Programacao II");
        d.setCreditos(6);
        d.setCargaHoraria(90);
        CHECK(d.getNome() == "Programacao II");
        CHECK(d.getCreditos() == 6);
        CHECK(d.getCargaHoraria() == 90);
        CHECK_THROWS_AS(d.setCreditos(0), DadoInvalidoException);
        CHECK(d.getCreditos() == 6);  // continua como estava
    }

    SUBCASE("Pontos distribuidos, conquistados e em disputa") {
        d.adicionarAvaliacao(Avaliacao("Prova 1", 30.0));
        d.adicionarAvaliacao(Avaliacao("Prova 2", 30.0));
        d.buscarAvaliacao("Prova 1").registrarNota(20.0);
        d.buscarAvaliacao("Prova 2").definirEstimativa(25.0);

        CHECK(d.pontosDistribuidos() == doctest::Approx(60.0));
        CHECK(d.pontosConquistados() == doctest::Approx(20.0));
        CHECK(d.pontosEmDisputa() == doctest::Approx(70.0));      // 100 - 30 ja realizados
        CHECK(d.calcularNotaFinal() == doctest::Approx(20.0));
        CHECK(d.calcularNotaProjetada() == doctest::Approx(45.0)); // 20 oficial + 25 estimada
        CHECK(d.calcularNotaMaxima() == doctest::Approx(90.0));    // 20 + 70
    }

    SUBCASE("Avaliacoes: duplicada, limite de 100, alterar e remover") {
        d.adicionarAvaliacao(Avaliacao("Prova 1", 40.0));
        CHECK_THROWS_AS(d.adicionarAvaliacao(Avaliacao("Prova 1", 10.0)), AvaliacaoDuplicadaException);
        CHECK_THROWS_AS(d.adicionarAvaliacao(Avaliacao("Trabalho", 70.0)), LimitePontuacaoExcedidoException);

        d.alterarValorAvaliacao("Prova 1", 50.0);
        CHECK(d.buscarAvaliacao("Prova 1").getValorTotal() == doctest::Approx(50.0));
        CHECK_THROWS_AS(d.alterarValorAvaliacao("Prova 1", 120.0), LimitePontuacaoExcedidoException);

        d.removerAvaliacao("Prova 1");
        CHECK(d.getAvaliacoes().empty());
        CHECK_THROWS_AS(d.removerAvaliacao("Prova 1"), AvaliacaoNaoEncontradaException);
        CHECK_THROWS_AS(d.buscarAvaliacao("Prova 1"), AvaliacaoNaoEncontradaException);

        const Disciplina& constante = d;
        CHECK_THROWS_AS(constante.buscarAvaliacao("Prova 1"), AvaliacaoNaoEncontradaException);
    }

    SUBCASE("Faltas: limite de 25% e faltas restantes") {
        CHECK(d.limiteFaltas() == 15);
        d.registrarFalta(10);
        CHECK(d.getFaltas() == 10);
        CHECK(d.faltasRestantes() == 5);
        CHECK(d.percentualFaltas() == doctest::Approx(100.0 * 10 / 60));
        d.removerFalta(4);
        CHECK(d.getFaltas() == 6);
        CHECK_THROWS_AS(d.removerFalta(10), DadoInvalidoException);
        d.registrarFalta(20);
        CHECK(d.excedeuLimiteFaltas());
        CHECK(d.faltasRestantes() == 0);
    }

    SUBCASE("Horarios da propria disciplina nao podem colidir") {
        Horario terca(DiaSemana::Terca, 14, 0, 15, 40, "ICEx");
        d.adicionarHorario(terca);
        CHECK_THROWS_AS(d.adicionarHorario(Horario(DiaSemana::Terca, 15, 0, 16, 0)), ConflitoHorarioException);
        d.removerHorario(terca);
        CHECK(d.getHorarios().empty());
    }

    SUBCASE("Situacao: a frequencia e verificada primeiro") {
        d.adicionarAvaliacao(Avaliacao("Trabalho", 100.0));
        CHECK(d.situacao() == Situacao::EmAndamento);

        d.buscarAvaliacao("Trabalho").registrarNota(90.0);
        CHECK(d.situacao() == Situacao::Aprovado);

        d.registrarFalta(16);  // passou do limite de 15
        CHECK(d.situacao() == Situacao::ReprovadoPorInfrequencia);
    }

    SUBCASE("Situacao: reprovado por nota quando 60 ficou impossivel") {
        d.adicionarAvaliacao(Avaliacao("Prova", 100.0));
        d.buscarAvaliacao("Prova").registrarNota(30.0);
        CHECK(d.situacao() == Situacao::ReprovadoPorNota);
    }

    SUBCASE("Texto das situacoes") {
        CHECK_FALSE(paraTexto(Situacao::EmAndamento).empty());
        CHECK_FALSE(paraTexto(Situacao::Aprovado).empty());
        CHECK_FALSE(paraTexto(Situacao::ReprovadoPorNota).empty());
        CHECK_FALSE(paraTexto(Situacao::ReprovadoPorInfrequencia).empty());
    }
}

TEST_CASE("Ciclo de status da Avaliacao") {
    Avaliacao a("Prova 1", 30.0, "15/10/2026");
    CHECK(a.getStatus() == StatusAvaliacao::Pendente);
    CHECK(a.getDataLimite() == "15/10/2026");

    SUBCASE("Pendente -> Estimada -> Pendente") {
        a.definirEstimativa(25.0);
        CHECK(a.isEstimativa());
        CHECK(a.getNotaObtida() == doctest::Approx(25.0));
        a.removerEstimativa();
        CHECK(a.getStatus() == StatusAvaliacao::Pendente);
        CHECK(a.getNotaObtida() == doctest::Approx(0.0));
    }

    SUBCASE("Realizada nao aceita estimativa, mas pode ser cancelada") {
        a.registrarNota(28.0);
        CHECK(a.isRealizada());
        CHECK_THROWS_AS(a.definirEstimativa(20.0), SemestriaException);
        a.cancelarRegistro();
        CHECK(a.getStatus() == StatusAvaliacao::Pendente);
    }

    SUBCASE("Setters validam nome, valor e data") {
        a.setNome("Prova Final");
        a.setValorTotal(40.0);
        a.setDataLimite("01/12/2026");
        CHECK(a.getNome() == "Prova Final");
        CHECK(a.getValorTotal() == doctest::Approx(40.0));
        CHECK(a.getDataLimite() == "01/12/2026");
        CHECK_THROWS_AS(a.setNome("   "), DadoInvalidoException);
        CHECK_THROWS_AS(a.setValorTotal(0.0), DadoInvalidoException);
        CHECK_THROWS_AS(a.setDataLimite("30/02/2026"), DadoInvalidoException);
        CHECK_THROWS_AS(a.definirEstimativa(50.0), NotaInvalidaException);
    }

    SUBCASE("Texto dos status") {
        CHECK_FALSE(paraTexto(StatusAvaliacao::Pendente).empty());
        CHECK_FALSE(paraTexto(StatusAvaliacao::Estimada).empty());
        CHECK_FALSE(paraTexto(StatusAvaliacao::Realizada).empty());
    }
}
