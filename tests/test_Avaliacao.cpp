#include "doctest.h"
#include "Avaliacao.hpp"
using namespace semestria;

TEST_CASE("Testes da classe Avaliacao") {
    SUBCASE("Criacao de avaliacao e atribuicao de nota") {
        Avaliacao a("Prova 1", 30.0);
        CHECK(a.getNome() == "Prova 1");
        CHECK(a.getValorTotal() == 30.0);
    }
    SUBCASE("Validacao de nota obtida") {
        Avaliacao a("Prova 1", 30.0);
        CHECK_NOTHROW(a.registrarNota(25.0));
        CHECK(a.getNotaObtida() == 25.0);
    }
}
