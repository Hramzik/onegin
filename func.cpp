

#include "func.hpp"


int readfile_into_Text (char* file_name, Text* ptrtext)
{
    FILE*  file      = fopen  (file_name, "rb");

    if (file == nullptr) {

        fprintf (stderr, "Please, enter correct file name when starting the program\n\n");
        return (1); //код ошибки блаблабла
    }

    fseek                     (file, 0 , SEEK_END);
    (*ptrtext).buffer_len         = ftell  (file);
    fseek                     (file, 0 , SEEK_SET);

    (*ptrtext).buffer             = (char*) calloc ((*ptrtext).buffer_len + 1, CHAR_SIZE);
    if ((*ptrtext).buffer == nullptr) {
        fprintf (stderr, "Memory error\n\n");
        return    (1); //код ошибки блаблабла
    }


    fread                     ((*ptrtext).buffer, CHAR_SIZE, (*ptrtext).buffer_len, file);


    * ((*ptrtext).buffer + (*ptrtext).buffer_len) = '\0'; // null-terminator
    delete_slash_r            ((*ptrtext).buffer);


    fclose                    (file);


    return 0;
}

Text*  initialize_text (char* file_name)
{
    static Text text;


    readfile_into_Text                   (file_name, &text);
    text.num_lines =        get_num_rows (text.buffer);
    text.lines     =                     (Line*) calloc (text.num_lines + 1, LINE_SIZE);
    initialize_lines                     (&text);


    text.lines[text.num_lines].ptr              = nullptr; // null-terminator
    slash_n_to_slash_zero                       (text.buffer);


    return &text;
}

void   sort_lines_from_start (Text* ptrtext)
{
    qsort ((*ptrtext).lines, (*ptrtext).num_lines, LINE_SIZE, l_linecmp);
}

void   sort_lines_from_end   (Text* ptrtext)
{
    qsort ((*ptrtext).lines, (*ptrtext).num_lines, LINE_SIZE, r_linecmp);
}

void   sort_lines_original   (Text* ptrtext)
{
    qsort ((*ptrtext).lines, (*ptrtext).num_lines, LINE_SIZE, original_linecmp);
}

int l_linecmp (const void* first, const void* second)
{
    const Line*  first_line  = (const Line*) first;
    const Line*  second_line = (const Line*) second;

    return l_strcmp (first_line->ptr, second_line->ptr);
}

int r_linecmp (const void* first, const void* second)
{
    const Line* first_line  = (const Line*) first;
    const Line* second_line = (const Line*) second;

    return r_strcmp (first_line->ptr, second_line->ptr);
}

int    original_linecmp      (const void* first, const void* second)
{
    const Line* first_line  = (const Line*) first;
    const Line* second_line = (const Line*) second;

    if (first_line->start_index > second_line->start_index)
        return 1;

    if (first_line->start_index < second_line->start_index)
        return -1;

  //if (first_line->start_index > second_line->start_index)
        return 0;
}

void   print_lines           (Text* ptrtext)
{
    //setvbuff (stdout, nullptr, _IOFBF, 0);

    size_t ind = 0;

    while ((*ptrtext).lines[ind].ptr != nullptr) {

        printf ("%s\n", (*ptrtext).lines[ind].ptr);
        ind++;
    }

    printf ("--------------------\n");
}

void   print_lines_spaceless (Text* ptrtext)
{
    //setvbuff (stdout, nullptr, _IONBF, 0);


    size_t ind = 0;

    while ((*ptrtext).lines[ind].ptr != nullptr) {

        if ( *((*ptrtext).lines[ind].ptr) != '\0')
            printf ("%s\n", (*ptrtext).lines[ind].ptr);
        
        ind++;
    }

    printf ("--------------------\n");
}

void   fprint_lines           (Text* ptrtext, char* file_name, const char* file_mode)
{
    //check "a" or "w"
    FILE*  destination = fopen  (file_name, file_mode);
    //setvbuff (destination, nullptr, _IONBF, 0);

    size_t ind = 0;
    //setvbuff

    while ((*ptrtext).lines[ind].ptr != nullptr) {

        fprintf (destination, "%s\n", (*ptrtext).lines[ind].ptr);
        ind++;
    }

    fprintf (destination, "--------------------\n");


    fclose (destination);
}

