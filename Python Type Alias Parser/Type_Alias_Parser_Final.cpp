#include <bits/stdc++.h>
#include <unordered_set>
// #include "Class_ExpressionParser.h"
using namespace std;

struct Token {
    string type;
    string value;
};

vector<Token> tokenStream;
int tokenIndex = 0;  // Start at first token
unordered_set<string> typeKeywords = {"int", "str", "float", "bool", "list", "dict", "tuple"};

// Check if word is a type keyword
bool isTypeKeyword(const string& word) {
    return typeKeywords.count(word);
}

// Get current token without advancing
Token getToken() {
    if (tokenIndex < tokenStream.size()) {
        return tokenStream[tokenIndex];
    }
    return {"EOF", ""};
}

// Only function that advances the token stream
void ungetToken() {
    tokenIndex++;
}

bool isIdentifier(string word) {
    if(word.empty()) return false;
    if(typeKeywords.count(word)) return false;
    return (word[0] >= 'a' && word[0] <= 'z') ||
           (word[0] >= 'A' && word[0] <= 'Z');
}

string typeOfToken(const string& word) {
    if (word == "type") return "KEYWORD";
    if (isTypeKeyword(word)) return "TYPE";
    if (word == "=" || word == "(" || word == ")" || 
        word == "[" || word == "]" || word == "," ||
        word == "{" || word == "}" || word == ":") 
        return "SYMBOL";
    return isIdentifier(word) ? "IDENTIFIER" : "UNKNOWN";
}

vector<Token> tokenize(const string &input) {
    vector<Token> tokens;
    string word = "";
    
    for (char ch : input) {
        if (ch == ' ' || ch == '=' || ch == '(' || ch == ')' || 
            ch == '[' || ch == ']' || ch == ',' || ch == '{' ||
            ch == '}' || ch == ':') {
            if (!word.empty()) {
                tokens.push_back({typeOfToken(word), word});
                word.clear();
            }
            if (ch != ' ') {
                string sym(1, ch);
                tokens.push_back({typeOfToken(sym), sym});
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

// This needs to be fixed.
bool parseTypeParams() {
    Token token = getToken();
    if (token.value == "]"){
        cout << "Error: Expected type parameter list" << endl;
        return false;
    }

    while (true) {
        token = getToken();
        if (token.value != "TYPE" && token.type != "IDENTIFIER") {
            return false;
        }
        ungetToken();  // Advance after matching type
        
        token = getToken();
        if (token.value == ")") {
            ungetToken();  // Advance past closing )
            return true;
        }
        if (token.value != ",") return false;
        ungetToken();  // Advance past comma
        
        token = getToken();
    }
}



class ExpressionParser {
    public:
    
        ExpressionParser() {}
    
        bool parseTuple(){
            Token token = getToken();
            if(token.value != "("){
                cout << "Error: Expected '(' at the start of tuple." << endl;
                return false;
            }
            ungetToken();
            if(token.value == ")"){
                cout << "Error: Empty tuple." << endl;
                return false;
            }
            if(!parseExpression()){
                cout << "Error: Expected expression in tuple." << endl;
                return false;
            }
            token = getToken();
            
            while(token.value == ","){
                ungetToken();
                if(!parseExpression()){
                    cout << "Error: Expected expression in tuple." << endl;
                    return false;
                }
                token = getToken();
            }
            token = getToken();
            if(token.value == ")") {
                ungetToken();
                return true;
            }
            if(!parseExpression()){
                cout << "Error: Expected expression in tuple." << endl;
                return false;
            }
            token = getToken();
            if(token.value == ")"){
                ungetToken();
                return true;
            }
            return false;
    
        }
    
        bool parseExpression(){
            Token token = getToken();
            if(typeKeywords.find(token.value) == typeKeywords.end()){
                cout << "Error: " << token.value << " is not a valid type" << endl;
                return false;
            }
            if(token.value != "list" && token.value != "dict" && token.value != "tuple"){
                ungetToken();
                token = getToken();
                if(isTypeKeyword(token.value)) {
                    cout << "Error: Not expecting type keyword here" << endl;
                    return false;
                }
                return true;
            }
            if(token.value == "list"){
                ungetToken();
                token = getToken();
                if(token.value != "["){
                    cout << "Error: Expected '[' here!";
                    return false;
                }
                ungetToken();
                token = getToken();
                if(token.value == "]"){
                    cout << "Error: List[] must have a type!";
                    return false;
                }
                if(!parseExpression()){
                    cout << "Error: Not a valid expression!";
                    return false;
                }
                token = getToken();
                if(token.value != "]"){
                    cout << "Error: Expected ']' here!";
                    return false;
                }
                ungetToken();
                return true;
            }
            if(token.value == "dict"){
                ungetToken();
                token = getToken();
                if(token.value != "{"){
                    cout << "Error: Expected '{' here!";
                    return false;
                }
                ungetToken();
                token = getToken();
                if(token.value == "}"){
                    cout << "Error: Dictionary must have a key_type and a value_type!";
                    return false;
                }
                if(!parseExpression()){
                    cout << "Error: key_type must be an expression!";
                    return false;
                }
                token = getToken();
                if(token.value != ":"){
                    cout << "Error: Expected ':' here!";
                    return false;
                }
                ungetToken();
                if(!parseExpression()){
                    cout << "Error: key_type must be an expression!";
                    return false;
                }
                token = getToken();
                if(token.value != "}"){
                    cout << "Error: Expected '}' here!";
                    return false;
                }
                ungetToken();
                return true;
            }
            if(token.value == "tuple"){
                ungetToken();
                if(parseTuple()) {
                    return true;
                }
                else return false;
            }
            if(token.value == "("){
                if(parseTuple()){
                    return true;
                }
                else{
                    cout << "Error: Expected a tuple here!";
                    return false;
                }
            }
            return false;
        }
    
};



ExpressionParser EP;

bool parseTypeAlias() {
    Token token = getToken();
    if (token.value != "type") {
        cout << "Error: Missing 'type' keyword\n";
        return false;
    }
    ungetToken();  // Advance past 'type'
    
    token = getToken();
    if (token.type != "IDENTIFIER") {
        cout << "Error: Invalid alias name\n";
        return false;
    }
    ungetToken();  // Advance past identifier

    token = getToken();
    if(token.value == "[") {
        ungetToken();
        if (!parseTypeParams()) return false;
    }
    
    token = getToken();
    if (token.value != "=") {
        cout << "Error: Missing assignment operator\n";
        return false;
    }
    ungetToken();  // Advance past =
    
    return EP.parseExpression();
}

int main() {
    vector<string> tests = {
        "type MyAlias = int",
        "type Vector = list[int]",
        "type Matrix = list[list[float]]",
        "type Point = (int, str)",
        "type T = dict",
        "MyAlias = int",
        "type int = str",
        "type MyAlias = ()",
        "type MyAlias = list[]"
    };
    
    for(string test : tests){
        auto tokens = tokenize(test);
        tokenStream = tokens;
        cout << "INPUT: " << test << '\n';
        cout << "TOKENS IDENTIFIED:\n";
        for(auto token : tokens){
            cout << "   type: \"" << token.type << "\", " << "value: \"" << token.value << "\"\n";
        }
        cout << "  PARSING RESULT: " << (parseTypeAlias() ? "Success\n\n" : "Failed\n\n");
        tokenIndex = 0;
    }
    return 0;
}