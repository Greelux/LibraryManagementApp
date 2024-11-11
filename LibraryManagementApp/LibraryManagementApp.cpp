#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;


struct Book {                     // Структура для зберігання информації о книзі
    string title;    // Назва книги
    string author;   // Автор книги
    int year;        // Рік видання
    string genre;    // Жанр книги
    int id;          // Унікальний ідентифікатор книги
};


vector<Book> library;           // Вектор для зберігання книг


          // *Прототипи* функцій              
void addBook();                             // Додає нову книгу до бібліотеки
void deleteBook();                          // Видаляє книгу з бібліотеки за ID
void searchBook();                          // Пошук книги за назвою
void editBook();                            // Редагування інформації про книгу за ID
void displayBooks();                        // Відображення всіх книг у бібліотеці
void saveToFile();                          // Збереження даних у файл
void loadFromFile();                        // Завантаження даних із файлу
int countBooks();                           // Підрахунок загальної кількості книг
int countBooksByGenre(const string& genre); // Підрахунок кількості книг за жанром
void menu();                                // Головне меню програми

void addBook() {                 // Функція для додавання книг
    Book newBook;
    cout << "Введіть назву книги: ";
    cin.ignore(); //  очищення буферу обміну
    getline(cin, newBook.title);
    cout << "Введіть автора книги: ";
    getline(cin, newBook.author);
    cout << "Введіть рік видання книги: ";
    cin >> newBook.year;
    cout << "Введіть жанр книги: ";
    cin.ignore();
    getline(cin, newBook.genre);


    int maxId = 0;                        // унікальный ID книги
    for (const auto& book : library) {
        if (book.id > maxId) maxId = book.id;
    }
    newBook.id = maxId + 1;

    library.push_back(newBook);             // Додаємо книгу до бібліотеки
    cout << "Книжку додано!\n";
}


                                            // Функція для видалення книги по ID
void deleteBook() {
    int id;
    cout << "Введіть ID книги для видалення: ";
    cin >> id;
    auto it = remove_if(library.begin(), library.end(), [id](Book& book) {
        return book.id == id;
        });
    if (it != library.end()) {
        library.erase(it, library.end());  // Видаляємо книгу з вектора
        cout << "Книга видалена!\n";
    }
    else {
        cout << "Книгу не знайдено!\n";
    }
}

                                            // Функція для пошуку книги по назві
void searchBook() {
    string title;
    cout << "Введіть назву книги для пошуку: ";
    cin.ignore();
    getline(cin, title);
    for (const auto& book : library) {
        if (book.title == title) {
            cout << "ID: " << book.id << "\nНазва: " << book.title << "\nАвтор: " << book.author << "\nРік видання: " << book.year << "\nЖанр: " << book.genre << "\n";
            return;
        }
    }
    cout << "Книгу не знайдено!\n";
}


void editBook() {               // Функція для редагування книги по ID
    int id;
    cout << "Введіть ID книги для редагування: ";
    cin >> id;
    for (auto& book : library) {
        if (book.id == id) {
            cout << "Введіть нову назву книги: ";
            cin.ignore();
            string title;
            getline(cin, title);
            if (!title.empty()) book.title = title;

            cout << "Введіть нового автора книги: ";
            string author;
            getline(cin, author);
            if (!author.empty()) book.author = author;

            cout << "Введіть новий рік видання книги: ";
            string year_str;
            getline(cin, year_str);
            if (!year_str.empty()) book.year = stoi(year_str);

            cout << "Введіть новий жанр книги: ";
            string genre;
            getline(cin, genre);
            if (!genre.empty()) book.genre = genre;

            cout << "Книжку оновлено!\n";
            return;
        }
    }
    cout << "Книгу не знайдено!\n";
}


void displayBooks() {               // Функція для відображення усіх книг!
    cout << "\nСписок книг:\n";
    for (const auto& book : library) {
        cout << "ID: " << book.id << "\nНазва: " << book.title << "\nАвтор: " << book.author << "\nРік видання: " << book.year << "\nЖанр: " << book.genre << "\n\n";
    }
}


                                       // Функція для зберігання даних у файл
void saveToFile() {
    ofstream file("library.txt");
    if (file.is_open()) {
        for (const auto& book : library) {
            file << book.id << "|" << book.title << "|" << book.author << "|"
                << book.year << "|" << book.genre << "\n";
        }
        file.close();
        cout << "Дані збережені у файлі!\n";
    }
    else {
        cout << "Неможливо відкрити файл для запису!\n";
    }
}

                            
void loadFromFile() {                                   // Функція для завантаження даних із файлу
    ifstream file("library.txt");
    if (file.is_open()) {
        library.clear();
        string line;
        while (getline(file, line)) {
            Book book;              // Створюємо об'єкт книги для зберігання даних.
            size_t pos = 0;         // Змінна для збереження поз. роздільника "|".

            // Витягуємо ID
            pos = line.find("|");
            if (pos == string::npos) continue;  // Якщо роздільник не знайдений, пропускаємо цей рядок
            book.id = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            // Витягуємо название
            pos = line.find("|");
            if (pos == string::npos) continue;
            book.title = line.substr(0, pos);
            line.erase(0, pos + 1);

            // Витягуємо автора
            pos = line.find("|");
            if (pos == string::npos) continue;
            book.author = line.substr(0, pos);
            line.erase(0, pos + 1);

            // Витягуємо год
            pos = line.find("|");
            if (pos == string::npos) continue;
            book.year = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            book.genre = line;

            library.push_back(book);
        }
        file.close();
        cout << "Дані завантажені із файлу!\n";
    }
    else {
        cout << "Неможливо відкрити файл для читання!\n";
    }
}


            // Функція для підрахунку загальної кількості книг
int countBooks() {
    return library.size();
}

            // Функція для підрахунку кількості книг за жанром
int countBooksByGenre(const string& genre) {
    return count_if(library.begin(), library.end(), [&genre](Book& book) { // перевіряє, чи відповідає жанр книги з жанром 
        return book.genre == genre;
        });
}

            // Головне меню програми
void menu() {
    int choice;
    do {
        cout << "\nМеню:\n"
            << "1. Додати книгу\n"
            << "2. Видалити книгу\n"
            << "3. Знайти книгу\n"
            << "4. Редагувати книгу\n"
            << "5. Відобразити всі книги\n"
            << "6. Зберегти дані у файл\n"
            << "7. Завантажити дані з файлу\n"
            << "8. Показати кількість книг\n"
            << "9. Показати кількість книг по жанру\n"
            << "0. Вийти\n"
            << "Ваш вибір:";
        cin >> choice;

        switch (choice) {
        case 1: addBook(); break;
        case 2: deleteBook(); break;
        case 3: searchBook(); break;
        case 4: editBook(); break;
        case 5: displayBooks(); break;
        case 6: saveToFile(); break;
        case 7: loadFromFile(); break;
        case 8: cout << "Загальна кількість книг: " << countBooks() << endl; break;
        case 9: {
            cout << "Введіть жанр: ";
            cin.ignore();
            string genre;
            getline(cin, genre);
            cout << "Кількість книг у жанрі \"" << genre << "\": " << countBooksByGenre(genre) << endl;
            break;
        }
        case 0:
            cout << "Вихід із програми...\n";
            break;
        default:
            cout << "Неправильний вибір. Спробуйте ще раз.\n";
            break;
        }
    } while (choice != 0);
}

           // Основна функция
int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    menu();  // Запуск функції меню

    return 0;
}
