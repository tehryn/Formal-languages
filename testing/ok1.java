class Main {
    static int input1;
    static int input2;
    static double input3;
    static double input4;
    static void run() {
        ifj16.print("Nacteme 2x integer\n");
        input1 = ifj16.readint();
        input2 = ifj16.readint();
        int x = proc_int.add(input1, input2);
        String str = "";
        ifj16.print(str + input1 + str +" + " + input2 +" = "+ x);
        ifj16.print(str + input1 + " * " + input2 +" = "+ proc_int.mul(input1, input2));
        str = "Co je dobre?\n";
        ifj16.print(str + input1 + " - " input2 " = " proc_int.sub(input1, input2) + " nebo " + input1 +  "- " + input2 " = " proc_int.sub(input1, input2));
        str = "\n";
        ifj16.print(str + input1 + " * " input2 + " = " + proc_int.mul(input1, input2));
        ifj16.print(str + input2 +" * "+ input1 + " = " + proc_int.mul(input1, input2));
        ifj16.print("A ted neco odporneho\n");
        x = proc_int.add(input1,proc_int.mul(input1, proc_int.div(input1, proc_int.sub(input1, input2))));
        ifj16.print("Vysledek odporne operace c. 1 je pry " + x + "\n");
        x = proc_int.foo();
        ifj16.print("Vysledek odporne operace c. 2 je pry " + x + "\n");

        ifj16.print("Nacteme 2x double\n");
        input1 = ifj16.readdouble();
        input2 = ifj16.readdouble();
        double y = proc_double.add(input3, input4);
        str = "%f + %f = %f\012";
        ifj16.print(str, input3, input4, y);
        str = "%f\t*\t%f\t=\t%f\n";
        ifj16.print(str, input3, input4, proc_double.mul(input3, input4));
        str = "Co je dobre?\n%f\t-\t%f\t=\t%f\nnebo\n%f\t-\t%f\t=\t%f\n";
        ifj16.print(str, input3, input4, proc_double.sub(input3, input4), input3, input4, proc_double.sub(input3, input4));
        str = "%f\t\\t%f\t=\t%f\n";
        ifj16.print(str, input3, input4, proc_double.mul(input3, input4));
        ifj16.print(str, input4, input3, proc_double.mul(input3, input4));
        ifj16.print("A ted neco odporneho\n");
        y = proc_double.add(input3,proc_double.mul(input3, proc_double.div(input3, proc_double.sub(input3, input4))));
        ifj16.print("Vysledek odporne operace c. 1 je pry %f\n", y);
        y = proc_double.foo();
        ifj16.print("Vysledek odporne operace c. 2 je pry %f\n", y);
        ifj16.print("A nyni seradime retezec:");
        str = "qwertyuioplkjhgfdsazxcvbnm,./;\'\'\"\\[]{}:!@#$%^&*()_+\t \n";
        ifj16.print("Na serazeni: \"%s\"\n", str);
        str = ifj16.sort(str);
        ifj16.print("Serazeno: \"%s\"\n", str);
        ifj16.print("Zadej retezec:\n");
        str = ifj16.readstring();
        str = ifj16.sort();
        ifj16.print("Serazeno: \"%s\"\n", str);
        ifj16.print("Funkce tridy proc_int byly volany dohromady %i krat", proc_int.calls);
        return;
    }
}

class proc_int {
    static int add(int a, int b) {
        int c = a + b;
        calls = 1 + calls;
        return c;
    }

    static void add2sum(int a) {
        calls = 1 + calls;
        sum = sum + a;
    }

    static int sub(int a, int b) {
        int result;
        if (a > b || a == b || a>=b) {
            result = a - b;
        }
        else {
            result = b - a;
        }
        calls = 1 + calls;
        return result;
    }

    static int mul(int a, int b) {
        calls = 1 + calls;
        return a*b;
    }

    static int div(int a, int b) {
        int result = 0;
        calls = 1 + calls;
        result = ((((a/b)/1)/1) * 1);
        return result + 1 - 1 + 1 - 1;
    }

    static int foo() {
        ifj16.print("2 * 5 + 3 * 2 - 10 / 2 + 1; = x (7)\n");
        int x = 2 * 5 + 3 * 2 - 10 / 2 + 1;
        return x;
    }

    static void print_calls() {
        ifj16.print("%i\n", calls);
    }
    static int sum = 0;
    static int calls = sum;
}
class proc_double {
    static double add(double a, double b) {
        double c = a + b;
        calls = 1 + calls;
        return c;
    }

    static void add2sum(double a) {
        calls = 1 + calls;
        sum = sum + a;
    }

    static double sub(double a, double b) {
        double result;
        if (a > b || a == b || a>=b) {
            result = a - b;
        }
        else {
            result = b - a;
        }
        calls = 1 + calls;
        return result;
    }

    static double mul(double a, double b) {
        calls = 1 + calls;
        return a*b;
    }

    static double div(double a, double b) {
        double result = 0;
        calls = 1 + calls;
        result = ((((a/b)/1)/1) * 1);
        return result + 1 - 1 + 1 - 1;
    }

    static double foo() {
        ifj16.print("2 * 5 + 3 * 2 - 10 / 2 + 1; = x (7)\n");
        double x = 2 * 5 + 3 * 2 - 10 / 2 + 1;
        return x;
    }

    static void print_calls() {
        ifj16.print("%i\n", calls);
        return;
    }
    static double sum = 0;
    static double calls = sum;
}
