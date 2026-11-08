#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "Semestre.hpp"
#include "Disciplina.hpp"
#include "Avaliacao.hpp"
#include "Horario.hpp"
#include "RelatorioSemestral.hpp"
#include "Excecoes.hpp"

using namespace std;
using namespace semestria;

void popularDadosIniciais(Semestre& semestre) {
    try {
        // Disciplina 1: DCC004 - PDS II
        Disciplina pds2("DCC004", "PDS II", 4, 60);
        pds2.adicionarHorario(Horario(DiaSemana::Terca, 14, 0, 15, 40, "Auditorio 3 ICEx"));
        pds2.adicionarHorario(Horario(DiaSemana::Quinta, 14, 0, 15, 40, "Lab 102 DCC"));
        pds2.adicionarAvaliacao(Avaliacao("VPLs", 20.0));
        pds2.adicionarAvaliacao(Avaliacao("Prova 1", 20.0));
        pds2.adicionarAvaliacao(Avaliacao("Prova 2", 20.0));
        pds2.adicionarAvaliacao(Avaliacao("Trabalho Pratico", 40.0));

        pds2.buscarAvaliacao("VPLs").registrarNota(19.0);
        pds2.buscarAvaliacao("Prova 1").registrarNota(16.0);
        pds2.buscarAvaliacao("Prova 2").definirEstimativa(18.0);
        pds2.buscarAvaliacao("Trabalho Pratico").definirEstimativa(38.0);
        pds2.registrarFalta(4);

        semestre.adicionarDisciplina(pds2);

        // Disciplina 2: MAT002 - Calculo II
        Disciplina calc2("MAT002", "Calculo II", 6, 90);
        calc2.adicionarHorario(Horario(DiaSemana::Segunda, 8, 0, 9, 40, "Sala 2012 ICEx"));
        calc2.adicionarHorario(Horario(DiaSemana::Quarta, 8, 0, 9, 40, "Sala 2012 ICEx"));
        calc2.adicionarHorario(Horario(DiaSemana::Sexta, 8, 0, 9, 40, "Sala 2012 ICEx"));
        calc2.adicionarAvaliacao(Avaliacao("Prova 1", 30.0));
        calc2.adicionarAvaliacao(Avaliacao("Prova 2", 35.0));
        calc2.adicionarAvaliacao(Avaliacao("Prova 3", 35.0));

        calc2.buscarAvaliacao("Prova 1").registrarNota(26.0);
        calc2.buscarAvaliacao("Prova 2").definirEstimativa(28.0);
        calc2.registrarFalta(8);

        semestre.adicionarDisciplina(calc2);
    } catch (const std::exception& e) {
        cerr << "Erro ao inicializar dados demo: " << e.what() << endl;
    }
}

void exibirMenu() {
    cout << "\n========================================================\n";
    cout << "             SEMESTRIA - GESTAO ACADEMICA               \n";
    cout << "========================================================\n";
    cout << "  1. Cadastrar Disciplina\n";
    cout << "  2. Adicionar Horario de Aula a Disciplina\n";
    cout << "  3. Cadastrar Avaliacao em Disciplina\n";
    cout << "  4. Lancar Nota Oficial / Estimada em Avaliacao\n";
    cout << "  5. Registrar Faltas (Horas-aula)\n";
    cout << "  6. Exibir Relatorio Geral do Semestre e NSG\n";
    cout << "  7. Exibir Relatorio Completo (Grade, Notas e Faltas)\n";
    cout << "  8. Exportar Relatorio para Arquivo Texto\n";
    cout << "  9. [DEMO DEFENSIVA] Simular Choque de Horario\n";
    cout << " 10. [DEMO DEFENSIVA] Simular Ultrapassar 100 Pontos\n";
    cout << "  0. Sair\n";
    cout << "--------------------------------------------------------\n";
    cout << "Escolha uma opcao: ";
}

