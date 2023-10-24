#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include "../libacl/include/sys/acl.h"
#include "../libft/libft.h"
#include "ls_types.h"

#define ERRNO_RESET 0
#define EXIT_INVALID_OPTION 1
#define NO_OPTION 0x0
#define RESET_BIT(opt, bit) (opt &= ~(bit << 0))

#define OPT_LIST 0x1
#define OPT_IS_LIST(opt) (opt & OPT_LIST)
#define OPT_RECRSV 0x2
#define OPT_IS_RECRSV(opt) (opt & OPT_RECRSV)
#define OPT_HIDDN 0x4
#define OPT_IS_HIDDEN(opt) (opt & OPT_HIDDN)
#define OPT_REVERS 0x8
#define OPT_IS_REVERS(opt) (opt & OPT_REVERS)
#define OPT_COLOR 0x10
#define OPT_IS_COLOR(opt) (opt & OPT_COLOR)
#define OPT_FORCE 0x20
#define OPT_IS_FORCE(opt) (opt & OPT_FORCE)
#define OPT_NO_USERLIST 0x40
#define OPT_IS_NO_USERLIST(opt) (opt & OPT_NO_USERLIST)
#define OPT_ONLYDIR 0x80
#define OPT_IS_ONLYDIR(opt) (opt & OPT_ONLYDIR)
#define OPT_NO_GRPLIST 0x400
#define OPT_IS_NO_GRPLIST(opt) (opt & OPT_NO_GRPLIST)
#define OPT_SHOW_ATTR_ACL 0x800
#define OPT_IS_SHOW_ATTR_ACL(opt) (opt & OPT_SHOW_ATTR_ACL)

#define PRINT_DIR_NAME 0x100
#define DO_PRINT_DIR_NAME(opt) (opt & PRINT_DIR_NAME)
#define NOTDIR 0x200
#define ISNOTDIR(opt) (opt & NOTDIR)

#define LS_SUCCESS 0
#define MALLOC_FAIL 666

#define XATTR_SIZE 10000

#define COLOR_RESET "\x1b[0m"
#define COLOR_EXEC "\x1b[1;32m"
#define COLOR_DIR "\x1b[1;34m"
#define COLOR_LINK "\x1b[1;36m"
#define COLOR_FIFO "\x1b[40;33m"
#define COLOR_SOCK "\x1b[1;35m"
#define COLOR_CHR "\x1b[40;33;1m"
#define COLOR_BROKEN "\x1b[40;31;1m"

int ls(char *argv, t_print_opt *print);

// Parsing
void parse_option(char **argv, t_print_opt *print);
void remove_flags_argv(int *argc, char **argv, t_print_opt *print);

// Linked list functions
t_file *lst_new(const char *file_name, const struct stat *sb, t_print_opt *print);
void lst_addback(t_file **lst, t_file *new);
void lst_clear(t_file **lst);
size_t lst_size(t_file *lst);

// Sort
typedef int (*cmp_func)(const t_metadata *, const t_metadata *);
void sort_lst_file(t_file **head_ref, t_print_opt *print);

// Print
void print_default_format(t_file *file_lst, t_print_opt *print);
void print_list_format(char *argv, t_file *file_lst, t_print_opt *print);

void get_cols_max_width(t_metadata *metadata, t_col_width col_width[NCOLS]);

// Color
void get_color(char *color, mode_t mode);

// Utils
void get_complete_path(char *str1, size_t len, char *str2, char *str3);
int max(int a, int b);
void lltoa_no_alloc(char *str, long long nb);
int mini_sprintf(char *buff, const char *format, ...);
void reverse_str(char *str);