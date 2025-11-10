#include <bits/stdc++.h>

using namespace std;


// Constantes para auxiliar na busca
constexpr int NODE_NOT_FOUND = -1;

// Estrutura que representa um nó (registrador) no grafo de interferência.
struct RegisterNode {
    int id;                // Número do registrador (virtual ou físico)
    set<int> neighbors;    // Conjunto de IDs dos vizinhos (interferências)
    bool is_potential_spill = false; // Marcador para saída (apenas para a saída)

    // Construtor
    RegisterNode(int i) : id(i) {}
};



// ==================== Funções internas ==================== //

int graph_number = 0;
int initial_k = 0; // K original do arquivo
map<int, RegisterNode> initial_graph; // Grafo lido (base para cada tentativa)
    
// Resultados para o resumo final
map<int, string> results_summary; 

// Estruturas usadas durante o processo de coloração (por tentativa de K)
list<pair<int, bool>> simplify_stack; // pair<node_id, is_spill>
map<int, int> allocated_colors; // Cores finais atribuídas (registrador -> cor)
map<int, RegisterNode> current_graph; // Grafo que está sendo simplificado

// Retorna o grau de um nó no grafo atual.
int get_degree(int node_id) {
    if (current_graph.count(node_id)) {
        return current_graph.at(node_id).neighbors.size();
    }
    return 0;
}

/* 
 *  Encontra o nó elegível para a fase Simplify.
 *  Segue as regras: grau < current_k, menor grau, menor número.
 */
int find_simplifiable_node(int current_k) {
    int best_node = NODE_NOT_FOUND;
    int min_degree = numeric_limits<int>::max();

    for (auto const& [id, node] : current_graph) {
        // Ignora registradores físicos/cores (id < initial_k)
        if (id < initial_k) continue;
        int deg = get_degree(id);

        // Condição principal: grau < K
        if (deg < current_k) {
            if (deg < min_degree) {
                // Desempate 1: Menor grau
                min_degree = deg;
                best_node = id;
            } else if (deg == min_degree) {
                // Desempate 2: Menor número do registrador virtual
                if (id < best_node) {
                    best_node = id;
                }
            }
        }
    }
    return best_node;
}

/*
 *  Encontra o nó potencial para Spill (quando nenhum nó é simplificável).
 *  Segue as regras: maior grau, menor número.
 */
int find_spill_candidate() {
    int best_node = NODE_NOT_FOUND;
    int max_degree = -1;

    for (auto const& [id, node] : current_graph) {
        // Ignora registradores físicos/cores (id < initial_k)
        if (id < initial_k) continue;

        int deg = get_degree(id);

        if (deg > max_degree) {
            // Desempate 1: Maior grau
            max_degree = deg;
            best_node = id;
        } else if (deg == max_degree) {
            // Desempate 2: Menor número do registrador virtual
            if (id < best_node) {
                best_node = id;
            }
        }
    }
    return best_node;
}

// Remove um nó do grafo, atualiza vizinhos e empilha.
void remove_node(int node_id, bool is_spill) {
    // 1. Imprimir a linha 'Push: node_id [*]'
    cout << "Push: " << node_id;
    if (is_spill) {
        cout << " *"; // Indicação de potencial spill
    }
    cout << endl;

    // 2. Colocar o node_id na pilha (simplify_stack)
    simplify_stack.push_front({node_id, is_spill});

    // 3. Remover arestas e o nó do grafo (Build)
    if (current_graph.count(node_id)) {
        // Para cada vizinho, remove a aresta de interferência
        for (int neighbor_id : current_graph.at(node_id).neighbors) {
            if (current_graph.count(neighbor_id)) {
                // Não importa se o vizinho é físico ou virtual, a aresta é removida dele.
                current_graph.at(neighbor_id).neighbors.erase(node_id);
            }
        }
        // Remove o nó do grafo
        current_graph.erase(node_id);
    }
}

// Fase Simplify e Potencial Spill (Iterativa)
void simplify_and_spill(int current_k) {
    // CORREÇÃO: O loop agora checa explicitamente se há nós virtuais restantes.
    while (true) {
        // Checa se há algum nó virtual restante no grafo.
        bool has_virtual_nodes = false;
        for (auto const& pair : current_graph) {
            if (pair.first >= initial_k) {
                has_virtual_nodes = true;
                break;
            }
        }
        if (!has_virtual_nodes) break; // Todos os virtuais foram empilhados.

        // 1. Tenta simplificar
        int node_to_remove = find_simplifiable_node(current_k);

        if (node_to_remove != NODE_NOT_FOUND) {
            // Nó simplificável encontrado (Grau < K)
            remove_node(node_to_remove, false);
        } else {
            // 2. Nenhum nó simplificável (Potencial Spill)
            node_to_remove = find_spill_candidate();
            if (node_to_remove != NODE_NOT_FOUND) {
                remove_node(node_to_remove, true);
            } else {
                // Devemos sempre encontrar um nó se has_virtual_nodes for true, 
                // mas este break é uma salvaguarda final.
                break; 
            }
        }
    }
}

/*
 *  Fase Select/Assign (Desempilhamento e Coloração)
 *  Return: true se a alocação for bem-sucedida, false se houver spill.
 */