void   fprint_lines_spaceless (Text* ptrtext, char* file_name, const char* file_mode)
{
    //check "a" or "w"
    FILE*  destination = fopen  (file_name, file_mode);


    size_t ind = 0;
    //setvbuff

    while ((*ptrtext).lines[ind].ptr != nullptr) {

        if ( *((*ptrtext).lines[ind].ptr) != '\0')
            fprintf (destination, "%s\n", (*ptrtext).lines[ind].ptr);
        
        ind++;
    }

    fprintf (destination, "--------------------\n");


    fclose (destination);
}

int    l_strcmp              (char* first, char* second)
{
    size_t ind_first  = 0;
    size_t ind_second = 0;
    while (first[ind_first] != '\0' and second[ind_second] != '\0') {

        if (!isalpha (first[ind_first])) {

            ind_first++;
            continue;
        }

        if (!isalpha (second[ind_second])) {

            ind_second++;
            continue;
        }


        if (first[ind_first] != second[ind_second])
            return (int) first[ind_first] - second[ind_second];
        ind_first ++;
        ind_second++;
    }


    return (int) first[ind_first] - second[ind_second];
}

int    r_strcmp              (char* first, char* second)
{
    size_t len_first  = strlen (first);
    size_t len_second = strlen (second);

    size_t ind_first_plus_one  = len_first;
    size_t ind_second_plus_one = len_second;

    while (ind_first_plus_one >= 1 && ind_second_plus_one >= 1) {

        if (!isalpha (first[ind_first_plus_one - 1])) {

            ind_first_plus_one--;
            continue;
        }

        if (!isalpha (second[ind_second_plus_one - 1])) {

            ind_second_plus_one--;
            continue;
        }


        if ( first[ind_first_plus_one - 1] != second[ind_second_plus_one - 1])
            return (int) first[ind_first_plus_one - 1] - second[ind_second_plus_one - 1];
        ind_first_plus_one --;
        ind_second_plus_one--;
    }

    if (ind_first_plus_one == 0 && ind_second_plus_one == 0)
        return  0;
    if (ind_first_plus_one == 0)
        return  1;
  //if (ind_second_plus_one == 0)
        return -1;

}

char*  delete_slash_r        (char* str)
{
    for (size_t read = 0, write = 0; str[read-1] != '\0'; ) {

        if (str[read] != '\r') {

            str[write] = str[read];
            read ++;
            write++;
        }

        else {

            read++;
        }
    }


    return str;
}

char*  slash_n_to_slash_zero (char* str)
{
    for (size_t ind = 0; str[ind] != '\0'; ind++) {

        if (str[ind] == '\n') { str[ind] = '\0'; }
    }


    return str;
}

size_t get_num_rows         (char* str)
{
    size_t num_rows = 1;

    for (size_t ind = 0; str[ind] != '\0'; ind++) {

        if (str[ind] == '\n') { num_rows++; }
    }


    return num_rows;
}

int  initialize_lines  (Text* ptrtext)
{
    bool   addnext                              = true;
    size_t line_ind                             = 0;

    for (size_t source_ind = 0; (*ptrtext).buffer[source_ind] != '\0'; source_ind++) {

        if (addnext) {

            (*ptrtext).lines[line_ind].ptr         = &(*ptrtext).buffer[source_ind];
            (*ptrtext).lines[line_ind].start_index =  line_ind;
            addnext                          =  false;
            line_ind                        += 1;
        }

        if ((*ptrtext).buffer[source_ind] == '\n') { addnext = true; }

    }


    return 0;
}

void cleanmemory (Text* ptrtext)
{
    free ((*ptrtext).lines);
    free ((*ptrtext).buffer);

    (*ptrtext).lines  = lines_freed;
    (*ptrtext).buffer = str_freed;
}

/*
void mysort ( void * arr, size_t n, size_t size, int ( *comparator ) ( const void*, const void* ) ) {

    ;
}*/

