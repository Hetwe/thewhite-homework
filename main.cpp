#include <iostream>
#include <list>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cmath>
using std::cout;
using std::endl;
using std::cin;
using std::setw;
void filling(std::list<int>&);
void print(const std::list<int>&);
int summaElements(const std::list<int>&);
void Function(std::list<int>&, int&);
int main() {
setlocale(LC_ALL, "Russian");
srand(time(NULL));
std::list<int> intList(30);
filling(intList);
cout << "Исходный массив элементов" << endl;
print(intList);
auto iter = intList.begin();
int min = *iter;
for (auto item : intList) {
if (item < min) {
min = item;
}
}
float summa = summaElements(intList);
cout << endl << "Сумма элементов массива: " << summa << endl;
