class Main {
    static int input1;
    static int input2;
    static double input3;
    static double input4;
    static void run() {
        ifj16.print("Nacteme 2x integer\n");
        input1 = ifj16.readInt();
        input2 = ifj16.readInt();
        int x = proc_int.add(input1, input2);
        String str = "";
        ifj16.print(str + input1 + str +" + " + input2 +" = "+ x);
        ifj16.print(str + input1 + " * " + input2 +" = "+ proc_int.mul(input1, input2));
        str = "Co je dobre?\n";
        
		//								   +      +																			+	   +							
		//ifj16.print(str + input1 + " - "  input2   " = " proc_int.sub(input1, input2) + " nebo " + input1 +  "- " + input2 " = "  proc_int.sub(input1, input2));
        ifj16.print(str + input1 + " - " +  input2 + " = " + proc_int.sub(input1, input2) + " nebo " + input1 +  "- " + input2 + " = " + proc_int.sub(input1, input2));	// odje torima
        
		str = "\n";
        
		//								   +
		//ifj16.print(str + input1 + " * "  input2 + " = " + proc_int.mul(input1, input2));
		ifj16.print(str + input1 + " * " + input2 + " = " + proc_int.mul(input1, input2));
        
		ifj16.print(str + input2 +" * "+ input1 + " = " + proc_int.mul(input1, input2));
        ifj16.print(str + input2 +" \\ "+ input1 + " = " + proc_int.div(input1, input2));
        ifj16.print("A ted neco odporneho\n");
        x = proc_int.add(input1,proc_int.mul(input1, proc_int.div(input1, proc_int.sub(input1, input2))));
        ifj16.print("Vysledek odporne operace c. 1 je pry " + x + "\n");
        x = proc_int.foo();
        ifj16.print("Vysledek odporne operace c. 2 je pry " + x + "\n");

        ifj16.print("Nacteme 2x double\n");
        
		// input1 a input2 jsou typu int, ne double
		//input1 = ifj16.readDouble();	
        //input2 = ifj16.readDouble();
		input1 = ifj16.readInt();
		input2 = ifj16.readInt();
        
		// input3 a input4 nejsou inicializovany
		//double y = proc_double.add(input3, input4);
		input3 = 3; input4=0.2;
		double y = proc_double.add(input3, input4);
        
		str = "HEY ";
        ifj16.print(str + input3 + " + " + input4 + " = " + y);
        str = "%f\t*\t%f\t=\t%f\n";
        ifj16.print("nasobeni: " + input3 + " * " + input4 + " = " + proc_double.mul(input3, input4));
        str = "Co je dobre?\n";
        ifj16.print(str + input3 +" - "+ input4 +" = " + proc_double.sub(input3, input4) + " nebo " + input3 + " - " + input4 + "=" + proc_double.sub(input3, input4));
        str = "%f\t\\t%f\t=\t%f\n";
        ifj16.print(input3 +" \\ "+ input4 + " = " + proc_double.div(input3, input4));
        ifj16.print(input4 +" \\ "+ input3 + " = " + proc_double.div(input4, input3));
        ifj16.print("A ted neco odporneho\n");
        y = proc_double.add(input3,proc_double.mul(input3, proc_double.div(input3, proc_double.sub(input3, input4))));
        ifj16.print("Vysledek odporne operace c. 1 je pry " + y + "\n");
        y = proc_double.foo();
        ifj16.print("Vysledek odporne operace c. 2 je pry " + y +"\n");
        ifj16.print("A nyni seradime retezec:");
        str = "qwertyuioplkjhgfdsazxcvbnm,./;\'\'\"\\[]{}:!@#$%^&*()_+\t \n";
        
		// funkce print ma pouze 1 parametr typu string, na rozdil od printf
		//ifj16.print("Na serazeni: \"%s\"\n", str);	// odje torima
		ifj16.print("Na serazeni: " +str+"\n");
        
		str = ifj16.sort(str);
        
		// funkce print ma pouze 1 parametr typu string, na rozdil od printf
		//ifj16.print("Serazeno: \"%s\"\n", str);// odje torima
		ifj16.print("Serazenoi: " +str+"\n");
        
		ifj16.print("Zadej retezec:\n");
        str = ifj16.readString();
        
		// sort se vola bez parametru, 
		//str = ifj16.sort();	// odje torima
		str = ifj16.sort(str);
        
		ifj16.print("Serazeno: \"" + str +"\"\n");
        
		
		// chyba interpretu, proc_int.calls je neinicializovana prom. (i kdyz je inic. v jine tride) 
		//ifj16.print("Funkce tridy proc_int byly volany dohromady " + proc_int.calls + " krat");
		ifj16.print("Funkce tridy proc_int byly volany dohromady " + 0 + " krat");
        
		
		return;
    }
}

class proc_int {
	
	// static promenne tridy se z nejakeho duvodu museji definovat pouze hned na zacatku, takze asi chyba interprety
	static int sum = 0;
    static int calls = 0;
    
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
        ifj16.print(calls + "\n");
    }

}
class proc_double {
	// static promenne tridy se z nejakeho duvodu museji definovat pouze hned na zacatku, takze asi chyba interprety
	static double sum = 0;
    static double calls = sum;
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
        ifj16.print(calls + "\n");
        return;
    }

}
