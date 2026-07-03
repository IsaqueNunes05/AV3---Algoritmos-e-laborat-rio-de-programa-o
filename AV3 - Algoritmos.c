#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <unistd.h>
#include <ctype.h>

#define max 50
int op=0, sop=0, ida=0, idc=0, idt=0, idm=0, cont;
char resp, nome1[15], nome2[15];
_Bool enc=false, enc_fk=false;

struct aluno{
    char pk_cpf[15];
    char nome[max];
    char admissao[11];
    char dtNasc[11];
    char escolaridade[max];
};

struct curso{
    int pk_codCurso;
    char nome[max];
    int cargaHoraria;
    char modalidade[20];
};

struct turma{
    int pk_codTurma;
    char nomeTurma[max];
    int qtdAlunos;
    char turno[20];
};

struct matricula{
    int pk_ra;
    char fk_cpf[15];
    int fk_codTurma;
    int fk_codCurso;
};

struct aluno a;
struct curso c;
struct turma t;
struct matricula m;
struct aluno tbaluno[max];
struct curso tbcurso[max];
struct turma tbturma[max];
struct matricula tbmatricula[max];


/* ==========================================================
   UTILITÁRIOS / VALIDAÇÕES
   ========================================================== */

/* Limpa o buffer de entrada (resto do \n deixado por scanf) */
void limparBuffer(){
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* Valida CPF no formato 000.000.000-00 */
_Bool validarCPF(char cpf[15]){
    if (strlen(cpf) != 14) return false;
    for (int i=0; i<14; i++){
        if (i==3 || i==7){
            if (cpf[i] != '.') return false;
        } else if (i==11){
            if (cpf[i] != '-') return false;
        } else {
            if (!isdigit((unsigned char)cpf[i])) return false;
        }
    }
    return true;
}

/* Valida datas no formato DD/MM/AAAA (checa formato e faixas básicas) */
_Bool validarData(char data[11]){
    if (strlen(data) != 10) return false;
    for (int i=0; i<10; i++){
        if (i==2 || i==5){
            if (data[i] != '/') return false;
        } else {
            if (!isdigit((unsigned char)data[i])) return false;
        }
    }
    int dia, mes, ano;
    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    return true;
}

/* Verifica se um campo texto está vazio ou só com espaços */
_Bool campoVazio(char *campo){
    int len = strlen(campo);
    if (len == 0) return true;
    for (int i=0; i<len; i++){
        if (!isspace((unsigned char)campo[i])) return false;
    }
    return true;
}

/* ==========================================================
   MENUS
   ========================================================== */

void subMenu(){
    printf("***********************************\n");
    printf("*            SUBMENU              *\n");
    printf("***********************************\n");
    printf("*   1 - ALUNO                     *\n");
    printf("*   2 - CURSO                     *\n");
    printf("*   3 - TURMA                     *\n");
    printf("*   4 - MATRICULA                 *\n");
    printf("*   5 - RETORNAR                  *\n");
    printf("***********************************\n\n");
}

void menu(){
    printf("***********************************\n");
    printf("*         MENU PRINCIPAL          *\n");
    printf("***********************************\n");
    printf("*   1 - CADASTRAR                 *\n");
    printf("*   2 - CONSULTAR                 *\n");
    printf("*   3 - ALTERAR                   *\n");
    printf("*   4 - EXCLUIR                   *\n");
    printf("*   5 - ORDENAR                   *\n");
    printf("*   6 - IMPRIMIR                  *\n");
    printf("*   7 - RELATORIO GERAL           *\n");
    printf("*   8 - FINALIZAR                 *\n");
    printf("***********************************\n\n");
}

void submenuOrdenar(){
    printf("***********************************\n");
    printf("*             ORDENAR             *\n");
    printf("***********************************\n");
    printf("*   1 - CRESCENTE                 *\n");
    printf("*   2 - DECRESCENTE               *\n");
    printf("*   3 - RETORNAR                  *\n");
    printf("***********************************\n\n");
}

void submenuOpcaoOrdenar(int opcao){
    switch(opcao){
        case 1:
            strcpy(nome1,"CPF");
            strcpy(nome2,"Nome");
            break;
        case 2:
            strcpy(nome1,"Cod. Curso");
            strcpy(nome2,"Nome Curso");
            break;
        case 3:
            strcpy(nome1,"Cod. Turma");
            strcpy(nome2,"Nome Turma");
            break;
        case 4:
            strcpy(nome1,"RA");
            strcpy(nome2,"CPF Aluno");
            break;
        default:
            if ((opcao<1) || (opcao>4)){
                system("clear");
                printf("Opção inválida!");
                sleep(2);
            }
            break;
    }

    printf("***********************************\n");
    printf("             ORDENAR               \n");
    printf("***********************************\n");
    printf("   1 - %s                    \n",nome1);
    printf("   2 - %s                    \n",nome2);
    printf("   3 - RETORNAR                    \n");
    printf("***********************************\n\n");
}

/* ==========================================================
   BUSCAS (chave primária)
   ========================================================== */

_Bool buscar_aluno_pk_cpf(char cpf[15]){
    _Bool enc=false;
    int i=0;
    while((i<ida)&&(enc==false)){
        if (strcmp(tbaluno[i].pk_cpf,cpf)==0){
           enc=true;
        }else{
            i++;
        }
    }
    return enc;
}

_Bool buscar_curso_pk_codCurso(int codCurso){
    _Bool enc=false;
    int i=0;
    while((i<idc)&&(enc==false)){
        if (tbcurso[i].pk_codCurso==codCurso){
           enc=true;
        }else{
            i++;
        }
    }
    return enc;
}

_Bool buscar_turma_pk_codTurma(int codTurma){
    _Bool enc=false;
    int i=0;
    while((i<idt)&&(enc==false)){
        if (tbturma[i].pk_codTurma==codTurma){
           enc=true;
        }else{
            i++;
        }
    }
    return enc;
}

_Bool buscar_matricula_pk_ra(int ra){
    _Bool enc=false;
    int i=0;
    while((i<idm)&&(enc==false)){
        if (tbmatricula[i].pk_ra==ra){
           enc=true;
        }else{
            i++;
        }
    }
    return enc;
}

/* Retorna o índice do registro no vetor (-1 se não encontrar) */
int indice_aluno(char cpf[15]){
    for(int i=0;i<ida;i++) if(strcmp(tbaluno[i].pk_cpf,cpf)==0) return i;
    return -1;
}
int indice_curso(int cod){
    for(int i=0;i<idc;i++) if(tbcurso[i].pk_codCurso==cod) return i;
    return -1;
}
int indice_turma(int cod){
    for(int i=0;i<idt;i++) if(tbturma[i].pk_codTurma==cod) return i;
    return -1;
}
int indice_matricula(int ra){
    for(int i=0;i<idm;i++) if(tbmatricula[i].pk_ra==ra) return i;
    return -1;
}

/* Verifica se existe alguma matrícula usando a FK informada (bloqueia exclusão) */
_Bool existeMatriculaAluno(char cpf[15]){
    for(int i=0;i<idm;i++) if(strcmp(tbmatricula[i].fk_cpf,cpf)==0) return true;
    return false;
}
_Bool existeMatriculaCurso(int cod){
    for(int i=0;i<idm;i++) if(tbmatricula[i].fk_codCurso==cod) return true;
    return false;
}
_Bool existeMatriculaTurma(int cod){
    for(int i=0;i<idm;i++) if(tbmatricula[i].fk_codTurma==cod) return true;
    return false;
}

/* ==========================================================
   1) CADASTRO
   ========================================================== */

void cadastrarAluno(){
    system("clear");
    printf("=== CADASTRO DE ALUNO ===\n\n");
    if (ida >= max){
        printf("Limite máximo de alunos atingido!\n");
        sleep(2);
        return;
    }

    char cpf[15];
    do{
        printf("CPF (000.000.000-00): ");
        scanf(" %14[^\n]", cpf);
        limparBuffer();
        if (!validarCPF(cpf)){
            printf("CPF em formato inválido! Use 000.000.000-00.\n");
        } else if (buscar_aluno_pk_cpf(cpf)){
            printf("Já existe um aluno cadastrado com esse CPF!\n");
        }
    } while(!validarCPF(cpf) || buscar_aluno_pk_cpf(cpf));
    strcpy(a.pk_cpf, cpf);

    char nomeA[max];
    do{
        printf("Nome: ");
        scanf(" %49[^\n]", nomeA);
        limparBuffer();
        if (campoVazio(nomeA)) printf("Campo obrigatório!\n");
    } while(campoVazio(nomeA));
    strcpy(a.nome, nomeA);

    char adm[11];
    do{
        printf("Data de Admissão (DD/MM/AAAA): ");
        scanf(" %10[^\n]", adm);
        limparBuffer();
        if (!validarData(adm)) printf("Data inválida!\n");
    } while(!validarData(adm));
    strcpy(a.admissao, adm);

    char nasc[11];
    do{
        printf("Data de Nascimento (DD/MM/AAAA): ");
        scanf(" %10[^\n]", nasc);
        limparBuffer();
        if (!validarData(nasc)) printf("Data inválida!\n");
    } while(!validarData(nasc));
    strcpy(a.dtNasc, nasc);

    char esc[max];
    do{
        printf("Escolaridade: ");
        scanf(" %49[^\n]", esc);
        limparBuffer();
        if (campoVazio(esc)) printf("Campo obrigatório!\n");
    } while(campoVazio(esc));
    strcpy(a.escolaridade, esc);

    tbaluno[ida] = a;
    ida++;
    printf("\nAluno cadastrado com sucesso!\n");
    sleep(2);
}

void cadastrarCurso(){
    system("clear");
    printf("=== CADASTRO DE CURSO ===\n\n");
    if (idc >= max){
        printf("Limite máximo de cursos atingido!\n");
        sleep(2);
        return;
    }

    int cod;
    do{
        printf("Código do Curso: ");
        scanf("%d", &cod);
        limparBuffer();
        if (buscar_curso_pk_codCurso(cod)) printf("Já existe um curso com esse código!\n");
    } while(buscar_curso_pk_codCurso(cod));
    c.pk_codCurso = cod;

    char nomeC[max];
    do{
        printf("Nome do Curso: ");
        scanf(" %49[^\n]", nomeC);
        limparBuffer();
        if (campoVazio(nomeC)) printf("Campo obrigatório!\n");
    } while(campoVazio(nomeC));
    strcpy(c.nome, nomeC);

    int ch;
    do{
        printf("Carga Horária: ");
        scanf("%d", &ch);
        limparBuffer();
        if (ch <= 0) printf("A carga horária deve ser maior que zero!\n");
    } while(ch <= 0);
    c.cargaHoraria = ch;

    char mod[20];
    do{
        printf("Modalidade: ");
        scanf(" %19[^\n]", mod);
        limparBuffer();
        if (campoVazio(mod)) printf("Campo obrigatório!\n");
    } while(campoVazio(mod));
    strcpy(c.modalidade, mod);

    tbcurso[idc] = c;
    idc++;
    printf("\nCurso cadastrado com sucesso!\n");
    sleep(2);
}

void cadastrarTurma(){
    system("clear");
    printf("=== CADASTRO DE TURMA ===\n\n");
    if (idt >= max){
        printf("Limite máximo de turmas atingido!\n");
        sleep(2);
        return;
    }

    int cod;
    do{
        printf("Código da Turma: ");
        scanf("%d", &cod);
        limparBuffer();
        if (buscar_turma_pk_codTurma(cod)) printf("Já existe uma turma com esse código!\n");
    } while(buscar_turma_pk_codTurma(cod));
    t.pk_codTurma = cod;

    char nomeT[max];
    do{
        printf("Nome da Turma: ");
        scanf(" %49[^\n]", nomeT);
        limparBuffer();
        if (campoVazio(nomeT)) printf("Campo obrigatório!\n");
    } while(campoVazio(nomeT));
    strcpy(t.nomeTurma, nomeT);

    int qtd;
    do{
        printf("Quantidade de Alunos: ");
        scanf("%d", &qtd);
        limparBuffer();
        if (qtd < 0) printf("A quantidade não pode ser negativa!\n");
    } while(qtd < 0);
    t.qtdAlunos = qtd;

    char turno[20];
    do{
        printf("Turno: ");
        scanf(" %19[^\n]", turno);
        limparBuffer();
        if (campoVazio(turno)) printf("Campo obrigatório!\n");
    } while(campoVazio(turno));
    strcpy(t.turno, turno);

    tbturma[idt] = t;
    idt++;
    printf("\nTurma cadastrada com sucesso!\n");
    sleep(2);
}

void cadastrarMatricula(){
    system("clear");
    printf("=== CADASTRO DE MATRICULA ===\n\n");
    if (idm >= max){
        printf("Limite máximo de matrículas atingido!\n");
        sleep(2);
        return;
    }
    if (ida==0 || idc==0 || idt==0){
        printf("É necessário ter ao menos 1 aluno, 1 curso e 1 turma cadastrados!\n");
        sleep(3);
        return;
    }

    int ra;
    do{
        printf("RA (Registro do Aluno): ");
        scanf("%d", &ra);
        limparBuffer();
        if (buscar_matricula_pk_ra(ra)) printf("Já existe uma matrícula com esse RA!\n");
    } while(buscar_matricula_pk_ra(ra));
    m.pk_ra = ra;

    char cpf[15];
    do{
        printf("CPF do Aluno: ");
        scanf(" %14[^\n]", cpf);
        limparBuffer();
        if (!buscar_aluno_pk_cpf(cpf)) printf("Aluno não encontrado! Cadastre o aluno primeiro.\n");
    } while(!buscar_aluno_pk_cpf(cpf));
    strcpy(m.fk_cpf, cpf);

    int codC;
    do{
        printf("Código do Curso: ");
        scanf("%d", &codC);
        limparBuffer();
        if (!buscar_curso_pk_codCurso(codC)) printf("Curso não encontrado!\n");
    } while(!buscar_curso_pk_codCurso(codC));
    m.fk_codCurso = codC;

    int codT;
    do{
        printf("Código da Turma: ");
        scanf("%d", &codT);
        limparBuffer();
        if (!buscar_turma_pk_codTurma(codT)) printf("Turma não encontrada!\n");
    } while(!buscar_turma_pk_codTurma(codT));
    m.fk_codTurma = codT;

    tbmatricula[idm] = m;
    idm++;
    printf("\nMatrícula cadastrada com sucesso!\n");
    sleep(2);
}

/* ==========================================================
   2) CONSULTA
   ========================================================== */

void consultarAluno(){
    system("clear");
    printf("=== CONSULTA DE ALUNO ===\n\n");
    if (ida==0){ printf("Cadastre primeiro o(s) aluno(s)!\n"); sleep(2); return; }

    char cpf[15];
    printf("Digite o CPF: ");
    scanf(" %14[^\n]", cpf);
    limparBuffer();

    int idx = indice_aluno(cpf);
    if (idx == -1){
        printf("Aluno não encontrado!\n");
    } else {
        printf("\nCPF: %s\n", tbaluno[idx].pk_cpf);
        printf("Nome: %s\n", tbaluno[idx].nome);
        printf("Admissão: %s\n", tbaluno[idx].admissao);
        printf("Data de Nascimento: %s\n", tbaluno[idx].dtNasc);
        printf("Escolaridade: %s\n", tbaluno[idx].escolaridade);
    }
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void consultarCurso(){
    system("clear");
    printf("=== CONSULTA DE CURSO ===\n\n");
    if (idc==0){ printf("Cadastre primeiro o(s) curso(s)!\n"); sleep(2); return; }

    int cod;
    printf("Digite o Código do Curso: ");
    scanf("%d", &cod);
    limparBuffer();

    int idx = indice_curso(cod);
    if (idx == -1){
        printf("Curso não encontrado!\n");
    } else {
        printf("\nCódigo: %d\n", tbcurso[idx].pk_codCurso);
        printf("Nome: %s\n", tbcurso[idx].nome);
        printf("Carga Horária: %d\n", tbcurso[idx].cargaHoraria);
        printf("Modalidade: %s\n", tbcurso[idx].modalidade);
    }
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void consultarTurma(){
    system("clear");
    printf("=== CONSULTA DE TURMA ===\n\n");
    if (idt==0){ printf("Cadastre primeiro a(s) turma(s)!\n"); sleep(2); return; }

    int cod;
    printf("Digite o Código da Turma: ");
    scanf("%d", &cod);
    limparBuffer();

    int idx = indice_turma(cod);
    if (idx == -1){
        printf("Turma não encontrada!\n");
    } else {
        printf("\nCódigo: %d\n", tbturma[idx].pk_codTurma);
        printf("Nome: %s\n", tbturma[idx].nomeTurma);
        printf("Quantidade de Alunos: %d\n", tbturma[idx].qtdAlunos);
        printf("Turno: %s\n", tbturma[idx].turno);
    }
    printf("\nPressione ENTER para continuar...");
    getchar();
}

/* Consulta com "join" manual usando as FKs (fk_cpf, fk_codCurso, fk_codTurma) */
void consultarMatricula(){
    system("clear");
    printf("=== CONSULTA DE MATRICULA ===\n\n");
    if (idm==0){ printf("Cadastre primeiro a(s) matricula(s)!\n"); sleep(2); return; }

    int ra;
    printf("Digite o RA: ");
    scanf("%d", &ra);
    limparBuffer();

    int idx = indice_matricula(ra);
    if (idx == -1){
        printf("Matrícula não encontrada!\n");
    } else {
        printf("\nRA: %d\n", tbmatricula[idx].pk_ra);

        printf("CPF do Aluno: %s", tbmatricula[idx].fk_cpf);
        int ia = indice_aluno(tbmatricula[idx].fk_cpf);
        if (ia != -1) printf(" (Nome: %s)", tbaluno[ia].nome);
        printf("\n");

        printf("Código do Curso: %d", tbmatricula[idx].fk_codCurso);
        int ic = indice_curso(tbmatricula[idx].fk_codCurso);
        if (ic != -1) printf(" (Nome: %s)", tbcurso[ic].nome);
        printf("\n");

        printf("Código da Turma: %d", tbmatricula[idx].fk_codTurma);
        int it = indice_turma(tbmatricula[idx].fk_codTurma);
        if (it != -1) printf(" (Nome: %s)", tbturma[it].nomeTurma);
        printf("\n");
    }
    printf("\nPressione ENTER para continuar...");
    getchar();
}

/* ==========================================================
   3) ALTERAÇÃO
   ========================================================== */

void alterarAluno(){
    system("clear");
    printf("=== ALTERAÇÃO DE ALUNO ===\n\n");
    if (ida==0){ printf("Cadastre primeiro o(s) aluno(s)!\n"); sleep(2); return; }

    char cpf[15];
    printf("Digite o CPF do aluno a alterar: ");
    scanf(" %14[^\n]", cpf);
    limparBuffer();

    int idx = indice_aluno(cpf);
    if (idx == -1){ printf("Aluno não encontrado!\n"); sleep(2); return; }

    printf("\nDados atuais:\n");
    printf("Nome: %s | Admissão: %s | Nascimento: %s | Escolaridade: %s\n\n",
        tbaluno[idx].nome, tbaluno[idx].admissao, tbaluno[idx].dtNasc, tbaluno[idx].escolaridade);

    char nomeA[max];
    do{
        printf("Novo Nome: ");
        scanf(" %49[^\n]", nomeA);
        limparBuffer();
        if (campoVazio(nomeA)) printf("Campo obrigatório!\n");
    } while(campoVazio(nomeA));
    strcpy(tbaluno[idx].nome, nomeA);

    char adm[11];
    do{
        printf("Nova Admissão (DD/MM/AAAA): ");
        scanf(" %10[^\n]", adm);
        limparBuffer();
        if (!validarData(adm)) printf("Data inválida!\n");
    } while(!validarData(adm));
    strcpy(tbaluno[idx].admissao, adm);

    char nasc[11];
    do{
        printf("Nova Data de Nascimento (DD/MM/AAAA): ");
        scanf(" %10[^\n]", nasc);
        limparBuffer();
        if (!validarData(nasc)) printf("Data inválida!\n");
    } while(!validarData(nasc));
    strcpy(tbaluno[idx].dtNasc, nasc);

    char esc[max];
    do{
        printf("Nova Escolaridade: ");
        scanf(" %49[^\n]", esc);
        limparBuffer();
        if (campoVazio(esc)) printf("Campo obrigatório!\n");
    } while(campoVazio(esc));
    strcpy(tbaluno[idx].escolaridade, esc);

    printf("\nAluno alterado com sucesso!\n");
    sleep(2);
}

void alterarCurso(){
    system("clear");
    printf("=== ALTERAÇÃO DE CURSO ===\n\n");
    if (idc==0){ printf("Cadastre primeiro o(s) curso(s)!\n"); sleep(2); return; }

    int cod;
    printf("Digite o Código do curso a alterar: ");
    scanf("%d", &cod);
    limparBuffer();

    int idx = indice_curso(cod);
    if (idx == -1){ printf("Curso não encontrado!\n"); sleep(2); return; }

    printf("\nDados atuais:\n");
    printf("Nome: %s | Carga Horária: %d | Modalidade: %s\n\n",
        tbcurso[idx].nome, tbcurso[idx].cargaHoraria, tbcurso[idx].modalidade);

    char nomeC[max];
    do{
        printf("Novo Nome: ");
        scanf(" %49[^\n]", nomeC);
        limparBuffer();
        if (campoVazio(nomeC)) printf("Campo obrigatório!\n");
    } while(campoVazio(nomeC));
    strcpy(tbcurso[idx].nome, nomeC);

    int ch;
    do{
        printf("Nova Carga Horária: ");
        scanf("%d", &ch);
        limparBuffer();
        if (ch <= 0) printf("Deve ser maior que zero!\n");
    } while(ch <= 0);
    tbcurso[idx].cargaHoraria = ch;

    char mod[20];
    do{
        printf("Nova Modalidade: ");
        scanf(" %19[^\n]", mod);
        limparBuffer();
        if (campoVazio(mod)) printf("Campo obrigatório!\n");
    } while(campoVazio(mod));
    strcpy(tbcurso[idx].modalidade, mod);

    printf("\nCurso alterado com sucesso!\n");
    sleep(2);
}

void alterarTurma(){
    system("clear");
    printf("=== ALTERAÇÃO DE TURMA ===\n\n");
    if (idt==0){ printf("Cadastre primeiro a(s) turma(s)!\n"); sleep(2); return; }

    int cod;
    printf("Digite o Código da turma a alterar: ");
    scanf("%d", &cod);
    limparBuffer();

    int idx = indice_turma(cod);
    if (idx == -1){ printf("Turma não encontrada!\n"); sleep(2); return; }

    printf("\nDados atuais:\n");
    printf("Nome: %s | Qtd Alunos: %d | Turno: %s\n\n",
        tbturma[idx].nomeTurma, tbturma[idx].qtdAlunos, tbturma[idx].turno);

    char nomeT[max];
    do{
        printf("Novo Nome da Turma: ");
        scanf(" %49[^\n]", nomeT);
        limparBuffer();
        if (campoVazio(nomeT)) printf("Campo obrigatório!\n");
    } while(campoVazio(nomeT));
    strcpy(tbturma[idx].nomeTurma, nomeT);

    int qtd;
    do{
        printf("Nova Quantidade de Alunos: ");
        scanf("%d", &qtd);
        limparBuffer();
        if (qtd < 0) printf("Não pode ser negativa!\n");
    } while(qtd < 0);
    tbturma[idx].qtdAlunos = qtd;

    char turno[20];
    do{
        printf("Novo Turno: ");
        scanf(" %19[^\n]", turno);
        limparBuffer();
        if (campoVazio(turno)) printf("Campo obrigatório!\n");
    } while(campoVazio(turno));
    strcpy(tbturma[idx].turno, turno);

    printf("\nTurma alterada com sucesso!\n");
    sleep(2);
}

void alterarMatricula(){
    system("clear");
    printf("=== ALTERAÇÃO DE MATRICULA ===\n\n");
    if (idm==0){ printf("Cadastre primeiro a(s) matricula(s)!\n"); sleep(2); return; }

    int ra;
    printf("Digite o RA a alterar: ");
    scanf("%d", &ra);
    limparBuffer();

    int idx = indice_matricula(ra);
    if (idx == -1){ printf("Matrícula não encontrada!\n"); sleep(2); return; }

    printf("\nDados atuais:\n");
    printf("CPF: %s | Curso: %d | Turma: %d\n\n",
        tbmatricula[idx].fk_cpf, tbmatricula[idx].fk_codCurso, tbmatricula[idx].fk_codTurma);

    char cpf[15];
    do{
        printf("Novo CPF do Aluno: ");
        scanf(" %14[^\n]", cpf);
        limparBuffer();
        if (!buscar_aluno_pk_cpf(cpf)) printf("Aluno não encontrado!\n");
    } while(!buscar_aluno_pk_cpf(cpf));
    strcpy(tbmatricula[idx].fk_cpf, cpf);

    int codC;
    do{
        printf("Novo Código do Curso: ");
        scanf("%d", &codC);
        limparBuffer();
        if (!buscar_curso_pk_codCurso(codC)) printf("Curso não encontrado!\n");
    } while(!buscar_curso_pk_codCurso(codC));
    tbmatricula[idx].fk_codCurso = codC;

    int codT;
    do{
        printf("Novo Código da Turma: ");
        scanf("%d", &codT);
        limparBuffer();
        if (!buscar_turma_pk_codTurma(codT)) printf("Turma não encontrada!\n");
    } while(!buscar_turma_pk_codTurma(codT));
    tbmatricula[idx].fk_codTurma = codT;

    printf("\nMatrícula alterada com sucesso!\n");
    sleep(2);
}

/* ==========================================================
   4) EXCLUSÃO
   ========================================================== */

void excluirAluno(){
    system("clear");
    printf("=== EXCLUSÃO DE ALUNO ===\n\n");
    if (ida==0){ printf("Cadastre primeiro o(s) aluno(s)!\n"); sleep(2); return; }

    char cpf[15];
    printf("Digite o CPF do aluno a excluir: ");
    scanf(" %14[^\n]", cpf);
    limparBuffer();

    int idx = indice_aluno(cpf);
    if (idx == -1){ printf("Aluno não encontrado!\n"); sleep(2); return; }

    if (existeMatriculaAluno(cpf)){
        printf("Não é possível excluir: existe(m) matrícula(s) vinculada(s) a este aluno!\n");
        sleep(3); return;
    }

    for (int i=idx; i<ida-1; i++) tbaluno[i] = tbaluno[i+1];
    ida--;
    printf("Aluno excluído com sucesso!\n");
    sleep(2);
}

void excluirCurso(){
    system("clear");
    printf("=== EXCLUSÃO DE CURSO ===\n\n");
    if (idc==0){ printf("Cadastre primeiro o(s) curso(s)!\n"); sleep(2); return; }

    int cod;
    printf("Digite o Código do curso a excluir: ");
    scanf("%d", &cod);
    limparBuffer();

    int idx = indice_curso(cod);
    if (idx == -1){ printf("Curso não encontrado!\n"); sleep(2); return; }

    if (existeMatriculaCurso(cod)){
        printf("Não é possível excluir: existe(m) matrícula(s) vinculada(s) a este curso!\n");
        sleep(3); return;
    }

    for (int i=idx; i<idc-1; i++) tbcurso[i] = tbcurso[i+1];
    idc--;
    printf("Curso excluído com sucesso!\n");
    sleep(2);
}

void excluirTurma(){
    system("clear");
    printf("=== EXCLUSÃO DE TURMA ===\n\n");
    if (idt==0){ printf("Cadastre primeiro a(s) turma(s)!\n"); sleep(2); return; }

    int cod;
    printf("Digite o Código da turma a excluir: ");
    scanf("%d", &cod);
    limparBuffer();

    int idx = indice_turma(cod);
    if (idx == -1){ printf("Turma não encontrada!\n"); sleep(2); return; }

    if (existeMatriculaTurma(cod)){
        printf("Não é possível excluir: existe(m) matrícula(s) vinculada(s) a esta turma!\n");
        sleep(3); return;
    }

    for (int i=idx; i<idt-1; i++) tbturma[i] = tbturma[i+1];
    idt--;
    printf("Turma excluída com sucesso!\n");
    sleep(2);
}

void excluirMatricula(){
    system("clear");
    printf("=== EXCLUSÃO DE MATRICULA ===\n\n");
    if (idm==0){ printf("Cadastre primeiro a(s) matricula(s)!\n"); sleep(2); return; }

    int ra;
    printf("Digite o RA a excluir: ");
    scanf("%d", &ra);
    limparBuffer();

    int idx = indice_matricula(ra);
    if (idx == -1){ printf("Matrícula não encontrada!\n"); sleep(2); return; }

    for (int i=idx; i<idm-1; i++) tbmatricula[i] = tbmatricula[i+1];
    idm--;
    printf("Matrícula excluída com sucesso!\n");
    sleep(2);
}

/* ==========================================================
   5) ORDENAÇÃO (crescente/decrescente, para as 4 entidades)
   ========================================================== */

/* ordem: 1 = crescente | 2 = decrescente
   subop: 1 = primeiro campo (chave)  | 2 = segundo campo (nome) */
void ordenar(int ident, int subop, int ordem){
    int qtd;
    switch(ident){
        case 1: qtd = ida; break;
        case 2: qtd = idc; break;
        case 3: qtd = idt; break;
        case 4: qtd = idm; break;
        default: qtd = 0;
    }

    for (int i=0; i<qtd-1; i++){
        for (int y=0; y<qtd-1-i; y++){
            _Bool troca = false;
            int cmp;
            switch(ident){
                case 1: /* ALUNO */
                    cmp = (subop==1) ? strcmp(tbaluno[y].pk_cpf, tbaluno[y+1].pk_cpf)
                                      : strcmp(tbaluno[y].nome, tbaluno[y+1].nome);
                    troca = (ordem==1) ? (cmp>0) : (cmp<0);
                    if (troca){ a=tbaluno[y]; tbaluno[y]=tbaluno[y+1]; tbaluno[y+1]=a; }
                    break;
                case 2: /* CURSO */
                    if (subop==1) troca = (ordem==1) ? (tbcurso[y].pk_codCurso > tbcurso[y+1].pk_codCurso)
                                                       : (tbcurso[y].pk_codCurso < tbcurso[y+1].pk_codCurso);
                    else {
                        cmp = strcmp(tbcurso[y].nome, tbcurso[y+1].nome);
                        troca = (ordem==1) ? (cmp>0) : (cmp<0);
                    }
                    if (troca){ c=tbcurso[y]; tbcurso[y]=tbcurso[y+1]; tbcurso[y+1]=c; }
                    break;
                case 3: /* TURMA */
                    if (subop==1) troca = (ordem==1) ? (tbturma[y].pk_codTurma > tbturma[y+1].pk_codTurma)
                                                       : (tbturma[y].pk_codTurma < tbturma[y+1].pk_codTurma);
                    else {
                        cmp = strcmp(tbturma[y].nomeTurma, tbturma[y+1].nomeTurma);
                        troca = (ordem==1) ? (cmp>0) : (cmp<0);
                    }
                    if (troca){ t=tbturma[y]; tbturma[y]=tbturma[y+1]; tbturma[y+1]=t; }
                    break;
                case 4: /* MATRICULA */
                    if (subop==1) troca = (ordem==1) ? (tbmatricula[y].pk_ra > tbmatricula[y+1].pk_ra)
                                                       : (tbmatricula[y].pk_ra < tbmatricula[y+1].pk_ra);
                    else {
                        cmp = strcmp(tbmatricula[y].fk_cpf, tbmatricula[y+1].fk_cpf);
                        troca = (ordem==1) ? (cmp>0) : (cmp<0);
                    }
                    if (troca){ m=tbmatricula[y]; tbmatricula[y]=tbmatricula[y+1]; tbmatricula[y+1]=m; }
                    break;
            }
        }
    }
    system("clear");
    printf("Ordenação concluída com sucesso!\n");
    sleep(2);
}

/* ==========================================================
   6) IMPRESSÃO / RELATÓRIOS
   ========================================================== */

void imprimir(int ident){
    system("clear");
    switch(ident){
        case 1:
            if (ida>0){
                for (int i=0;i<ida;i++){
                    printf("CPF: %s ",tbaluno[i].pk_cpf);
                    printf("\nNome: %s ",tbaluno[i].nome);
                    printf("\nAdmissão: %s ",tbaluno[i].admissao);
                    printf("\nData de Nascimento: %s ",tbaluno[i].dtNasc);
                    printf("\nEscolaridade: %s \n\n",tbaluno[i].escolaridade);
                }
            }else{
                printf("Cadastre primeiro o(s) aluno(s)!\n");
            }
            break;
        case 2:
            if (idc>0){
                for (int i=0;i<idc;i++){
                    printf("Código do Curso: %d ",tbcurso[i].pk_codCurso);
                    printf("\nNome: %s ",tbcurso[i].nome);
                    printf("\nCarga Horária: %d ",tbcurso[i].cargaHoraria);
                    printf("\nModalidade: %s \n\n",tbcurso[i].modalidade);
                }
            }else{
                printf("Cadastre primeiro o(s) curso(s)!\n");
            }
            break;
        case 3:
            if (idt>0){
                for (int i=0;i<idt;i++){
                    printf("Código da Turma: %d ",tbturma[i].pk_codTurma);
                    printf("\nNome da Turma: %s ",tbturma[i].nomeTurma);
                    printf("\nQuantidade de Alunos: %d ",tbturma[i].qtdAlunos);
                    printf("\nTurno: %s \n\n",tbturma[i].turno);
                }
            }else{
                printf("Cadastre primeiro a(s) turma(s)!\n");
            }
            break;
        case 4:
            if (idm>0){
                for (int i=0;i<idm;i++){
                    printf("Registro do Aluno: %d ",tbmatricula[i].pk_ra);
                    printf("\nCPF do Aluno: %s ",tbmatricula[i].fk_cpf);
                    printf("\nCódigo da Turma: %d ",tbmatricula[i].fk_codTurma);
                    printf("\nCódigo do Curso: %d \n\n",tbmatricula[i].fk_codCurso);
                }
            }else{
                printf("Cadastre primeiro a(s) matricula(s)!\n");
            }
            break;
    }
    printf("Pressione ENTER para continuar...");
    getchar();
}

void relatorioGeral(){
    system("clear");
    printf("================ RELATÓRIO GERAL ================\n\n");

    printf("--- ALUNOS (%d) ---\n", ida);
    for(int i=0;i<ida;i++){
        printf("CPF: %s | Nome: %s | Admissão: %s | Nasc: %s | Escolaridade: %s\n",
            tbaluno[i].pk_cpf, tbaluno[i].nome, tbaluno[i].admissao,
            tbaluno[i].dtNasc, tbaluno[i].escolaridade);
    }

    printf("\n--- CURSOS (%d) ---\n", idc);
    for(int i=0;i<idc;i++){
        printf("Código: %d | Nome: %s | CH: %d | Modalidade: %s\n",
            tbcurso[i].pk_codCurso, tbcurso[i].nome, tbcurso[i].cargaHoraria, tbcurso[i].modalidade);
    }

    printf("\n--- TURMAS (%d) ---\n", idt);
    for(int i=0;i<idt;i++){
        printf("Código: %d | Nome: %s | Qtd Alunos: %d | Turno: %s\n",
            tbturma[i].pk_codTurma, tbturma[i].nomeTurma, tbturma[i].qtdAlunos, tbturma[i].turno);
    }

    printf("\n--- MATRÍCULAS (%d) ---\n", idm);
    for(int i=0;i<idm;i++){
        int ia = indice_aluno(tbmatricula[i].fk_cpf);
        int ic = indice_curso(tbmatricula[i].fk_codCurso);
        int it = indice_turma(tbmatricula[i].fk_codTurma);
        printf("RA: %d | Aluno: %s | Curso: %s | Turma: %s\n",
            tbmatricula[i].pk_ra,
            (ia!=-1)?tbaluno[ia].nome:"?",
            (ic!=-1)?tbcurso[ic].nome:"?",
            (it!=-1)?tbturma[it].nomeTurma:"?");
    }

    printf("\n===================================================\n");
    printf("Pressione ENTER para continuar...");
    getchar();
}

/* ==========================================================
   FLUXOS DE SUBMENU
   ========================================================== */

void fluxoCadastrar(){
    do{
        system("clear");
        subMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &sop);
        limparBuffer();
        switch(sop){
            case 1: cadastrarAluno(); break;
            case 2: cadastrarCurso(); break;
            case 3: cadastrarTurma(); break;
            case 4: cadastrarMatricula(); break;
            case 5: break;
            default:
                printf("Opção inválida!\n");
                sleep(1);
        }
    } while(sop != 5);
}

void fluxoConsultar(){
    do{
        system("clear");
        subMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &sop);
        limparBuffer();
        switch(sop){
            case 1: consultarAluno(); break;
            case 2: consultarCurso(); break;
            case 3: consultarTurma(); break;
            case 4: consultarMatricula(); break;
            case 5: break;
            default:
                printf("Opção inválida!\n");
                sleep(1);
        }
    } while(sop != 5);
}

void fluxoAlterar(){
    do{
        system("clear");
        subMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &sop);
        limparBuffer();
        switch(sop){
            case 1: alterarAluno(); break;
            case 2: alterarCurso(); break;
            case 3: alterarTurma(); break;
            case 4: alterarMatricula(); break;
            case 5: break;
            default:
                printf("Opção inválida!\n");
                sleep(1);
        }
    } while(sop != 5);
}

