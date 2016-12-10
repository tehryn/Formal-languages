class Main {
    static void run() {
        ifj16.print(p.power(2,10));
    }
}

class p {
    static double power(double a, int b) {
        if (b == 0) {
            return 1;
        }
        else {
            return a * power(a, b-1);
        }
    }
}