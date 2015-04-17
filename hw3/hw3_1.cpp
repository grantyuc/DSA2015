#include <iostream>
#include <stack>
#include <cstring>
#include <cstdlib>

using namespace std;

bool isNum(char const& c){
    return ((c >= '0')&&(c <= '9')) || (c == 'N');
}

int precedence(char const& operation){
    int prec;
    switch(operation){
        case '(':
            prec = 2;
            break;
        //+
        case 'p':
            prec = 3;
            break;
        //-
        case 'n':
            prec = 3;
            break;
        case '!':
            prec = 3;
            break;
        case '~':
            prec = 3;
            break;
        case '*':
            prec = 5;
            break;
        case '/':
            prec = 5;
            break;
        case '%':
            prec = 5;
            break;
        case '+':
            prec = 6;
            break;
        case '-':
            prec = 6;
            break;
        //<<
        case 'l':
            prec = 7;
            break;
        //>>
        case 'r':
            prec = 7;
            break;
        case '&':
            prec = 10;
            break;
        case '^':
            prec = 11;
            break;
        case '|':
            prec = 12;
            break;
        //&&
        case 'a':
            prec = 13;
            break;
        //||
        case 'o':
            prec = 14;
            break;
        default:
            cout << operation << " Precedence undefined" << endl;
            return -1;
    }
    return 15-prec;
}

int operantNumber(char const& operation){
    int operantNum;
    switch(operation){
        //+
        case 'p':
            operantNum = 1;
            break;
        //-
        case 'n':
            operantNum = 1;
            break;
        case '!':
            operantNum = 1;
            break;
        case '~':
            operantNum = 1;
            break;
        case '*':
            operantNum = 2;
            break;
        case '/':
            operantNum = 2;
            break;
        case '%':
            operantNum = 2;
            break;
        case '+':
            operantNum = 2;
            break;
        case '-':
            operantNum = 2;
            break;
        //<<
        case 'l':
            operantNum = 2;
            break;
        //>>
        case 'r':
            operantNum = 2;
            break;
        case '&':
            operantNum = 2;
            break;
        case '^':
            operantNum = 2;
            break;
        case '|':
            operantNum = 2;
            break;
        //&&
        case 'a':
            operantNum = 2;
            break;
        //||
        case 'o':
            operantNum = 2;
            break;
        default:
            cout << operation << " No operantNum" << endl;
            return -1;
    }
    return operantNum;
}

int toPostfix(char const* const infix, int const& len, char* postfix){
    stack<char> operatorStack;
    int plen = 0;
    for(int i = 0; i<len; ++i){
        char c = infix[i];
        if(isNum(c)){
            postfix[plen] = 'N';
            ++plen;
        }
        else{
            if(operatorStack.empty() || operatorStack.top() == '('){
                operatorStack.push(c);
            }
            else if(c == ')'){
                while(operatorStack.top() != '('){
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                }
                operatorStack.pop();
            }
            else if(precedence(operatorStack.top()) == 15-3){
                if(precedence(operatorStack.top()) <= precedence(c)){
                    operatorStack.push(c);
                }
                else{
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                    --i;
                }
            }
            else{
                if(precedence(operatorStack.top()) < precedence(c)){
                    operatorStack.push(c);
                }
                else{
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                    --i;
                }
            
            }
        }
    }
    while(!operatorStack.empty()){
        postfix[plen] = operatorStack.top();
        operatorStack.pop();
        ++plen;
    }
    postfix[plen] = '\0';
    return plen;
}

