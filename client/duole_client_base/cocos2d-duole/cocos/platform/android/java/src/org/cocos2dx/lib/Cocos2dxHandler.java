/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
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

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Message;

import java.lang.ref.WeakReference;

public class Cocos2dxHandler extends Handler {
    // ===========================================================
    // Constants
    // ===========================================================
    public final static int HANDLER_SHOW_DIALOG = 1;
    public final static int HANDLER_SHOW_EDITBOX_DIALOG = 2;
    // EditArea: Added by Joi Yao.
    public final static int HANDLER_SHOW_EDITAREA_DIALOG = 3;
    // ImagePicker: Added by Joi Yao.
    public final static int HANDLER_SHOW_IMAGE_PICKER = 4;
    
    // ===========================================================
    // Fields
    // ===========================================================
    private WeakReference<Cocos2dxActivity> mActivity;
    
    // ===========================================================
    // Constructors
    // ===========================================================
    public Cocos2dxHandler(Cocos2dxActivity activity) {
        this.mActivity = new WeakReference<Cocos2dxActivity>(activity);
    }

    // ===========================================================
    // Getter & Setter
    // ===========================================================

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================
    
    // ===========================================================
    // Methods
    // ===========================================================

    public void handleMessage(Message msg) {
        switch (msg.what) {
        case Cocos2dxHandler.HANDLER_SHOW_DIALOG:
            showDialog(msg);
            break;
        case Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG:
            showEditBoxDialog(msg);
            break;
        // EditArea: Added by Joi Yao.
        case Cocos2dxHandler.HANDLER_SHOW_EDITAREA_DIALOG:
        	showEditAreaDialog(msg);
        	break;
        case Cocos2dxHandler.HANDLER_SHOW_IMAGE_PICKER:
        	showImagePicker(msg);
        	break;
        }
    }
    
    private void showDialog(Message msg) {
        Cocos2dxActivity theActivity = this.mActivity.get();
        DialogMessage dialogMessage = (DialogMessage)msg.obj;
        new AlertDialog.Builder(theActivity)
        .setTitle(dialogMessage.title)
        .setMessage(dialogMessage.message)
        .setPositiveButton("Ok", 
                new DialogInterface.OnClickListener() {
                    
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        // TODO Auto-generated method stub
                        
                    }
                }).create().show();
    }

    private void showEditBoxDialog(Message msg) {
        EditBoxMessage editBoxMessage = (EditBoxMessage)msg.obj;
        new Cocos2dxEditBoxDialog(this.mActivity.get(),
                editBoxMessage.title,
                editBoxMessage.content,
                editBoxMessage.inputMode,
                editBoxMessage.inputFlag,
                editBoxMessage.returnType,
                editBoxMessage.maxLength).show();
    }
    // EditArea: Added by Joi Yao.
    private void showEditAreaDialog(Message msg) {
    	EditAreaMessage editAreaMessage = (EditAreaMessage)msg.obj;
    	new Cocos2dxEditAreaDialog(this.mActivity.get(),
    			editAreaMessage.title,
    			editAreaMessage.placeholder,
    			editAreaMessage.text,
    			editAreaMessage.maxLength).show();
    }
    // ImagePicker: Added by Joi Yao.
    private void showImagePicker(Message msg) {
    	ImagePickerMessage imagePickerMessage = (ImagePickerMessage)msg.obj;
    	Cocos2dxImagePicker.getInstance().show(imagePickerMessage.ratioX, imagePickerMessage.ratioY, imagePickerMessage.width, imagePickerMessage.height, imagePickerMessage.allowsEditing);
    }
    
    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    public static class DialogMessage {
        public String title;
        public String message;
        
        public DialogMessage(String title, String message) {
            this.title = title;
            this.message = message;
        }
    }
    public static class EditBoxMessage {
        public String title;
        public String content;
        public int inputMode;
        public int inputFlag;
        public int returnType;
        public int maxLength;
        
        public EditBoxMessage(String title, byte[] content, int inputMode, int inputFlag, int returnType, int maxLength){
            this.content = new String(content);
            this.title = title;
            this.inputMode = inputMode;
            this.inputFlag = inputFlag;
            this.returnType = returnType;
            this.maxLength = maxLength;
        }
    }
    // EditArea: Added by Joi Yao.
    public static class EditAreaMessage {
    	public String title;
    	public String placeholder;
    	public String text;
    	public int maxLength;
    	
    	public EditAreaMessage(String title, String placeholder, byte[] text, int maxLength) {
    		this.title = title;
    		this.placeholder = placeholder;
    		this.text = new String(text);
    		this.maxLength = maxLength;
    	}
    }
    // ImagePicker: Added by Joi Yao.
    public static class ImagePickerMessage {
    	public int ratioX;
    	public int ratioY;
    	public int width;
    	public int height;
        public boolean allowsEditing;
    	public ImagePickerMessage(int ratioX, int ratioY, int width, int height, boolean allowsEditing) {
    		this.ratioX = ratioX;
    		this.ratioY = ratioY;
    		this.width = width;
    		this.height = height;
            this.allowsEditing = allowsEditing;
    	}
    }
}
