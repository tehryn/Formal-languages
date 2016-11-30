class Main {
    static void run() { return; }
    static int a = 0;
}

class Hello {
    static int b = Main.a;
    static int c = b;
    static int d = c;
}
