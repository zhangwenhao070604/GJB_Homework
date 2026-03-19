#include <string.h>
int main() {
    char dst[20] = "Hello, ";
    char src[] = "world";
    strncat(dst, src, sizeof(dst) - strlen(dst) - 1);
    return 0;
}