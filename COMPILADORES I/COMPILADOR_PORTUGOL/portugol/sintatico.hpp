#include "automato.hpp"

using namespace std;


class Sintatico {
   public:

    void algFunction() {
        if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "algoritmo") {
            coluna -= token_lido.size();
            cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
            exit(0);
        }

        s.pop();
        pedir_novo_token = true;
    }

    void pointFunction() {
        pedir_novo_token = false;

        if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ".") {
            coluna -= token_lido.size();
            cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
            exit(0);
        }
        s.pop();
        pedir_novo_token = true;
    }

    void idFunction() {
        pedir_novo_token = false;
            
        if (s.top() == "DeclaraParametros") {
            s.pop();
            s.push("Declaracoes");
        } else if (s.top() == "Declaracoes") {
            s.pop();
            s.push("Declaracoes\'");
            s.push("DeclaraVariaveis");
        } else if (s.top() == "DeclaraVariaveis") {
            s.pop();
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("TipoBasico");
        } else if (s.top() == "DeclaraIdentificador") {
            s.pop();
            s.push("DeclaraIdentificador\'");
            s.push("id");
        } else if (s.top() == "TipoBasico") {
            s.pop();
            s.push("id");
        } else if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push("ComandosExtra");
            s.push("id");
        } else if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("FatorExtra");
            s.push("id");
        } else if (s.top() == "Variavel") {
            s.pop();
            s.push("Variavel\'");
            s.push("id");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIterExtra");
            s.push("id");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("id");
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("id");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandosExtra\'");
            s.push("id");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper)) {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void semicolFunction() {
        pedir_novo_token = false;
        if (s.top() == "Parametros") {
            s.pop();
        } else if (s.top() == "ComandosExtra") {
            s.pop();
            s.push("Comandos\'\'\'");
        } else if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "DeclaraIdentificador\'") {
            s.pop();
        } else if (s.top() == "ListaComandosExtra\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos\'\'\'");
        } else if (s.top() == "Comandos\'\'\'") {
            s.pop();
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ";") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void procFunction() {
        pedir_novo_token = false;
        if (s.top() == "ProcedimentoFuncao") {
            s.pop();
            s.push("ProcedimentoFuncao");
            s.push("DeclaraProcedimento");
        } else if (s.top() == "DeclaraProcedimento") {
            s.pop();
            s.push(";");
            s.push("BlocoComandos");
            s.push("BlocoVariaveis");
            s.push("DeclaraParametros");
            s.push(";");
            s.push("Parametros");
            s.push("id");
            s.push("procedimento");
        } else if (s.top() == "BlocoVariaveis") {
            s.pop();
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "procedimento") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void functFunction() {
        pedir_novo_token = false;

        if (s.top() == "ProcedimentoFuncao") {
            s.pop();
            s.push("ProcedimentoFuncao");
            s.push("DeclaraFuncao");
        } else if (s.top() == "DeclaraFuncao") {
            s.pop();
            s.push(";");
            s.push("BlocoComandos");
            s.push("BlocoVariaveis");
            s.push("DeclaraParametros");
            s.push(";");
            s.push("TipoBasico");
            s.push(":");
            s.push("Parametros");
            s.push("id");
            s.push("funcao");
        } else if (s.top() == "BlocoVariaveis") {
            s.pop();
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "funcao") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void colonFunction() {
        pedir_novo_token = false;
        if (s.top() == "Parametros") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ":") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void openParamFunction() {
        pedir_novo_token = false;
        if (s.top() == "Parametros") {
            s.pop();
            s.push(")");
            s.push("DeclaraIdentificador");
            s.push("(");
        } else if (s.top() == "ComandosExtra") {
            s.pop();
            s.push("Comandos\'\'\'");
        } else if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push(")");
            s.push("Expressao");
            s.push("(");
        } else if (s.top() == "FatorExtra") {
            s.pop();
            s.push(")");
            s.push("ExprIter");
            s.push("(");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push(")");
            s.push("Expressao");
            s.push("(");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push(")");
            s.push("ExprIter");
            s.push("(");
        } else if (s.top() == "ListaComandosExtra\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos\'\'\'");
        } else if (s.top() == "Comandos\'\'\'") {
            s.pop();
            s.push(")");
            s.push("ExprIter");
            s.push("(");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "(") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void closeParamFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "DeclaraIdentificador\'") {
            s.pop();
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "ExprIter\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ")") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void varFunction() {
        pedir_novo_token = false;
        if (s.top() == "DeclaraParametros") {
            s.pop();
        } else if (s.top() == "BlocoVariaveis") {
            s.pop();
            s.push("Declaracoes");
            s.push("variaveis");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "variaveis") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void typeFunction() {
        pedir_novo_token = false;
        if (s.top() == "DeclaraParametros") {
            s.pop();
            s.push("Declaracoes");
        } else if (s.top() == "Declaracoes") {
            s.pop();
            s.push("Declaracoes\'\'");
            s.push("DeclaraTipo");
        } else if (s.top() == "DeclaraTipo") {
            s.pop();
            s.push(";");
            s.push("TipoBasico");
            s.push("]");
            s.push("Dimensao");
            s.push("[");
            s.push("VetorMatriz");
            s.push("=");
            s.push("id");
            s.push("tipo");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
            s.push("Declaracoes\'\'");
            s.push(";");
            s.push("TipoBasico");
            s.push("]");
            s.push("Dimensao");
            s.push("[");
            s.push("VetorMatriz");
            s.push("=");
            s.push("id");
            s.push("tipo");
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
            s.push("Declaracoes\'\'");
            s.push(";");
            s.push("TipoBasico");
            s.push("]");
            s.push("Dimensao");
            s.push("[");
            s.push("VetorMatriz");
            s.push("=");
            s.push("id");
            s.push("tipo");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "tipo") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void equalFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Expressao\'") {
            s.pop();
            s.push("ExpressaoSimples");
            s.push("=");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("Expressao\'");
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "=") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void openBracketFunction() {
        pedir_novo_token = false;
        if (s.top() == "ComandosExtra") {
            s.pop();
            s.push("Expressao");
            s.push("<-");
            s.push("Variavel\'");
        } else if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "ListaComandosExtra\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Expressao");
            s.push("<-");
            s.push("Variavel\'");
        } else if (s.top() == "Variavel\'") {
            s.pop();
            s.push("]");
            s.push("ExprIter");
            s.push("[");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "[") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }
    }

    void closeBracketFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Dimensao\'") {
            s.pop();
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "ExprIter\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "]") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }

            s.pop();
            pedir_novo_token = true;
        }

    }

    void vectorFunction() {
        pedir_novo_token = false;
        if (s.top() == "VetorMatriz") {
            s.pop();
            s.push("vetor");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "vetor") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void matrixFunction() {
        pedir_novo_token = false;
        if (s.top() == "VetorMatriz") {
            s.pop();
            s.push("matriz");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "matriz") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void numIntFunction() {
        pedir_novo_token = false;
        if (s.top() == "Dimensao") {
            s.pop();
            s.push("Dimensao\'");
            s.push("numero_inteiro");
            s.push(":");
            s.push("numero_inteiro");
        } else if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("numero_inteiro");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("numero_inteiro");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "numero_inteiro") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void integerFunction() {
        pedir_novo_token = false;
        if (s.top() == "DeclaraParametros") {
            s.pop();
            s.push("Declaracoes");
        } else if (s.top() == "Declaracoes") {
            s.pop();
            s.push("Declaracoes\'");
            s.push("DeclaraVariaveis");
        } else if (s.top() == "DeclaraVariaveis") {
            s.pop();
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("TipoBasico");
        } else if (s.top() == "TipoBasico") {
            s.pop();
            s.push("inteiro");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("inteiro");
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("inteiro");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "inteiro") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void realFunction() {
        pedir_novo_token = false;
        if (s.top() == "DeclaraParametros") {
            s.pop();
            s.push("Declaracoes");
        } else if (s.top() == "Declaracoes") {
            s.pop();
            s.push("Declaracoes\'");
            s.push("DeclaraVariaveis");
        } else if (s.top() == "DeclaraVariaveis") {
            s.pop();
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("TipoBasico");
        } else if (s.top() == "TipoBasico") {
            s.pop();
            s.push("real");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("real");
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("real");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "real") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void charFunction() {
        pedir_novo_token = false;
        if (s.top() == "DeclaraParametros") {
            s.pop();
            s.push("Declaracoes");
        } else if (s.top() == "Declaracoes") {
            s.pop();
            s.push("Declaracoes\'");
            s.push("DeclaraVariaveis");
        } else if (s.top() == "DeclaraVariaveis") {
            s.pop();
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("TipoBasico");
        } else if (s.top() == "TipoBasico") {
            s.pop();
            s.push("caractere");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("caractere");
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("caractere");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "caractere") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void logicalFunction() {
        pedir_novo_token = false;
        if (s.top() == "DeclaraParametros") {
            s.pop();
            s.push("Declaracoes");
        } else if (s.top() == "Declaracoes") {
            s.pop();
            s.push("Declaracoes\'");
            s.push("DeclaraVariaveis");
        } else if (s.top() == "DeclaraVariaveis") {
            s.pop();
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("TipoBasico");
        } else if (s.top() == "TipoBasico") {
            s.pop();
            s.push("logico");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("logico");
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
            s.push("Declaracoes\'");
            s.push(";");
            s.push("DeclaraIdentificador");
            s.push(":");
            s.push("logico");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "logico") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void beginFunction() {
        pedir_novo_token = false;
        if (s.top() == "ProcedimentoFuncao") {
            s.pop();
        } else if (s.top() == "DeclaraParametros") {
            s.pop();
        } else if (s.top() == "BlocoVariaveis") {
            s.pop();
        } else if (s.top() == "BlocoComandos") {
            s.pop();
            s.push("fim");
            s.push("ListaComandos");
            s.push("inicio");
        } else if (s.top() == "Declaracoes\'") {
            s.pop();
        } else if (s.top() == "Declaracoes\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "inicio") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void endFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos\'") {
            s.pop();
        } else if (s.top() == "Comandos\'\'") {
            s.pop();
            s.push("se");
            s.push("fim");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "fim") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void ifFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push("Comandos\'\'");
            s.push("ListaComandos");
            s.push("entao");
            s.push("Expressao");
            s.push("se");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos\'\'");
            s.push("ListaComandos");
            s.push("entao");
            s.push("Expressao");
            s.push("se");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "se") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void thenFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "entao") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void forFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push("Comandos\'");
            s.push("Expressao");
            s.push("ate");
            s.push("Expressao");
            s.push("de");
            s.push("id");
            s.push("para");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos\'");
            s.push("Expressao");
            s.push("ate");
            s.push("Expressao");
            s.push("de");
            s.push("id");
            s.push("para");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "para") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void fromFunction() {
        pedir_novo_token = false;

        if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "de") {
            coluna -= token_lido.size();
            cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
            exit(0);
        }
        s.pop();
        pedir_novo_token = true;
    }

    void untilFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "ate") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void whileFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push("enquanto");
            s.push("fim");
            s.push("ListaComandos");
            s.push("faca");
            s.push("Expressao");
            s.push("enquanto");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("enquanto");
            s.push("fim");
            s.push("ListaComandos");
            s.push("faca");
            s.push("Expressao");
            s.push("enquanto");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "enquanto") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void doFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "Comandos\'") {
            s.pop();
            s.push("para");
            s.push("fim");
            s.push("ListaComandos");
            s.push("faca");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "faca") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void repeatFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push("Expressao");
            s.push("ate");
            s.push("ListaComandos");
            s.push("repita");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Expressao");
            s.push("ate");
            s.push("ListaComandos");
            s.push("repita");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "repita") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void readFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push(")");
            s.push("Variavel");
            s.push("(");
            s.push("leia");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push(")");
            s.push("Variavel");
            s.push("(");
            s.push("leia");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "leia") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void printFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Comandos");
        } else if (s.top() == "Comandos") {
            s.pop();
            s.push(")");
            s.push("ExprIter");
            s.push("(");
            s.push("imprima");
        } else if (s.top() == "ListaComandos\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push(")");
            s.push("ExprIter");
            s.push("(");
            s.push("imprima");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "imprima") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void attribFunction() {
        pedir_novo_token = false;
        if (s.top() == "ComandosExtra") {
            s.pop();
            s.push("Expressao");
            s.push("<-");
            s.push("Variavel\'");
        } else if (s.top() == "ListaComandosExtra\'") {
            s.pop();
            s.push("ListaComandos\'");
            s.push(";");
            s.push("Expressao");
            s.push("<-");
            s.push("Variavel\'");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "<-") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void plusFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
            s.push("+");
        } else if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("+");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "ExpressaoSimples\'") {
            s.pop();
            s.push("Termo");
            s.push("+");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("ExpressaoSimples\'");
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "+") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void minusFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
            s.push("-");
        } else if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("-");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "ExpressaoSimples\'") {
            s.pop();
            s.push("Termo");
            s.push("-");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("ExpressaoSimples\'");
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "-") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void notFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("Fator");
            s.push("nao");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Fator");
            s.push("nao");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "nao") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void realNumFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("numero_real");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("numero_real");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "numero_real") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void trueFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("verdadeiro");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("verdadeiro");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "verdadeiro") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void falseFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("falso");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("falso");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "falso") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void textFunction() {
        pedir_novo_token = false;
        if (s.top() == "Expressao") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples");
        } else if (s.top() == "ExpressaoSimples") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Termo");
        } else if (s.top() == "Termo") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Fator");
        } else if (s.top() == "Fator") {
            s.pop();
            s.push("texto");
        } else if (s.top() == "ExprIter") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("texto");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "texto") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void commaFunction() {
        pedir_novo_token = false;

        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "DeclaraIdentificador\'") {
            s.pop();
            s.push("DeclaraIdentificador");
            s.push(",");
        } else if (s.top() == "Dimensao\'") {
            s.pop();
            s.push("Dimensao");
            s.push(",");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "ExprIter\'") {
            s.pop();
            s.push("ExprIter");
            s.push(",");
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ",") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void stepFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "Comandos\'") {
            s.pop();
            s.push("para");
            s.push("fim");
            s.push("ListaComandos");
            s.push("faca");
            s.push("Expressao");
            s.push("passo");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "passo") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void elseFunction() {
        pedir_novo_token = false;
        if (s.top() == "ListaComandos\'") {
            s.pop();
        } else if (s.top() == "Comandos\'\'") {
            s.pop();
            s.push("se");
            s.push("fim");
            s.push("ListaComandos");
            s.push("senao");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "senao") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void diffFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Expressao\'") {
            s.pop();
            s.push("ExpressaoSimples");
            s.push("<>");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("Expressao\'");
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "<>") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void lessFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Expressao\'") {
            s.pop();
            s.push("ExpressaoSimples");
            s.push("<");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("Expressao\'");
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "<") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void lessEqualFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Expressao\'") {
            s.pop();
            s.push("ExpressaoSimples");
            s.push("<=");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("Expressao\'");
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "<=") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void greaterFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Expressao\'") {
            s.pop();
            s.push("ExpressaoSimples");
            s.push(">");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("Expressao\'");
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ">") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void greaterEqualFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Expressao\'") {
            s.pop();
            s.push("ExpressaoSimples");
            s.push(">=");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Expressao\'\'") {
            s.pop();
            s.push("Expressao\'\'");
            s.push("Expressao\'");
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != ">=") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void orFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra\'") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "ExpressaoSimples\'") {
            s.pop();
            s.push("Termo");
            s.push("ou");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "ExpressaoSimples\'\'") {
            s.pop();
            s.push("ExpressaoSimples\'\'");
            s.push("ExpressaoSimples\'");
        } else if (s.top() == "Termo\'\'") {
            s.pop();
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "ou") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void timesFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Termo\'") {
            s.pop();
            s.push("Fator");
            s.push("*");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Termo\'");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "*") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void divisionFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Termo\'") {
            s.pop();
            s.push("Fator");
            s.push("/");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Termo\'");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "/") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void divFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Termo\'") {
            s.pop();
            s.push("Fator");
            s.push("div");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Termo\'");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "div") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }

    void andFunction() {
        pedir_novo_token = false;
        if (s.top() == "FatorExtra") {
            s.pop();
            s.push("Variavel\'");
        } else if (s.top() == "ExprIterExtra") {
            s.pop();
            s.push("ExprIter\'");
            s.push("Expressao\'\'");
            s.push("ExpressaoSimples\'\'");
            s.push("Termo\'\'");
            s.push("Variavel\'");
        } else if (s.top() == "Termo\'") {
            s.pop();
            s.push("Fator");
            s.push("e");
        } else if (s.top() == "Variavel\'") {
            s.pop();
        } else if (s.top() == "Termo\'\'") {
            s.pop();
            s.push("Termo\'\'");
            s.push("Termo\'");
        } else {
            if (any_of(s.top().begin(), s.top().end(), ::isupper) || s.top() != "e") {
                coluna -= token_lido.size();
                cout << "ERRO DE SINTAXE. Linha: " << linha << " Coluna: " << coluna << " -> \'" << token_lido << "\'" << endl;
                exit(0);
            }
            s.pop();
            pedir_novo_token = true;
        }
    }
    
