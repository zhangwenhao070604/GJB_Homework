#include <string.h>
int main() {
    char dst[20] = "Hello, ";
    char src[] = "world";
    strcat(dst, src);  // 违规
    return 0;
}
