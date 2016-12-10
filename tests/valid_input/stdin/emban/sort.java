class Main {
    static void run() {
        ifj16.print("program ukoncete pomoci ctrl+c\n")
        while(true) {
            ifj16.print("Zadejte retezec na serazeni:\n");
            ifj16.print("Serazeny retezec: " + ifj16.sort(ifj16.readString()) + "\n");
        }
    }
}