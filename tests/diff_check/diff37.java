class Main {
    static void run() {
        int a = 5;
        int b = 3;
        double c = 6.12;
        boolean test = a < b && a < c;
        if (test) {
            ifj16.print("true\n");
        }
        else if (test == false){
            ifj16.print("false\n");
        }
        test = test || true;
        if (test) {
            ifj16.print("true\n");
        }
    }
}
/*
if (!false) // its funny becouse its true

 */