/*class Main {
	static int f() {
		while(true)
			b();
		if (false)
			if (false)
				print(string);
			else
				p();
		else
			return 0;
	}
	static int b;
} */

class Main {
    static int input1;
    static int input2;
    static void run() {
        ifj16.print("Nacteme 2x integer\n");
        input1 = ifj16.readint();
        input2 = ifj16.readint();
        int x = proc_int.add(input1, input2);
        String str = "%i + %i = %i\012";
        ifj16.print(str, input1, input2, x);
        str = "%i\t*\t%i\t=\t%i\n";
        ifj16.print(str, input1, input2, proc_int.mul(input1, input2));
        str = "Co je dobre?\n%i\t-\t%i\t=\t%i\nnebo\n%i\t-\t%i\t=\t%i\n";
        ifj16.print(str, input1, input2, proc_int.sub(input1, input2), input1, input2, proc_int.sub(input1, input2));
        str = "%i\t\\t%i\t=\t%i\n";
        ifj16.print(str, input1, input2, proc_int.mul(input1, input2));
        ifj16.print(str, input2, input1, proc_int.mul(input1, input2));
        ifj16.print("A ted neco odporneho\n");
        x = proc_int.add(input1,proc_int.mul(input1, proc_int.div(input1, proc_int.sub(input1, input2))));
        ifj16.print("Vysledek odporne operace c. 1 je pry %i\n", x);
        x = proc_int.foo();
        ifj16.print("Vysledek odporne operace c. 2 je pry %i\n", x);
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