bool select_and_assign(int current_k) {
    bool spill_occurred = false;

    // Reconstruir o grafo e colorir na ordem inversa da pilha (LIFO)
    while (!simplify_stack.empty()) {
        int node_id = simplify_stack.front().first;
        simplify_stack.pop_front();

         // O nó é reintroduzido para checagem de vizinhos.
        // Para otimização, usamos o initial_graph que é completo.

        // 1. Encontrar a menor cor disponível (0 a current_k - 1)
        set<int> unavailable_colors;
            
        // Verifica a cor dos vizinhos no grafo inicial.
        for (int neighbor_id : initial_graph.at(node_id).neighbors) {
            // Se o vizinho é um registrador físico (cor), ele é indisponível
            if (neighbor_id < initial_k) {
                if (neighbor_id < current_k) {
                     // Apenas as cores no range atual (0 a current_k-1) importam
                    unavailable_colors.insert(neighbor_id);
                }
            }
            // Se o vizinho é virtual e JÁ FOI COLORIDO (está no allocated_colors)
            else if (allocated_colors.count(neighbor_id)) {
                unavailable_colors.insert(allocated_colors.at(neighbor_id));
            }
        }
        
        int chosen_color = -1;
        // Procurar a menor cor (de 0 até current_k - 1)
        for (int color = 0; color < current_k; ++color) {
            if (unavailable_colors.find(color) == unavailable_colors.end()) {
                chosen_color = color;
                break;
            }
        }

        // 2. Atribuir cor ou indicar spill
        cout << "Pop: " << node_id;
        if (chosen_color != -1) {
            // Atribuir cor
            allocated_colors[node_id] = chosen_color;
            cout << " -> " << chosen_color << endl;
        } else {
            // Spill: Nenhuma cor disponível
            cout << " -> NO COLOR AVAILABLE" << endl;
            spill_occurred = true;
            break; // Termina a coloração para este K
        }
    }

    return !spill_occurred;
}


// ==================== Funções externas (chamadas pela main) ==================== //

// Fase Build: lê o grafo do arquivo de entrada (STDIN).
bool read_graph(istream& input) {
    string line;
    bool reading_interference = false;
        
    // Limpar estruturas para o novo grafo
    initial_graph.clear();
    results_summary.clear();
    graph_number = 0;
    initial_k = 0;

    // CORREÇÃO: O loop agora roda até o EOF, lendo todas as linhas
    while (getline(input, line)) {
        stringstream ss(line);
        string token;

        if (line.rfind("Grafo", 0) == 0) { // Início do grafo
            ss >> token; // Grafo
            ss >> graph_number;
            graph_number = abs(graph_number);
            reading_interference = false; // Reset para o K
            continue;
        }
            
        if (graph_number == 0) continue; // Ainda não encontrou o header

        if (line.rfind("K=", 0) == 0) { // Número de cores (K)
            ss.ignore(2); // Ignora "K="
            ss >> initial_k;
            reading_interference = true;
            continue;
        }

        if (!reading_interference) continue; // Linhas entre K e a primeira interferência

        // Leitura das interferências: NNN --> V1 V2 V3...
        int node_id;
        ss >> node_id;
            
        if (ss.fail()) continue; // Não conseguiu ler o ID (linha vazia, etc.)

        string separator;
        ss >> separator; // Tenta ler '-->' ou '->'

        if (separator == "-->" || separator == "->") {
            // Garante que o nó base existe (virtual ou físico)
            if (initial_graph.find(node_id) == initial_graph.end()) {
                initial_graph.emplace(node_id, RegisterNode(node_id));
            }
            
            int neighbor_id;
            while (ss >> neighbor_id) {
                // Adiciona interferência (node_id interfere com neighbor_id)
                initial_graph.at(node_id).neighbors.insert(neighbor_id);
                
                // Garante que o vizinho exista e adicione a interferência recíproca
                if (initial_graph.find(neighbor_id) == initial_graph.end()) {
                    initial_graph.emplace(neighbor_id, RegisterNode(neighbor_id));
                }
                initial_graph.at(neighbor_id).neighbors.insert(node_id);
            }
        }
    }
    
    // Retorna true se um grafo completo (Grafo e K) foi lido no último ciclo
    return (graph_number != 0 && initial_k != 0);
}

// Orquestra o processo de alocação (K, K-1, ..., 2).
void run_allocation() {
    if (graph_number == 0 || initial_k == 0) return;

    cout << "Graph " << graph_number << " -> Physical Registers: " << initial_k << endl;
    cout << "----------------------------------------\n";

     // Tentar colorir de K até 2 cores
    for (int current_k = initial_k; current_k >= 2; --current_k) {
            
        // 1. Inicialização da Tentativa
        cout << "----------------------------------------\n";
        cout << "K = " << current_k << "\n\n";
        current_graph = initial_graph;
        simplify_stack.clear();
        allocated_colors.clear();

        // 2. Fase Simplify/Spill
        simplify_and_spill(current_k);

        // 3. Fase Select/Assign
        bool success = select_and_assign(current_k);

        // 4. Registrar Resultado
        if (success) {
            results_summary[current_k] = "Successful Allocation";
        } else {
            results_summary[current_k] = "SPILL";
        }
    }
}

// Imprime o resumo final de todas as tentativas.
void print_summary() {
    cout << "----------------------------------------\n";
    cout << "----------------------------------------\n";
    for (auto i = results_summary.rbegin(); i != results_summary.rend(); ++i) {
        int k_value = i->first;
        const string& result = i->second;
        if (k_value >= 10) {
            cout << "Graph " << graph_number << " -> K = " << k_value << ": " << result << endl;
        } else if (k_value == 2) {
            cout << "Graph " << graph_number << " -> K =  " << k_value << ": " << result;
        } else {
            cout << "Graph " << graph_number << " -> K =  " << k_value << ": " << result << endl;
        }
    }
}



int main() {
    // Desabilitar sincronização com stdio (melhora desempenho de I/O)
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

        if (read_graph(cin)) {
        run_allocation();
        print_summary();
    }

    return 0;
}