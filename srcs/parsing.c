#include "../inc/ft_ls.h"

static void handle_u_flag(t_print_opt *print)
{
    print->time_to_print = PRINT_LAST_ACCESS;
    if (print->sort_by == SORT_LAST_MODIF && OPT_IS_LIST(print->option))
        print->sort_by = SORT_LAST_ACCESS;
    else if (OPT_IS_LIST(print->option))
        print->sort_by = SORT_ALPHABETICAL;
    else
        print->sort_by = SORT_LAST_ACCESS;
}

void parse_option(char **argv, t_print_opt *print)
{
    bool u_present = false;
    for (size_t i = 1; argv[i]; i++)
    {
        for (size_t j = 1; argv[i][0] == '-' && argv[i][j]; j++)
        {
            switch (argv[i][j])
            {
            case 'l':
                print->option |= OPT_LIST;
                break;
            case 't':
                print->sort_by = SORT_LAST_MODIF;
                break;
            case 'a':
                print->option |= OPT_HIDDN;
                break;
            case 'G':
                print->option |= OPT_NO_GRPLIST;
                break;
            case 'r':
                if (!OPT_IS_FORCE(print->option))
                    print->option |= OPT_REVERS;
                break;
            case 'R':
                print->option |= PRINT_DIR_NAME;
                print->option |= OPT_RECRSV;
                break;
            case 'f':
                print->sort_by = NO_SORT;
                print->option |= OPT_FORCE;
                print->option |= OPT_HIDDN;
                RESET_BIT(print->option, OPT_COLOR);
                RESET_BIT(print->option, OPT_REVERS);
                RESET_BIT(print->option, OPT_LIST);
                break;
            case 'g':
                print->option |= OPT_NO_USERLIST;
                print->option |= OPT_LIST;
                break;
            case 'd':
                print->option |= OPT_ONLYDIR;
                RESET_BIT(print->option, PRINT_DIR_NAME);
                break;
            case 'u':
                u_present = true;
                // Can't handle -u since it looks at other flag presence and not order
                break;
            case 'z':
                // Flag for color
                if (!OPT_IS_FORCE(print->option))
                    print->option |= OPT_COLOR;
                break;
            case 'S':
                print->sort_by = SORT_SIZE;
                break;
            case 'j':
                print->option |= OPT_SHOW_ATTR_ACL;
                break;
            default:
                ft_dprintf(STDERR_FILENO, "ls: invalid option -- '%c'\n", argv[i][j]);
                exit(EXIT_INVALID_OPTION);
            }
        }
    }
    if (u_present)
        handle_u_flag(print);
}

// Remove option and binary argv[0] from argv list
void remove_flags_argv(int *argc, char **argv, t_print_opt *print)
{
    for (int argv_idx = 0; argv_idx < *argc; argv_idx++)
        argv[argv_idx] = argv[argv_idx + 1];

    (*argc)--;
    for (int argv_idx = 0; argv[argv_idx]; argv_idx++)
        if (argv[argv_idx][0] == '-' && ft_strlen(argv[argv_idx]) > 1)
        {
            for (int idx = argv_idx; idx < *argc; idx++)
                argv[idx] = argv[idx + 1];
            (*argc)--;
            argv_idx--;
        }

    if (*argc > 1)
        print->option |= PRINT_DIR_NAME;
}