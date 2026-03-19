int main() {
    long l = 100000;
    short s = (short)l;  // 可能丢失数据，违规
    return s;
}