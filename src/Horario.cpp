#include "Horario.hpp"
#include "ValidadorAcademico.hpp"
#include "Excecoes.hpp"
#include <sstream>
#include <iomanip>
namespace semestria {
std::string paraTexto(DiaSemana d){switch(d){case DiaSemana::Domingo:return "Domingo";case DiaSemana::Segunda:return "Segunda";case DiaSemana::Terca:return "Terça";case DiaSemana::Quarta:return "Quarta";case DiaSemana::Quinta:return "Quinta";case DiaSemana::Sexta:return "Sexta";case DiaSemana::Sabado:return "Sábado";}return "Desconhecido";}
Horario::Horario(DiaSemana d,int hi,int mi,int hf,int mf,const std::string& s):dia_(d),horaInicio_(hi),minutoInicio_(mi),horaFim_(hf),minutoFim_(mf),sala_(s){if(!ValidadorAcademico::horaValida(hi,mi)||!ValidadorAcademico::horaValida(hf,mf)||inicioEmMinutos()>=fimEmMinutos()) throw HorarioInvalidoException("Intervalo de horário inválido.");}
DiaSemana Horario::getDia()const{return dia_;} int Horario::getHoraInicio()const{return horaInicio_;} int Horario::getMinutoInicio()const{return minutoInicio_;} int Horario::getHoraFim()const{return horaFim_;} int Horario::getMinutoFim()const{return minutoFim_;} const std::string& Horario::getSala()const{return sala_;} void Horario::setSala(const std::string&s){sala_=s;}
int Horario::inicioEmMinutos()const{return horaInicio_*60+minutoInicio_;} int Horario::fimEmMinutos()const{return horaFim_*60+minutoFim_;} int Horario::duracaoEmMinutos()const{return fimEmMinutos()-inicioEmMinutos();}
bool Horario::colideCom(const Horario&o)const{return dia_==o.dia_&&inicioEmMinutos()<o.fimEmMinutos()&&o.inicioEmMinutos()<fimEmMinutos();}
std::string Horario::toString()const{std::ostringstream o;o<<paraTexto(dia_)<<' '<<std::setfill('0')<<std::setw(2)<<horaInicio_<<':'<<std::setw(2)<<minutoInicio_<<'-'<<std::setw(2)<<horaFim_<<':'<<std::setw(2)<<minutoFim_;if(!sala_.empty())o<<" ("<<sala_<<")";return o.str();}
bool Horario::operator<(const Horario&o)const{return dia_==o.dia_?inicioEmMinutos()<o.inicioEmMinutos():static_cast<int>(dia_)<static_cast<int>(o.dia_);} bool Horario::operator==(const Horario&o)const{return dia_==o.dia_&&inicioEmMinutos()==o.inicioEmMinutos()&&fimEmMinutos()==o.fimEmMinutos();}
}
