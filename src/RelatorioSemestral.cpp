#include "RelatorioSemestral.hpp"
#include "Excecoes.hpp"
#include <fstream>
#include <iomanip>
namespace semestria
{
    RelatorioSemestral::RelatorioSemestral(const Semestre &s, const AlertaAcademico &a) : semestre_(s), alerta_(a) {}
    void RelatorioSemestral::gerarGradeHoraria(std::ostream &o) const
    {
        o << "GRADE HORÁRIA - " << semestre_.getIdentificador() << "\n";
        for (const auto &d : semestre_.getDisciplinas())
            for (const auto &h : d.getHorarios())
                o << d.getCodigo() << " - " << d.getNome() << ": " << h.toString() << "\n";
    }
    void RelatorioSemestral::gerarExtratoNotas(std::ostream &o) const
    {
        o << "EXTRATO DE NOTAS\n";
        for (const auto &d : semestre_.getDisciplinas())
        {
            o << d.getCodigo() << " - " << d.getNome() << "\n";
            for (const auto &a : d.getAvaliacoes())
                o << "  " << a.getNome() << ": " << a.getNotaObtida() << "/" << a.getValorTotal() << " (" << paraTexto(a.getStatus()) << ")\n";
            o << "  Distribuídos: " << d.pontosDistribuidos() << ", conquistados: " << d.pontosConquistados() << ", em disputa: " << d.pontosEmDisputa() << "\n";
        }
    }
    void RelatorioSemestral::gerarHistoricoFaltas(std::ostream &o) const
    {
        o << "HISTÓRICO DE FALTAS\n";
        for (const auto &d : semestre_.getDisciplinas())
            o << d.getCodigo() << " - " << d.getNome() << ": " << d.getFaltas() << "/" << alerta_.limiteFaltas(d) << " horas (" << d.percentualFaltas() << "%)\n";
    }
    void RelatorioSemestral::gerarVisaoGeral(std::ostream &o) const
    {
        o << "VISÃO GERAL - " << semestre_.getIdentificador() << "\nDisciplinas: " << semestre_.quantidadeDisciplinas() << "\nCréditos: " << semestre_.totalCreditos() << "\nCarga horária: " << semestre_.cargaHorariaTotal() << "\nNSG atual: " << semestre_.calcularNSGAtual() << "\nNSG projetada: " << semestre_.calcularNSGProjetada() << "\nNSG máxima: " << semestre_.calcularNSGMaxima() << "\n";
        for (const auto &d : semestre_.getDisciplinas())
            o << d.getCodigo() << " - " << d.getNome() << ": " << paraTexto(d.situacao()) << "\n";
        for (const auto &a : alerta_.verificar(semestre_))
            o << "AVISO [" << a.codigoDisciplina << "] " << a.mensagem << "\n";
    }
    void RelatorioSemestral::gerarCompleto(std::ostream &o) const
    {
        gerarVisaoGeral(o);
        o << "\n";
        gerarGradeHoraria(o);
        o << "\n";
        gerarExtratoNotas(o);
        o << "\n";
        gerarHistoricoFaltas(o);
    }
    void RelatorioSemestral::exportar(const std::string &c) const
    {
        std::ofstream f(c.c_str());
        if (!f)
            throw ArquivoException("Não foi possível abrir o arquivo para escrita: " + c);
        gerarCompleto(f);
        if (!f)
            throw ArquivoException("Falha ao escrever relatório: " + c);
    }
}
