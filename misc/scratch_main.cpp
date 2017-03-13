#include <iostream>
#include <string>

using namespace std;

class Animal {

	public:
		string name;
		Animal(): name("Harry") {}
		virtual void speak();

};

class Cat : Animal {
	int x;

	public:
	virtual void speak();
	Cat(int x): x(x) {}
};


void Animal::speak() {
	cout << name << endl;
}


void Cat::speak() {
	for(int i =0 ; i < x; i++) {
		cout << "Meow" << endl;
	}
}


int main(void) {

	Animal *a = new Animal;
	Cat *b = new Cat(3);
	Animal *c = b;

	a->speak();
	b->speak();

}
