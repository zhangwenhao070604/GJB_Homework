int main() {
    long l = 100;
    short s = (short)l;  // 值在范围内，不报错（但检测器可能仍会警告，视实现而定）
    return s;
}
// 更安全的做法是避免转换，或先检查范围