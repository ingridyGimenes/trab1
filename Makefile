# ==========================
# Bocha Geométrica — Makefile
# ==========================

# Nome do binário final
APP      := app

# Diretórios do projeto
SRC_DIRS := . operadorde_de_arquivo tads_gerais formas tads_trabalho
BUILD    := build
BIN      := bin

# Descobre todos os .c automaticamente
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
OBJS := $(patsubst %.c,$(BUILD)/%.o,$(SRCS))

# Compilador e flags
CC      := gcc
CSTD    := -std=c99
WARN    := -Wall -Wextra
SEC     := -fstack-protector-all
OPT     := -O2
# Define M_PI se faltar no math.h (corrige "M_PI undefined")
MATHPI  := -DM_PI=3.14159265358979323846
# Se seu TEXTO não tem estilo, descomente a linha abaixo:
# TEXTMODE := -DTEXTO_SEM_ESTILO
# Se seu TEXTO tem estilo, descomente esta:
# TEXTMODE := -DTEXTO_TEM_ESTILO

CFLAGS  := $(CSTD) $(WARN) $(SEC) $(OPT) $(MATHPI) $(TEXTMODE)

# Inclui diretórios de headers
INCLUDES := -I. -Ioperadorde_de_arquivo -Itads_gerais -Iformas -Itads_trabalho

# Bibliotecas de link
LDLIBS  := -lm
LDFLAGS :=

# Sanitizers opcionais: make SAN=1
ifeq ($(SAN),1)
CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address,undefined
endif

# ==========================
# Alvos principais
# ==========================
.PHONY: all clean run dirs

all: dirs $(BIN)/$(APP)

# Link final
$(BIN)/$(APP): $(OBJS)
	@echo "  [LD] $@"
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Regra geral de compilação: build/xxx.o a partir de xxx.c
$(BUILD)/%.o: %.c | dirs
	@mkdir -p $(dir $@)
	@echo "  [CC] $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Cria pastas de build/bin se não existirem
dirs:
	@mkdir -p $(BIN) $(BUILD)
	@$(foreach d,$(SRC_DIRS), mkdir -p $(BUILD)/$(d);)

# Executar rapidamente: make run ARGS="-e in -f a.geo -o out -q a.qry"
run: $(BIN)/$(APP)
	@$(BIN)/$(APP) $(ARGS)

# Limpeza
clean:
	@echo "  [CLEAN]"
	@rm -rf $(BUILD) $(BIN)

# ==========================
# Dicas de uso:
#  - Compilar:            make
#  - Compilar com ASAN:   make SAN=1
#  - Rodar com args:      make run ARGS="-e in -f cidade.geo -o out -q a.qry"
#  - Limpar:              make clean
# ==========================
