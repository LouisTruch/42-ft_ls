#include "../inc/ft_ls.h"

static int cmp_alpha_asc(const t_metadata *file1, const t_metadata *file2)
{
    // Ignore first . in filename by +1ing the pointer
    char *cmp1 = !ft_strncmp(file1->name, ".", 1) ? file1->name + 1 : file1->name;
    char *cmp2 = !ft_strncmp(file2->name, ".", 1) ? file2->name + 1 : file2->name;

    // If one file is ".", compare original file name
    if (!ft_strlen(cmp1) || !ft_strlen(cmp2))
        return ft_strcasecmp(file1->name, file2->name);
    return ft_strcasecmp(cmp1, cmp2);
}

static int cmp_alpha_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_alpha_asc(file1, file2));
}

static int cmp_chrono_last_modif_asc(const t_metadata *file1, const t_metadata *file2)
{
    if (!(file2->last_modif - file1->last_modif))
        return cmp_alpha_asc(file1, file2);
    return file2->last_modif - file1->last_modif;
}

static int cmp_chrono_last_modif_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_chrono_last_modif_asc(file1, file2));
}

static int cmp_chrono_last_access_asc(const t_metadata *file1, const t_metadata *file2)
{
    if (!(file2->last_access - file1->last_access))
        return cmp_alpha_asc(file1, file2);
    return file2->last_access - file1->last_access;
}

static int cmp_chrono_last_access_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_chrono_last_access_asc(file1, file2));
}

static int cmp_size_asc(const t_metadata *file1, const t_metadata *file2)
{
    if (!(file2->size - file1->size))
        return cmp_alpha_asc(file1, file2);
    return file2->size - file1->size;
}

static int cmp_size_dsc(const t_metadata *file1, const t_metadata *file2)
{
    return -(cmp_size_asc(file1, file2));
}

cmp_func choose_sort_func(e_sort_option sort_by, bool reverse)
{
    switch (sort_by)
    {
    case SORT_ALPHABETICAL:
        return reverse ? cmp_alpha_dsc : cmp_alpha_asc;
    case SORT_LAST_MODIF:
        return reverse ? cmp_chrono_last_modif_dsc : cmp_chrono_last_modif_asc;
    case SORT_LAST_ACCESS:
        return reverse ? cmp_chrono_last_access_dsc : cmp_chrono_last_access_asc;
    case SORT_SIZE:
        return reverse ? cmp_size_dsc : cmp_size_asc;
    default:
        return NULL;
    }
}