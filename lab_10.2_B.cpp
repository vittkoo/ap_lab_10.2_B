#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

enum Specialnist { COMPUTER_SCIENCE, INFORMATICS, MATH_AND_ECONOMICS, PHYSICS_AND_INFORMATICS, LABOR_TRAINING };

string specialnistStr[] = {
    "Комп'ютерні науки",
    "Інформатика",
    "Математика та економіка",
    "Фізика та інформатика",
    "Трудове навчання"
};

struct Student {
    string prizv;
    int kurs;
    Specialnist spec;
    int fizyka;
    int matematika;
    union {
        int programuvannia;    // для COMPUTER_SCIENCE
        int chyselni_metody;   // для INFORMATICS
        int pedahohika;        // для інших спец.
    };
};

void Create(Student* students, const int N);
void Print(const Student* students, const int N);
void Sort(Student* students, const int N);
int* IndexSort(Student* students, const int N);
void PrintIndexSorted(const Student* students, const int* index, const int N);
int BinSearch(const Student* students, const int N, const string& prizv, const Specialnist spec, const int prof_bal);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int N;
    cout << "Введіть кількість студентів: ";
    cin >> N;

    Student* students = new Student[N];
    int* index = nullptr;

    int menuItem;
    do {
        cout << "\nМеню:\n";
        cout << "1 - Введення даних\n";
        cout << "2 - Вивід таблиці студентів\n";
        cout << "3 - Фізичне впорядкування масиву\n";
        cout << "4 - Індексне впорядкування масиву\n";
        cout << "5 - Бінарний пошук студента\n";
        cout << "0 - Вихід\n";
        cout << "Виберіть дію: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            Create(students, N);
            break;
        case 2:
            Print(students, N);
            break;
        case 3:
            Sort(students, N);
            cout << "Масив фізично впорядковано.\n";
            break;
        case 4:
            delete[] index;
            index = IndexSort(students, N);
            PrintIndexSorted(students, index, N);
            break;
        case 5: {
            string prizv;
            int spec, prof_bal;
            cout << "Введіть прізвище: ";
            cin.ignore();
            getline(cin, prizv);
            cout << "Введіть спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, "
                << "2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
            cin >> spec;
            cout << "Введіть оцінку з профільного предмету: ";
            cin >> prof_bal;

            int result = BinSearch(students, N, prizv, (Specialnist)spec, prof_bal);
            if (result != -1) {
                cout << "Студент знайдений у позиції: " << result + 1 << endl;
            }
            else {
                cout << "Студента не знайдено.\n";
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "Неправильний вибір!\n";
        }
    } while (menuItem != 0);

    delete[] students;
    delete[] index;
    return 0;
}

void Create(Student* students, const int N) {
    int spec;
    for (int i = 0; i < N; i++) {
        cout << "Студент № " << i + 1 << ":\n";

        cin.ignore();
        cout << "Прізвище: ";
        getline(cin, students[i].prizv);

        cout << "Курс (1-4): ";
        cin >> students[i].kurs;

        cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, "
            << "2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
        cin >> spec;
        students[i].spec = (Specialnist)spec;

        cout << "Оцінка з фізики: ";
        cin >> students[i].fizyka;

        cout << "Оцінка з математики: ";
        cin >> students[i].matematika;

        if (students[i].spec == COMPUTER_SCIENCE) {
            cout << "Оцінка з програмування: ";
            cin >> students[i].programuvannia;
        }
        else if (students[i].spec == INFORMATICS) {
            cout << "Оцінка з чисельних методів: ";
            cin >> students[i].chyselni_metody;
        }
        else {
            cout << "Оцінка з педагогіки: ";
            cin >> students[i].pedahohika;
        }

        cout << endl;
    }
}

void Print(const Student* students, const int N) {
    cout << "====================================================================================================================" << endl;
    cout << "| №  | Прізвище       | Курс | Спеціальність            | Фізика | Математика | Програм. | Числ. мет. | Педагогіка |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < N; i++) {
        cout << "| " << setw(3) << right << i + 1 << " "
            << "| " << setw(15) << left << students[i].prizv
            << "| " << setw(4) << right << students[i].kurs << " "
            << "| " << setw(24) << left << specialnistStr[students[i].spec]
            << "| " << setw(7) << right << students[i].fizyka
            << "| " << setw(11) << right << students[i].matematika;

        if (students[i].spec == COMPUTER_SCIENCE) {
            cout << "| " << setw(9) << right << students[i].programuvannia
                << "| " << setw(11) << right << "-"
                << "| " << setw(11) << right << "-" << " |" << endl;
        }
        else if (students[i].spec == INFORMATICS) {
            cout << "| " << setw(9) << right << "-"
                << "| " << setw(11) << right << students[i].chyselni_metody
                << "| " << setw(11) << right << "-" << " |" << endl;
        }
        else {
            cout << "| " << setw(9) << right << "-"
                << "| " << setw(11) << right << "-"
                << "| " << setw(11) << right << students[i].pedahohika << " |" << endl;
        }
    }

    cout << "====================================================================================================================" << endl;
}

