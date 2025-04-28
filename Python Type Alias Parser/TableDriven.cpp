#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cctype>

using namespace std;

struct Token {
    string type;
    string value;
};

vector<Token> tokenStream;
int tokenIndex = 0;

// Tokenization implementation
unordered_set<string> typeKeywords = {"int", "str", "float", "bool", "list", "dict", "tuple"};

bool isTypeKeyword(const string& word) {
    return typeKeywords.count(word);
}

string typeOfToken(const string& word) {
    string lowerWord;
    for (char c : word) lowerWord += tolower(c);

    if (word == "type") return "KEYWORD";
    if (isTypeKeyword(lowerWord)) return "TYPE"; // Case-insensitive check
    if (word == "=" || word == "(" || word == ")" || 
        word == "[" || word == "]" || word == "," || 
        word == "{" || word == "}" || word == ":") 
        return "SYMBOL";
    return (isalpha(word[0])) ? "IDENTIFIER" : "UNKNOWN";
}

vector<Token> tokenize(const string &input) {
    vector<Token> tokens;
    string word;
    
    for (char ch : input) {
        if (isspace(ch) || string("=()[],{}:").find(ch) != string::npos) {
            if (!word.empty()) {
                tokens.push_back({typeOfToken(word), word});
                word.clear();
            }
            if (!isspace(ch)) {
                tokens.push_back({"SYMBOL", string(1, ch)});
            }
        } else {
            word += ch;
        }
    }
    
    if (!word.empty()) {
        tokens.push_back({typeOfToken(word), word});
    }
    
    return tokens;
}

enum NonTerminal {
    NT_TypeAlias, NT_TypeParamsOpt, NT_TypeParams, NT_TypeParamList,
    NT_TypeParamRest, NT_Expression, NT_GenericType, NT_BaseType,
    NT_Identifier, NT_TupleExpr, NT_TupleArgs, NT_ExpressionList,
    NT_MoreExpressions, NT_VariadicMarker
};

class TableDrivenParser {
    stack<int> parseStack;
    bool errorFlag = false;

    Token getToken() {
        return tokenIndex < tokenStream.size() ? tokenStream[tokenIndex] : Token{"EOF", ""};
    }

    void ungetToken() { tokenIndex++; }

