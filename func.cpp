

#include "func.hpp"


Return_code readfile_into_Text (char* file_name, Text* ptrtext)
{
    assert (file_name != nullptr);
    assert (ptrtext   != nullptr);


    FILE*  file = fopen (file_name, "rb");

    if (file == nullptr) {

        log_error (FILE_ERR);
        return     FILE_ERR;
    }


    fseek                         (file, 0 , SEEK_END);
    (*ptrtext).buffer_len = ftell (file);
    fseek                         (file, 0 , SEEK_SET);


    (*ptrtext).buffer           = (char*) calloc ((*ptrtext).buffer_len + 1, CHAR_SIZE);

    if ((*ptrtext).buffer == nullptr) {
        log_error (MEMORY_ERR);
        return     MEMORY_ERR;
    }


    fread                     ((*ptrtext).buffer, CHAR_SIZE, (*ptrtext).buffer_len, file);

    if ( * ((*ptrtext).buffer + (*ptrtext).buffer_len - 1) == '\n') {

        * ((*ptrtext).buffer + (*ptrtext).buffer_len)     = '\n';
        * ((*ptrtext).buffer + (*ptrtext).buffer_len + 1) = '\0';
    }
    else {

        * ((*ptrtext).buffer + (*ptrtext).buffer_len) = '\0'; // null-terminator
    }
    delete_slash_r            ((*ptrtext).buffer);


    fclose                    (file);


    return SUCCESS;
}

Text*  initialize_text (char* file_name)
{
    assert (file_name != nullptr);


    static Text text;


    Return_code return_code = readfile_into_Text              (file_name, &text);
    if (return_code) {
        log_message ("error while reading file into text structure\n");
        return nullptr;
    }


    text.num_lines     =       get_num_rows (text.buffer);
    if (text.num_lines == 0) { log_message ("buffer error occured\n"); return nullptr; }


    text.lines     =                     (Line*) calloc (text.num_lines + 1, LINE_SIZE);

    if (text.lines == nullptr) {
        log_error (MEMORY_ERR);
        return nullptr;
    }


    initialize_lines                     (&text);
    text.lines[text.num_lines].ptr              = nullptr; // null-terminator

    slash_n_to_slash_zero                       (text.buffer);


    return &text;
}

Return_code   sort_lines_from_start (Text* ptrtext)
{
    if (ptrtext        == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }
    if (ptrtext->lines == nullptr) { log_error (BAD_ARGS); log_message ("can't sort uninitialized lines!\n");     return BAD_ARGS; }


    _mysort ((*ptrtext).lines, (*ptrtext).num_lines, LINE_SIZE, _l_linecmp);
    return SUCCESS;
}

Return_code   sort_lines_from_end   (Text* ptrtext)
{
    if (ptrtext        == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }
    if (ptrtext->lines == nullptr) { log_error (BAD_ARGS); log_message ("can't sort uninitialized lines!\n");     return BAD_ARGS; }


    qsort ((*ptrtext).lines, (*ptrtext).num_lines, LINE_SIZE, _r_linecmp);
    return SUCCESS;
}

Return_code   sort_lines_original   (Text* ptrtext)
{
    if (ptrtext        == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }
    if (ptrtext->lines == nullptr) { log_error (BAD_ARGS); log_message ("can't sort uninitialized lines!\n");     return BAD_ARGS; }


    qsort ((*ptrtext).lines, (*ptrtext).num_lines, LINE_SIZE, _original_linecmp);
    return SUCCESS;
}

int _l_linecmp (const void* first, const void* second)
{
    assert (first  != nullptr);
    assert (second != nullptr);


    const Line*  first_line  = (const Line*) first;
    const Line*  second_line = (const Line*) second;

    return _l_strcmp (first_line->ptr, second_line->ptr);
}

int _r_linecmp (const void* first, const void* second)
{
    assert (first  != nullptr);
    assert (second != nullptr);


    const Line* first_line  = (const Line*) first;
    const Line* second_line = (const Line*) second;

    return _r_strcmp (first_line->ptr, second_line->ptr);
}

int    _original_linecmp      (const void* first, const void* second)
{
    assert (first  != nullptr);
    assert (second != nullptr);


    const Line* first_line  = (const Line*) first;
    const Line* second_line = (const Line*) second;

    if (first_line->start_index > second_line->start_index)
        return 1;

    if (first_line->start_index < second_line->start_index)
        return -1;

  //if (first_line->start_index > second_line->start_index)
        return 0;
}

Return_code   print_lines           (Text* ptrtext)
{
    if (ptrtext == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }


    setvbuf (stdout, nullptr, _IOFBF, (*ptrtext).buffer_len);


    size_t ind = 0;
    while ((*ptrtext).lines[ind].ptr != nullptr) {

        printf ("%s\n", (*ptrtext).lines[ind].ptr);
        ind++;
    }

    printf ("--------------------\n");
    return SUCCESS;
}

