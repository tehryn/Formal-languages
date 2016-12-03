class Main {
    static void run(){
        int a = 10;
        int fact = 1;
        while (a > 1) {
            fact = fact * a;
            a = a - 1;
        }
        ifj16.print(fact);
    }
}
