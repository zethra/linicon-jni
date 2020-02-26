public class Example {
    public static void main(String args[]) {
        IconLookup iter = IconLookup.lookup("Papirus", "firefox", 128, 1);
        for (IconPath icon : iter) {
            System.out.printf("path: %s\n", icon.path);
        }
    }
}