#include "AlertaAcademico.hpp"
#include "Disciplina.hpp"
#include "Semestre.hpp"
#include "Excecoes.hpp"
#include <sstream>
namespace semestria
{
    std::string paraTexto(TipoAviso t)
    {
        switch (t)
        {
        case TipoAviso::RiscoInfrequencia:
            return "Risco de infrequência";
        case TipoAviso::ReprovacaoInfrequencia:
            return "Reprovação por infrequência";
        case TipoAviso::RiscoNota:
            return "Risco por nota";
        case TipoAviso::ReprovacaoNota:
            return "Reprovação por nota";
        }
        return "Aviso";
    }
    AlertaAcademico::AlertaAcademico(double p, double n, double f) : percentualMaximoFaltas_(p), notaMinimaAprovacao_(n), fracaoAlertaFaltas_(f)
    {
        if (p < 0 || p > 1 || n < 0 || n > 100 || f < 0 || f > 1)
            throw DadoInvalidoException("Parâmetros de alerta inválidos.");
    }
    double AlertaAcademico::getPercentualMaximoFaltas() const { return percentualMaximoFaltas_; }
    double AlertaAcademico::getNotaMinimaAprovacao() const { return notaMinimaAprovacao_; }
    double AlertaAcademico::getFracaoAlertaFaltas() const { return fracaoAlertaFaltas_; }
    unsigned int AlertaAcademico::limiteFaltas(const Disciplina &d) const { return static_cast<unsigned int>(d.getCargaHoraria() * percentualMaximoFaltas_); }
    bool AlertaAcademico::emRiscoDeInfrequencia(const Disciplina &d) const { return d.getFaltas() >= fracaoAlertaFaltas_ * limiteFaltas(d) && d.getFaltas() <= limiteFaltas(d); }
    bool AlertaAcademico::reprovadoPorInfrequencia(const Disciplina &d) const { return d.getFaltas() > limiteFaltas(d); }
    double AlertaAcademico::pontuacaoNecessaria(const Disciplina &d) const
    {
        double x = notaMinimaAprovacao_ - d.calcularNotaFinal();
        return x > 0 ? x : 0;
    }
    bool AlertaAcademico::emRiscoPorNota(const Disciplina &d) const
    {
        double valor = 0;
        for (const auto &a : d.getAvaliacoes())
            if (a.isRealizada())
                valor += a.getValorTotal();
        return valor > 0 && d.pontosConquistados() / valor * 100.0 < notaMinimaAprovacao_;
    }
    bool AlertaAcademico::reprovadoPorNota(const Disciplina &d) const { return d.calcularNotaMaxima() < notaMinimaAprovacao_; }
    std::vector<Aviso> AlertaAcademico::verificar(const Disciplina &d) const
    {
        std::vector<Aviso> r;
        std::ostringstream m;
        if (reprovadoPorInfrequencia(d))
        {
            m << "Faltas (" << d.getFaltas() << ") ultrapassaram o limite (" << limiteFaltas(d) << ").";
            r.push_back({TipoAviso::ReprovacaoInfrequencia, d.getCodigo(), m.str()});
        }
        else if (emRiscoDeInfrequencia(d))
        {
            m << "Você já usou " << d.getFaltas() << " das " << limiteFaltas(d) << " horas de falta permitidas.";
            r.push_back({TipoAviso::RiscoInfrequencia, d.getCodigo(), m.str()});
        }
        m.str("");
        m.clear();
        if (reprovadoPorNota(d))
        {
            m << "A nota máxima possível é inferior a " << notaMinimaAprovacao_ << " pontos.";
            r.push_back({TipoAviso::ReprovacaoNota, d.getCodigo(), m.str()});
        }
        else if (emRiscoPorNota(d))
        {
            m << "O aproveitamento atual está abaixo da nota mínima.";
            r.push_back({TipoAviso::RiscoNota, d.getCodigo(), m.str()});
        }
        return r;
    }
    std::vector<Aviso> AlertaAcademico::verificar(const Semestre &s) const
    {
        std::vector<Aviso> r;
        for (const auto &d : s.getDisciplinas())
        {
            std::vector<Aviso> x = verificar(d);
            r.insert(r.end(), x.begin(), x.end());
        }
        return r;
    }
}
