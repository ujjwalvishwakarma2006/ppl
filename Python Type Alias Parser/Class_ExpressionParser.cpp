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