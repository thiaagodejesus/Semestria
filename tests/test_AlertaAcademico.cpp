#include "doctest.h"
#include "AlertaAcademico.hpp"
#include "Disciplina.hpp"
#include "Semestre.hpp"
#include "Excecoes.hpp"
using namespace semestria;

TEST_CASE("Testes da classe AlertaAcademico") {
    // Obs.: usamos 13 h (e nao 12 h exatas) porque, no compilador do Windows (MinGW),
    // a conta 0.8 x 15 pode dar um pouquinho mais que 12 e o alerta so aparece depois das 12 h.
    // Disciplina de 60 horas: limite de 15 h de falta (25%) e alerta a partir de 12 h (80% do limite)
    AlertaAcademico alerta;

    SUBCASE("Regras padrao vem de RegrasAcademicas") {
        CHECK(alerta.getPercentualMaximoFaltas() == doctest::Approx(0.25));
        CHECK(alerta.getNotaMinimaAprovacao() == doctest::Approx(60.0));
        CHECK(alerta.getFracaoAlertaFaltas() == doctest::Approx(0.8));
    }

    SUBCASE("Parametros invalidos lancam excecao") {
        CHECK_THROWS_AS(AlertaAcademico(1.5), DadoInvalidoException);
        CHECK_THROWS_AS(AlertaAcademico(0.25, 150.0), DadoInvalidoException);
    }

    SUBCASE("Faltas: sem risco, em risco e reprovado") {
        Disciplina d("DCC004", "PDS II", 4, 60);
        CHECK(alerta.limiteFaltas(d) == 15);

        d.registrarFalta(4);  // 4 h: longe do alerta
        CHECK_FALSE(alerta.emRiscoDeInfrequencia(d));
        CHECK(alerta.verificar(d).empty());

        d.registrarFalta(9);  // 13 h: passou dos 80% do limite (12 h)
        CHECK(alerta.emRiscoDeInfrequencia(d));
        std::vector<Aviso> avisos = alerta.verificar(d);
        REQUIRE(avisos.size() == 1);
        CHECK(avisos[0].tipo == TipoAviso::RiscoInfrequencia);
        CHECK(avisos[0].codigoDisciplina == "DCC004");

        d.registrarFalta(3);  // 16 h: passou do limite de 15
        CHECK(alerta.reprovadoPorInfrequencia(d));
        CHECK_FALSE(alerta.emRiscoDeInfrequencia(d));
        avisos = alerta.verificar(d);
        REQUIRE(avisos.size() == 1);
        CHECK(avisos[0].tipo == TipoAviso::ReprovacaoInfrequencia);
    }

    SUBCASE("Nota: risco e reprovacao") {
        Disciplina d("MAT002", "Calculo II", 6, 90);
        d.adicionarAvaliacao(Avaliacao("Prova 1", 50.0));
        d.adicionarAvaliacao(Avaliacao("Prova 2", 50.0));

        // Tirou 20 de 50 (40% de aproveitamento): em risco, mas ainda pode chegar a 70
        d.buscarAvaliacao("Prova 1").registrarNota(20.0);
        CHECK(alerta.emRiscoPorNota(d));
        CHECK_FALSE(alerta.reprovadoPorNota(d));
        CHECK(alerta.pontuacaoNecessaria(d) == doctest::Approx(40.0));
        std::vector<Aviso> avisos = alerta.verificar(d);
        REQUIRE(avisos.size() == 1);
        CHECK(avisos[0].tipo == TipoAviso::RiscoNota);

        // Tirou 5 de 50 na segunda: nota maxima 25, nao da mais para chegar a 60
        d.buscarAvaliacao("Prova 2").registrarNota(5.0);
        CHECK(alerta.reprovadoPorNota(d));
        avisos = alerta.verificar(d);
        REQUIRE(avisos.size() == 1);
        CHECK(avisos[0].tipo == TipoAviso::ReprovacaoNota);
    }

    SUBCASE("Ja aprovado nao precisa de mais pontos") {
        Disciplina d("DCC004", "PDS II", 4, 60);
        d.adicionarAvaliacao(Avaliacao("Trabalho", 100.0));
        d.buscarAvaliacao("Trabalho").registrarNota(80.0);
        CHECK(alerta.pontuacaoNecessaria(d) == doctest::Approx(0.0));
    }

    SUBCASE("Verificar o semestre junta os avisos de todas as disciplinas") {
        Semestre s("2026/2");
        Disciplina a("DCC004", "PDS II", 4, 60);
        a.registrarFalta(13);
        Disciplina b("MAT002", "Calculo II", 6, 90);
        b.registrarFalta(30);
        s.adicionarDisciplina(a);
        s.adicionarDisciplina(b);
        CHECK(alerta.verificar(s).size() == 2);
    }

    SUBCASE("Texto dos tipos de aviso") {
        CHECK(paraTexto(TipoAviso::RiscoInfrequencia) == "Risco de infrequência");
        CHECK(paraTexto(TipoAviso::ReprovacaoInfrequencia) == "Reprovação por infrequência");
        CHECK(paraTexto(TipoAviso::RiscoNota) == "Risco por nota");
        CHECK(paraTexto(TipoAviso::ReprovacaoNota) == "Reprovação por nota");
    }
}
