#include "Avaliacao.hpp"
#include "ValidadorAcademico.hpp"
#include "Excecoes.hpp"
namespace semestria {
std::string paraTexto(StatusAvaliacao s){switch(s){case StatusAvaliacao::Pendente:return "Pendente";case StatusAvaliacao::Estimada:return "Estimada";case StatusAvaliacao::Realizada:return "Realizada";}return "Desconhecido";}
Avaliacao::Avaliacao(const std::string& n,double v,const std::string& d):nome_(n),valorTotal_(v),nota_(0),dataLimite_(d),status_(StatusAvaliacao::Pendente){if(!ValidadorAcademico::textoValido(n))throw DadoInvalidoException("Nome da avaliação inválido.");if(!ValidadorAcademico::valorAvaliacaoValido(v))throw DadoInvalidoException("Valor da avaliação inválido.");if(!d.empty()&&!ValidadorAcademico::dataValida(d))throw DadoInvalidoException("Data limite inválida.");}
const std::string& Avaliacao::getNome()const{return nome_;} double Avaliacao::getValorTotal()const{return valorTotal_;} const std::string& Avaliacao::getDataLimite()const{return dataLimite_;} double Avaliacao::getNotaObtida()const{return nota_;} StatusAvaliacao Avaliacao::getStatus()const{return status_;} bool Avaliacao::isRealizada()const{return status_==StatusAvaliacao::Realizada;} bool Avaliacao::isEstimativa()const{return status_==StatusAvaliacao::Estimada;}
void Avaliacao::setNome(const std::string& n){if(!ValidadorAcademico::textoValido(n))throw DadoInvalidoException("Nome inválido.");nome_=n;}
void Avaliacao::setValorTotal(double v){if(!ValidadorAcademico::valorAvaliacaoValido(v))throw DadoInvalidoException("Valor inválido.");if(status_!=StatusAvaliacao::Pendente&&nota_>v)throw NotaInvalidaException("Nota maior que o novo valor total.");valorTotal_=v;}
void Avaliacao::setDataLimite(const std::string& d){if(!d.empty()&&!ValidadorAcademico::dataValida(d))throw DadoInvalidoException("Data inválida.");dataLimite_=d;}
void Avaliacao::registrarNota(double n){if(!ValidadorAcademico::notaValida(n,valorTotal_))throw NotaInvalidaException("Nota fora do intervalo permitido.");nota_=n;status_=StatusAvaliacao::Realizada;}
void Avaliacao::definirEstimativa(double n){if(status_==StatusAvaliacao::Realizada)throw SemestriaException("Avaliação já realizada.");if(!ValidadorAcademico::notaValida(n,valorTotal_))throw NotaInvalidaException("Estimativa fora do intervalo permitido.");nota_=n;status_=StatusAvaliacao::Estimada;}
void Avaliacao::removerEstimativa(){if(status_==StatusAvaliacao::Estimada){nota_=0;status_=StatusAvaliacao::Pendente;}} void Avaliacao::cancelarRegistro(){nota_=0;status_=StatusAvaliacao::Pendente;}
}
