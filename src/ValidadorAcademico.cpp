#include "ValidadorAcademico.hpp"
#include "RegrasAcademicas.hpp"
#include <cctype>
#include <cmath>
#include <sstream>

namespace semestria
{
    bool ValidadorAcademico::textoValido(const std::string &s)
    {
        for (std::size_t i = 0; i < s.size(); ++i)
            if (!std::isspace(static_cast<unsigned char>(s[i])))
                return true;
        return false;
    }
    bool ValidadorAcademico::codigoDisciplinaValido(const std::string &s)
    {
        if (s.empty())
            return false;
        for (std::size_t i = 0; i < s.size(); ++i)
            if (!std::isalnum(static_cast<unsigned char>(s[i])))
                return false;
        return true;
    }
    bool ValidadorAcademico::creditosValidos(int n) { return n > 0; }
    bool ValidadorAcademico::cargaHorariaValida(int n) { return n > 0; }
    bool ValidadorAcademico::valorAvaliacaoValido(double n) { return std::isfinite(n) && n > 0 && n <= regras::PONTUACAO_MAXIMA_DISCIPLINA; }
    bool ValidadorAcademico::notaValida(double n, double max) { return std::isfinite(n) && std::isfinite(max) && n >= 0 && n <= max; }
    bool ValidadorAcademico::horaValida(int h, int m) { return h >= 0 && h <= 23 && m >= 0 && m <= 59; }
    bool ValidadorAcademico::dataValida(const std::string &s)
    {
        if (s.size() != 10 || s[2] != '/' || s[5] != '/')
            return false;
        for (int i = 0; i < 10; ++i)
            if (i != 2 && i != 5 && !std::isdigit(static_cast<unsigned char>(s[i])))
                return false;
        int d = (s[0] - '0') * 10 + s[1] - '0', m = (s[3] - '0') * 10 + s[4] - '0', y = (s[6] - '0') * 1000 + (s[7] - '0') * 100 + (s[8] - '0') * 10 + s[9] - '0';
        if (y < 1 || m < 1 || m > 12)
            return false;
        int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)))
            dias[1] = 29;
        return d >= 1 && d <= dias[m - 1];
    }
}
