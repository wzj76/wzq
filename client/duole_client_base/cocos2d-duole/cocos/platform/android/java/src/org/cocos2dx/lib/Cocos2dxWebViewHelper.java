/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

package org.cocos2dx.lib;

import android.annotation.TargetApi;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.util.SparseArray;
import android.view.View;
import android.webkit.WebView;
import android.widget.FrameLayout;
import android.webkit.WebSettings;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

import android.view.KeyEvent;
import android.widget.ImageButton;

import android.util.TypedValue;
import android.view.Gravity;
import android.widget.ImageView;


public class Cocos2dxWebViewHelper {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();
    private static Handler sHandler;
    private static Cocos2dxActivity sCocos2dxActivity;
    private static FrameLayout sLayout;
    private static ImageButton mButtonBack;
    private static Bitmap sBtnImage = null;

    private static SparseArray<Cocos2dxWebView> webViews;
    private static int viewTag = 0;

    public Cocos2dxWebViewHelper(FrameLayout layout) {
        Cocos2dxWebViewHelper.sLayout = layout;
        Cocos2dxWebViewHelper.sHandler = new Handler(Looper.myLooper());

        Cocos2dxWebViewHelper.sCocos2dxActivity = (Cocos2dxActivity) Cocos2dxActivity.getContext();
        Cocos2dxWebViewHelper.webViews = new SparseArray<Cocos2dxWebView>();
    }

    private static native boolean shouldStartLoading(int index, String message);

    public static boolean _shouldStartLoading(int index, String message) {
        return !shouldStartLoading(index, message);
    }

    private static native void didFinishLoading(int index, String message);

    public static void _didFinishLoading(int index, String message) {
        didFinishLoading(index, message);
    }

    private static native void didFailLoading(int index, String message);

    public static void _didFailLoading(int index, String message) {
        didFailLoading(index, message);
    }

    private static native void onJsCallback(int index, String message);

    public static void _onJsCallback(int index, String message) {
        onJsCallback(index, message);
    }

    private static native void onKeyCallback(int index, String message);
    
    public static void _onKeyCallback(int index, String message) {    	
    	Cocos2dxGLSurfaceView.getInstance().requestFocus();
        onKeyCallback(index, message);
    }
    public static int createWebView() {
        final int index = viewTag;
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = new Cocos2dxWebView(sCocos2dxActivity, index);
                FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                        FrameLayout.LayoutParams.WRAP_CONTENT,
                        FrameLayout.LayoutParams.WRAP_CONTENT);
                sLayout.addView(webView, lParams);

