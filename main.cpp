

#include "func.hpp"
/*
int comp (const void* a, const void* b)
{
    return *((int*) a) - *((int*) b);
}*/

int main (int argc, char** argv)
{
    if (argc < 3) {

        fprintf (stderr, "Please, enter source and output file names when starting the program\n\n");
        return 0;
    }
    if (!strcmp(argv[1], argv[2])) {

        fprintf (stderr, "Can't read from and write to a single file!\n\n");
        return 0;
    }

    log_start();


    Text* ptrtext = initialize_text (argv[1]);
    
    sort_lines_from_start  (ptrtext);
    fprint_lines_spaceless (ptrtext, argv[2], "w");

    sort_lines_from_end    (ptrtext);
    fprint_lines_spaceless (ptrtext, argv[2], "a");

    sort_lines_original    (ptrtext);
    fprint_lines           (ptrtext, argv[2], "a");


    cleanmemory (ptrtext);


    return 0;
}

