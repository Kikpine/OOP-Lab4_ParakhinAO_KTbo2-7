
#include <limits>
#include <vld.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <windows.h>

using namespace std;
#define MAXLENOFNAME 99

class Library {
public:
    string authorName = "";
    string bookName = "";
    string releaseYear = "";
    string quantity = "";

    Library() {};

    ~Library() {};
};

class Container {
public:
    Container() {};

    // Операция сортировки вектора
    virtual void sortContainer() = 0;

    // Операция поиска элемента в векторе
    virtual Library* searchInContainerByKey(string& key, int startYear) = 0;

    // Операция ввода элементов в вектор
    virtual void inputContainer() = 0;

    // Операция вывода элементов вектора
    virtual void outputContainer(Library*) = 0;

    // Операция очистки элементов в векторе
    virtual void deleteContainer() = 0;

    virtual ~Container() {};
};

class auxiVector : public Container {
public:
    vector <Library> libVector;

    // Операция сортировки авторов по алфавиту и годов выпуска произведений по возрастанию
    void sortContainer() override {
        sort(libVector.begin(), libVector.end(), [](const Library& one, const Library&
            next) -> bool
            {
                if (one.authorName == next.authorName) {
                    return stoi(one.releaseYear) < stoi(next.releaseYear);
                }
                else {
                    return one.authorName < next.authorName;
                }
            });
    }

    // Операция поиска элемента в векторе
    Library* searchInContainerByKey(string& key, int startYear) {
        Library* itEnd = nullptr;
        itEnd = &libVector.back();

        Library* it = nullptr;
        it = &libVector[0];

        while (it != itEnd) {
            if ((*it).authorName == key && stoi((*it).releaseYear) >= startYear) {
                break;
            }
            it++;
        }

        if (it == itEnd) {
            if ((*it).authorName != key) {  // Случай, когда последний уникален
                it = nullptr;
                return it;
            }

            if ((*it).authorName == key && stoi((*it).releaseYear) < startYear) {  // Случай, когда последний того же имени, но не подходит год
                it = nullptr;
                return it;
            }
        }

        return it;  // Успешное нахождение как минимум одного
    }

    // Операция ввода элементов в вектор
    void inputContainer() override {
        ifstream fin("input.txt");

        int size = -1; // Размер библитеки

        if (!fin.is_open()) // Eсли файл не открыт
            throw (1);
        else {
            fin >> size;


            for (int i = 0; i < size; i++) {
                Library templib;

                fin >> templib.authorName; // Буферный ввод ненужного программе номера строки

                char text[MAXLENOFNAME];
                fin.getline(text, MAXLENOFNAME, '"');
                fin.getline(text, MAXLENOFNAME, '"');
                templib.authorName = text;

                fin.getline(text, MAXLENOFNAME, '"');
                fin.getline(text, MAXLENOFNAME, '"');
                templib.bookName = text;

                fin >> templib.releaseYear >> templib.quantity;
                this->libVector.push_back(templib);

            }
        }
        fin.close();
    }

    // Операция вывода элементов вектора
    void outputContainer(Library* it) override {

        if (it == nullptr) {
            cout << "В наличии нет книг данного автора начиная с заданного года издания." << endl;
            return;
        }

        for (auto i = it; i != nullptr && (*i).authorName == (*it).authorName; i++) {
            cout << "Название: \"" << (*i).bookName << "\", год: " << (*i).releaseYear << ", количество: " << (*i).quantity << endl;
        }
        return;
    }

    void deleteContainer() {
        this->libVector.erase(libVector.begin(), libVector.end());
    }
};

class auxiMap : public Container {
public:
        multimap<string, Library> libMap;

        // Операция сортировки ассоциативного массива по значению
        void sortContainer() override {
        // Приведение multimap к vector для sort
        vector <pair<string, Library>> temp(this->libMap.begin(), this->libMap.end());

        auto comp = [](pair<string, Library> const& one, pair<string, Library> const& next) {
            if (one.second.authorName == next.second.authorName) {
                return stoi(one.second.releaseYear) < stoi(next.second.releaseYear);
            }
            else {
                return one.second.authorName < next.second.authorName;
            }
            };

        sort(temp.begin(), temp.end(), comp);

        this->libMap.clear();
        this->libMap.insert(temp.begin(), temp.end());
    }

