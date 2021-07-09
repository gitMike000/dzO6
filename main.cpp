#include <iostream>
#include <sstream>

using namespace std;

//------ Task 2

std::ostream& endll(std::ostream&out) {
    out.put('\n');
    out << '\n';
    out.flush();
    return out;
}

// ----- for BlackJack
#include <vector>

class Card {
public:
  enum znashen {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
           JACK, QUEEN, KING};
  enum mast {CLUBS, DIAMONDS, HEARTS, SPADES};

  // перегрузка оператора вывода для Task 5
  // если карта перевернута рубашкой вверх (мы ее не видим),
  // вывести ХХ, если мы ее видим, вывести масть и номинал карты
  friend ostream& operator<<(ostream& os, const Card& aCard);

  Card(znashen r = ACE, mast s = SPADES, bool ifUp = true): m_Znashen(r), m_Mast(s), m_IsFaceUp(ifUp)
  {}

  // возвращает значение карты, пока можно считать, что туз = 1
  int GetValue() const
  {
   int value = 0;
   if (m_IsFaceUp)
    {
      value = m_Znashen;
      if (value > 10) value = 10;
    }
   return value;
  };

  // переворачивает карту
  void Flip()
  {
     m_IsFaceUp = !(m_IsFaceUp);
  };

private:
  znashen m_Znashen;
  mast m_Mast;
  bool m_IsFaceUp;
};

class Hand
{

protected:
  vector <Card*> m_Cards;

public:
  Hand()
  {
   m_Cards.reserve(7);
  }
         ;

  // добавляет в коллекцию карт новую карту
  void Add(Card* pCard)
  {
    m_Cards.push_back(pCard);
  }

  // очищает руку от карт
  void Clear()
  {
    //iterate through vector, freeing all memory on the heap
    vector <Card*>::iterator iter = m_Cards.begin();
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
      delete *iter;
      *iter = 0; //get rid of dangling pointer - null pointer
    }
    m_Cards.clear();
  }

  // возвращает сумму очков карт руки, туз принимает значения 1 или 11
  int GetTotal() const
{

  //если нет карт, возвращает 0
  if (m_Cards.empty()) return 0;

  // если первая карта 0, то карты закрыта, возвращает 0
  if (m_Cards[0]->GetValue() == 0) return 0;

  // сложение значения карт, относимся к каждому тузу как к 1
  int total = 0;
  vector<Card*>::const_iterator iter;
  for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
  total += (*iter)->GetValue();

  // определяем, есть ли в руке туз
  bool containsAce = false;
  for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
  if ((*iter)->GetValue() == Card::ACE)
    containsAce = true;

  // если в руке есть туз, а сумма достаточно низкая, то туз считается 11
  if (containsAce && total <= 11)
  { // добавляем только 10, так как мы уже добавили 1 для туза
   total += 10;
  }

  return total;
}

  ~Hand()
  {
    Clear();
  }
};

//абстрактный класс базового игрока
class GenericPlayer : public Hand
{
  friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);

public:
  GenericPlayer(const string& name = "");

  virtual ~GenericPlayer();

  // чисто виртуальная функция, возвращает информацию, нужна ли игроку
  // еще одна карта
  virtual bool IsHitting() const = 0;

  //возвращает bool значение, есть ли у игрока перебор
  bool IsBusted() const;

  //выводит на экран имя игрока и объявляет, что у него перебор
  void Bust() const;

protected:
  string m_Name;
};

GenericPlayer::GenericPlayer(const string& name): m_Name(name) {}
GenericPlayer::~GenericPlayer() {}

bool GenericPlayer::IsBusted() const
{
  return (GetTotal() > 21);
}
void GenericPlayer::Bust() const
{
  cout << m_Name << " busts.\n";
}


//------ Task 3

class Player : public GenericPlayer
{
public:

  Player(const string& name): GenericPlayer(name) {}
  virtual ~Player() {}

  //      Метод спрашивает у пользователя, нужна ли ему еще одна карта и возвращает ответ
  //      пользователя в виде true или false.
  virtual bool IsHitting() const
  {
    cout << m_Name << ", do you want a hit? (Y/N): ";
    char response;
    cin >> response;
    return (response == 'y' || response == 'Y');
  }


  //выводит на экран имя игрока и сообщение, что он выиграл
  void Win() const
  {
    cout << m_Name << " wins.\n";
  }

  //выводит на экран имя игрока и сообщение, что он проиграл
  void Lose() const
  {
    cout << m_Name << " loses.\n";
  }

  //выводит на экран имя игрока и сообщение, что он сыграл вничью.
  void Push() const
  {
    cout << m_Name << " pushes.\n";
  }
};

//------ Task 4

