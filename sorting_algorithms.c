#include "sorting_algorithms.h" // Penting: include header-nya sendiri
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Fungsi Bantu ---

// Menukar dua nilai integer
void swapInt(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Menukar dua pointer string (char*)
void swapStr(char **str1, char **str2) {
    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}

// --- Implementasi Sorting Integer ---

void bubbleSortInt(int arr[], int n) {
    int i, j;
    int swapped; // Penanda apakah ada pertukaran dalam satu pass
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        // Loop untuk membandingkan elemen berdekatan
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swapInt(&arr[j], &arr[j + 1]);
                swapped = 1; // Tandai ada pertukaran
            }
        }
        // Jika tidak ada dua elemen yang ditukar oleh loop dalam, maka array sudah terurut
        if (swapped == 0)
            break;
    }
}

void selectionSortInt(int arr[], int n) {
    int i, j, min_idx;
    // Satu per satu memindahkan batas dari sub-array tak terurut
    for (i = 0; i < n - 1; i++) {
        // Cari elemen minimum di array tak terurut
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        // Tukar elemen minimum yang ditemukan dengan elemen pertama (jika berbeda)
        if (min_idx != i)
           swapInt(&arr[min_idx], &arr[i]);
    }
}

void insertionSortInt(int arr[], int n) {
    int i, key, j;
    // Mulai dari elemen kedua (indeks 1)
    for (i = 1; i < n; i++) {
        key = arr[i]; // Elemen yang akan disisipkan
        j = i - 1;
        // Pindahkan elemen dari arr[0..i-1] yang lebih besar dari key
        // ke satu posisi di depan posisi mereka saat ini
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key; // Sisipkan key ke posisi yang benar
    }
}

// Fungsi bantu untuk menggabungkan dua sub-array (untuk Merge Sort integer)
void mergeInt(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1; // Ukuran sub-array kiri
    int n2 = r - m;     // Ukuran sub-array kanan

    // Buat array sementara secara dinamis untuk data besar
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    if (!L || !R) {
        perror("Gagal mengalokasikan memori di mergeInt");
        // Bebaskan salah satu jika yang lain gagal
        free(L); 
        free(R);
        exit(EXIT_FAILURE); // Error Kritis, keluar dari program
    }

    // Salin data ke array sementara L[] dan R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Gabungkan array sementara kembali ke arr[l..r]
    i = 0; // Indeks awal sub-array pertama
    j = 0; // Indeks awal sub-array kedua
    k = l; // Indeks awal sub-array hasil gabungan
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Salin sisa elemen L[], jika ada
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Salin sisa elemen R[], jika ada
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L); // Bebaskan memori array sementara
    free(R); // Bebaskan memori array sementara
}

// Fungsi utama Merge Sort (rekursif)
void mergeSortInt(int arr[], int l, int r) {
    if (l < r) {
        // Cari titik tengah untuk menghindari overflow
        int m = l + (r - l) / 2;
        // Urutkan paruh pertama dan kedua
        mergeSortInt(arr, l, m);
        mergeSortInt(arr, m + 1, r);
        // Gabungkan kedua paruh yang sudah terurut
        mergeInt(arr, l, m, r);
    }
}

// Fungsi bantu partisi untuk QuickSort integer
int partitionInt(int arr[], int low, int high) {
    int pivot = arr[high]; // Pilih pivot (elemen terakhir)
    int i = (low - 1); // Indeks elemen yang lebih kecil
    for (int j = low; j <= high - 1; j++) {
        // Jika elemen saat ini lebih kecil dari pivot
        if (arr[j] < pivot) {
            i++; // Tingkatkan indeks elemen yang lebih kecil
            swapInt(&arr[i], &arr[j]); // Tukar arr[i] dan arr[j]
        }
    }
    swapInt(&arr[i + 1], &arr[high]); // Tukar arr[i+1] dengan pivot
    return (i + 1); // Kembalikan indeks partisi (posisi pivot)
}

// Fungsi utama QuickSort (rekursif)
void quickSortInt(int arr[], int low, int high) {
    if (low < high) {
        // pi adalah indeks partisi, arr[pi] sekarang berada di tempat yang benar
        int pi = partitionInt(arr, low, high);
        // Urutkan elemen sebelum dan sesudah partisi secara terpisah
        quickSortInt(arr, low, pi - 1);
        quickSortInt(arr, pi + 1, high);
    }
}

