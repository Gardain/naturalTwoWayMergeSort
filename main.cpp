#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <map>
#include <vector>

void naturalTwoWayMergeSort(int *arr, int N) {
    int *R = new int[2 * N];

    for (int i = 0; i < N; i++) {
        R[i] = arr[i];
    }

    //  N1 Начальная установка
    int i, j, k, l, d, f;
    int s = 1;

    do {
        bool flag = false;
        s = 1 - s;
        // N2: Подготовка к просмотру
        if (s == 0) {
            i = 0;
            j = N - 1;
            k = N;
            l = 2 * N - 1;
        } else {
            i = N;
            j = 2 * N - 1;
            k = 0;
            l = N - 1;
        }

        d = 1;
        f = 1;


        while (true) {
            // N3
            if (R[i] <= R[j] && i != j) {
                // N4
                R[k] = R[i];
                k += d;

                // N5
                i += 1;

                if (R[i - 1] <= R[i]) {
                    continue;
                }

                // N6
                do {
                    R[k] = R[j];
                    k += d;

                    // N7
                    j -= 1;
                } while (R[j + 1] <= R[j]);
            } else if (i == j) {
                R[k] = R[i];
                // N13
                flag = true;
                break;
            } else {
                // N8
                R[k] = R[j];
                k += d;

                // N9
                j -= 1;

                if (R[j + 1] <= R[j]) {
                    continue;
                }

                // N10
                do {
                    R[k] = R[i];
                    k += d;

                    // N11
                    i += 1;
                } while (R[i - 1] <= R[i]);
            }

            // N12
            if (!flag) {
                f = 0;
                d = -d;
                std::swap(k, l);
                continue;
            }

            // N13
        }
    } while (f == 0);

    for (int m = 0; m < N; m++) {
        if (s == 1) {
            arr[m] = R[m];
        } else {
            arr[m] = R[m + N];
        }
    }

    delete[] R;
}

// Функция сортировки пузырьком
void bubbleSort(int *arr, int N) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void writeArrayToFile(const int *arr, int N, const std::string &filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    for (int i = 0; i < N; ++i) {
        outfile << arr[i] << std::endl;
    }
    outfile.close();
    std::cout << "Sorted array written to " << filename << "\n";
}

bool loadArrayFromFile(int *&arr, int &N, const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return false;
    }

    std::vector<int> values;
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int number;
        while (iss >> number) {
            values.push_back(number);
        }
    }

    infile.close();

    if (values.empty()) {
        std::cerr << "Error: File " << filename << " is empty or contains invalid data.\n";
        return false;
    }

    N = values.size();

    arr = new int[N];
    std::copy(values.begin(), values.end(), arr);

    return true;
}

bool isSorted(const int *arr, int N) {
    for (int i = 1; i < N; ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int N;
    int *arr = nullptr;

    std::map<int, std::string> files = {
        {16, "./d16.txt"},
        {100, "./d100.txt"},
        {500, "./d500.txt"},
        {1000, "./d1000.txt"},
        {5000, "./d5000.txt"}
    };

    while (true) {
        int fileChoice;
        std::cout << "Choose the number of elements in the file (16, 100, 500, 1000, 5000): ";
        std::cin >> fileChoice;

        if (files.find(fileChoice) == files.end()) {
            std::cerr << "Error: Invalid choice. Available options are 16, 100, 500, 1000, 5000.\n";
            return 1;
        }

        if (!loadArrayFromFile(arr, N, files[fileChoice])) {
            return 1;
        }

        int choice;
        std::cout << "Choose sorting method:\n";
        std::cout << "1. naturalTwoWayMergeSort\n";
        std::cout << "2. qsort\n";
        std::cout << "3. bubbleSort\n";
        std::cout << "4. exit\n";
        std::cin >> choice;

        std::chrono::time_point<std::chrono::system_clock> start, end;

        switch (choice) {
            case 1:
                start = std::chrono::high_resolution_clock::now();
                naturalTwoWayMergeSort(arr, N); // Функция, которую нужно реализовать
                break;
            case 2:
                start = std::chrono::high_resolution_clock::now();
                std::sort(arr, arr + N);
                break;
            case 3:
                start = std::chrono::high_resolution_clock::now();
                bubbleSort(arr, N);
                break;
            case 4:
                delete[] arr;
                return 0;
            default:
                std::cerr << "Error: Invalid sorting choice.\n";
                delete[] arr;
                return 1;
        }

        end = std::chrono::high_resolution_clock::now();
        long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (isSorted(arr, N)) {
            std::cout << "Array is correctly sorted.\n";
            writeArrayToFile(arr, N, "sorted_array.txt");
        } else {
            std::cerr << "Error: Array is not sorted correctly.\n";
        }

        std::cout << "Sorting time: " << duration << " microseconds\n";

        delete[] arr;
    }
}
