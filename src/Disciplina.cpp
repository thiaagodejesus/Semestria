#include "Disciplina.hpp"
#include "ValidadorAcademico.hpp"
#include "Excecoes.hpp"
#include "RegrasAcademicas.hpp"
#include <algorithm>
namespace semestria
{
    std::string paraTexto(Situacao s)
    {
        switch (s)
        {
        case Situacao::EmAndamento:
            return "Em andamento";
        case Situacao::Aprovado:
            return "Aprovado";
        case Situacao::ReprovadoPorNota:
            return "Reprovado por nota";
        case Situacao::ReprovadoPorInfrequencia:
            return "Reprovado por infrequência";
        }
        return "Desconhecida";
    }
    Disciplina::Disciplina(const std::string &c, const std::string &n, int cr, int ch) : codigo_(c), nome_(n), creditos_(cr), cargaHoraria_(ch), faltas_(0)
    {
        if (!ValidadorAcademico::codigoDisciplinaValido(c) || !ValidadorAcademico::textoValido(n) || !ValidadorAcademico::creditosValidos(cr) || !ValidadorAcademico::cargaHorariaValida(ch))
            throw DadoInvalidoException("Dados da disciplina inválidos.");
    }
    const std::string &Disciplina::getCodigo() const { return codigo_; }
    const std::string &Disciplina::getNome() const { return nome_; }
    int Disciplina::getCreditos() const { return creditos_; }
    int Disciplina::getCargaHoraria() const { return cargaHoraria_; }
    void Disciplina::setNome(const std::string &n)
    {
        if (!ValidadorAcademico::textoValido(n))
            throw DadoInvalidoException("Nome inválido.");
        nome_ = n;
    }
    void Disciplina::setCreditos(int n)
    {
        if (n <= 0)
            throw DadoInvalidoException("Créditos inválidos.");
        creditos_ = n;
    }
    void Disciplina::setCargaHoraria(int n)
    {
        if (n <= 0)
            throw DadoInvalidoException("Carga horária inválida.");
        cargaHoraria_ = n;
    }
    void Disciplina::adicionarAvaliacao(const Avaliacao &a)
    {
        for (auto &i : avaliacoes_)
            if (i.getNome() == a.getNome())
                throw AvaliacaoDuplicadaException("Avaliação já cadastrada.");
        if (pontosDistribuidos() + a.getValorTotal() > 100.0)
            throw LimitePontuacaoExcedidoException("Pontuação distribuída ultrapassa 100.");
        avaliacoes_.push_back(a);
    }
    void Disciplina::removerAvaliacao(const std::string &n)
    {
        for (auto i = avaliacoes_.begin(); i != avaliacoes_.end(); ++i)
            if (i->getNome() == n)
            {
                avaliacoes_.erase(i);
                return;
            }
        throw AvaliacaoNaoEncontradaException("Avaliação não encontrada.");
    }
    Avaliacao &Disciplina::buscarAvaliacao(const std::string &n)
    {
        for (auto &i : avaliacoes_)
            if (i.getNome() == n)
                return i;
        throw AvaliacaoNaoEncontradaException("Avaliação não encontrada.");
    }
    const Avaliacao &Disciplina::buscarAvaliacao(const std::string &n) const
    {
        for (const auto &i : avaliacoes_)
            if (i.getNome() == n)
                return i;
        throw AvaliacaoNaoEncontradaException("Avaliação não encontrada.");
    }
    void Disciplina::alterarValorAvaliacao(const std::string &n, double v)
    {
        Avaliacao &a = buscarAvaliacao(n);
        double soma = pontosDistribuidos() - a.getValorTotal() + v;
        if (soma > 100)
            throw LimitePontuacaoExcedidoException("Pontuação distribuída ultrapassa 100.");
        a.setValorTotal(v);
    }
    const std::vector<Avaliacao> &Disciplina::getAvaliacoes() const { return avaliacoes_; }
    double Disciplina::pontosDistribuidos() const
    {
        double s = 0;
        for (const auto &a : avaliacoes_)
            s += a.getValorTotal();
        return s;
    }
    double Disciplina::pontosConquistados() const
    {
        double s = 0;
        for (const auto &a : avaliacoes_)
            if (a.isRealizada())
                s += a.getNotaObtida();
        return s;
    }
    double Disciplina::pontosEmDisputa() const
    {
        double realizadas = 0;
        for (const auto &a : avaliacoes_)
            if (a.isRealizada())
                realizadas += a.getValorTotal();
        return 100.0 - realizadas;
    }
    double Disciplina::calcularNotaFinal() const { return pontosConquistados(); }
    double Disciplina::calcularNotaProjetada() const
    {
        double s = 0;
        for (const auto &a : avaliacoes_)
            s += a.getNotaObtida();
        return s;
    }
    double Disciplina::calcularNotaMaxima() const { return pontosConquistados() + pontosEmDisputa(); }
    void Disciplina::registrarFalta(unsigned int h) { faltas_ += h; }
    void Disciplina::removerFalta(unsigned int h)
    {
        if (h > faltas_)
            throw DadoInvalidoException("Não é possível remover mais faltas do que as registradas.");
        faltas_ -= h;
    }
    unsigned int Disciplina::getFaltas() const { return faltas_; }
    unsigned int Disciplina::limiteFaltas() const { return static_cast<unsigned int>(cargaHoraria_ * regras::PERCENTUAL_MAXIMO_FALTAS); }
    unsigned int Disciplina::faltasRestantes() const { return faltas_ >= limiteFaltas() ? 0 : limiteFaltas() - faltas_; }
    double Disciplina::percentualFaltas() const { return cargaHoraria_ ? 100.0 * faltas_ / cargaHoraria_ : 0; }
    bool Disciplina::excedeuLimiteFaltas() const { return faltas_ > limiteFaltas(); }
    void Disciplina::adicionarHorario(const Horario &h)
    {
        for (const auto &x : horarios_)
            if (x.colideCom(h))
                throw ConflitoHorarioException("Horários da disciplina colidem.");
        horarios_.push_back(h);
    }
    void Disciplina::removerHorario(const Horario &h)
    {
        for (auto i = horarios_.begin(); i != horarios_.end(); ++i)
            if (*i == h)
            {
                horarios_.erase(i);
                return;
            }
    }
    const std::vector<Horario> &Disciplina::getHorarios() const { return horarios_; }
    bool Disciplina::conflitaCom(const Disciplina &o) const
    {
        for (const auto &a : horarios_)
            for (const auto &b : o.horarios_)
                if (a.colideCom(b))
                    return true;
        return false;
    }
    Situacao Disciplina::situacao() const
    {
        if (excedeuLimiteFaltas())
            return Situacao::ReprovadoPorInfrequencia;
        if (calcularNotaFinal() >= regras::NOTA_MINIMA_APROVACAO)
            return Situacao::Aprovado;
        if (calcularNotaMaxima() < regras::NOTA_MINIMA_APROVACAO)
            return Situacao::ReprovadoPorNota;
        return Situacao::EmAndamento;
    }
}
