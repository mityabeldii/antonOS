// createed by dmytro beldii, 2018
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char x[128][10];

int compare(const void *a, const void *b)
{
    char *x = (char *)a;
    char *y = (char *)b;
    for (int i = 0; i < 10; i++)
    {
        if (x[i] < y[i])
        {
            return -1;
        }
        else
        {
            if (x[i] == y[i])
            {
                continue;
            }
            else
            {
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    int n = 128;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%s", x[i]);
        char temp[10] = {x[i][6], x[i][7], x[i][8], x[i][9], '/', x[i][3], x[i][4], '/', x[i][0], x[i][1]};
        for (int j = 0; j < 10; j++)
        {
            x[i][j] = temp[j];
        }
        // printf("%s\n", x[i]);
        // printf("%s\n", x[0]);
    }
    qsort(x, n, 8 * sizeof(char), compare);
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", x[i]);
    }
    return 0;
}