void shellSortInt(int arr[], int n) {
    // Mulai dengan gap besar, lalu kurangi gap (urutan Knuth)
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;
    }
    // Lakukan insertion sort dengan gap
    while (h >= 1) {
        for (int i = h; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= h && arr[j - h] > temp; j -= h) {
                arr[j] = arr[j - h];
            }
            arr[j] = temp;
        }
        h = h / 3; // Kurangi gap
    }
}

// --- Implementasi Sorting String ---

void bubbleSortStr(char *arr[], int n) {
    int i, j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            // Bandingkan string menggunakan strcmp
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                swapStr(&arr[j], &arr[j + 1]); // Tukar pointer string
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
}

void selectionSortStr(char *arr[], int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            // Bandingkan string menggunakan strcmp
            if (strcmp(arr[j], arr[min_idx]) < 0)
                min_idx = j;
         // Tukar pointer string jika minimum ditemukan di posisi lain
         if(min_idx != i)
            swapStr(&arr[min_idx], &arr[i]);
    }
}

void insertionSortStr(char *arr[], int n) {
    int i, j;
    char *key; // Pointer ke string yang akan disisipkan
    for (i = 1; i < n; i++) {
        key = arr[i]; // Simpan pointer
        j = i - 1;
        // Pindahkan pointer ke string yang lebih besar dari key
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j]; // Pindahkan pointer
            j = j - 1;
        }
        arr[j + 1] = key; // Tempatkan pointer key ke posisi yang benar
    }
}

// Fungsi bantu merge untuk string (menggabungkan array pointer)
void mergeStr(char *arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Buat array sementara berisi pointer char*
    char **L = (char **)malloc(n1 * sizeof(char *));
    char **R = (char **)malloc(n2 * sizeof(char *));
     if (!L || !R) {
        perror("Gagal mengalokasikan memori di mergeStr");
        free(L);
        free(R);
        exit(EXIT_FAILURE);
    }

    // Salin pointer ke array sementara
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Gabungkan pointer kembali ke arr[] berdasarkan perbandingan strcmp
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0) { // Bandingkan stringnya
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Salin sisa pointer dari L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Salin sisa pointer dari R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L); // Bebaskan array pointer sementara
    free(R); // Bebaskan array pointer sementara
}

// Fungsi utama Merge Sort untuk string
void mergeSortStr(char *arr[], int l, int r) {
     if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortStr(arr, l, m);
        mergeSortStr(arr, m + 1, r);
        mergeStr(arr, l, m, r);
    }
}

// Fungsi bantu partisi untuk QuickSort string
int partitionStr(char *arr[], int low, int high) {
    char *pivot = arr[high]; // Pivot adalah pointer ke string terakhir
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        // Bandingkan string menggunakan strcmp
        if (strcmp(arr[j], pivot) < 0) {
            i++;
            swapStr(&arr[i], &arr[j]); // Tukar pointer
        }
    }
    swapStr(&arr[i + 1], &arr[high]); // Tempatkan pivot (pointer) ke posisi benar
    return (i + 1);
}

// Fungsi utama QuickSort untuk string
void quickSortStr(char *arr[], int low, int high) {
     if (low < high) {
        int pi = partitionStr(arr, low, high);
        quickSortStr(arr, low, pi - 1);
        quickSortStr(arr, pi + 1, high);
    }
}

// Fungsi Shell Sort untuk string
void shellSortStr(char *arr[], int n) {
    // Urutan gap Knuth
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;
    }
    // Gapped insertion sort
    while (h >= 1) {
        for (int i = h; i < n; i++) {
            char *temp = arr[i]; // Simpan pointer
            int j;
            // Geser pointer ke string yang lebih besar dari temp
            for (j = i; j >= h && strcmp(arr[j - h], temp) > 0; j -= h) {
                arr[j] = arr[j - h]; // Pindahkan pointer
            }
            arr[j] = temp; // Tempatkan pointer temp ke posisi benar
        }
        h = h / 3; // Kurangi gap
    }
}