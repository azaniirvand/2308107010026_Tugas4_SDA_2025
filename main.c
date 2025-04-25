#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting_algorithms.h" // Include deklarasi fungsi sorting

// Include header khusus Windows (hanya jika dikompilasi di Windows)
#ifdef _WIN32
#include <windows.h>
#include <psapi.h> // Untuk GetProcessMemoryInfo
#endif

#define MAX_STR_LEN 101 // Panjang string maksimum + 1 untuk null terminator
#define MAX_DATA_SIZE 2000000 // Jumlah item data maksimum yang mungkin dibaca

// --- Fungsi Pembacaan Data (Sama seperti sebelumnya) ---

// Membaca 'count' integer dari file 'filename' ke dalam array 'arr'
int read_numbers(const char *filename, int *arr, int count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return -1; // Error ditangani oleh pemanggil
    }
    int i = 0;
    while (i < count && fscanf(fp, "%d", &arr[i]) == 1) {
        i++;
    }
    fclose(fp);
    return i; // Kembalikan jumlah yang dibaca
}

// Membaca 'count' string dari file 'filename' ke dalam array pointer 'arr'
int read_strings(const char *filename, char **arr, int count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return -1; // Error ditangani oleh pemanggil
    }
    char buffer[MAX_STR_LEN]; // Buffer sementara
    int i = 0;
    while (i < count && fscanf(fp, "%100s", buffer) == 1) { // Batasi bacaan buffer
        arr[i] = strdup(buffer);
        if (!arr[i]) {
            perror("Gagal mengalokasikan memori untuk string saat membaca");
            for (int j = 0; j < i; j++) free(arr[j]);
            fclose(fp);
            return -2; // Kode error berbeda untuk alokasi gagal
        }
        i++;
    }
    fclose(fp);
    return i; // Kembalikan jumlah yang dibaca
}

// Membebaskan memori yang dialokasikan untuk array string
void free_strings(char **arr, int count) {
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        if (arr[i] != NULL) {
             free(arr[i]);
             arr[i] = NULL;
        }
    }
    // Jangan free(arr) di sini
}

// --- Fungsi Bantuan untuk Mencetak Penggunaan ---
void print_usage(const char *prog_name) {
    fprintf(stderr, "Penggunaan: %s <nama_algoritma> <tipe_data> <ukuran_data>\n", prog_name);
    fprintf(stderr, "  <nama_algoritma>: bubble | selection | insertion | merge | quick | shell\n");
    fprintf(stderr, "  <tipe_data>   : int | str\n");
    fprintf(stderr, "  <ukuran_data> : angka positif (misal: 10000, 500000, 2000000)\n");
    fprintf(stderr, "Contoh: %s merge int 500000\n", prog_name);
}

// --- Fungsi untuk Menampilkan Info Memori Puncak (Windows API) ---
#ifdef _WIN32
// Fungsi ini hanya akan dikompilasi di Windows
void print_windows_peak_memory_usage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    // ZeroMemory lebih aman jika windows.h disertakan sebelum stdlib.h/string.h
    // jika tidak, bisa gunakan memset(&pmc, 0, sizeof(pmc));
    memset(&pmc, 0, sizeof(PROCESS_MEMORY_COUNTERS_EX));
    pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX); // Set ukuran struktur

    HANDLE hProcess = GetCurrentProcess();
    if (hProcess == NULL) {
        fprintf(stderr, "Warning: Tidak bisa mendapatkan handle proses untuk info memori.\n");
        return;
    }

    // Dapatkan info memori (Cast ke PROCESS_MEMORY_COUNTERS diperlukan)
    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // pmc.PeakWorkingSetSize adalah memori fisik puncak dalam bytes
        double peak_memory_mb = (double)pmc.PeakWorkingSetSize / (1024.0 * 1024.0);
        printf("Peak Memory Usage (Windows - Working Set): %.2f MB\n", peak_memory_mb);
    } else {
        fprintf(stderr, "Warning: GetProcessMemoryInfo gagal mendapatkan info memori (Error code: %lu).\n", GetLastError());
    }
    // Handle dari GetCurrentProcess() adalah pseudo-handle, tidak perlu di-CloseHandle().
}
#else
// Fungsi dummy untuk sistem operasi lain agar kompilasi tidak gagal
void print_windows_peak_memory_usage() {
    printf("Info memori puncak (Windows API) tidak tersedia di platform ini.\n");
    printf("Gunakan Valgrind/Massif (Linux/macOS) atau tool OS spesifik lainnya.\n");
}
#endif


