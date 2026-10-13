# User Stories - Semestria

## US01: Cadastro e Gerenciamento de Disciplinas
* **Descrição:** Como aluno de graduação, quero cadastrar as disciplinas do meu semestre informando nome, código, carga horária e créditos, para manter minha grade acadêmica organizada.
* **Critérios de Aceitação:**
  * O sistema deve validar se o código da disciplina é único no semestre.
  * O usuário deve conseguir definir a quantidade de créditos e a carga horária em horas-aula.
  * O sistema deve permitir editar ou remover uma disciplina cadastrada.
  * O sistema deve rejeitar disciplinas cadastradas com carga horária negativa ou zerada.

## US02: Monitoramento e Alerta de Frequência
* **Descrição:** Como estudante com rotina concorrida, quero registrar minhas faltas e visualizar o percentual de frequência em tempo real, para evitar reprovação por infrequência (FI).
* **Critérios de Aceitação:**
  * O sistema deve calcular o limite máximo permitido de faltas com base na carga horária (25%).
  * O sistema deve emitir um alerta visual de perigo quando o aluno atingir 80% do limite de faltas permitidas.
  * O sistema deve notificar que o aluno foi reprovado por infrequência caso o limite seja ultrapassado.
  * O aluno deve conseguir incrementar ou decrementar o contador de faltas de uma matéria.

## US03: Lançamento e Acompanhamento de Avaliações
* **Descrição:** Como aluno, quero cadastrar avaliações (provas, trabalhos, VPLs) associadas a cada disciplina, com seus respectivos pesos e notas, para acompanhar meus pontos acumulados.
* **Critérios de Aceitação:**
  * Cada avaliação cadastrada deve conter nome, valor total e nota obtida pelo aluno.
  * A soma dos valores máximos das avaliações de uma disciplina não pode ultrapassar 100 pontos.
  * O sistema deve exibir o total de pontos já distribuídos e o total de pontos já conquistados.
  * O sistema deve calcular quantos pontos ainda restam em disputa na matéria.

## US04: Projeção de Nota Semestral Global (NSG)
* **Descrição:** Como graduando focado em rendimento acadêmico, quero simular e visualizar a projeção da minha NSG com base nas notas atuais e estimadas, para planejar meu esforço até o fim do semestre.
* **Critérios de Aceitação:**
  * O cálculo da NSG ponderada deve considerar o aproveitamento final de cada matéria multiplicado pelo seu número de créditos.
  * O sistema deve permitir inserir notas hipotéticas nas avaliações futuras para simular diferentes cenários de NSG.
  * O relatório final deve exibir a NSG atual (apenas pontos consolidados) e a NSG máxima possível.

## US05: Gestão de Horários e Detecção de Conflitos
* **Descrição:** Como aluno matriculado em múltiplos turnos, quero cadastrar os horários semanais das minhas aulas, para visualizar minha grade horária e prevenir choque de horários.
* **Critérios de Aceitação:**
  * O aluno pode associar dias da semana e intervalos de horário a cada disciplina cadastrada.
  * O sistema deve emitir um erro explícito caso o usuário tente cadastrar duas aulas no mesmo dia e horário coincidente.
  * O sistema deve fornecer uma visualização formatada da grade horária semanal.

## US06: Exportação de Relatório de Desempenho
* **Descrição:** Como monitor ou orientador acadêmico, quero exportar um relatório consolidado contendo a situação de todas as matérias de um aluno, para analisar histórico de faltas e probabilidade de aprovação.
* **Critérios de Aceitação:**
  * O relatório deve listar todas as matérias com status: "Aprovado", "Em Andamento" ou "Reprovado".
  * O documento deve exibir o total de créditos cursados e o somatório de horas acumuladas no semestre.
  * O relatório deve detalhar quais disciplinas estão com risco iminente de reprovação por nota (< 60%) ou faltas.
