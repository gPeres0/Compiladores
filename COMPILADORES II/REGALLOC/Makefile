TARGET = regalloc
SOURCES = regalloc.cpp
CXX = g++

# Flags de compilação:
# -Wall: Habilita todos os warnings (boa prática)
# -Wextra: Habilita warnings extras
# -O3: Nível máximo de otimização (opcional, mas recomendado)
# -std=c++17: Define o padrão C++ a ser usado (o código usa features modernas)
CXXFLAGS = -Wall -Wextra -O3 -std=c++17

# Regra principal (default): compila o executável
all: $(TARGET)

# Regra para compilar o executável final (regalloc)
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Regra para limpar o diretório, removendo o executável e outros arquivos gerados
clean:
	rm -f $(TARGET) *.o

# Torna 'all' e 'clean' targets não-arquivo
.PHONY: all clean