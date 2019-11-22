#include <iostream>
#include "md5.h"
 
using std::cout; using std::endl;
 
int main(int argc, char *argv[])
{
    cout << "md5 of 'grape': " << md5("R124339076") << endl;
    return 0;
}
