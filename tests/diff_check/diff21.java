class Main {
    static void run(){
        int a = 10;
        ifj16.print(fact(a));
    }

    static int fact(int a) {
        if (a <= 1)
            return 1;
        else
            return a * fact(a - 1);
    }
    static int a = 8;
}
