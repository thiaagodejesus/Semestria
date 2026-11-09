#include "doctest.h"
#include "RelatorioSemestral.hpp"
#include "Semestre.hpp"
#include "Excecoes.hpp"
#include <sstream>
#include <fstream>
#include <cstdio>
using namespace semestria;

// Monta um semestre pequeno, parecido com os dados de demonstracao do main.cpp
static Semestre semestreExemplo() {
    Semestre s("2026/2");
    Disciplina pds("DCC004", "PDS II", 4, 60);
    pds.adicionarHorario(Horario(DiaSemana::Terca, 14, 0, 15, 40, "ICEx"));
    pds.adicionarAvaliacao(Avaliacao("Prova 1", 40.0, "15/10/2026"));
    pds.adicionarAvaliacao(Avaliacao("Trabalho", 60.0));
    pds.buscarAvaliacao("Prova 1").registrarNota(30.0);
    pds.buscarAvaliacao("Trabalho").definirEstimativa(50.0);
    pds.registrarFalta(13);  // dispara o alerta de faltas
    s.adicionarDisciplina(pds);
    return s;
}

TEST_CASE("Testes da classe RelatorioSemestral") {
    Semestre s = semestreExemplo();
    RelatorioSemestral rel(s);

    // Os metodos recebem um std::ostream: aqui usamos um ostringstream para ler o texto gerado
    SUBCASE("Visao geral mostra disciplinas e o aviso de faltas") {
        std::ostringstream saida;
        rel.gerarVisaoGeral(saida);
        std::string texto = saida.str();
        CHECK(texto.find("2026/2") != std::string::npos);
        CHECK(texto.find("PDS II") != std::string::npos);
        CHECK(texto.find("13 das 15") != std::string::npos);
    }

    SUBCASE("Cada parte do relatorio gera texto") {
        std::ostringstream grade, notas, faltas;
        rel.gerarGradeHoraria(grade);
        rel.gerarExtratoNotas(notas);
        rel.gerarHistoricoFaltas(faltas);
        CHECK(grade.str().find("DCC004") != std::string::npos);
        CHECK(notas.str().find("Prova 1") != std::string::npos);
        CHECK_FALSE(faltas.str().empty());
    }

    SUBCASE("Relatorio completo junta todas as partes") {
        std::ostringstream completo;
        rel.gerarCompleto(completo);
        CHECK(completo.str().find("Prova 1") != std::string::npos);
        CHECK(completo.str().find("PDS II") != std::string::npos);
    }

    SUBCASE("Exportar grava o relatorio em arquivo") {
        const std::string caminho = "build/teste_relatorio.txt";
        CHECK_NOTHROW(rel.exportar(caminho));
        std::ifstream arquivo(caminho.c_str());
        CHECK(arquivo.good());
        std::string linha;
        CHECK(static_cast<bool>(std::getline(arquivo, linha)));
        arquivo.close();
        std::remove(caminho.c_str());
    }

    SUBCASE("Exportar para um caminho invalido lanca ArquivoException") {
        CHECK_THROWS_AS(rel.exportar("pasta_que_nao_existe/relatorio.txt"), ArquivoException);
    }
}
