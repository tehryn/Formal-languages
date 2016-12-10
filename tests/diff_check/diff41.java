class Main {
    static void run() {
        int a = 1;
        int b = 2;
        while (a != 10) {
            while (b != 20) {
                foo(b);
                b = b + 1;
            }
            a = a + 1
        }
        ifj16.print("" + a + b + "\n");
    }
    static void foo (int a) {
        while (a != 10) {
            a = a + 1;
            foo(a);
        }
    }
}
