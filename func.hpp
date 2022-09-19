

#include <locale.h>
#include <assert.h>
#include <string.h>
#include  <ctype.h>
#include  <stdio.h>
#include   <math.h>
#include   <time.h>


#define CHAR_SIZE     sizeof (char)
#define LINE_SIZE     sizeof (Line)
#define log_file_name "logs.txt"
#define time_str_len  40


typedef struct Line_structure Line;
typedef struct Text_structure Text;


Line* const lines_freed = 0;
char* const   str_freed = 0;


enum Return_code {

    SUCCESS          = 0,
    MEMORY_ERROR     = 1,
    WRONG_PARAMETERS = 2,
    FILE_ERROR       = 3,
};

struct Line_structure {

    char*   ptr = nullptr;
    size_t  len = 0;
    size_t  start_index = 0;
};

struct Text_structure {

    Line*   lines = nullptr;
    size_t  num_lines = 0;
    char*   buffer = nullptr;
    size_t  buffer_len = 0;
};

Return_code readfile_into_Text     (char* file_name, Text* ptrtext);
char*  delete_slash_r         (char* str);
size_t get_num_rows           (char* str);
char*  slash_n_to_slash_zero  (char* str);
int    initialize_lines       (Text* ptrtext);
Text*  initialize_text        (char* file_name);

size_t get_lines_len          (Line* lines);

void   sort_lines_from_start  (Text* ptrtext);
void   sort_lines_from_end    (Text* ptrtext);
void   sort_lines_original    (Text* ptrtext);
int    l_linecmp              (const void* first, const void* second);
int    r_linecmp              (const void* first, const void* second);
int    original_linecmp       (const void* first, const void* second);
int    l_strcmp               (char* first, char* second);
int    r_strcmp               (char* first, char* second);

void   print_lines            (Text* ptrtext);
void   print_lines_spaceless  (Text* ptrtext);
void   fprint_lines           (Text* ptrtext, char* file_name, const char* file_mode);
void   fprint_lines_spaceless (Text* ptrtext, char* file_name, const char* file_mode);

void   cleanmemory            (Text* ptrtext);


void   mysort                 (void* arr, size_t n, size_t size, int ( * comparator ) (void* first, void* second));

void   log                    (Return_code code);
void   print_log_time         (void);
char*  tm_to_str              (struct tm* time_structure);