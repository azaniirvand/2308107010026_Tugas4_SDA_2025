#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stddef.h> // Untuk definisi NULL yang paling standar

// --- Pembangkitan Angka ---
void generate_random_numbers(const char *filename, int count, int max_value) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error membuka file untuk angka"); // Pesan error jika file gagal dibuka
        return;
    }
    printf("Membuat %d angka acak hingga %d ke dalam %s...\n", count, max_value, filename);
    // Tidak perlu seed srand lagi jika dipanggil berdekatan dengan pembangkitan kata
    for (int i = 0; i < count; i++) {
        int num = rand() % max_value;
        fprintf(fp, "%d\n", num);
    }
    fclose(fp);
    printf("Pembuatan angka selesai.\n");
}

// --- Pembangkitan Kata ---
static const char charset[] = "abcdefghijklmnopqrstuvwxyz"; // Karakter yang mungkin untuk kata
const int MIN_WORD_LENGTH = 3; // Panjang kata minimum

// Membuat kata acak dengan panjang tertentu
void random_word(char *word, int length) {
    if (length < MIN_WORD_LENGTH) length = MIN_WORD_LENGTH; // Pastikan panjang minimum
    for (int i = 0; i < length; i++) {
        int key = rand() % (sizeof(charset) - 1); // sizeof menyertakan '\0', jadi kurangi 1
        word[i] = charset[key];
    }
    word[length] = '\0'; // Tambahkan null terminator di akhir string
}

// Membuat sejumlah kata acak dan menyimpannya ke file
void generate_random_words(const char *filename, int count, int max_word_length) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error membuka file untuk kata"); // Pesan error jika file gagal dibuka
        return;
    }
    printf("Membuat %d kata acak (panjang %d-%d) ke dalam %s...\n", count, MIN_WORD_LENGTH, max_word_length, filename);

    // Alokasikan buffer kata dengan ukuran yang cukup + 1 untuk null terminator
    char *word = (char *)malloc((max_word_length + 1) * sizeof(char));
    if (!word) {
         perror("Gagal alokasi memori untuk buffer kata");
         fclose(fp);
         return;
    }


    for (int i = 0; i < count; i++) {
        // Buat panjang acak antara MIN_WORD_LENGTH dan max_word_length
        int length = (rand() % (max_word_length - MIN_WORD_LENGTH + 1)) + MIN_WORD_LENGTH;
        random_word(word, length); // Buat kata acaknya
        fprintf(fp, "%s\n", word); // Tulis kata ke file
    }
    
    free(word); // Bebaskan memori buffer kata
    fclose(fp);
    printf("Pembuatan kata selesai.\n");
}

int main() {
    // Inisialisasi seed (benih) untuk generator angka acak *sekali saja*
    srand(time(NULL));

    int data_count = 2000000; // Dua juta sesuai deskripsi teks tugas
    int max_num_value = 2000000; // Nilai maksimum untuk angka acak
    int max_word_len = 20; // Panjang maksimum untuk kata acak

    // Panggil fungsi untuk membuat file data
    generate_random_numbers("data_angka.txt", data_count, max_num_value);
    generate_random_words("data_kata.txt", data_count, max_word_len);

    return 0;
}