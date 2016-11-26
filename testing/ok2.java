class Main {
    static void run() {
		ifj.printf("A1\n");
		int x;
		x=fff.read_and_compare;
		x=fff.read_and_compare2;
		ifj.printf((x-1));
		String text="Some random text\n";
		text=fff.sorting(text);
		fff.read2();
		double z;
		z=fff.foo();
//		int x;
		int y;
		x=31/3;
		y=x/2;
		if (1){
			x=fff.foo1(x,y);
		}
		else{
			x=fff.foo1(y,x);
		}
		if (x==x && (x-1)==(x-1) || x/5==1)
			ifj.printf("(Y)\n");


	}


}


class fff{
	static int s1=5;

	static int read_and_compare(){
		String text=ifj16.readstring();
		String pat=ifj16.readstring();
		return ifj.compare(text,pat);
	}
	static int read_and_compare2(){
		return read_and_compare();
	}
	static String sorting(String text){
		return ifj16.sort(text);
	}
	static void read2(){
		String a1=ifj16.readstring();
		a1=ifj16.readstring();
	}
	static double foo(){
		a2=ifj.readdouble();
		return a2/ifj.readdouble();
	}

	static int foo1(int a, int b)
	{
		int result=0;
		if (a>(b/2+3*4)/1)
		{
			result = a/2;
		}
		else{
			if (a==0 && b==a)
				result=0;
		}
		return result;
	}


}
