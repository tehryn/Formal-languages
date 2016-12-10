class Main {
    static void run() {
        while (test.c) test2.run(); ifj16.print(test2.count);
    }
}

class test2 {
    static int count = 20;
    static void run() {
        if (count > 5) {
            count = count - 1;
        }
        else {
            test.c = false;
        }
    }
}

class test {
    static boolean c = true;
}