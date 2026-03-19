#include <string.h>
int main() {
    char src[] = "hello";
    char dst[10];
    strncpy(dst, src, sizeof(dst) - 1);
    dst[sizeof(dst) - 1] = '\0';
    return 0;
}