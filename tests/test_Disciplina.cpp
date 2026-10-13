#include "doctest.h"
#include "Disciplina.hpp"

TEST_CASE("Testes da classe Disciplina") {
    SUBCASE("Instanciacao e getters de Disciplina") {
        Disciplina d("PDS II", "DCC204", 60);
        CHECK(d.getNome() == "PDS II");
        CHECK(d.getCodigo() == "DCC204");
        CHECK(d.getCargaHoraria() == 60);
    }
    SUBCASE("Adicao de avaliacoes na disciplina") {
        Disciplina d("PDS II", "DCC204", 60);
        CHECK_NOTHROW(d.adicionarAvaliacao("Prova 1", 30.0));
    }
}