Return_code   print_lines_spaceless (Text* ptrtext)
{
    if (ptrtext == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }


    setvbuf (stdout, nullptr, _IOFBF, (*ptrtext).buffer_len);


    size_t ind = 0;
    while ((*ptrtext).lines[ind].ptr != nullptr) {

        if ( *((*ptrtext).lines[ind].ptr) != '\0')
            printf ("%s\n", (*ptrtext).lines[ind].ptr);
        
        ind++;
    }

    printf ("--------------------\n");
    return SUCCESS;
}

Return_code   fprint_lines           (Text* ptrtext, char* file_name, const char* file_mode)
{
    if (ptrtext   == nullptr)                                { log_error (BAD_ARGS); return BAD_ARGS; }
    if (file_name == nullptr)                                { log_error (BAD_ARGS); return BAD_ARGS; }
    if (file_mode == nullptr)                                { log_error (BAD_ARGS); return BAD_ARGS; }
    if ( strcmp (file_mode, "a") && strcmp (file_mode, "w")) { log_error (BAD_ARGS); return BAD_ARGS; }


    FILE*  destination = fopen  (file_name, file_mode);
    if (destination == nullptr) { log_error (FILE_ERR); return FILE_ERR; }


    setvbuf (destination, nullptr, _IOFBF, (*ptrtext).buffer_len);


    size_t ind = 0;
    while ((*ptrtext).lines[ind].ptr != nullptr) {

        fprintf (destination, "%s\n", (*ptrtext).lines[ind].ptr);
        ind++;
    }

    fprintf (destination, "--------------------\n");


    fclose (destination);
    return SUCCESS;
}

Return_code   fprint_lines_spaceless (Text* ptrtext, char* file_name, const char* file_mode)
{
    if (ptrtext   == nullptr)                                { log_error (BAD_ARGS); return BAD_ARGS; }
    if (file_name == nullptr)                                { log_error (BAD_ARGS); return BAD_ARGS; }
    if (file_mode == nullptr)                                { log_error (BAD_ARGS); return BAD_ARGS; }
    if ( strcmp (file_mode, "a") && strcmp (file_mode, "w")) { log_error (BAD_ARGS); return BAD_ARGS; }


    FILE*  destination = fopen  (file_name, file_mode);
    if (destination == nullptr) { log_error (FILE_ERR); return FILE_ERR; }

    setvbuf (destination, nullptr, _IOFBF, (*ptrtext).buffer_len);


    size_t ind = 0;
    while ((*ptrtext).lines[ind].ptr != nullptr) {

        if ( *((*ptrtext).lines[ind].ptr) != '\0' and !isblank ((*ptrtext).lines[ind].ptr) )
            fprintf (destination, "%s\n", (*ptrtext).lines[ind].ptr);
        
        ind++;
    }


    fprintf (destination, "--------------------\n");


    fclose (destination);
    return SUCCESS;
}

