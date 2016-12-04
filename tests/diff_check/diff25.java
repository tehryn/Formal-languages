class Main {
    static void run(){
        A.print("Hello world\n");
    }
}

class A {
    static int a = 10;
    static void print(String str) {
        ifj16.print((a - 1) + str);
        ifj16.print(a);
    }
}