int calculate(char const* postfix, int const intarr[]){
    stack<int> operantStack;
    char c;
    int operant[2], result = 0, intIndex = 0;
    for(int i = 0; postfix[i] != '\0'; ++i){
        c = postfix[i];
        if(isNum(c)){
            operantStack.push(intarr[intIndex]);
            ++intIndex;
        }
        else{
            int operantNum = operantNumber(c);
            for(int j = 0; j < operantNum; ++j){
                operant[j] = operantStack.top();
                operantStack.pop();
            }
            switch(c){
                //+
                case 'p':
                    result = operant[0];
                    break;
                //-
                case 'n':
                    result = -operant[0];
                    break;
                case '!':
                    result = !operant[0];
                    break;
                case '~':
                    result = ~operant[0];
                    break;
                case '*':
                    result = operant[1] * operant[0];
                    break;
                case '/':
                    result = operant[1] / operant[0];
                    break;
                case '%':
                    result = operant[1] % operant[0];
                    break;
                case '+':
                    result = operant[1] + operant[0];
                    break;
                case '-':
                    result = operant[1] - operant[0];
                    break;
                //<<
                case 'l':
                    result = operant[1] << operant[0];
                    break;
                //>>
                case 'r':
                    result = operant[1] >> operant[0];
                    break;
                case '&':
                    result = operant[1] & operant[0];
                    break;
                case '^':
                    result = operant[1] ^ operant[0];
                    break;
                case '|':
                    result = operant[1] | operant[0];
                    break;
                //&&
                case 'a':
                    result = operant[1] && operant[0];
                    break;
                //||
                case 'o':
                    result = operant[1] || operant[0];
                    break;
                default:
                    cout << c << " calculate error" << endl;
                    return -1;
            }
            operantStack.push(result);
        }
    }
    return result;
}

int main(){
    int len = 0, j;
    char spaceFilter = '\0', c = '\0', lastc = '\0';
    char infix[1000000], postfix[2000000];
    int intarr[1000000], intNum = 0;
    char intbuff[10] = {'\0'};
    while(true){
    len = 0;
    spaceFilter = '\0'; 
    c = '\0';
    lastc = '\0';
    intNum = 0;
    intbuff[0] = '\0';
    while(cin.get(spaceFilter)){
        if(spaceFilter != '\t' && spaceFilter != ' ' && spaceFilter != '\n'){
            c = spaceFilter;
            if(isNum(c)){
                if(!isNum(lastc)){
                    intbuff[0] = c;
                    intbuff[1] = '\0';
                }
                else{
                    strncat(intbuff, &c, 1);
                }
            }
            else{
                if(isNum(lastc) && !isNum(c)){
                    intarr[intNum] = atoi(intbuff);
                    ++intNum;
                    intbuff[0] = '\0';
                    infix[len] = 'N';
                    ++len;
                }
                if((c == '+' || c == '-') && (!isNum(lastc) && lastc != ')')){
                    c = (c == '+')? 'p' : 'n';
                }
                if(c == lastc){
                    switch(c){
                        case '&':
                            infix[len-1] = 'a';
                            break;
                        case '|':
                            infix[len-1] = 'o';
                            break;
                        case '<':
                            infix[len-1] = 'l';
                            break;
                        case '>':
                            infix[len-1] = 'r';
                            break;
                        default:
                            infix[len] = c;
                            ++len;
                            break;
                    }
                }
                else{
                    infix[len] = c;
                    ++len; 
                }
            }
        }
        else if(spaceFilter == '\n'){
            c = spaceFilter;
            if(isNum(lastc) && !isNum(c)){
                intarr[intNum] = atoi(intbuff);
                ++intNum;
                intbuff[0] = '\0';
                infix[len] = 'N';
                ++len;
            }
            infix[len] = '\0';
            break;
        }
        lastc = c;
    }
   
    toPostfix(infix, len, postfix);

    cout << "RESULT: " << calculate(postfix, intarr) << endl;

    cout << "infix expression: " << infix << endl;
    cout << "infix expression: ";

    j = 0;
    for(int i = 0; i<len; ++i){
        if(infix[i] == 'N'){
            cout << intarr[j];
            ++j;
        }
        else{
            cout << infix[i];
        }
    }
    cout << endl;

    cout << "postfix expression: " << postfix << endl;
    cout << "postfix expression: ";

    j = 0;
    for(int i = 0; i<len; ++i){
        if(postfix[i] == 'N'){
            cout << intarr[j];
            ++j;
        }
        else{
            cout << postfix[i];
        }
    }
    cout << endl;
}
}
