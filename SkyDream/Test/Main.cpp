#include <stdio.h>
#include <iostream>
#include <proto/test.pb.h>
int main()
{
	tutorial::Person* person = new tutorial::Person();
	person->set_age(23);
	person->set_name("nimamaipi");
	std::cout << person->age() << "\t" << person->name().c_str() << std::endl;
	while(1);
}