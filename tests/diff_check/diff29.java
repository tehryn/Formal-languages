class Main {
    static void run(){
        int i = 10;
        double a = 0xFF.2p-2;
        double c = a * i + 1;
        foo(i, a, c);
        c = foo(i, a, c);
        c = i;
        c = 1;
    }
    static double foo(int a, double b, double c) {
        double d = a * b * c - 0b001;
        return d;
    }
}
