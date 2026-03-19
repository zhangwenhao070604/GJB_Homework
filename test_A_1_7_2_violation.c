int foo(int a, int b) {
    return a;  // b 未使用，违规
}
int main() {
    foo(1, 2);
    return 0;
}