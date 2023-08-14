#include <stdio.h>
#include "../inc/ft_ls.h"

void parse_flags(const char argv[], bool (*flags)[5]) {
    if (argv[0] == '-') {
        for (size_t i = 1 ; i < ft_strlen(argv) ; ++i) {
            switch (argv[i]) {
                case 'l':
                (*flags)[l_flag] = true;
                break;
                case 'r':
                (*flags)[r_flag] = true;
                break;
                case 'R':
                (*flags)[R_flag] = true;
                break;
                case 'a':
                (*flags)[a_flag] = true;
                break;
                case 't':
                (*flags)[t_flag] = true;
                break;
                default:
                // Replace with dprintf on fd 2
                ft_printf("ls: invalid option -- '%c'\n", argv[i]);
                exit (EXIT_FAILURE);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    bool flags[5];

    (void) argc;
    (void) argv;
    ft_bzero(flags, 5 * sizeof(bool));
    for (int i = 1 ; i < argc ; ++i) {
        parse_flags(argv[i], &flags);
    }
    
    return (EXIT_SUCCESS);
}