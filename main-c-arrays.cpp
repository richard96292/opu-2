#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstring>

struct studentas {
    std::string firstName, lastName;
    int *grades;
    int gradesSize = 0;
    int gradeCount = 0, examGrade = 0;
    double finalGradeAvg = 0, finalGradeMedian = 0;
};

void addGrade(int *&grades, int &gradesSize, const int newGrade) {
    int *temp = new int[gradesSize + 1];
    if (gradesSize != 0) {
        std::memcpy(temp, grades, gradesSize * sizeof(int));
        delete[] grades;
    }
    temp[gradesSize] = newGrade;
    grades = temp;
    gradesSize++;
}

void findAverage(studentas &s) {
    if (s.gradeCount == 0) {
        s.finalGradeAvg = s.examGrade * 0.6;
    } else {
        s.finalGradeAvg = s.finalGradeAvg / s.gradeCount * 0.4 + s.examGrade * 0.6;
    }
}

void findMedian(studentas &s) {
    if (s.gradeCount == 0) {
        s.finalGradeMedian = s.examGrade * 0.6;
        return;
    }
    std::sort(s.grades, s.grades + s.gradeCount);
    if (s.gradeCount % 2 == 0) {
        s.finalGradeMedian =
                ((s.grades[s.gradeCount / 2 - 1] + s.grades[s.gradeCount / 2]) / 2.0) * 0.4 + s.examGrade * 0.6;
    } else {
        s.finalGradeMedian = s.grades[s.gradeCount / 2] * 0.4 + s.examGrade * 0.6;
    }
}

void inputGrades(studentas &s) {
    for (size_t j = 0; j < s.gradeCount; j++) {
        int temp;
        std::cout << "Įveskite pažymį Nr." << j + 1 << ": ";
        // input check
        while (!(std::cin >> temp) || temp < -1 || temp > 10) {
            std::cout << "Klaidingas pažymys, bandikyte įvesti dar kartą: ";
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        addGrade(s.grades, s.gradesSize, temp);
        s.finalGradeAvg += temp;
    }
}

void randomizeGrades(studentas &s) {
    std::srand(std::time(nullptr));
    for (size_t j = 0; j < s.gradeCount; j++) {
        // generates the same numbers
        int temp = std::rand() % 11;
        addGrade(s.grades, s.gradesSize, temp);
        s.finalGradeAvg += temp;
    }
}

void inputGradesEndless(studentas &s) {
    int tempGradeSize = 0;
    // endless loop
    for (;;) {
        int temp = 0;
        std::cout << "Įveskite nauja pažymį (įveskite \"-1\", kai norite baigti įvedimą): ";
        // input check
        while (!(std::cin >> temp) || temp < -1 || temp > 10) {
            std::cout << "Klaidingas pažymys, bandikyte įvesti dar kartą: ";
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        // exit the for loop if temp == -1
        if (temp == -1) {
            break;
        }

        addGrade(s.grades, s.gradesSize, temp);
        s.finalGradeAvg += temp;
        tempGradeSize++;
    }
    s.gradeCount = tempGradeSize;
}

void input(std::vector<studentas> &s) {
    for (int i = 0;; i++) {
        s.emplace_back();
        std::cout << "Įveskite studento vardą: ";
        std::cin >> s[i].firstName;
        std::cout << "Įveskite studento pavardę: ";
        std::cin >> s[i].lastName;
        std::cout
                << "Ar žinote kiek pažymių turi studentas"
                << " (\"-1\" jeigu nežinote n)? ";
        // input check
        while (!(std::cin >> s[i].gradeCount) || s[i].gradeCount < -1) {
            std::cout << "Klaidingas kiekis, bandikyte įvesti dar kartą: ";
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // input grades
        if (s[i].gradeCount > 0) {
            std::string choice;
            std::cout
                    << "Sugeneruoti pažymius automatiškai (taip arba ne)? ";
            if (std::cin >> choice && choice == "taip") {
                randomizeGrades(s[i]);
            } else {
                inputGrades(s[i]);
            }
        }
        if (s[i].gradeCount == -1) {
            inputGradesEndless(s[i]);
        }

        std::cout << "Įveskite studento egzamino pažymį: ";
        while (!(std::cin >> s[i].examGrade) || s[i].examGrade < -1 ||
               s[i].examGrade > 10) {
            std::cout << "Klaidingas pažymys, bandikyte įvesti dar kartą: ";
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        findAverage(s[i]);
        findMedian(s[i]);

        std::cout
                << "Ar norite įvesti dar vieno studento pažymius (taip arba ne)? ";
        std::string continueInput;
        if (std::cin >> continueInput && continueInput != "taip") {
            return;
        }
    }
}

void output(const std::vector<studentas> &s) {
    std::string choice;
    std::cout << "Jei norite išvesti medianą įveskite \"m\" (vidurkis - default): ";
    if (std::cin >> choice && choice == "m") {
        std::cout << std::left << std::setw(15) << "\n\nPavarde" << std::setw(15) << "Vardas" << std::setw(15)
                  << "Galutinis (Med.)\n";
        std::cout << std::string(45, '-') << "\n";
        for (const auto &stud: s) {
            std::cout << std::left << std::setw(15) << stud.firstName
                      << std::setw(15) << stud.lastName << std::setw(15)
                      << std::fixed << std::setprecision(2) << stud.finalGradeMedian << "\n";
        }
    } else {
        std::cout << std::left << std::setw(15) << "\n\nPavarde" << std::setw(15) << "Vardas" << std::setw(15)
                  << "Galutinis (Vid.)\n";
        std::cout << std::string(45, '-') << "\n";
        for (const auto &stud: s) {
            std::cout << std::left << std::setw(15) << stud.lastName
                      << std::setw(15) << stud.firstName << std::setw(15)
                      << std::fixed << std::setprecision(2) << stud.finalGradeAvg << "\n";
        }
    }
}

int main() {
    std::vector<studentas> s;
    input(s);
    output(s);
    return 0;
}