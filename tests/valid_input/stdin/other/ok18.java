/* Program 3: Prace s retezci a vestavenymi funkcemi */
class Main
{
    static int x;
    static void run()
    {
        String str1;
        str1 = "Toto je nejaky text";
        String str2;
        str2 = str1 + ", ktery jeste trochu obohatime";
        Main.x = ifj16.find(str2, "text");
        ifj16.print("Pozice retezce \"text\" v retezci str2: " + x + "\n");
        Game.play(str1);
    }
// end of static void run()
}
// end of class Main
class Game
{
    static void play(String str)
    {
        ifj16.print("Zadejte nejakou posloupnost vsech malych pismen a-h, ");
        ifj16.print("pricemz se pismena nesmeji v posloupnosti opakovat:");
        str = ifj16.readString();
        str = ifj16.sort(str);
        int cmp = ifj16.compare(str, "abcdefgh");
        if (cmp != 0) {
            while (cmp != 0) {
                ifj16.print("Spatne zadana posloupnost, zkuste znovu:");
                str = ifj16.readString();
                str = ifj16.sort(str);
                cmp = ifj16.compare(str, "abcdefgh");
            }
        }
        return;
    }
    // end of static void play(String)
}
// end of class Game
