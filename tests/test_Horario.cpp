#include "doctest.h"
#include "Horario.hpp"
using namespace semestria;

TEST_CASE("Testes da classe Horario") {
    SUBCASE("Validacao de dia e hora da aula") {
        Horario h(DiaSemana::Segunda, 14, 0, 16, 0);
        CHECK(h.getDia() == DiaSemana::Segunda);
        CHECK(h.getHoraInicio() == 14);
        CHECK(h.getMinutoInicio() == 0);
        CHECK(h.getHoraFim() == 16);
    }
}