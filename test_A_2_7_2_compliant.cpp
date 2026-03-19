void foo(const int* p) {  // ╪сак const
    int x = *p;
    (void)x;
}
int main() { int a = 5; foo(&a); return 0; }