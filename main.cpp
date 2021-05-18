#include <iostream>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cmath>

using std::cout;
using std::endl;
using std::cin;
using std::setw;

void filling(std::deque<int>&);
void print(const std::deque<int>&);
int summaElements(const std::deque<int>&);
void Function(std::deque<int>&, int&);

int main() {
  setlocale(LC_ALL, "Russian");
  srand(time(NULL));

  std::deque<int> intDeq(30);
  filling(intDeq);
  cout << "Исходный массив элементов" << endl;
  print(intDeq);

  auto iter = intDeq.begin();
  int min = *iter;
  for (auto item : intDeq) {
    if (item < min) {
      min = item;
    }
  }

  float summa = summaElements(intDeq);
  cout << endl << "Сумма элементов массива: " << summa << endl;
  cout << "Среднее арифметическое элементов массива: " << (summa / intDeq.size()) << endl;
  cout << "Заменим отрицателные числа квадратом минимума" << endl;

  Function(intDeq, min);
  print(intDeq);
  return 0;
}



void filling(std::deque<int>& intDeq) {
  std::deque<int>::iterator iter;
  for (iter = intDeq.begin(); iter != intDeq.end(); iter++) {
    *iter = -25 + rand() % 50;
  }
}

void print(const std::deque<int>& intDeq) {
  int i = 0;
  std::deque<int>::const_iterator iter;
  for (iter = intDeq.cbegin(); iter != intDeq.cend(); iter++) {
    if (i % 5 == 0) {
      cout << endl;
    }
    cout << *iter << setw(3) << " | " << setw(3);
    i++;
  }
  cout << endl;
}



int summaElements(const std::deque<int>& intDeq) {
  int summa = 0;
  for (const auto& item : intDeq) {
    summa = summa + item;
  }
  return summa;
}

void Function(std::deque<int>& intDeq, int& min) {
  for (auto& item : intDeq) {
    if (item < 0) {
      item = pow(min, 2);
    }
  }
}
