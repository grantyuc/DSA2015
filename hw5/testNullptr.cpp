#include <iostream>

int main(){
    std::cout << ((nullptr == 0)? "nullptr == 0" : "nullptr != 0" )<< std::endl;
    std::cout << ((NULL == 0)? "null == 0" : "null != 0" )<< std::endl;
}
