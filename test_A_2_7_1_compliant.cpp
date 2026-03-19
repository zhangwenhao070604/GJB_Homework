class Foo {
public:
    void func();    // 只声明
};
void Foo::func() {} // 类外定义
int main() { Foo f; f.func(); return 0; }