void analisadorSintatico(int token_value) {

        switch (token_value) {
            case ALGORITMO:
                algFunction();
                break;
            case PONTO:
                pointFunction();
                break;
            case ID:
                idFunction();
                break;
            case PONTO_VIRGULA:
                semicolFunction();
                break;
            case PROCEDIMENTO:
                procFunction();
                break;
            case FUNCAO:
                functFunction();
                break;
            case DOIS_PONTOS:
                colonFunction();
                break;
            case ABRE_PAR:
                openParamFunction();
                break;
            case FECHA_PAR:
                closeParamFunction();
                break;
            case VARIAVEIS:
                varFunction();
                break;
            case TIPO:
                typeFunction();
                break;
            case IGUAL:
                equalFunction();
                break;
            case ABRE_COL:
                openBracketFunction();
                break;
            case FECHA_COL:
                closeBracketFunction();
                break;
            case VETOR:
                vectorFunction();
                break;
            case MATRIZ:
                matrixFunction();
                break;
            case NUMERO_INTEIRO:
                numIntFunction();
                break;
            case INTEIRO:
                integerFunction();
                break;
            case REAL:
                realFunction();
                break;
            case CARACTERE:
                charFunction();
                break;
            case LOGICO:
                logicalFunction();
                break;
            case INICIO:
                beginFunction();
                break;
            case FIM:
                endFunction();
                break;
            case SE:
                ifFunction();
                break;
            case ENTAO:
                thenFunction();
                break;
            case PARA:
                forFunction();
                break;
            case DE:
                fromFunction();
                break;
            case ATE:
                untilFunction();
                break;
            case ENQUANTO:
                whileFunction();
                break;
            case FACA:
                doFunction();
                break;
            case REPITA:
                repeatFunction();
                break;
            case LEIA:
                readFunction();
                break;
            case IMPRIMA:
                printFunction();
                break;
            case ATRIBUICAO:
                attribFunction();
                break;
            case MAIS:
                plusFunction();
                break;
            case MENOS:
                minusFunction();
                break;
            case NAO:
                notFunction();
                break;
            case NUMERO_REAL:
                realNumFunction();
                break;
            case VERDADEIRO:
                trueFunction();
                break;
            case FALSO:
                falseFunction();
                break;
            case TEXTO:
                textFunction();
                break;
            case VIRGULA:
                commaFunction();
                break;
            case PASSO:
                stepFunction();
                break;
            case SENAO:
                elseFunction();
                break;
            case DIFERENTE:
                diffFunction();
                break;
            case MENOR:
                lessFunction();
                break;
            case MENOR_IGUAL:
                lessEqualFunction();
                break;
            case MAIOR:
                greaterFunction();
                break;
            case MAIOR_IGUAL:
                greaterEqualFunction();
                break;
            case OU:
                orFunction();
                break;
            case VEZES:
                timesFunction();
                break;
            case DIVISAO:
                divisionFunction();
                break;
            case DIV:
                divFunction();
                break;
            case E:
                andFunction();
                break;
            default:
                break;
            break;
        }
    }
};
