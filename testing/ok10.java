class Main {
    static void run () {
        int a = 3;
        int b = 4;
        if (a > b)
            if (a > b+1)
                if (a > b+2)
                    if (a > b+3)
                        b = 0;
                    else
                        b = 1;
                else
                    b = 1;
            else
                b = 1;
        else
            b = 1;
    }
}
