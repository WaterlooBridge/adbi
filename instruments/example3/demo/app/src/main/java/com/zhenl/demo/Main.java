package com.zhenl.demo;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.webkit.WebView;

import com.lody.whale.xposed.ClassUtils;
import com.lody.whale.xposed.XC_MethodHook;
import com.lody.whale.xposed.XposedBridge;
import com.lody.whale.xposed.XposedHelpers;

public class Main {

    private static GCDetect detect;

    private static Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 0:
                    Log.e("whale.hook", "keep alive is running");
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
        XposedBridge.hookAllMethods(WebView.class, "loadUrl", new XC_MethodHook() {
            @Override
            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                Log.e("whale.hook", "loadUrl:" + param.args[0]);
            }
        });
        String SQLiteDatabaseClass = "com.tencent.wcdb.database.SQLiteDatabase";
        try {
            Class cls = ClassUtils.getClass(SQLiteDatabaseClass);
            XposedHelpers.findAndHookMethod(cls, "rawQueryWithFactory",
                    ClassUtils.getClass(SQLiteDatabaseClass + ".CursorFactory"), String.class, Object[].class, String.class, "com.tencent.wcdb.support.CancellationSignal",
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
            Log.e("whale.hook", "Final redemption");
            handler.sendEmptyMessageDelayed(1, 1000);
            super.finalize();
        }
    }
}
