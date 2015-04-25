#include "hw3.h"
#include <sstream>
#include <iomanip>
#include <stack>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace std;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6){
    std::ostringstream out;
    out << a_value;
    //out << fixed << std::setprecision(n) << a_value;
    return out.str();
}

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
        //sin
        case 'S':
        cout << "sin";
            break;
        //cos
        case 'C':
        cout << "cos";
            break;
        //log
        case 'L':
        cout << "log";
            break;
        //exp
        case 'E':
        cout << "exp";
            break;
        //pow
        case 'P':
        cout << "pow";
            break;
        //sqrt
        case 'Q':
        cout << "sqrt";
            break;
        //fabs
        case 'A':
        cout << "fabs";
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
        //sin
        case 'S':
        s += "sin";
            break;
        //cos
        case 'C':
        s += "cos";
            break;
        //log
        case 'L':
        s += "log";
            break;
        //exp
        case 'E':
        s += "exp";
            break;
        //pow
        case 'P':
        s += "pow";
            break;
        //sqrt
        case 'Q':
        s += "sqrt";
            break;
        //fabs
        case 'A':
        s += "fabs";
            break;
        default :
        s += c;
    }
}

bool isNum(char const& c){
    return ((c >= '0')&&(c <= '9')) || (c == 'N') || (c == '.');
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
int toPostfix(char const* const infix, int const& len, char* postfix, double const douarr[]){

    cout << "# transform from infix to postfix" << endl;

    stack<char> operatorStack;
    string output;

    int plen = 0, douIndex = 0;
    //go through infix[i]
    for(int i = 0; i<len; ++i){
        char c = infix[i];
        if(isNum(c)){
            cout << "encounter " << douarr[douIndex] << ", output directly" << endl;
            output += to_string_with_precision(douarr[douIndex], 2);
            output += " ";
            ++douIndex;
            //mark 'N' and move to next
            postfix[plen] = 'N';
            ++plen;
        }
        else{
            cout << "encounter ";
            printOperator(c);
            if(c == ')' || c == ','){
                cout << ", flush the stack to output until meeting ( or ," << endl;
                while(operatorStack.top() != '(' && operatorStack.top() != ','){
                    putOperator(operatorStack.top(), output);
                    output += " ";
                    postfix[plen] = operatorStack.top();
                    operatorStack.pop();
                    ++plen;
                }
                operatorStack.pop();
                if(c == ','){
                    operatorStack.push(c);
                }
            }
            else if(operatorStack.empty() || operatorStack.top() == '(' || operatorStack.top() == ','){
                cout << ", push to stack" << endl;
                operatorStack.push(c);
            }
            // +, -, ~, !, should be computed from right to left
            else if(precedence(operatorStack.top()) == 15-3 || precedence(operatorStack.top()) == 15-2){
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

    cout << "# postfix expression transforming complete" << endl << endl;

    return plen;
}

double calculate(char const* postfix, double const douarr[]){
    stack<double> operantStack;
    char c;
    double operant[2], result = 0;
    int douIndex = 0;
    for(int i = 0; postfix[i] != '\0'; ++i){
        c = postfix[i];
        if(isNum(c)){
            operantStack.push(douarr[douIndex]);
            ++douIndex;
        }
        else{
            int operantNum = operantNumber(c);
            for(int j = 0; j < operantNum; ++j){
                //in reverse
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
                    result = ~int(operant[0]);
                    break;
                case '*':
                    result = operant[1] * operant[0];
                    break;
                case '/':
                    result = operant[1] / operant[0];
                    break;
                case '%':
                    result = int(operant[1]) % int(operant[0]);
                    break;
                case '+':
                    result = operant[1] + operant[0];
                    break;
                case '-':
                    result = operant[1] - operant[0];
                    break;
                //<<
                case 'l':
                    result = int(operant[1]) << int(operant[0]);
                    break;
                //>>
                case 'r':
                    result = int(operant[1]) >> int(operant[0]);
                    break;
                case '&':
                    result = int(operant[1]) & int(operant[0]);
                    break;
                case '^':
                    result = int(operant[1]) ^ int(operant[0]);
                    break;
                case '|':
                    result = int(operant[1]) | int(operant[0]);
                    break;
                //&&
                case 'a':
                    result = operant[1] && operant[0];
                    break;
                //||
                case 'o':
                    result = operant[1] || operant[0];
                    break;
                //sin
                case 'S':
                    result = sin(operant[0]);
                    break;
                //cos
                case 'C':
                    result = cos(operant[0]);
                    break;
                //log
                case 'L':
                    result = log(operant[0]);
                    break;
                //exp
                case 'E':
                    result = exp(operant[0]);
                    break;
                //pow
                case 'P':
                    result = pow(operant[1], operant[0]);
                    break;
                //sqrt
                case 'Q':
                    result = sqrt(operant[0]);
                    break;
                //fabs
                case 'A':
                    result = fabs(operant[0]);
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
    double* douarr = new double[1000000];
    int douNum = 0;
    char doubuff[10] = {'\0'};
    bool isEOF = false;
    while(true){
        len = 0;
        c = '\0';
        lastc = '\0';
        douNum = 0;
        doubuff[0] = '\0';
        while(true){
            if(!cin.get(spaceFilter)){
                isEOF = true;
                break;
            }
            if(spaceFilter != '\t' && spaceFilter != ' ' && spaceFilter != '\n'){
                c = spaceFilter;
                //if c is 0-9 or '.' or 'N'
                if(isNum(c)){
                    if(!isNum(lastc)){
                        doubuff[0] = c;
                        doubuff[1] = '\0';
                    }
                    //both c and lastc are Num
                    else{
                        strncat(doubuff, &c, 1);
                    }
                }
                else{
                    if(isNum(lastc) && !isNum(c)){
                        douarr[douNum] = atof(doubuff);
                        ++douNum;
                        doubuff[0] = '\0';
                        infix[len] = 'N';
                        ++len;
                    }
                    /*
                    if(c == ','){
                        infix[len] = ')';
                        ++len;
                        infix[len] = '(';
                        ++len;
                        continue;
                    }
                    */
                    if((c == '+' || c == '-') && (!isNum(lastc) && lastc != ')')){
                        c = (c == '+')? 'p' : 'n';
                    }
                    switch(c){
                        //sin or sqrt
                        case 's':{
                            char c1 = cin.get(), c2 = cin.get();
                            if(c1 == 'i' && c2 == 'n'){
                                c = 'S';
                                break;
                            }
                            if(cin.get() == 't'){
                                c = 'Q';
                            }
                            break;
                        }
                        //cos
                        case 'c':{
                            char c1 = cin.get(), c2 = cin.get();
                            if(c1 == 'o' && c2 == 's'){
                                c = 'C';
                            }
                            break;
                        }
                        case 'l':{
                            char c1 = cin.get(), c2 = cin.get();
                            if(c1 == 'o' && c2 == 'g'){
                                c = 'L';
                            }
                            break;
                        }
                        case 'e':{
                            char c1 = cin.get(), c2 = cin.get();
                            if(c1 == 'x' && c2 == 'p'){
                                c = 'E';
                            }
                            break;
                        }
                        case 'p':{
                            char c1 = cin.get(), c2 = cin.get();
                            if(c1 == 'o' && c2 == 'w'){
                                c = 'P';
                            }
                            break;
                        }
                        case 'f':{
                            char c1 = cin.get(), c2 = cin.get(), c3 = cin.get();
                            if(c1 == 'a' && c2 == 'b' && c3 == 's'){
                                c = 'A';      
                            }
                            break;
                        }
                    }
                    if(c == lastc){
                        switch(c){
                            //&&
                            case '&':
                                infix[len-1] = 'a';
                                break;
                            //||
                            case '|':
                                infix[len-1] = 'o';
                                break;
                            //<<
                            case '<':
                                infix[len-1] = 'l';
                                break;
                            //>>
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
                    douarr[douNum] = atof(doubuff);
                    ++douNum;
                    doubuff[0] = '\0';
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
       
        toPostfix(infix, len, postfix, douarr);
    
        cout << "Postfix Exp:";
        cout << fixed << setprecision(6);
        for(int i = 0, j=0; i<len; ++i){
            if(postfix[i] == 'N'){
                cout << " " << douarr[j];
                ++j;
            }
            else{
                cout << " ";
                printOperator(postfix[i]);
            }
        }
        cout << endl;
    
        //printf("RESULT: %.6f\n", calculate(postfix, douarr));
        cout << "RESULT: " << calculate(postfix, douarr) << endl;
        cout.unsetf(ios::fixed);
    }
    delete [] douarr;
}
