class Main {
    static void run() {
        ifj16.print("program ukoncete pomoci ctrl+c\n")
        ifj16.print("Zadejte retezec ve kterem se bude hledat\n");
        String str_find = ifj16.readString();
        String str_seek;
        int ret = 0;
        while(true) {
            ifj16.print("Zadejte hledany retezec:\n");
            str_seek = ifj16.readString();
            ret = ifj16.find(str_find, str_seek);
            ifj16.print("Navratova hodnota: " + ret + "\n");
        }
    }
}