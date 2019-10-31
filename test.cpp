#include <iostream>
#include <typeinfo>
int main(){
	std::cout << typeid("hola").name() << std::endl;
}
