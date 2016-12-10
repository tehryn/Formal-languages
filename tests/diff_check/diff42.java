class Main {
    static void run() {
        boolean test = (4 < 5 || 9 > 8);
        int a = 0;
        while(test) {
            ifj16.print("true\n");
            test = (1 < 2 && 8 < 3) || a < 2;
            a = a + 1;
        }
    }
}
