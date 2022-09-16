

#include "func.hpp"


int main (int argc, char* argv[]) {

    if (argc < 3) {

        fprintf (stderr, "Please, enter source and output file names when starting the program\n\n");
        exit    (0);
    }


    char*  text_str   = readtext              (argv[1]);

    line*  text_lines = initialize_structures (text_str);


    sort_lines_from_start  (text_lines);
    fprint_lines_spaceless (text_lines, argv[2], "w");

    sort_lines_from_end    (text_lines);
    fprint_lines_spaceless (text_lines, argv[2], "a");

    sort_lines_original    (text_lines);
    fprint_lines           (text_lines, argv[2], "a");


    cleanmemory (text_lines, text_str);


    return 0;
}

