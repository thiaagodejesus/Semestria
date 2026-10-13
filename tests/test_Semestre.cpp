#include "doctest.h"
#include "Semestre.hpp"

TEST_CASE("Testes da classe Semestre") {
    SUBCASE("Cadastro de disciplinas no semestre") {
        Semestre sem("2026/2");
        CHECK(sem.getIdentificador() == "2026/2");
        CHECK(sem.getQuantidadeDisciplinas() == 0);
    }
}
