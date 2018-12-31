package com.zhenl.demo;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.webkit.WebView;

import com.taobao.android.dexposed.ClassUtils;
import com.taobao.android.dexposed.DexposedBridge;
import com.taobao.android.dexposed.XC_MethodHook;

public class Main {

    private static GCDetect detect;

    private static Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 0:
                    Log.e("epic.hook", "keep alive is running");
                    sendEmptyMessageDelayed(0, 10000);
                    break;
                case 1:
                    init();
                    break;
            }
        }
    };

    public static void init() {
        detect = new GCDetect();
        handler.removeMessages(0);
        handler.sendEmptyMessageDelayed(0, 10000);
        DexposedBridge.findAndHookMethod(WebView.class, "loadUrl", String.class, new XC_MethodHook() {
            @Override
            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                Log.e("epic.hook", "loadUrl:" + param.args[0]);
            }
        });
        String SQLiteDatabaseClass = "com.tencent.wcdb.database.SQLiteDatabase";
        try {
            Class cls = ClassUtils.getClass(SQLiteDatabaseClass);
            DexposedBridge.findAndHookMethod(cls, "rawQueryWithFactory",
                    SQLiteDatabaseClass + ".CursorFactory", String.class, String[].class, String.class, "com.tencent.wcdb.support.CancellationSignal",
                    new XC_MethodHook() {
                        @Override
                        protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                            Log.e("rawQueryWithFactory", param.args[1] + ":" + param.args[3]);
                        }
                    });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static class GCDetect {
        @Override
        protected void finalize() throws Throwable {
            Log.e("epic.hook", "Final redemption");
            handler.sendEmptyMessageDelayed(1, 1000);
            super.finalize();
        }
    }
}
