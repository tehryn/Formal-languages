static class Main {
    static void run() { return; }
    static int a = 0;
}

static class Hello() {
    static int b = a;
    static int c = b;
    static int d = c;
}
