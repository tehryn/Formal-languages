class Main {
    static void run() {
        double a = ifj16.readDouble();
        double b = ifj16.readDouble();
        double result = div(a, b);
        double result2 = a/b;
        //ifj16.print(result + "\n" + (a/b));
        ifj16.print(result + "\n" + result2 +"\n");
    }
    static double div(double a, double b) {
        return a/b;
    }
}
