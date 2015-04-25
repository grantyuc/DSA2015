#include <iostream>
using namespace std;

int precedence(char const& operation){
    int prec;
    switch(operation){
        case ',':
            prec = 1;
            break;
        case '(':
            prec = 1;
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
        //sin
        case 'S':
            prec = 2;
            break;
        //cos
        case 'C':
            prec = 2;
            break;
        //log
        case 'L':
            prec = 2;
            break;
        //exp
        case 'E':
            prec = 2;
            break;
        //pow
        case 'P':
            prec = 2;
            break;
        //sqrt
        case 'Q':
            prec = 2;
            break;
        //fabs
        case 'A':
            prec = 2;
            break;
        default:
            cout << endl << operation << " Precedence undefined" << endl;
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
        //sin
        case 'S':
            operantNum = 1;
            break;
        //cos
        case 'C':
            operantNum = 1;
            break;
        //log
        case 'L':
            operantNum = 1;
            break;
        //exp
        case 'E':
            operantNum = 1;
            break;
        //pow
        case 'P':
            operantNum = 2;
            break;
        //sqrt
        case 'Q':
            operantNum = 1;
            break;
        //fabs
        case 'A':
            operantNum = 1;
            break;
        default:
            cout << operation << " No operantNum" << endl;
            return -1;
    }
    return operantNum;
}

