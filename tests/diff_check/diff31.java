class Main {
    static void run() {
        test.a = 5;
        test.b = 0b101;
        test.c = 0x5;
        test2.print();
    }
}

class test {
    static int a;
    static int b;
    static int c;
}

class test2 {
    static double print() {
        ifj16.print("" + test.a + test.b + test.c);
        return 0x4p-2;
    }
}
