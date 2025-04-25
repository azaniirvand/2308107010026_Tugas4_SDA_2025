#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

// Penjaga Inklusi (Include Guard) untuk mencegah definisi ganda

// --- Fungsi Sorting Integer ---

/* Bubble Sort: Secara berulang menelusuri daftar, membandingkan elemen
 * yang berdekatan dan menukarnya jika urutannya salah. */
void bubbleSortInt(int arr[], int n);

/* Selection Sort: Membagi daftar input menjadi dua bagian: sub-daftar
 * terurut dan sub-daftar tidak terurut. */
void selectionSortInt(int arr[], int n);

/* Insertion Sort: Membangun array akhir yang terurut satu item pada satu
 * waktu. */
void insertionSortInt(int arr[], int n);

/* Merge Sort: Algoritma divide-and-conquer. Membutuhkan ruang tambahan. */
void mergeSortInt(int arr[], int l, int r); // l = indeks kiri, r = indeks kanan

/* Quick Sort: Algoritma divide-and-conquer. */
void quickSortInt(int arr[], int low, int high); // low = indeks awal, high = indeks akhir

/* Shell Sort: Optimalisasi dari insertion sort menggunakan gap sequence. */
void shellSortInt(int arr[], int n);


// --- Fungsi Sorting String ---

/* Bubble Sort untuk string menggunakan strcmp. */
void bubbleSortStr(char *arr[], int n);

/* Selection Sort untuk string menggunakan strcmp. */
void selectionSortStr(char *arr[], int n);

/* Insertion Sort untuk string menggunakan strcmp. */
void insertionSortStr(char *arr[], int n);

/* Merge Sort untuk string menggunakan strcmp. Membutuhkan ruang tambahan. */
void mergeSortStr(char *arr[], int l, int r); // l = indeks kiri, r = indeks kanan

/* Quick Sort untuk string menggunakan strcmp. */
void quickSortStr(char *arr[], int low, int high); // low = indeks awal, high = indeks akhir

/* Shell Sort untuk string menggunakan strcmp. */
void shellSortStr(char *arr[], int n);


#endif // SORTING_ALGORITHMS_H