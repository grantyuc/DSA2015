#include "hw3.h"
#include <stack>
#include <cstring>
#include <cstdlib>

using namespace std;

void printOperator(char const& c){
    switch(c){
        //+
        case 'p':
        cout << "+";
            break;
        //-
        case 'n':
        cout << "-";
            break;
        //<<
        case 'l':
        cout << "<<";
            break;
        //>>
        case 'r':
        cout << ">>";
            break;
        //&&
        case 'a':
        cout << "&&";
            break;
        //||
        case 'o':
        cout << "||";
            break;
        default :
        cout << c;
    }
}

void putOperator(char const& c, string& s){
    switch(c){
        //+
        case 'p':
        s += "+";
            break;
        //-
        case 'n':
        s += "-";
            break;
        //<<
        case 'l':
        s += "<<";
            break;
        //>>
        case 'r':
        s += ">>";
            break;
        //&&
        case 'a':
        s += "&&";
            break;
        //||
        case 'o':
        s += "||";
            break;
        default :
        s += c;
    }
}

bool isNum(char const& c){
    return ((c >= '0')&&(c <= '9')) || (c == 'N');
}

void printStack(stack<char>& operatorStack){
    stack<char> stack;
    while(!operatorStack.empty()){
        stack.push(operatorStack.top());
        operatorStack.pop();
    }
    while(!stack.empty()){
        printOperator(stack.top());
        cout << " ";
        operatorStack.push(stack.top());
        stack.pop();
    }
    cout << endl;
}
int toPostfix(char const* const infix, int const& len, char* postfix, int const intarr[]){

    //cout << "# transform from infix to postfix" << endl;

    stack<char> operatorStack;
    string output;

    int plen = 0, intIndex = 0;
    //go through infix[i]
    for(int i = 0; i<len; ++i){
        char c = infix[i];
        if(isNum(c)){
            //cout << "encounter " << intarr[intIndex] << ", output directly" << endl;
            output += to_string(intarr[intIndex]);
            output += " ";
            ++intIndex;
            //mark 'N' and move to next
            postfix[plen] = 'N';
            ++plen;
        }
        else{
            //cout << "encounter ";
            //printOperator(c);
            if(c == ')'){
                //cout << ", flush the stack to output until meeting (" << endl;
                while(operatorStack.top() != '('){
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                }
                operatorStack.pop();
            }
            else if(operatorStack.empty() || operatorStack.top() == '('){
                //cout << ", push to stack" << endl;
                operatorStack.push(c);
            }
            // +, -, ~, !, should be computed from right to left
            else if(precedence(operatorStack.top()) == 15-3){
                if(precedence(operatorStack.top()) <= precedence(c)){
                    //cout << ", push to stack" << endl;
                    operatorStack.push(c);
                }
                else{
                    //cout << "\n*** stack.top() has greater precedence, so hold on and output the top of stack" << endl;
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                    --i;
                }
            }
            // else computed from left to right
            else{
                if(precedence(operatorStack.top()) < precedence(c)){
                    //cout << ", push to stack" << endl;
                    operatorStack.push(c);
                }
                else{
                    //cout << "\n*** stack.top() has greater precedence, so hold on and output the top of stack" << endl;
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                    --i;
                }
            
            }
        }
        //cout << "\tcurrent output: " << output << endl;
        //cout << "\tcurrent stack: ";
        //printStack(operatorStack);
        //cout << endl;
    }
    //cout << "encounter NOTHING, flush the stack to output" << endl;
    while(!operatorStack.empty()){
        putOperator(operatorStack.top(), output);
        output += " ";
        postfix[plen] = operatorStack.top();
        operatorStack.pop();
        ++plen;
    }
    postfix[plen] = '\0';
        
    //cout << "\tcurrent output: " << output << endl;
    //cout << "\tcurrent stack: ";
    //printStack(operatorStack);

    //cout << "# postfix expression transforming complete" << endl;

    return plen;
}
int toPostfix_with_step(char const* const infix, int const& len, char* postfix, int const intarr[]){

    cout << "# transform from infix to postfix" << endl;

    stack<char> operatorStack;
    string output;

    int plen = 0, intIndex = 0;
    //go through infix[i]
    for(int i = 0; i<len; ++i){
        char c = infix[i];
        if(isNum(c)){
            cout << "encounter " << intarr[intIndex] << ", output directly" << endl;
            output += to_string(intarr[intIndex]);
            output += " ";
            ++intIndex;
            //mark 'N' and move to next
            postfix[plen] = 'N';
            ++plen;
        }
        else{
            cout << "encounter ";
            printOperator(c);
            if(c == ')'){
                cout << ", flush the stack to output until meeting (" << endl;
                while(operatorStack.top() != '('){
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                }
                operatorStack.pop();
            }
            else if(operatorStack.empty() || operatorStack.top() == '('){
                cout << ", push to stack" << endl;
                operatorStack.push(c);
            }
            // +, -, ~, !, should be computed from right to left
            else if(precedence(operatorStack.top()) == 15-3){
                if(precedence(operatorStack.top()) <= precedence(c)){
                    cout << ", push to stack" << endl;
                    operatorStack.push(c);
                }
                else{
                    cout << "\n*** stack.top() has greater precedence, so hold on and output the top of stack" << endl;
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                    --i;
                }
            }
            // else computed from left to right
            else{
                if(precedence(operatorStack.top()) < precedence(c)){
                    cout << ", push to stack" << endl;
                    operatorStack.push(c);
                }
                else{
                    cout << "\n*** stack.top() has greater precedence, so hold on and output the top of stack" << endl;
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                    --i;
                }
            
            }
        }
        cout << "\tcurrent output: " << output << endl;
        cout << "\tcurrent stack: ";
        printStack(operatorStack);
        cout << endl;
    }
    cout << "encounter NOTHING, flush the stack to output" << endl;
    while(!operatorStack.empty()){
        putOperator(operatorStack.top(), output);
        output += " ";
        postfix[plen] = operatorStack.top();
        operatorStack.pop();
        ++plen;
    }
    postfix[plen] = '\0';
        
    cout << "\tcurrent output: " << output << endl;
    cout << "\tcurrent stack: ";
    printStack(operatorStack);

    cout << "# postfix expression transforming complete" << endl;

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
    if(!operantStack.empty()){
        result = operantStack.top();
    }
    return result;
}

int main(){
    int len = 0;
    char spaceFilter = '\0', c = '\0', lastc = '\0';
    char infix[1000001], postfix[1000001];
    int intarr[1000000], intNum = 0;
    char intbuff[10] = {'\0'};
    bool isEOF = false;
    while(true){
        len = 0;
        c = '\0';
        lastc = '\0';
        intNum = 0;
        intbuff[0] = '\0';
        while(true){
            if(!cin.get(spaceFilter)){
                isEOF = true;
                break;
            }
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
        if(isEOF){
            break;
        }
   
        //toPostfix_with_step(infix, len, postfix, intarr);
        toPostfix(infix, len, postfix, intarr);

        cout << "Postfix Exp:";
        for(int i = 0, j=0; postfix[i] != '\0'; ++i){
            if(postfix[i] == 'N'){
                cout << " " << intarr[j];
                ++j;
            }
            else{
                cout << " ";
                printOperator(postfix[i]);
            }
        }
        cout << endl;

        cout << "RESULT: " << calculate(postfix, intarr) << endl;
    }
}
