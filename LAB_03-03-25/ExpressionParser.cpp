#include <bits/stdc++.h>
#include <unordered_set>
#include "CompactExpressionParser.cpp"
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
    return (word[0] >= 'a' && word[0] <= 'z') ||
           (word[0] >= 'A' && word[0] <= 'Z');
}

string typeOfToken(const string& word) {
    if (word == "type") return "KEYWORD";
    if (typeKeywords.count(word)) return "TYPE";
    if (word == "=" || word == "(" || word == ")" || 
       word == "[" || word == "]" || word == ",") return "SYMBOL";
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


ExpressionParser P;
int totalPassed = 0;

void runTest(const string& input, bool expected) {
    tokenStream = tokenize(input);
    tokenIndex = 0;

    cout << "INPUT: " << input << '\n';
    cout << "TOKENS IDENTIFIED:\n";
    for(auto token : tokenStream){
        cout << "   type: \"" << token.type << "\",     " << "value: \"" << token.value << "\"\n";
    }
    bool result = P.parseExpression();
    cout << "Result: " << (result ? "VALID" : "INVALID");
    cout << " \t[" << (result == expected ? "PASS" : "FAIL") << "]\n\n";
    if(result == expected) totalPassed++;
}

int main() {
    // Valid test cases
    runTest("int", true);
    runTest("list[int]", true);
    runTest("dict{str:int}", true);
    runTest("tuple(int,float)", true);
    runTest("list[tuple(str,int)]", true);
    runTest("dict{tuple(int,int):str}", true);
    runTest("list[dict{str:float}]", true);
    runTest("tuple(list[int],dict{str:bool})", true);

    // Invalid test cases
    runTest("list[]", false);
    runTest("dict{str}", false);
    runTest("tuple()", false);
    runTest("list[int", false);
    runTest("dict{str:int", false);
    runTest("tuple(int,)", false);
    runTest("list[list]", false);
    runTest("dict{int}", false);
    runTest("tuple(int,str", false);
    runTest("lisp[int]", false);
    runTest("dict{str:}", false);
    runTest("tuple(,int)", false);
    runTest("list[dict{str}]", false);

    cout << totalPassed;

    return 0;
}