void fluxoExcluir(){
    do{
        system("clear");
        subMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &sop);
        limparBuffer();
        switch(sop){
            case 1: excluirAluno(); break;
            case 2: excluirCurso(); break;
            case 3: excluirTurma(); break;
            case 4: excluirMatricula(); break;
            case 5: break;
            default:
                printf("Opção inválida!\n");
                sleep(1);
        }
    } while(sop != 5);
}

void fluxoOrdenar(){
    int ident, ordemOp;
    do{
        system("clear");
        subMenu();
        printf("Escolha a entidade a ordenar: ");
        scanf("%d", &ident);
        limparBuffer();
        if (ident>=1 && ident<=4){
            int op2;
            do{
                system("clear");
                submenuOrdenar();
                printf("Escolha o tipo de ordenação: ");
                scanf("%d", &op2);
                limparBuffer();
                if (op2==1 || op2==2){
                    int campo;
                    system("clear");
                    submenuOpcaoOrdenar(ident);
                    printf("Escolha o campo: ");
                    scanf("%d", &campo);
                    limparBuffer();
                    if (campo==1 || campo==2){
                        ordenar(ident, campo, op2);
                    } else if (campo!=3){
                        printf("Opção inválida!\n");
                        sleep(1);
                    }
                } else if (op2!=3){
                    printf("Opção inválida!\n");
                    sleep(1);
                }
            } while(op2!=3);
        } else if (ident!=5){
            printf("Opção inválida!\n");
            sleep(1);
        }
    } while(ident!=5);
}

void fluxoImprimir(){
    do{
        system("clear");
        subMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &sop);
        limparBuffer();
        switch(sop){
            case 1: imprimir(1); break;
            case 2: imprimir(2); break;
            case 3: imprimir(3); break;
            case 4: imprimir(4); break;
            case 5: break;
            default:
                printf("Opção inválida!\n");
                sleep(1);
        }
    } while(sop != 5);
}

/* ==========================================================
   MAIN
   ========================================================== */

int main()
{
    setlocale(LC_ALL, "");

    do{
        system("clear");
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &op);
        limparBuffer();

        switch(op){
            case 1: fluxoCadastrar(); break;
            case 2: fluxoConsultar(); break;
            case 3: fluxoAlterar(); break;
            case 4: fluxoExcluir(); break;
            case 5: fluxoOrdenar(); break;
            case 6: fluxoImprimir(); break;
            case 7: relatorioGeral(); break;
            case 8:
                system("clear");
                printf("Encerrando o sistema. Até logo!\n");
                break;
            default:
                system("clear");
                printf("Opção inválida!\n");
                sleep(1);
        }
    } while(op != 8);

    return 0;
}