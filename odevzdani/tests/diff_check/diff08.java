class Main {
    static void run(){
        ifj16.print("2 + 4 = " + (add(1, 1) + add(2, 2)));
    }
    static int add(int a, int b) {
        int c = a + a;
        return c - a + b;
    }
}