// --- Eksekusi Tes Utama ---
int main(int argc, char *argv[]) {
    // 1. Periksa argumen command line
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    // 2. Ambil argumen
    const char *algorithm_name = argv[1];
    const char *data_type = argv[2];
    int requested_size = atoi(argv[3]);

    // 3. Validasi argumen
    if (requested_size <= 0) {
        fprintf(stderr, "Error: Ukuran data harus angka positif.\n");
        print_usage(argv[0]);
        return 1;
    }

    int is_int_data;
    if (strcmp(data_type, "int") == 0) {
        is_int_data = 1;
    } else if (strcmp(data_type, "str") == 0) {
        is_int_data = 0;
    } else {
        fprintf(stderr, "Error: Tipe data tidak valid '%s'. Gunakan 'int' atau 'str'.\n", data_type);
        print_usage(argv[0]);
        return 1;
    }

    const char *valid_algorithms[] = {"bubble", "selection", "insertion", "merge", "quick", "shell"};
    int num_valid_algorithms = sizeof(valid_algorithms) / sizeof(valid_algorithms[0]);
    int is_valid_algorithm = 0;
    for (int i = 0; i < num_valid_algorithms; ++i) {
        if (strcmp(algorithm_name, valid_algorithms[i]) == 0) {
            is_valid_algorithm = 1;
            break;
        }
    }
    if (!is_valid_algorithm) {
        fprintf(stderr, "Error: Nama algoritma tidak valid '%s'.\n", algorithm_name);
        print_usage(argv[0]);
        return 1;
    }

    // 4. Alokasi Memori
    printf("Mengalokasikan memori awal (hingga %d item)...\n", MAX_DATA_SIZE);
    int *all_numbers = NULL;
    char **all_strings = NULL;
    int *temp_numbers = NULL;
    char **temp_strings = NULL;
    int allocation_failed = 0;

    if (is_int_data) {
        all_numbers = (int *)malloc(MAX_DATA_SIZE * sizeof(int));
        temp_numbers = (int *)malloc(MAX_DATA_SIZE * sizeof(int));
        if (!all_numbers || !temp_numbers) allocation_failed = 1;
    } else {
        all_strings = (char **)malloc(MAX_DATA_SIZE * sizeof(char *));
        temp_strings = (char **)malloc(MAX_DATA_SIZE * sizeof(char *));
        if(all_strings) for(int i=0; i<MAX_DATA_SIZE; ++i) all_strings[i] = NULL; // Inisialisasi
        if(temp_strings) for(int i=0; i<MAX_DATA_SIZE; ++i) temp_strings[i] = NULL; // Inisialisasi
        if (!all_strings || !temp_strings) allocation_failed = 1;
    }

    if (allocation_failed) {
        perror("Gagal mengalokasikan array data utama/sementara");
        free(all_numbers); free(temp_numbers);
        free(all_strings); free(temp_strings);
        return 1;
    }
     printf("Alokasi memori berhasil.\n");

    // 5. Baca Data Sesuai Tipe
    const char *data_file = is_int_data ? "data_angka.txt" : "data_kata.txt";
    int total_items_read = 0;

    printf("Membaca data dari %s (hingga %d item)...\n", data_file, MAX_DATA_SIZE);
    if (is_int_data) {
        total_items_read = read_numbers(data_file, all_numbers, MAX_DATA_SIZE);
        if(total_items_read == -1) {
            perror("Gagal membuka file angka");
        }
    } else {
        total_items_read = read_strings(data_file, all_strings, MAX_DATA_SIZE);
         if(total_items_read == -1) {
            perror("Gagal membuka file string");
        } else if (total_items_read == -2) {
            total_items_read = -1; // Anggap error pembacaan jika alokasi gagal
        }
    }

    if (total_items_read <= 0) {
        fprintf(stderr, "Error: Gagal membaca data dari %s atau file kosong.\n", data_file);
        fprintf(stderr, "Pastikan file %s ada dan program generate_data sudah dijalankan.\n", data_file);
        free(all_numbers); free(temp_numbers);
        if(all_strings) free_strings(all_strings, MAX_DATA_SIZE);
        free(all_strings); free(temp_strings);
        return 1;
    }
    printf("Membaca %d item data.\n", total_items_read);

    if (requested_size > total_items_read) {
        fprintf(stderr, "Error: Ukuran data yang diminta (%d) lebih besar dari data yang tersedia (%d).\n", requested_size, total_items_read);
        free(all_numbers); free(temp_numbers);
         if(all_strings) free_strings(all_strings, total_items_read);
        free(all_strings); free(temp_strings);
        return 1;
    }

    // 6. Salin Data ke Array Sementara
    printf("Menyalin %d item ke buffer sementara...\n", requested_size);
    if (is_int_data) {
        memcpy(temp_numbers, all_numbers, requested_size * sizeof(int));
    } else {
        memcpy(temp_strings, all_strings, requested_size * sizeof(char *));
    }

    // 7. Jalankan Algoritma Sorting yang Dipilih dan Ukur Waktu
    printf("\nMenjalankan %s Sort pada %d data %s...\n", algorithm_name, requested_size, data_type);
    clock_t start, end;
    double cpu_time_used = -1.0;

    start = clock();

    // Panggil fungsi sorting yang sesuai
    if (is_int_data) {
        if (strcmp(algorithm_name, "bubble") == 0) bubbleSortInt(temp_numbers, requested_size);
        else if (strcmp(algorithm_name, "selection") == 0) selectionSortInt(temp_numbers, requested_size);
        else if (strcmp(algorithm_name, "insertion") == 0) insertionSortInt(temp_numbers, requested_size);
        else if (strcmp(algorithm_name, "merge") == 0) mergeSortInt(temp_numbers, 0, requested_size - 1);
        else if (strcmp(algorithm_name, "quick") == 0) quickSortInt(temp_numbers, 0, requested_size - 1);
        else if (strcmp(algorithm_name, "shell") == 0) shellSortInt(temp_numbers, requested_size);
    } else { // Data String
         if (strcmp(algorithm_name, "bubble") == 0) bubbleSortStr(temp_strings, requested_size);
         else if (strcmp(algorithm_name, "selection") == 0) selectionSortStr(temp_strings, requested_size);
         else if (strcmp(algorithm_name, "insertion") == 0) insertionSortStr(temp_strings, requested_size);
         else if (strcmp(algorithm_name, "merge") == 0) mergeSortStr(temp_strings, 0, requested_size - 1);
         else if (strcmp(algorithm_name, "quick") == 0) quickSortStr(temp_strings, 0, requested_size - 1);
         else if (strcmp(algorithm_name, "shell") == 0) shellSortStr(temp_strings, requested_size);
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 8. Tampilkan Hasil Waktu
    printf("Selesai.\n");
    printf("Waktu eksekusi: %.4f detik\n", cpu_time_used);

    // 9. Pembersihan Memori
    printf("\nMembersihkan memori...\n");
    free(all_numbers);
    free(temp_numbers);
    if (all_strings) {
        free_strings(all_strings, total_items_read); // Bebaskan string asli
    }
    free(all_strings);
    free(temp_strings);
    printf("Pembersihan selesai.\n");

    // 10. Tampilkan Info Memori Puncak (jika di Windows)
    // Panggil *setelah* semua free utama agar lebih fokus pada heap sorting jika memungkinkan
    // (meskipun PeakWorkingSetSize mencakup seluruh proses)
    print_windows_peak_memory_usage();

    return 0; // Program selesai dengan sukses
}