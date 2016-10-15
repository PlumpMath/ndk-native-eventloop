package com.example.cosnita.gettingstartedndk;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    private static native String sayHello(String msg);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onRestart() {
        super.onRestart();
    }

    public void showMessage(View view)
    {
        String nativeMsg = new NativeWrapper().sayHello();
        String staticMsg = sayHello("Hello from Java ...");
        EditText textField = (EditText)findViewById(R.id.edit_message);
        textField.setText("Native msg: " + nativeMsg + " .... Static msg: " + staticMsg);
    }

    public void testNativeEmitter(View view)
    {
        final EditText textField = (EditText)findViewById(R.id.edit_message);
        String evtName = "custom:event";
        EventEmitter eventEmitter = EventEmitter.getInstance();
        eventEmitter.on(evtName, new EventEmitterAction() {
            @Override
            public void doAction(final String data) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        textField.setText(data);
                    }
                });
            }
        });

        eventEmitter.emit(evtName, "Message 1 ...");
        eventEmitter.emit(evtName, "Message 2 ...");
    }

    public void sendMessage(View view)
    {
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        intent.putExtra("MESSAGE_CUSTOM", getMessage());
        startActivity(intent);
    }

    private String getMessage() {
        EditText textField = (EditText)findViewById(R.id.edit_message);
        return textField.getText().toString();
    }
}
