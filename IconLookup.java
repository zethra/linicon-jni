import java.util.Iterator;

public class IconLookup implements Iterator<IconPath>, Iterable<IconPath> {
    private long iterPtr;
    private IconPath cachedIcon;

    private IconLookup() {
    }

    public static native IconLookup lookup(String theme, String icon_name, int size, int scale);

    public static native IconLookup lookupWithExtraPaths(String theme, String icon_name, int size, int scale,
            String[] extraPaths);

    private native IconPath nextIcon();

    public native void finish();

    static {
        System.loadLibrary("IconLookup");
    }

    @Override
    public boolean hasNext() {
        if (cachedIcon != null) {
            return true;
        }
        cachedIcon = next();
        return cachedIcon != null;
    }

    @Override
    public IconPath next() {
        if (cachedIcon != null) {
            IconPath icon = cachedIcon;
            cachedIcon = null;
            return icon;
        }
        IconPath icon = nextIcon();
        if (icon == null) {
            finish();
        }
        return icon;
    }

    @Override
    public Iterator<IconPath> iterator() {
        return this;
    }
}