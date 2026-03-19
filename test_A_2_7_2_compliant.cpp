void foo(const int* p) {  // 加了 const
    int x = *p;
    (void)x;
}
int main() 
{
     int a = 5; 
     foo(&a); 
     return 0;
 }