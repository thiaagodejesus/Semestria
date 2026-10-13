#include "doctest.h"
#include "Horario.hpp"

TEST_CASE("Testes da classe Horario") {
    SUBCASE("Validacao de dia e hora da aula") {
        Horario h("Segunda", "14:00", "16:00");
        CHECK(h.getDia() == "Segunda");
        CHECK(h.getHoraInicio() == "14:00");
    }
}
