#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

enum Specialnist { COMPUTER_SCIENCE, INFORMATICS, MATH_AND_ECONOMICS, PHYSICS_AND_INFORMATICS, LABOR_TRAINING };

string specialnistStr[] = {
    "����'����� �����",
    "�����������",
    "���������� �� ��������",
    "Գ���� �� �����������",
    "������� ��������"
};

struct Student {
    string prizv;
    int kurs;
    Specialnist spec;
    int fizyka;
    int matematika;
    union {
        int programuvannia;    // ��� COMPUTER_SCIENCE
        int chyselni_metody;   // ��� INFORMATICS
        int pedahohika;        // ��� ����� ����.
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
    cout << "������ ������� ��������: ";
    cin >> N;

    Student* students = new Student[N];
    int* index = nullptr;

    int menuItem;
    do {
        cout << "\n����:\n";
        cout << "1 - �������� �����\n";
        cout << "2 - ���� ������� ��������\n";
        cout << "3 - Գ����� ������������� ������\n";
        cout << "4 - �������� ������������� ������\n";
        cout << "5 - �������� ����� ��������\n";
        cout << "0 - �����\n";
        cout << "������� ��: ";
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
            cout << "����� ������� ������������.\n";
            break;
        case 4:
            delete[] index;
            index = IndexSort(students, N);
            PrintIndexSorted(students, index, N);
            break;
        case 5: {
            string prizv;
            int spec, prof_bal;
            cout << "������ �������: ";
            cin.ignore();
            getline(cin, prizv);
            cout << "������ ������������ (0 - ����'����� �����, 1 - �����������, "
                << "2 - ���������� �� ��������, 3 - Գ���� �� �����������, 4 - ������� ��������): ";
            cin >> spec;
            cout << "������ ������ � ����������� ��������: ";
            cin >> prof_bal;

            int result = BinSearch(students, N, prizv, (Specialnist)spec, prof_bal);
            if (result != -1) {
                cout << "������� ��������� � �������: " << result + 1 << endl;
            }
            else {
                cout << "�������� �� ��������.\n";
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "������������ ����!\n";
        }
    } while (menuItem != 0);

    delete[] students;
    delete[] index;
    return 0;
}

void Create(Student* students, const int N) {
    int spec;
    for (int i = 0; i < N; i++) {
        cout << "������� � " << i + 1 << ":\n";

        cin.ignore();
        cout << "�������: ";
        getline(cin, students[i].prizv);

        cout << "���� (1-4): ";
        cin >> students[i].kurs;

        cout << "������������ (0 - ����'����� �����, 1 - �����������, "
            << "2 - ���������� �� ��������, 3 - Գ���� �� �����������, 4 - ������� ��������): ";
        cin >> spec;
        students[i].spec = (Specialnist)spec;

        cout << "������ � ������: ";
        cin >> students[i].fizyka;

        cout << "������ � ����������: ";
        cin >> students[i].matematika;

        if (students[i].spec == COMPUTER_SCIENCE) {
            cout << "������ � �������������: ";
            cin >> students[i].programuvannia;
        }
        else if (students[i].spec == INFORMATICS) {
            cout << "������ � ��������� ������: ";
            cin >> students[i].chyselni_metody;
        }
        else {
            cout << "������ � ���������: ";
            cin >> students[i].pedahohika;
        }

        cout << endl;
    }
}

void Print(const Student* students, const int N) {
    cout << "====================================================================================================================" << endl;
    cout << "| �  | �������       | ���� | ������������            | Գ���� | ���������� | �������. | ����. ���. | ��������� |" << endl;
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
        // 1. �� ������ ����.
        if (specialnistStr[a.spec] != specialnistStr[b.spec])
            return specialnistStr[a.spec] < specialnistStr[b.spec];

        // 2. �� ���������� ���������
        int prof_a = (a.spec == COMPUTER_SCIENCE) ? a.programuvannia :
            (a.spec == INFORMATICS) ? a.chyselni_metody : a.pedahohika;
        int prof_b = (b.spec == COMPUTER_SCIENCE) ? b.programuvannia :
            (b.spec == INFORMATICS) ? b.chyselni_metody : b.pedahohika;
        if (prof_a != prof_b)
            return prof_a < prof_b;

        // 3. �� ��������
        return a.prizv < b.prizv;
        });
}

int* IndexSort(Student* students, const int N) {
    int* index = new int[N];
    for (int i = 0; i < N; i++)
        index[i] = i;

    sort(index, index + N, [students](int a, int b) {
        // 1. �� ������ ����.
        if (specialnistStr[students[a].spec] != specialnistStr[students[b].spec])
            return specialnistStr[students[a].spec] < specialnistStr[students[b].spec];

        // 2. �� ���������� ���������
        int prof_a = (students[a].spec == COMPUTER_SCIENCE) ? students[a].programuvannia :
            (students[a].spec == INFORMATICS) ? students[a].chyselni_metody : students[a].pedahohika;
        int prof_b = (students[b].spec == COMPUTER_SCIENCE) ? students[b].programuvannia :
            (students[b].spec == INFORMATICS) ? students[b].chyselni_metody : students[b].pedahohika;
        if (prof_a != prof_b)
            return prof_a < prof_b;

        // 3. �� ��������
        return students[a].prizv < students[b].prizv;
        });

    return index;
}

void PrintIndexSorted(const Student* students, const int* index, const int N) {
    cout << "====================================================================================================================" << endl;
    cout << "| �  | �������       | ���� | ������������            | Գ���� | ���������� | �������. | ����. ���. | ��������� |" << endl;
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