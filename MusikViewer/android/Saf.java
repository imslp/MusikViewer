package org.qtproject.muse

import android.app.Activity;
import android.content.Intent


public class Saf {
    // Here are some examples of how you might call this method.
    // The first parameter is the MIME type, and the second parameter is the name
    // of the file you are creating:
    //
    // createFile("text/plain", "foobar.txt");
    // createFile("image/png", "mypicture.png");

    // Unique request code.
    private static final int WRITE_REQUEST_CODE = 43;

    public static void createFile(String mimeType, String fileName) {
        Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);

        // Filter to only show results that can be "opened", such as
        // a file (as opposed to a list of contacts or timezones).
        intent.addCategory(Intent.CATEGORY_OPENABLE);

        // Create a file with the requested MIME type.
        intent.setType(mimeType);
        intent.putExtra(Intent.EXTRA_TITLE, fileName);
        startActivityForResult(intent, WRITE_REQUEST_CODE);
    }
}
