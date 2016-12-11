class Main {
    static void run() {
        ifj16.print("program ukoncete pomoci ctrl+c\n");
        ifj16.print("Zadejte retezec ve kterem se bude hledat\n");
        String str_find = ifj16.readString();
        while(true) {
            ifj16.print("Zadejte oba indexy:\n");
            ifj16.print("Navratova hodnota: " + ifj16.substr(str_find, ifj16.readInt(), ifj16.readInt()) + "\n");
        }
    }
}