int    _l_strcmp              (char* first, char* second)
{
    assert (first  != nullptr);
    assert (second != nullptr);


    size_t ind_first  = 0;
    size_t ind_second = 0;
    while ( !((first[ind_first] == '\0'  &&  second[ind_second] == '\0' ) ||
              (first[ind_first] == '\0'  &&  isalpha(second[ind_second])) ||
              (isalpha(first[ind_first]) &&  second[ind_second] == '\0' )) ) {

        if (!isalpha (first[ind_first]) && first[ind_first]     != '\0') {

            ind_first++;
            continue;
        }

        if (!isalpha (second[ind_second]) && second[ind_second] != '\0') {

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

int    _r_strcmp              (char* first, char* second)
{
    assert (first  != nullptr);
    assert (second != nullptr);


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
    assert (str != nullptr);


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
    assert (str != nullptr);


    for (size_t ind = 0; str[ind] != '\0'; ind++) {

        if (str[ind] == '\n') { str[ind] = '\0'; }
    }


    return str;
}

size_t get_num_rows         (char* str)
{
    if (str == nullptr) { log_error (BAD_ARGS); log_message ("pointer to string is leading nowhere!\n"); return 0; }


    size_t num_rows = 1;


    size_t ind = 0;
    for (; str[ind] != '\0'; ind++) {

        if (str[ind] == '\n') { num_rows++; }
    }


    if (ind >= 2) {

        if (str[ind-1] == '\n' && str[ind-2] == '\n') num_rows--;
    }


    return num_rows;
}

Return_code initialize_lines (Text* ptrtext)
{
    if (ptrtext == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }


    bool   addnext                              = true;
    size_t line_ind                             = 0;

    for (size_t source_ind = 0; (*ptrtext).buffer[source_ind] != '\0'; source_ind++) {
        
        if (addnext) {

            (*ptrtext).lines[line_ind].ptr         = &(*ptrtext).buffer[source_ind];
            (*ptrtext).lines[line_ind].start_index =  line_ind;
            (*ptrtext).lines[line_ind].isblank     =  isblank ((*ptrtext).lines[line_ind].ptr);
            addnext                                =  false;
            line_ind                              += 1;
        }

        if ((*ptrtext).buffer[source_ind] == '\n') { addnext = true; }

    }


    return SUCCESS;
}

Return_code cleanmemory (Text* ptrtext)
{
    if (ptrtext == nullptr) { log_error (BAD_ARGS); log_message ("pointer to text is leading nowhere!\n"); return BAD_ARGS; }


    free ((*ptrtext).lines);
    free ((*ptrtext).buffer);

    (*ptrtext).lines  = lines_freed;
    (*ptrtext).buffer = str_freed;


    return SUCCESS;
}

void log_message (const char* message) {

    FILE* log_file = fopen (log_file_name, "a");
    setvbuf                (log_file, NULL, _IONBF, 0);


    print_log_time();
    fprintf (log_file, "%s\n", message);


    fclose (log_file);
}


void    _mysort                 (void* _list, size_t n, size_t size, int ( * comparator ) (const void* first, const void* second))
{
    assert (_list != nullptr);
    assert (size  != 0);

    if (n <= 1) return;


    char* list = (char*) _list;


    size_t  pivot_ind = n/2; // берем pivot
    _swap   ( list + (n - 1) * size, list + pivot_ind * size, size );
    pivot_ind = n-1;


    size_t  left_ind = 0;
    size_t right_ind = n - 1;

    while (left_ind <= right_ind && left_ind  < n - 1 && right_ind > 0) {

        while ( comparator (list + left_ind * size,  list + pivot_ind * size) <= 0 && left_ind  < n - 1) left_ind++;
        while ( comparator (list + right_ind * size, list + pivot_ind * size) >= 0 && right_ind >     0) right_ind--;


        if (left_ind  < right_ind) _swap (list + left_ind * size, list + right_ind * size, size);
    }

    _swap (list + left_ind * size, list + pivot_ind * size, size);

    _mysort                 (list,                            right_ind + 1, size, comparator);
    _mysort                 (list + (right_ind + 1) * size, n-right_ind - 1, size, comparator);
}

void        _swap                   (void* first, void* second, size_t size)
{
    assert (first  != nullptr);
    assert (second != nullptr);


    void* _first = calloc (1, size); //возможная нехватка памяти


    memcpy (_first,   first,  size);
    memcpy ( first,   second, size);
    memcpy ( second, _first,  size);


    free (_first);
}


char* tm_to_str (struct tm* time_structure)
{
    assert (time_structure != nullptr);


    char  temp[time_str_len];
    for (size_t i = 0; i < time_str_len; i++) temp[i] = '\0';


    strftime (temp, time_str_len, "%d.%m.%Y %H:%M:%S: ", time_structure);


    char*      time_str;
    time_str = (char*) calloc (sizeof temp, CHAR_SIZE);
    strcpy     (time_str, temp);


    return time_str;
}

void   print_log_time         (void)
{
    FILE* log_file = fopen (log_file_name, "a");
    setvbuf                (log_file, NULL, _IONBF, 0);

    struct tm*   time_structure;
    char*        time_str       = nullptr;
    const time_t cur_time       = time (nullptr);

    time_structure = localtime   (&cur_time);
    time_str       = tm_to_str (time_structure);
    fprintf (log_file, "%s", time_str);


    free   (time_str);
    fclose (log_file);
}

bool        isblank                (char* str)
{
    size_t ind = 0;
    while (str[ind] != '\0') {

        if (isalpha (str[ind])) return false;

        ind++;
    }

    return true;
}

void        log_start              (void)
{
    FILE* log_file = fopen (log_file_name, "a+");
    setvbuf                (log_file, NULL, _IONBF, 0);


    char log_divider[] = "-------STARTING THE PROGRAM...-------\n";
    /*char     log_divider[log_divider_count + 2] = "";
    size_t i = 0;
    for ( ; i < log_divider_count; i++) log_divider[i] = '-';
    log_divider[i] = '\n';*/
    fprintf (log_file, "%s", log_divider);


    fclose (log_file);
}

void        log_end                (void)
{
    FILE* log_file = fopen (log_file_name, "r");

    fseek                         (log_file, 0 , SEEK_END);
    size_t file_len =       ftell (log_file);
    fseek                         (log_file, 0 , SEEK_SET);


    char file_content[file_len + 1] = "";
    size_t ind = 0;
    while (ind < file_len) { file_content[ind] = (char) fgetc(log_file); ind++; }



    fclose(log_file);


    bool were_errors = false;
    for (size_t i = 0; i<log_divider_count; i++) { if (file_content[file_len - log_divider_count - 5 + i] != '-') were_errors = true; printf ("%zd %c\n", i, file_content[file_len - log_divider_count - 5 + i]);}

    if (were_errors) return;


    printf ("no errors\n");
    log_file = fopen (log_file_name, "w");

    setvbuf          (log_file, NULL, _IONBF, 0);

    file_content [file_len - log_divider_count - 5] = '\0';


    fprintf (log_file, "%s", file_content);


    fclose (log_file);
}