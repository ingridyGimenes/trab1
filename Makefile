# =====================[ Configuração ]=====================

# Nome do binário final
APP      := app

# Diretórios do projeto
SRC_DIRS := . operadorde_de_arquivo tads_gerais formas tads_trabalho
BUILD    := build
BIN      := bin

# Descoberta automática dos .c
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
OBJS := $(patsubst %.c,$(BUILD)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Compilador e flags
CC      := gcc
CSTD    := -std=c99
WARN    := -Wall -Wextra
SEC     := -fstack-protector-all
OPT     := -O2

# Define M_PI caso a libc não exponha
MATHPI  := -DM_PI=3.14159265358979323846

# Expor strdup e afins via headers padrão
# (GLIBC costuma precisar de _DEFAULT_SOURCE quando definimos _POSIX_C_SOURCE)
DEFINES := -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE $(MATHPI) $(TEXTMODE)

# Includes
INCLUDES := -I. -Ioperadorde_de_arquivo -Itads_gerais -Iformas -Itads_trabalho

# Bibliotecas e link flags
LDLIBS  := -lm
LDFLAGS :=

# CFLAGS finais
CFLAGS  := $(CSTD) $(WARN) $(SEC) $(OPT) $(DEFINES)

# Sanitizers opcionais: use `make SAN=1`
ifeq ($(SAN),1)
CFLAGS  += -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE
LDFLAGS += -fsanitize=address,undefined
endif

# =====================[ Alvos ]============================

.PHONY: all clean run dirs

all: dirs $(BIN)/$(APP)

# Link final
$(BIN)/$(APP): $(OBJS)
	@echo "  [LD] $@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilação com dependências automáticas
# -MMD -MP gera .d ao lado dos .o pra recompilar quando headers mudarem
$(BUILD)/%.o: %.c | dirs
	@mkdir -p $(dir $@)
	@echo "  [CC] $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Cria pastas de build/bin se não existirem
dirs:
	@mkdir -p $(BIN) $(BUILD)
	@$(foreach d,$(SRC_DIRS), mkdir -p $(BUILD)/$(d);)

# Executar rapidamente: make run ARGS="-e ./in -f teste.geo -o ./out -q teste.qry"
run: $(BIN)/$(APP)
	@$(BIN)/$(APP) $(ARGS)

# Limpeza
clean:
	@echo "  [CLEAN]"
	@rm -rf $(BUILD) $(BIN)

# Inclui dependências geradas (-MMD)
-include $(DEPS)
