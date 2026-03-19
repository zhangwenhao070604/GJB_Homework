#include <string.h>
int main() {
    char src[] = "hello";
    char dst[10];
    strcpy(dst, src);  // 违规
    return 0;
}
