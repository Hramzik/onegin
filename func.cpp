

#include "func.hpp"

char* readtext (char* file_name) {

    FILE*  file      = fopen  (file_name, "rb");

    if (file == nullptr) {

        fprintf (stderr, "Please, enter correct file name when starting the program\n\n");
        exit    (1);
    }

    fseek                     (file, 0 , SEEK_END);
    size_t file_size = ftell  (file);
    fseek                     (file, 0 , SEEK_SET);

    char*  file_content              = (char*) malloc ((file_size + 1) * CHAR_SIZE);
    if (file_content == nullptr) {
        fprintf (stderr, "Memory error\n\n");
        exit    (1);
    }

    * (file_content + file_size)     = '\0';
  //* (file_content + file_size + 1) = '\0';

    fread                     (file_content, CHAR_SIZE, file_size, file);

    delete_slash_r            (file_content);

    fclose                    (file);

    return file_content;
}

line*  initialize_structures (char* source) {

    size_t num_rows = get_num_rows (source);


    line* lines = (line*) malloc ((num_rows + 1) * LINE_SIZE);
    put_lines_into_array         (lines, source);
    lines[num_rows].ptr          = nullptr; // null-terminator


    slash_n_to_slash_zero (source);


    return lines;
}

size_t get_lines_len (line* lines) {

    size_t ind = 0;
    while (lines[ind].ptr != nullptr) { ind++; }


    return ind;
}

void   sort_lines_from_start (line* lines) {

    qsort (lines, get_lines_len (lines), LINE_SIZE, l_linecmp);
}

void   sort_lines_from_end (line* lines) {

    qsort (lines, get_lines_len (lines), LINE_SIZE, r_linecmp);
}

void   sort_lines_original   (line* lines) {

    qsort (lines, get_lines_len (lines), LINE_SIZE, original_linecmp);
}

int l_linecmp (const void* first, const void* second) {

    const line*  first_line  = (const line*) first;
    const line*  second_line = (const line*) second;

    return l_strcmp (first_line->ptr, second_line->ptr);
}

int r_linecmp (const void* first, const void* second) {

    const line* first_line  = (const line*) first;
    const line* second_line = (const line*) second;

    return r_strcmp (first_line->ptr, second_line->ptr);
}

int    original_linecmp      (const void* first, const void* second) {

    const line* first_line  = (const line*) first;
    const line* second_line = (const line*) second;

    if (first_line->start_index > second_line->start_index)
        return 1;

    if (first_line->start_index < second_line->start_index)
        return -1;

  //if (first_line->start_index > second_line->start_index)
        return 0;
}

void   print_lines           (line* lines) {

    //setvbuff (stdout, nullptr, _IOFBF, 0);


    size_t ind = 0;

    while (lines[ind].ptr != nullptr) {

        printf ("%s\n", lines[ind].ptr);
        ind++;
    }

    printf ("--------------------\n");
}

void   print_lines_spaceless (line* lines) {

    //setvbuff (stdout, nullptr, _IONBF, 0);


    size_t ind = 0;

    while (lines[ind].ptr != nullptr) {

        if ( *(lines[ind].ptr) != '\0')
            printf ("%s\n", lines[ind].ptr);
        
        ind++;
    }

    printf ("--------------------\n");
}

void   fprint_lines           (line* lines, char* file_name, const char* file_mode) {

    //check "a" or "w"
    FILE*  destination = fopen  (file_name, file_mode);
    //setvbuff (destination, nullptr, _IONBF, 0);

    size_t ind = 0;
    //setvbuff

    while (lines[ind].ptr != nullptr) {

        fprintf (destination, "%s\n", lines[ind].ptr);
        ind++;
    }

    fprintf (destination, "--------------------\n");


    fclose (destination);
}

void   fprint_lines_spaceless (line* lines, char* file_name, const char* file_mode) {

    //check "a" or "w"
    FILE*  destination = fopen  (file_name, file_mode);


    size_t ind = 0;
    //setvbuff

    while (lines[ind].ptr != nullptr) {

        if ( *(lines[ind].ptr) != '\0')
            fprintf (destination, "%s\n", lines[ind].ptr);
        
        ind++;
    }

    fprintf (destination, "--------------------\n");


    fclose (destination);
}

int    l_strcmp              (char* first, char* second) {

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

int    r_strcmp              (char* first, char* second) {

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

char*  delete_slash_r        (char* str) {

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

char*  slash_n_to_slash_zero (char* str) {

    for (size_t ind = 0; str[ind] != '\0'; ind++) {

        if (str[ind] == '\n') { str[ind] = '\0'; }
    }


    return str;
}

size_t get_num_rows         (char* str) {

    size_t num_rows = 1;

    for (size_t ind = 0; str[ind] != '\0'; ind++) {

        if (str[ind] == '\n') { num_rows++; }
    }


    return num_rows;
}

line*  put_lines_into_array  (line* lines, char* source) {

    bool   addnext                              = true;
    size_t line_ind                             = 0;

    for (size_t source_ind = 0; source[source_ind] != '\0'; source_ind++) {

        if (addnext) {

            lines[line_ind].ptr           = &source[source_ind];
            lines[line_ind].start_index =  line_ind;
            addnext                       =  false;
            line_ind                     += 1;
        }

        if (source[source_ind] == '\n') { addnext = true; }

    }


    return lines;
}

void cleanmemory (line* lines, char* source) {

    free (lines);
    free (source);
}

/*
void mysort ( void * arr, size_t n, size_t size, int ( *comparator ) ( const void*, const void* ) ) {

    ;
}*/