void Sort(Student* students, const int N) {
    sort(students, students + N, [](const Student& a, const Student& b) {
        // 1. за назвою спец.
        if (specialnistStr[a.spec] != specialnistStr[b.spec])
            return specialnistStr[a.spec] < specialnistStr[b.spec];

        // 2. за профільним предметом
        int prof_a = (a.spec == COMPUTER_SCIENCE) ? a.programuvannia :
            (a.spec == INFORMATICS) ? a.chyselni_metody : a.pedahohika;
        int prof_b = (b.spec == COMPUTER_SCIENCE) ? b.programuvannia :
            (b.spec == INFORMATICS) ? b.chyselni_metody : b.pedahohika;
        if (prof_a != prof_b)
            return prof_a < prof_b;

        // 3. за прізвищем
        return a.prizv < b.prizv;
        });
}

int* IndexSort(Student* students, const int N) {
    int* index = new int[N];
    for (int i = 0; i < N; i++)
        index[i] = i;

    sort(index, index + N, [students](int a, int b) {
        // 1. за назвою спец.
        if (specialnistStr[students[a].spec] != specialnistStr[students[b].spec])
            return specialnistStr[students[a].spec] < specialnistStr[students[b].spec];

        // 2. за профільним предметом
        int prof_a = (students[a].spec == COMPUTER_SCIENCE) ? students[a].programuvannia :
            (students[a].spec == INFORMATICS) ? students[a].chyselni_metody : students[a].pedahohika;
        int prof_b = (students[b].spec == COMPUTER_SCIENCE) ? students[b].programuvannia :
            (students[b].spec == INFORMATICS) ? students[b].chyselni_metody : students[b].pedahohika;
        if (prof_a != prof_b)
            return prof_a < prof_b;

        // 3. за прізвищем
        return students[a].prizv < students[b].prizv;
        });

    return index;
}

void PrintIndexSorted(const Student* students, const int* index, const int N) {
    cout << "====================================================================================================================" << endl;
    cout << "| №  | Прізвище       | Курс | Спеціальність            | Фізика | Математика | Програм. | Числ. мет. | Педагогіка |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < N; i++) {
        int idx = index[i];
        cout << "| " << setw(3) << right << i + 1 << " "
            << "| " << setw(15) << left << students[idx].prizv
            << "| " << setw(4) << right << students[idx].kurs << " "
            << "| " << setw(24) << left << specialnistStr[students[idx].spec]
            << "| " << setw(7) << right << students[idx].fizyka
            << "| " << setw(11) << right << students[idx].matematika;

        if (students[idx].spec == COMPUTER_SCIENCE) {
            cout << "| " << setw(9) << right << students[idx].programuvannia
                << "| " << setw(11) << right << "-"
                << "| " << setw(11) << right << "-" << " |" << endl;
        }
        else if (students[idx].spec == INFORMATICS) {
            cout << "| " << setw(9) << right << "-"
                << "| " << setw(11) << right << students[idx].chyselni_metody
                << "| " << setw(11) << right << "-" << " |" << endl;
        }
        else {
            cout << "| " << setw(9) << right << "-"
                << "| " << setw(11) << right << "-"
                << "| " << setw(11) << right << students[idx].pedahohika << " |" << endl;
        }
    }

    cout << "====================================================================================================================" << endl;
}

int BinSearch(const Student* students, const int N, const string& prizv, const Specialnist spec, const int prof_bal) {
    int left = 0, right = N - 1, mid;

    while (left <= right) {
        mid = (left + right) / 2;

        int prof_mid = (students[mid].spec == COMPUTER_SCIENCE) ? students[mid].programuvannia :
            (students[mid].spec == INFORMATICS) ? students[mid].chyselni_metody : students[mid].pedahohika;

        if (students[mid].spec == spec && prof_mid == prof_bal && students[mid].prizv == prizv) {
            return mid;
        }

        if (specialnistStr[students[mid].spec] < specialnistStr[spec] ||
            (specialnistStr[students[mid].spec] == specialnistStr[spec] && prof_mid < prof_bal) ||
            (specialnistStr[students[mid].spec] == specialnistStr[spec] && prof_mid == prof_bal && students[mid].prizv < prizv)) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}