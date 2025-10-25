#pragma once
#include <stdbool.h>

// Executor opaco do .QRY
// Orquestra pilhas (carregadores), arena, chão e o Processador.
typedef void* QRY_EXECUTOR;

// Cria um executor.
//  - chao: Fila* com as formas do chão (FORMA)
//  - arena: Fila* onde os disparos entram (FORMA)
//  - processador: Processador* (do processador.h)
//  - path_txt: caminho do arquivo .txt de saída (pode ser NULL para não gerar)
QRY_EXECUTOR qry_exec_create(void* chao, void* arena, void* processador, const char* path_txt);

// Lê e executa o arquivo .qry inteiro.
// Retorna true em sucesso completo; false se houve erro de arquivo ou sintaxe grave.
bool qry_exec_run_file(QRY_EXECUTOR qe, const char* filepath);

// Destroi o executor, fechando o .txt se aberto.
void qry_exec_destroy(QRY_EXECUTOR qe);
