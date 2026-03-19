class Foo {
public:
    void func() {}   // 在类内定义，违规
};
int main() { Foo f; f.func(); return 0; }