

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
#define log_divider_count 18
#define log_error(_code)\
\
    Return_code code = _code;\
    FILE* log_file = fopen (log_file_name, "a");\
    setvbuf                (log_file, NULL, _IONBF, 0);\
\
    switch (code) {\
\
    case SUCCESS:\
      /*fprintf (log_file, "everything ok!\n");*/                                                                           break;\
\
    case MEMORY_ERR:\
        print_log_time();\
        fprintf (log_file, "memory error in file %s at line %d\n", __FILE__, __LINE__);                                     break;\
\
    case BAD_ARGS:\
        print_log_time();\
        fprintf (log_file, "wrong parameters given to the function in file %s at line %d\n", __FILE__, __LINE__);           break;\
\
    case FILE_ERR:\
        print_log_time();\
        fprintf (log_file, "file opening error in file %s at line %d\n", __FILE__, __LINE__);                               break;\
\
    default:\
        print_log_time();\
        fprintf (log_file, "wrong error code given to the log_error function in file %s at line %d\n", __FILE__, __LINE__); break;\
    }\
\
    fclose (log_file)


typedef struct Line_structure Line;
typedef struct Text_structure Text;


Line* const lines_freed = 0;
char* const   str_freed = 0;


enum Return_code {

    SUCCESS        = 0,
    MEMORY_ERR     = 1,
    BAD_ARGS       = 2,
    FILE_ERR       = 3,
};

struct Line_structure {

    char*   ptr = nullptr;
    size_t  len = 0;
    size_t  start_index = 0;
    bool    isblank = false;
};

struct Text_structure {

    Line*   lines = nullptr;
    size_t  num_lines = 0;
    char*   buffer = nullptr;
    size_t  buffer_len = 0;
};

Return_code readfile_into_Text     (char* file_name, Text* ptrtext);
char*       delete_slash_r         (char* str);
size_t      get_num_rows           (char* str);
char*       slash_n_to_slash_zero  (char* str);
Return_code initialize_lines       (Text* ptrtext);
Text*       initialize_text        (char* file_name);

size_t      get_lines_len          (Line* lines);

Return_code sort_lines_from_start  (Text* ptrtext);
Return_code sort_lines_from_end    (Text* ptrtext);
Return_code sort_lines_original    (Text* ptrtext);
int         _l_linecmp             (const void* first, const void* second);
int         _r_linecmp             (const void* first, const void* second);
int         _original_linecmp      (const void* first, const void* second);
int         _l_strcmp              (char* first, char* second);
int         _r_strcmp              (char* first, char* second);

Return_code print_lines            (Text* ptrtext);
Return_code print_lines_spaceless  (Text* ptrtext);
Return_code fprint_lines           (Text* ptrtext, char* file_name, const char* file_mode);
Return_code fprint_lines_spaceless (Text* ptrtext, char* file_name, const char* file_mode);

Return_code cleanmemory            (Text* ptrtext);


void        _mysort                (void* _list, size_t n, size_t size, int ( * comparator ) (const void* first, const void* second));
void        _swap                  (void* first, void* second, size_t size);
int         comp                   (const void* a, const void* b);


void        log_message            (const char* message);
void        log_start              (void);
void        log_end                (void);
void        print_log_time         (void);
char*       tm_to_str              (struct tm* time_structure);
bool        isblank                (char* str);