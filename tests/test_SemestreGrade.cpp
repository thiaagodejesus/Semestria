#include "doctest.h"
#include "Semestre.hpp"
#include "Horario.hpp"
#include "Excecoes.hpp"
using namespace semestria;

TEST_CASE("Semestre: disciplinas, grade e NSG") {
    Semestre s("2026/2");
    Disciplina pds("DCC004", "PDS II", 4, 60);
    pds.adicionarAvaliacao(Avaliacao("Trabalho", 100.0));
    pds.buscarAvaliacao("Trabalho").registrarNota(80.0);
    Disciplina calc("MAT002", "Calculo II", 6, 90);
    calc.adicionarAvaliacao(Avaliacao("Prova", 100.0));
    calc.buscarAvaliacao("Prova").registrarNota(60.0);
    s.adicionarDisciplina(pds);
    s.adicionarDisciplina(calc);

    SUBCASE("Totais do semestre") {
        CHECK(s.getIdentificador() == "2026/2");
        CHECK(s.quantidadeDisciplinas() == 2);
        CHECK(s.totalCreditos() == 10);
        CHECK(s.cargaHorariaTotal() == 150);
    }

    SUBCASE("NSG e a media ponderada pelos creditos") {
        // (80 x 4 + 60 x 6) / 10 = 68
        CHECK(s.calcularNSGAtual() == doctest::Approx(68.0));
        CHECK(s.calcularNSGProjetada() == doctest::Approx(68.0));
        CHECK(s.calcularNSGMaxima() == doctest::Approx(68.0));
    }

    SUBCASE("Semestre vazio tem NSG zero") {
        Semestre vazio("2027/1");
        CHECK(vazio.calcularNSGAtual() == doctest::Approx(0.0));
        CHECK(vazio.calcularNSGProjetada() == doctest::Approx(0.0));
        CHECK(vazio.calcularNSGMaxima() == doctest::Approx(0.0));
    }

    SUBCASE("Buscar, remover e codigo duplicado") {
        CHECK(s.possuiDisciplina("DCC004"));
        CHECK(s.buscarDisciplina("MAT002").getNome() == "Calculo II");
        const Semestre& constante = s;
        CHECK(constante.buscarDisciplina("DCC004").getCreditos() == 4);
        CHECK_THROWS_AS(s.adicionarDisciplina(Disciplina("DCC004", "Outra", 2, 30)), DisciplinaDuplicadaException);

        s.removerDisciplina("DCC004");
        CHECK_FALSE(s.possuiDisciplina("DCC004"));
        CHECK_THROWS_AS(s.removerDisciplina("DCC004"), DisciplinaNaoEncontradaException);
        CHECK_THROWS_AS(s.buscarDisciplina("DCC004"), DisciplinaNaoEncontradaException);
        CHECK_THROWS_AS(constante.buscarDisciplina("DCC004"), DisciplinaNaoEncontradaException);
    }

    SUBCASE("Choque de horario entre disciplinas diferentes") {
        s.adicionarHorario("DCC004", Horario(DiaSemana::Terca, 14, 0, 15, 40));
        // Calculo II na mesma terca as 15h: colide com PDS II
        CHECK_THROWS_AS(s.adicionarHorario("MAT002", Horario(DiaSemana::Terca, 15, 0, 16, 40)), ConflitoHorarioException);
        // Comecar exatamente quando a outra termina nao e choque
        CHECK_NOTHROW(s.adicionarHorario("MAT002", Horario(DiaSemana::Terca, 15, 40, 17, 20)));
        CHECK(s.identificarConflitos().empty());
        CHECK(s.buscarDisciplina("MAT002").getHorarios().size() == 1);
    }
}

TEST_CASE("Horario: validacao, duracao e comparacao") {
    Horario h(DiaSemana::Segunda, 8, 0, 9, 40, "Sala 2012");

    SUBCASE("Getters e conversao para minutos") {
        CHECK(h.getDia() == DiaSemana::Segunda);
        CHECK(h.getHoraInicio() == 8);
        CHECK(h.getMinutoInicio() == 0);
        CHECK(h.getHoraFim() == 9);
        CHECK(h.getMinutoFim() == 40);
        CHECK(h.inicioEmMinutos() == 480);
        CHECK(h.fimEmMinutos() == 580);
        CHECK(h.duracaoEmMinutos() == 100);
    }

    SUBCASE("Sala e texto") {
        h.setSala("Sala 3015");
        CHECK(h.getSala() == "Sala 3015");
        CHECK(h.toString().find("08:00-09:40") != std::string::npos);
        CHECK(Horario(DiaSemana::Sexta, 10, 0, 11, 0).toString().find("10:00-11:00") != std::string::npos);
    }

    SUBCASE("Hora invalida lanca excecao") {
        CHECK_THROWS_AS(Horario(DiaSemana::Segunda, 25, 0, 26, 0), HorarioInvalidoException);
    }

    SUBCASE("Comparacao e colisao") {
        Horario igual(DiaSemana::Segunda, 8, 0, 9, 40);
        Horario depois(DiaSemana::Segunda, 10, 0, 11, 40);
        Horario outroDia(DiaSemana::Terca, 8, 0, 9, 40);
        CHECK(h == igual);
        CHECK(h < depois);
        CHECK(h < outroDia);
        CHECK_FALSE(h.colideCom(depois));
        CHECK_FALSE(h.colideCom(outroDia));
        CHECK(h.colideCom(igual));
    }

    SUBCASE("Nome dos dias da semana") {
        CHECK_FALSE(paraTexto(DiaSemana::Domingo).empty());
        CHECK_FALSE(paraTexto(DiaSemana::Segunda).empty());
        CHECK_FALSE(paraTexto(DiaSemana::Terca).empty());
        CHECK_FALSE(paraTexto(DiaSemana::Quarta).empty());
        CHECK_FALSE(paraTexto(DiaSemana::Quinta).empty());
        CHECK_FALSE(paraTexto(DiaSemana::Sexta).empty());
        CHECK_FALSE(paraTexto(DiaSemana::Sabado).empty());
    }
}