                webViews.put(index, webView);
            }
        });
        return viewTag++;
    }

    public static void removeWebView(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mButtonBack != null) {
                    sLayout.removeView(mButtonBack);
                }

                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webViews.remove(index);
                    sLayout.removeView(webView);
                }
            }
        });
    }

    public static void setVisible(final int index, final boolean visible) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setVisibility(visible ? View.VISIBLE : View.GONE);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setPlayback(final int index, final boolean bPlayback) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setPlayback(bPlayback);
                }
            }
        });
    }
	
    public static void setBackgroundTransparent(final int index) {
        if(android.os.Build.VERSION.SDK_INT >10) {
            sCocos2dxActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebView webView = webViews.get(index);
                    if (webView != null) {
                        webView.setBackgroundColor(Color.TRANSPARENT);
                        try {
                            Method method = webView.getClass().getMethod("setLayerType",int.class,Paint.class);
                            method.invoke(webView,WebView.LAYER_TYPE_SOFTWARE,null);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
        }
    }

    public static void setOpacityWebView(final int index, final float opacity) {
        if(android.os.Build.VERSION.SDK_INT >10){
            sCocos2dxActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Cocos2dxWebView webView = webViews.get(index);
                    if (webView != null) {
                        try {
                            Method method = webView.getClass().getMethod("setAlpha",float.class);
                            method.invoke(webView,opacity);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
        }
    }


    public static float getOpacityWebView(final int index) {
        if(android.os.Build.VERSION.SDK_INT >10){
            FutureTask<Float> futureResult = new FutureTask<Float>(new Callable<Float>() {
            @Override
            public Float call() throws Exception {
                float opacity=0.f;
                Cocos2dxWebView webView = webViews.get(index);
                Object valueToReturn=null;
                if (webView != null) {
                    try {
                        Method method = webView.getClass().getMethod("getAlpha");
                        valueToReturn = method.invoke(webView);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                return (Float) valueToReturn;
            }
            });
            sCocos2dxActivity.runOnUiThread(futureResult);
            try {
                return futureResult.get();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return 1;
    }
    private static int convertPixelToDip(int nP) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, nP, sCocos2dxActivity.getResources().getDisplayMetrics());
    }

    /**
     * 加载指定资源文件；
     *
     * @param fileName
     * @return
     */
    public static Bitmap getImageFromAssetsFile(String fileName) {
        Bitmap image = null;
        AssetManager am = sCocos2dxActivity.getResources().getAssets();
        try {
            InputStream is = am.open(fileName);
            image = BitmapFactory.decodeStream(is);
            is.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return image;
    }

    @SuppressWarnings("unused")
    public static void setMenuBack(final int index, final String image, final float width) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    // 创建返回按钮
                    mButtonBack = new ImageButton(sCocos2dxActivity);
                    // 设置按钮图片
                    if (sBtnImage == null) {
                        // 根据传入的按钮图片路径，加载图片；
                        Bitmap srcBtnImg = getImageFromAssetsFile(image);
                        if (srcBtnImg == null) {
                            return;
                        }
                        // 按显示尺寸缩放位图；
                        sBtnImage = Bitmap.createScaledBitmap(srcBtnImg, convertPixelToDip(50), convertPixelToDip(50), true);
                    }

                    mButtonBack.setImageBitmap(sBtnImage);
                    mButtonBack.setBackgroundColor(0);
                    FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                                                                         FrameLayout.LayoutParams.WRAP_CONTENT);
                    layoutParams.width = convertPixelToDip(50);
                    layoutParams.height = convertPixelToDip(50);
                    // layoutParams.gravity = Gravity.TOP | Gravity.LEFT;
                    layoutParams.gravity = Gravity.TOP | Gravity.RIGHT;

                    mButtonBack.setLayoutParams(layoutParams);
                    sLayout.addView(mButtonBack);
                    mButtonBack.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View paramView) {
                            _onKeyCallback(index, "");
                            sLayout.removeView(mButtonBack);
                        }
                    });
                }
            }
        });
    }

    public static void removeMenuBack() {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mButtonBack != null) {
                    sLayout.removeView(mButtonBack);
                }
            }
        });
    }
	
    public static void setWebViewRect(final int index, final int left, final int top, final int maxWidth, final int maxHeight) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setWebViewRect(left, top, maxWidth, maxHeight);
                }
            }
        });
    }

    public static void setJavascriptInterfaceScheme(final int index, final String scheme) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setJavascriptInterfaceScheme(scheme);
                }
            }
        });
    }

    public static void loadData(final int index, final String data, final String mimeType, final String encoding, final String baseURL) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseURL, data, mimeType, encoding, null);
                }
            }
        });
    }

    public static void loadHTMLString(final int index, final String data, final String baseUrl) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseUrl, data, null, null, null);
                }
            }
        });
    }

    public static void loadUrl(final int index, final String url, final boolean cleanCachedData) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.getSettings().setCacheMode(cleanCachedData ? WebSettings.LOAD_NO_CACHE
                                                                       : WebSettings.LOAD_DEFAULT);
                    webView.loadUrl(url);
                }
            }
        });
    }

    public static void loadFile(final int index, final String filePath) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl(filePath);
                }
            }
        });
    }

    public static void stopLoading(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.stopLoading();
                }
            }
        });

    }

    public static void reload(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.reload();
                }
            }
        });
    }

    public static <T> T callInMainThread(Callable<T> call) throws ExecutionException, InterruptedException {
        FutureTask<T> task = new FutureTask<T>(call);
        sHandler.post(task);
        return task.get();
    }

    public static boolean canGoBack(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                Cocos2dxWebView webView = webViews.get(index);
                return webView != null && webView.canGoBack();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    public static boolean canGoForward(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                Cocos2dxWebView webView = webViews.get(index);
                return webView != null && webView.canGoForward();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    public static void goBack(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goBack();
                }
            }
        });
    }

    public static void goForward(final int index) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goForward();
                }
            }
        });
    }

    public static void evaluateJS(final int index, final String js) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl("javascript:" + js);
                }
            }
        });
    }

    public static void setScalesPageToFit(final int index, final boolean scalesPageToFit) {
        sCocos2dxActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Cocos2dxWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setScalesPageToFit(scalesPageToFit);
                }
            }
        });
    }
}
