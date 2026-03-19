void foo(int* p) {   // p 未被修改，但未加 const，违规
    int x = *p;
    (void)x;
}
int main() 
{ int a = 5; 
    foo(&a); 
    return 0; 
}