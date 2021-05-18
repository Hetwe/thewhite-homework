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

// Так как метод summaElements() и findMin() не изменяют дек, нужно ли вынести их в отдельный класс,
// И нужно ли менять класс Worker и Output на шаблонный класс

class Worker{
  public:
    Worker(){}

    void exponentiation(std::deque<int>& intDeq, int& min){
      for (auto& item : intDeq) {
        if (item < 0) {
          item = pow(min, 2);
        }
      }
    }

    void filling(std::deque<int>& intDeq){
      std::deque<int>::iterator iter;
      for (iter = intDeq.begin(); iter != intDeq.end(); iter++) {
        *iter = -25 + rand() % 50;
      }
    }

    int summaElements(const std::deque<int>& intDeq) {
      int summa = 0;
      for (const auto& item : intDeq) {
        summa = summa + item;
      }
      return summa;
    }

    int findMin(std::deque<int>& intDeq){
      auto iter = intDeq.begin();
      int min = *iter;
      for (auto item : intDeq) {
        if (item < min) {
          min = item;
        }
      }
    }
};

// В дальнейшем можно добавить вывод на печать или в файл
class Output{
  public:
    Output(){}

    void print(const std::deque<int>& intDeq){
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
}

int main() {
  setlocale(LC_ALL, "Russian");
  srand(time(NULL));
  std::deque<int> intDeq(30);
  Output deqOut = new Output();
  Worker deqWork = new WOrker();

  cout << "Исходный массив элементов" << endl;
  deqOut->print(intDeq);

  float summa = deqWork->summaElements(intDeq);
  int min = deqWork->findMin(intDeq);

  cout << endl << "Сумма элементов массива: " << summa << endl;
  cout << "Среднее арифметическое элементов массива: " << (summa / intDeq.size()) << endl;
  cout << "Заменим отрицателные числа квадратом минимума" << endl;

  deqWork->exponentiation(intDeq, min);
  cout << "Преобразованный массив элементов" << endl;
  deqOut->print(intDeq);
}
