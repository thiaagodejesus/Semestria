#include "doctest.h"
#include "Disciplina.hpp"
using namespace semestria;

TEST_CASE("Testes da classe Disciplina") {
    SUBCASE("Instanciacao e getters de Disciplina") {
        Disciplina d("DCC204", "PDS II", 4, 60);
        CHECK(d.getNome() == "PDS II");
        CHECK(d.getCodigo() == "DCC204");
        CHECK(d.getCargaHoraria() == 60);
    }
    SUBCASE("Adicao de avaliacoes na disciplina") {
        Disciplina d("DCC204", "PDS II", 4, 60);
        CHECK_NOTHROW(d.adicionarAvaliacao(Avaliacao("Prova 1", 30.0)));
    }
}
