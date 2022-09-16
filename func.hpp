
#include <locale.h>
#include <assert.h>
#include <string.h>
#include  <ctype.h>
#include  <stdio.h>
#include   <math.h>

#define CHAR_SIZE sizeof (char)
#define LINE_SIZE sizeof (line)
typedef struct line_structure line;

struct line_structure {

    char*   ptr;
    size_t  len;
    size_t  start_index;
};

char*  readtext               (char* file_name);
char*  delete_slash_r         (char* str);
size_t get_num_rows           (char* str);
char*  slash_n_to_slash_zero  (char* str);
line*  put_lines_into_array   (line* lines, char* source);
line*  initialize_structures  (char* source);

size_t get_lines_len          (line* lines);

void   sort_lines_from_start  (line* lines);
void   sort_lines_from_end    (line* lines);
void   sort_lines_original    (line* lines);
int    l_linecmp              (const void* first, const void* second);
int    r_linecmp              (const void* first, const void* second);
int    original_linecmp       (const void* first, const void* second);
int    l_strcmp               (char* first, char* second);
int    r_strcmp               (char* first, char* second);

void   print_lines            (line* lines);
void   print_lines_spaceless  (line* lines);
void   fprint_lines           (line* lines, char* file_name, const char* file_mode);
void   fprint_lines_spaceless (line* lines, char* file_name, const char* file_mode);

void   cleanmemory            (line* lines, char* source);


void   mysort                 (void* arr, size_t n, size_t size, int ( * comparator ) (void* first, void* second));