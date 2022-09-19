

#include "func.hpp"


int main (int argc, char** argv)
{
    if (argc < 3) {

        fprintf (stderr, "Please, enter source and output file names when starting the program\n\n");
        return 0;
    }


    //setlocale (LC_ALL,   "c.utf-8");
    //setlocale (LC_CTYPE, "c.utf-8");


    Text* ptrtext = initialize_text (argv[1]);


    sort_lines_from_start  (ptrtext); //тут qsort стандартный нужен
    fprint_lines_spaceless (ptrtext, argv[2], "w");

    sort_lines_from_end    (ptrtext);
    fprint_lines_spaceless (ptrtext, argv[2], "a");

    sort_lines_original    (ptrtext);
    fprint_lines           (ptrtext, argv[2], "a");

    cleanmemory (ptrtext);


    return 0;
}

