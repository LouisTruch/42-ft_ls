#pragma once

#include "ft_ls.h"
#include <stdbool.h>

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
} e_idx_col;

typedef enum
{
    SORT_ALPHABETICAL,
    NO_SORT,
    SORT_LAST_MODIF,
    SORT_LAST_ACCESS,
    SORT_SIZE,
} e_sort_option;

typedef enum
{
    PRINT_LAST_MODIF,
    PRINT_LAST_ACCESS
} e_time_to_print;

typedef u_int32_t t_opt;
typedef u_int32_t t_col_width;

#define MAX_WIDTH_DATE 8
#define MAX_WIDTH_INT64 21
typedef struct
{
    char perms[12];
    char color[11];
    char nlink[MAX_WIDTH_INT64];
    char size[MAX_WIDTH_INT64];
    char month[MAX_WIDTH_DATE];
    char day[MAX_WIDTH_DATE];
    char hour[MAX_WIDTH_DATE];
} t_str_file_info;

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
    t_str_file_info str_file_info;
} t_metadata;

typedef int (*cmp_func)(const t_metadata *, const t_metadata *);

typedef struct
{
    t_opt option;
    cmp_func cmp_func;
    e_sort_option sort_by;
    e_time_to_print time_to_print;
    t_col_width col_width[NCOLS];
    blkcnt_t total_blcks_alloc;
} t_print_opt;

typedef struct s_file
{
    t_metadata *metadata;
    struct s_file *next;
    struct s_file *prev;
} t_file;

typedef struct
{
    int i;
    char buff[16384];
} t_printer;

#define MIN_COLUMN_WIDTH 3 // 2 spaces + 1 character
typedef struct
{
    bool valid;
    int line_len;
    int col_arr[256];
} column_info;