class House : public GenericPlayer
{
  public:

    House(const string& name = "House"): GenericPlayer( name) {}
    virtual ~House() {};

    //Метод указывает, нужна ли дилеру еще одна карта.
    //Если у дилера не больше 16 очков, то он берет еще одну карту.
    virtual bool IsHitting() const
    {
      return (GetTotal() <= 16);
    }

    //метод переворачивает первую карту дилера
    void FlipFirstCard()
    {
      if (!(m_Cards.empty()))
        m_Cards[0]->Flip();

      else
        cout << "No card to flip!\n";
    }
};

//------ Task 5

//    Написать перегрузку оператора вывода для класса Card. Если карта перевернута рубашкой
//    вверх (мы ее не видим), вывести ХХ, если мы ее видим, вывести масть и номинал карты.
//    Также для класса GenericPlayer написать перегрузку оператора вывода, который должен
//    отображать имя игрока и его карты, а также общую сумму очков его карт.

// Перегрузка оператора вывода для класса Card. Если карта перевернута рубашкой
// вверх (мы ее не видим), вывести ХХ, если мы ее видим, вывести масть и номинал карты.
ostream& operator<<(ostream& os, const Card& aCard)
{
  const string RANKS[] = {"0", "A", "2", "3", "4", "5", "6", "7", "8", "9",
                           "10", "J", "Q", "K"};

  const string SUITS[] = {"c", "d", "h", "s"};

  if (aCard.m_IsFaceUp)
      os << RANKS[aCard.m_Znashen] << SUITS[aCard.m_Mast];
  else
      os << "XX";

  return os;
}

// перегрузка оператора вывода, который должен
// отображать имя игрока и его карты, а также общую сумму очков его карт
ostream& operator <<(ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << aGenericPlayer.m_Name << ":\t";

    vector <Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin();
             pCard != aGenericPlayer.m_Cards.end(); ++pCard)
            os << *(*pCard) << "\t";

      if (aGenericPlayer.GetTotal() != 0)
          cout << "(" << aGenericPlayer.GetTotal() << ")";
    }
    else
      os << "<empty>";

  return os;
}

int main()
{
    cout << "Task 1" << endl;
//    Создать программу, которая считывает целое число типа int. И поставить «защиту от дурака»:
//    если пользователь вводит что-то кроме одного целочисленного значения, нужно вывести
//    сообщение об ошибке и предложить ввести число еще раз. Пример неправильных введенных строк:
//        rbtrb
//        nj34njkn
//        1n

    string num, temp;
    stringstream numb;
    double value;

    while (true)
    {
        cout << "Enter number:";
        cin >> num;
        numb << num;
        value=0;
        temp="";
        numb >> value >> temp;
        numb.str("");
        numb.clear();
        num.clear();
        if (value!=0 && temp==""){
            cout << "Number:" << value << endl;
            break;
        }
        cout<<"Enter number again!"<<endl;
    }



    cout << endl << "Task 2" << endl;
//    Создать собственный манипулятор endll для стандартного потока вывода, который выводит
//    два перевода строки и сбрасывает буфер.
    cout << "Hello" << endll << "World!" << endl;

    cout << endl << "Task 3" << endl;
    cout << "See code" << endl;
//    Реализовать класс Player, который наследует от класса GenericPlayer. У этого класса будет
//    4 метода:
//    - virtual bool IsHitting() const - реализация чисто виртуальной функции базового класса.
//      Метод спрашивает у пользователя, нужна ли ему еще одна карта и возвращает ответ
//      пользователя в виде true или false.
//    - void Win() const - выводит на экран имя игрока и сообщение, что он выиграл.
//    - void Lose() const - выводит на экран имя игрока и сообщение, что он проиграл.
//    - void Push() const - выводит на экран имя игрока и сообщение, что он сыграл вничью.

    cout << endl << "Task 4" << endl;
    cout << "See code" << endl;
//    Реализовать класс House, который представляет дилера. Этот класс наследует от класса
//    GenericPlayer. У него есть 2 метода:
//    - virtual bool IsHitting() const - метод указывает, нужна ли дилеру еще одна карта.
//      Если у дилера не больше 16 очков, то он берет еще одну карту.
//    - void FlipFirstCard() - метод переворачивает первую карту дилера.

    cout << endl << "Task 5" << endl;
    cout << "See code" << endl;
//    Написать перегрузку оператора вывода для класса Card. Если карта перевернута рубашкой
//    вверх (мы ее не видим), вывести ХХ, если мы ее видим, вывести масть и номинал карты.
//    Также для класса GenericPlayer написать перегрузку оператора вывода, который должен
//    отображать имя игрока и его карты, а также общую сумму очков его карт.

    return 0;
}