DiaSemana converterDia(int d) {
    switch (d) {
        case 1: return DiaSemana::Domingo;
        case 2: return DiaSemana::Segunda;
        case 3: return DiaSemana::Terca;
        case 4: return DiaSemana::Quarta;
        case 5: return DiaSemana::Quinta;
        case 6: return DiaSemana::Sexta;
        case 7: return DiaSemana::Sabado;
        default: throw HorarioInvalidoException("Dia da semana invalido (escolha de 1 a 7).");
    }
}

int main() {
    Semestre semestre("2026/2");
    popularDadosIniciais(semestre);

    cout << "\n>>> Bem-vindo ao Semestria (Periodo " << semestre.getIdentificador() << ") <<<\n";
    cout << "Dados de demonstracao carregados com sucesso (PDS II e Calculo II).\n";

    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n[ERRO]: Entrada invalida. Digite um numero valido do menu.\n";
            continue;
        }

        try {
            if (opcao == 1) {
                string codigo, nome;
                int creditos, ch;
                cout << "\n--- Cadastrar Disciplina ---\n";
                cout << "Codigo (ex: ELE032): "; cin >> codigo;
                cin.ignore();
                cout << "Nome da disciplina: "; getline(cin, nome);
                cout << "Numero de creditos: "; cin >> creditos;
                cout << "Carga horaria total (horas-aula): "; cin >> ch;

                semestre.adicionarDisciplina(Disciplina(codigo, nome, creditos, ch));
                cout << ">> Disciplina " << codigo << " cadastrada com sucesso!\n";

            } else if (opcao == 2) {
                string codigo, sala;
                int diaNum, hIni, mIni, hFim, mFim;
                cout << "\n--- Adicionar Horario de Aula ---\n";
                cout << "Codigo da disciplina: "; cin >> codigo;
                cout << "Dia (1=Dom, 2=Seg, 3=Ter, 4=Qua, 5=Qui, 6=Sex, 7=Sab): "; cin >> diaNum;
                cout << "Hora de Inicio (0-23): "; cin >> hIni;
                cout << "Minuto de Inicio (0-59): "; cin >> mIni;
                cout << "Hora de Termino (0-23): "; cin >> hFim;
                cout << "Minuto de Termino (0-59): "; cin >> mFim;
                cin.ignore();
                cout << "Sala / Local (ex: ICEx 2012): "; getline(cin, sala);

                DiaSemana dia = converterDia(diaNum);
                semestre.adicionarHorario(codigo, Horario(dia, hIni, mIni, hFim, mFim, sala));
                cout << ">> Horario adicionado com sucesso!\n";

            } else if (opcao == 3) {
                string codigo, nomeAv, data;
                double valor;
                cout << "\n--- Cadastrar Avaliacao ---\n";
                cout << "Codigo da disciplina: "; cin >> codigo;
                cin.ignore();
                cout << "Nome da avaliacao (ex: Seminario): "; getline(cin, nomeAv);
                cout << "Valor total da avaliacao (pontos): "; cin >> valor;
                cin.ignore();
                cout << "Data limite (opcional, DD/MM/AAAA ou enter para vazio): "; getline(cin, data);

                semestre.buscarDisciplina(codigo).adicionarAvaliacao(Avaliacao(nomeAv, valor, data));
                cout << ">> Avaliacao '" << nomeAv << "' adicionada a " << codigo << "!\n";

            } else if (opcao == 4) {
                string codigo, nomeAv;
                int tipoNota;
                double nota;
                cout << "\n--- Lancar Nota ---\n";
                cout << "Codigo da disciplina: "; cin >> codigo;
                cin.ignore();
                cout << "Nome da avaliacao: "; getline(cin, nomeAv);
                cout << "Tipo (1 = Nota Oficial/Realizada, 2 = Nota Estimada/Simulacao): "; cin >> tipoNota;
                cout << "Nota obtida/estimada: "; cin >> nota;

                Avaliacao& av = semestre.buscarDisciplina(codigo).buscarAvaliacao(nomeAv);
                if (tipoNota == 1) {
                    av.registrarNota(nota);
                    cout << ">> Nota oficial de " << nota << " registrada!\n";
                } else if (tipoNota == 2) {
                    av.definirEstimativa(nota);
                    cout << ">> Nota estimada de " << nota << " definida para projecao de NSG!\n";
                } else {
                    cout << "Tipo de nota invalido.\n";
                }

            } else if (opcao == 5) {
                string codigo;
                int horas;
                cout << "\n--- Registrar Faltas ---\n";
                cout << "Codigo da disciplina: "; cin >> codigo;
                cout << "Quantidade de horas-aula de falta a lancar: "; cin >> horas;

                semestre.buscarDisciplina(codigo).registrarFalta(horas);
                cout << ">> " << horas << " hora(s) de falta registrada(s) em " << codigo << "!\n";

            } else if (opcao == 6) {
                RelatorioSemestral rel(semestre);
                cout << "\n========================================================\n";
                cout << "               VISAO GERAL DO SEMESTRE                  \n";
                cout << "========================================================\n";
                rel.gerarVisaoGeral(cout);

            } else if (opcao == 7) {
                RelatorioSemestral rel(semestre);
                cout << "\n========================================================\n";
                cout << "                  RELATORIO COMPLETO                    \n";
                cout << "========================================================\n";
                rel.gerarCompleto(cout);

            } else if (opcao == 8) {
                string arquivo = "relatorio_semestria.txt";
                RelatorioSemestral rel(semestre);
                rel.exportar(arquivo);
                cout << "\n>> Relatorio exportado com sucesso para '" << arquivo << "'!\n";

            } else if (opcao == 9) {
                cout << "\n[DEMONSTRACAO DEFENSIVA: CONFLITO DE HORARIO]\n";
                cout << "Tentando cadastrar 'Sistemas Digitais' na Terca 14:00-15:40...\n";
                cout << "(PDS II ja ocupa esse horario no auditorio 3 do ICEx)\n";
                Disciplina con("ELE010", "Sistemas Digitais", 4, 60);
                con.adicionarHorario(Horario(DiaSemana::Terca, 14, 0, 15, 40, "Sala 1010"));
                semestre.adicionarDisciplina(con);

            } else if (opcao == 10) {
                cout << "\n[DEMONSTRACAO DEFENSIVA: LIMITE DE PONTUACAO]\n";
                cout << "Tentando adicionar avaliacao de 30 pontos em PDS II...\n";
                cout << "(PDS II ja tem 20 + 20 + 20 + 40 = 100 pontos distribuidos)\n";
                semestre.buscarDisciplina("DCC004").adicionarAvaliacao(Avaliacao("Prova Extra", 30.0));

            } else if (opcao == 0) {
                cout << "\nEncerrando o Semestria. Bom semestre e boa aprovacao!\n";
            } else {
                cout << "\n[AVISO]: Opcao inexistente no menu.\n";
            }

        } catch (const ConflitoHorarioException& e) {
            cout << "\n[EXCECAO CAPTURADA - CONFLITO DE HORARIO]: " << e.what() << "\n";
        } catch (const LimitePontuacaoExcedidoException& e) {
            cout << "\n[EXCECAO CAPTURADA - LIMITE DE PONTOS]: " << e.what() << "\n";
        } catch (const NotaInvalidaException& e) {
            cout << "\n[EXCECAO CAPTURADA - NOTA INVALIDA]: " << e.what() << "\n";
        } catch (const SemestriaException& e) {
            cout << "\n[EXCECAO SEMESTRIA CAPTURADA]: " << e.what() << "\n";
        } catch (const std::exception& e) {
            cout << "\n[ERRO GERAL]: " << e.what() << "\n";
        }
    }

    return 0;
}