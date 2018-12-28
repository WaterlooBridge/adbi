package com.zhenl.demo;

import android.util.Log;
import android.webkit.WebView;

import com.taobao.android.dexposed.DexposedBridge;
import com.taobao.android.dexposed.XC_MethodHook;

public class Main {

    public static void init() {
        DexposedBridge.findAndHookMethod(WebView.class, "loadUrl", String.class, new XC_MethodHook() {
            @Override
            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                Log.e("epic hook", "loadUrl:" + param.args[0]);
            }
        });
    }
}