    // Операция поиска элемента в ассоциативном массиве по ключу
    Library* searchInContainerByKey(string& key, int startYear) override {
        Library* it = nullptr;

        auto goal = this->libMap.find(key);
        it = &(*goal).second;

        auto i = this->libMap.begin();  // Создание и перенос итератора к нужному элементу
        while (i != this->libMap.end()) {
            if ((*i).second.bookName == (it)->bookName) {
                break;
            }
            i++;
        }

        goal = this->libMap.end();
        goal--;
        auto itEnd = &(*goal).second;

        while (it != itEnd) {
            it = &(*(i)).second;

            if ((*it).authorName == key && stoi((*it).releaseYear) >= startYear) {
                break;
            }
            i++;
        }

        if (it == itEnd) {
            if ((*it).authorName != key) {  // Случай, когда последний уникален
                it = nullptr;
                return it;
            }

            if ((*it).authorName == key && stoi((*it).releaseYear) < startYear) {  // Случай, когда последний того же имени, но не подходит год
                it = nullptr;
                return it;
            }
        }

        return it;  // Успешное нахождение как минимум одного
    }

    // Операция ввода элементов в ассоциативный массив
    void inputContainer() override {
        ifstream fin("input.txt");

        int size = -1; // Размер библитеки

        if (!fin.is_open()) // Eсли файл не открыт
            throw (1);
        else {
            fin >> size;

            for (int i = 0; i < size; i++) {
                Library templib;

                fin >> templib.authorName; // Буферный ввод ненужного программе номера строки

                char text[MAXLENOFNAME];
                fin.getline(text, MAXLENOFNAME, '"');
                fin.getline(text, MAXLENOFNAME, '"');
                templib.authorName = text;

                fin.getline(text, MAXLENOFNAME, '"');
                fin.getline(text, MAXLENOFNAME, '"');
                templib.bookName = text;

                fin >> templib.releaseYear >> templib.quantity;

                this->libMap.insert(pair <string, Library>(templib.authorName, templib));
            }
        }
        fin.close();
    }

    // Операция вывода элементов ассоциативного массива
    void outputContainer(Library* it) override {
        if (it == nullptr) {
            cout << "В наличии нет книг данного автора начиная с заданного года издания." << endl;
            return;
        }

        auto i = this->libMap.begin();  // Создание и перенос итератора к нужному элементу
        while (i != this->libMap.end()) {
            if ((*i).second.bookName == (it)->bookName) {
                break;
            }
            i++;
        }

        for (; i != this->libMap.end() && (*i).second.authorName == (*it).authorName; i++) {
            cout << "Название: \"" << (*i).second.bookName << "\", год: " << (*i).second.releaseYear << ", количество: " << (*i).second.quantity << endl;
        }

        return;
    }

    void deleteContainer() {
        this->libMap.erase(libMap.begin(), libMap.end());
    }
};

// Класс - интерфейс
class Interface {
public:
    static Container* createShape(char Ch)
    {
        switch (Ch) {
        case 'V':
            return new auxiVector();
            break;
        case 'M':
            return new auxiMap();
            break;
        default: throw 2;
        }
    }
};

int main()
{
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    setlocale(LC_CTYPE, "rus");

    try {

        cout << "Введите имя автора: ";
        string author;
        getline(cin, author);

        cout << "Введите начиная с какого года издания: ";
        int startYear;
        cin >> startYear;

        cout << "Введите тип используемого контейнера STL (V - vector, M - map): ";
        char typeofcon;
        cin >> typeofcon;

        Container* con1 = nullptr;
        con1 = Interface::createShape(typeofcon);

        con1->inputContainer();

        con1->sortContainer();

        auto it = con1->searchInContainerByKey(author, startYear);

        con1->outputContainer(it);

        con1->deleteContainer();

        if (con1) delete con1;

        return 0;
    }
    catch (int Error) {
        if (Error == 1) {
            cout << endl << "Не удалось открыть файл input.txt" << endl << endl;
            return 1;
        }
        if (Error == 2) {
            cout << endl << "Неверно введен тип контейнера" << endl << endl;
            return 1;
        }
    }
}