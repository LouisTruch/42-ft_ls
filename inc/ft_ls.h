#pragma once

#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "../libft/libft.h"

// Delete before push
#include <stdio.h>

#define ERRNO_RESET 0
// Mandatory flags
#define EXIT_INVALID_OPTION 1
#define NO_OPTION 0x0
#define RESET_BIT(opt, bit) (opt &= ~(bit << 0))

#define OPT_LIST 0x1
#define OPT_ISLIST(opt) (opt & OPT_LIST)
#define OPT_RECRSV 0x2
#define OPT_ISRECRSV(opt) (opt & OPT_RECRSV)
#define OPT_HIDDN 0x4
#define OPT_ISHIDDN(opt) (opt & OPT_HIDDN)
#define OPT_REVERS 0x8
#define OPT_ISREVERS(opt) (opt & OPT_REVERS)
#define OPT_COLOR 0x10
#define OPT_ISCOLOR(opt) (opt & OPT_COLOR)
#define OPT_FORCE 0x20
#define OPT_ISFORCE(opt) (opt & OPT_FORCE)
#define OPT_GRPONLY 0x40
#define OPT_ISGRPONLY(opt) (opt & OPT_GRPONLY)
#define OPT_ONLYDIR 0x80
#define OPT_ISONLYDIR(opt) (opt & OPT_ONLYDIR)

#define PRINT_DIR_NAME 0x100
#define DO_PRINT_DIR_NAME(opt) (opt & PRINT_DIR_NAME)
#define NOTDIR 0x200
#define ISNOTDIR(opt) (opt & NOTDIR)

#define LS_SUCCESS 0
#define MALLOC_FAIL 666

#define COLOR_RESET "\x1b[0m"
#define COLOR_EXEC "\x1b[1;32m"
#define COLOR_DIR "\x1b[1;34m"
#define COLOR_LINK "\x1b[1;36m"
#define COLOR_FIFO "\x1b[40;33m"
#define COLOR_SOCK "\x1b[1;35m"
#define COLOR_CHR "\x1b[40;33;1m"

#define MIN_COLUMN_WIDTH 3 // 2 spaces + 1 character

#define _6MONTHS_IN_SECONDS 15780000
#define HOUR_WIDTH 5
#define YEAR_WIDTH 4
#define NCOLS 8
typedef enum
{
    PERMS,
    NB_LINKS,
    OWNER,
    GROUP,
    SIZE,
    MONTH,
    DAY,
    HOUR
} e_idx_col_info;

typedef enum
{
    ALPHABETICAL,
    NO_SORT,
    CHRONOLOGICAL,
} e_sort_option;

typedef u_int32_t opt;

typedef struct
{
    opt option;
    e_sort_option sort_by;
} t_print_opt;

typedef struct s_metadata
{
    char *name;
    nlink_t nlink;
    uid_t owner;
    gid_t group;
    mode_t mode;
    off_t size;
    blkcnt_t blocks;
    time_t last_modif;
    time_t last_access;
    char color[11];
} t_metadata;

typedef struct s_file
{
    t_metadata *metadata;
    struct s_file *next;
    struct s_file *prev;
} t_file;

typedef struct
{
    bool valid;
    int line_len;
    int col_arr[256];
} column_info;

int ls(char *argv, t_print_opt *print);
void handle_recursive(char *argv, t_file *lst_file, t_print_opt *print);

// Parsing
void parse_option(char **argv, t_print_opt *print);
void remove_flags_argv(int *argc, char **argv, t_print_opt *print);

t_file *lst_new(const char *file_name, struct stat *sb, bool option_color);
void lst_addback(t_file **lst, t_file *new);
void lst_clear(t_file **lst);
size_t lst_size(t_file *lst);

typedef int (*cmp_func)(const t_metadata *, const t_metadata *);
void sort_lst_file(t_file **head_ref, t_print_opt *print);

void print_ls(char *argv, t_file *lst_file, t_print_opt *print);
void print_default_format(t_file *file_lst, t_print_opt *print);
void print_list_format(char *argv, t_file *file_lst, t_print_opt *print);
void print_file_name(t_metadata *metadata, t_print_opt *print);

// Color
void get_color(char *color, mode_t mode);

// Utils
void get_complete_path(char *str1, char *str2, char *str3);
int max(int a, int b);
int mini_sprintf(char *buff, const char *format, ...);

void reverse_str(char *str);