    void error(const string& msg) {
        cout << "Error: " << msg << " at token " << tokenIndex << endl;
        errorFlag = true;
    }

public:
    bool parse(const vector<Token>& tokens) {
        tokenStream = tokens;
        tokenIndex = 0;
        parseStack.push(NT_TypeAlias);

        while (!parseStack.empty() && !errorFlag) {
            int current = parseStack.top();
            parseStack.pop();
            
            Token lookahead = getToken();

            switch (current) {
                case NT_TypeAlias:
                    if (lookahead.value == "type") {
                        ungetToken(); // Consume 'type'
                        // Correct production order: type ID TypeParamsOpt = Expr
                        parseStack.push(NT_Expression);
                        parseStack.push('=');
                        parseStack.push(NT_TypeParamsOpt);
                        parseStack.push(NT_Identifier);
                    } else error("Expected 'type' keyword");
                    break;

                case NT_TypeParamsOpt:
                    if (lookahead.value == "[") {
                        parseStack.push(NT_TypeParams);
                    }
                    break;

                case NT_TypeParams:
                    if (lookahead.value == "[") {
                        ungetToken();
                        parseStack.push(']');
                        parseStack.push(NT_TypeParamList);
                        parseStack.push('[');
                    } else error("Expected '[' for type parameters");
                    break;

                case NT_TypeParamList:
                    if (lookahead.type == "IDENTIFIER") {
                        ungetToken();
                        parseStack.push(NT_TypeParamRest);
                        parseStack.push(NT_Identifier);
                    } else error("Expected identifier in type parameters");
                    break;

                case NT_TypeParamRest:
                    if (lookahead.value == ",") {
                        ungetToken();
                        parseStack.push(NT_TypeParamList);
                        parseStack.push(',');
                    }
                    break;

                case NT_Expression: {
                    string type = isTypeKeyword(lookahead.value) ? "TYPE" : lookahead.type;
                    if (type == "TYPE") {
                        if (lookahead.value == "list" || lookahead.value == "dict" || lookahead.value == "tuple")
                            parseStack.push(NT_GenericType);
                        else 
                            parseStack.push(NT_BaseType);
                    }
                    else if (lookahead.type == "IDENTIFIER") 
                        parseStack.push(NT_Identifier);
                    else if (lookahead.value == "(") 
                        parseStack.push(NT_TupleExpr);
                    else 
                        error("Expected type, identifier, or tuple");
                    break;
                }

                case NT_GenericType: {
                    string val = lookahead.value;
                    ungetToken();
                    if (val == "list") {
                        parseStack.push(']');
                        parseStack.push(NT_Expression);
                        parseStack.push('[');
                    } else if (val == "dict") {
                        parseStack.push('}');
                        parseStack.push(NT_Expression);
                        parseStack.push(':');
                        parseStack.push(NT_Expression);
                        parseStack.push('{');
                    } else if (val == "tuple") {
                        parseStack.push(NT_TupleArgs);
                    } else error("Invalid generic type");
                    break;
                }

                case NT_TupleArgs:
                    if (lookahead.value == "(") {
                        ungetToken();
                        parseStack.push(')');
                        parseStack.push(NT_ExpressionList);
                        parseStack.push('(');
                    } else error("Expected '(' for tuple");
                    break;

                case NT_ExpressionList:
                    parseStack.push(NT_MoreExpressions);
                    parseStack.push(NT_Expression);
                    break;

                case NT_MoreExpressions:
                    if (lookahead.value == ",") {
                        ungetToken();
                        Token next = getToken();
                        if (next.value == "." && 
                            tokenIndex+2 < tokenStream.size() && 
                            tokenStream[tokenIndex+1].value == "." &&
                            tokenStream[tokenIndex+2].value == ".") {
                            ungetToken();
                            parseStack.push(NT_VariadicMarker);
                        } else {
                            parseStack.push(NT_MoreExpressions);
                            parseStack.push(NT_Expression);
                            parseStack.push(',');
                        }
                    }
                    break;

                case NT_BaseType:
                    if (isTypeKeyword(lookahead.value)) {
                        ungetToken();
                    } else error("Expected base type");
                    break;

                case NT_Identifier:
                    if (lookahead.type == "IDENTIFIER") {
                        ungetToken();
                    } else error("Expected identifier");
                    break;

                case NT_TupleExpr:
                    if (lookahead.value == "(") {
                        ungetToken();
                        parseStack.push(')');
                        parseStack.push(NT_ExpressionList);
                        parseStack.push('(');
                    } else error("Expected '(' for tuple");
                    break;

                case NT_VariadicMarker:
                    if (lookahead.value == "...") {
                        ungetToken();
                    } else {
                        error("Expected '...' for variadic arguments");
                    }
                    break;

                default:
                    if (current < 256) {
                        if (lookahead.value == string(1, current)) {
                            ungetToken();
                        } else {
                            error("Unexpected token: " + lookahead.value);
                        }
                    }
            }
        }
        return !errorFlag && getToken().type == "EOF";
    }
};

int main() {
    TableDrivenParser parser;

    while(true) {
        cout << "\n-------------------------------------------------------------------------------------------------\n";
        cout << "Enter a string to Parse for type alias or 'band kar' to exit: ";
        string test;
        getline(cin, test);
        
        if(test == "band kar") break;
        if(test.empty()) continue;
        
        auto tokens = tokenize(test);
        tokenStream = tokens;
        cout << "INPUT: " << test << '\n';
        cout << "TOKENS IDENTIFIED:\n";
        for(auto& token : tokens) {
            cout << "   type: \"" << token.type << "\", value: \"" << token.value << "\"\n";
        }
        
        cout << "  PARSING RESULT: " << (parser.parse(tokenStream) ? "Success" : "Failed") << "\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        tokenIndex = 0;
    }

    return 0;
}