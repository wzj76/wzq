/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.
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
#ifndef __Java_org_cocos2dx_lib_Cocos2dxHelper_H__
#define __Java_org_cocos2dx_lib_Cocos2dxHelper_H__

#include <string>

typedef void (*EditTextCallback)(const char* text, void* ctx);
// EditArea: Added by Joi Yao.
typedef void (*EditAreaCallback)(const char* text, void* pea);
// ImagePicker: Added by Joi Yao.
typedef void (*ImagePickerCallback)(const char* path, bool succeed, void* pip);

extern const char * getApkPath();
extern std::string getPackageNameJNI();
extern int getObbAssetFileDescriptorJNI(const char* path, long* startOffset, long* size);
extern void conversionEncodingJNI(const char* src, int byteSize, const char* fromCharset, char* dst, const char* newCharset);

extern int getDeviceSampleRate();
extern int getDeviceAudioBufferSizeInFrames();

extern void showDialogJNI(const char * message, const char * title);
extern void showEditTextDialogJNI(const char* title, const char* content, int inputMode, int inputFlag, int returnType, int maxLength, EditTextCallback callback, void* ctx);
extern void resetEditTextDialog(EditTextCallback callback, void* ctx);
// EditArea: Added by Joi Yao.-BEGIN
extern void showEditAreaDialogJNI(const char* title, const char* placeholder, const char* text, int maxLength, EditAreaCallback callback, void* pea);
extern void resetEditAreaDialog(EditAreaCallback callback, void* pea);
// EditArea: Added by Joi Yao.-END
// ImagePicker: Added by Joi Yao.-BEGIN
extern void showImagePickerJNI(int ratioX, int ratioY, int width, int height, bool allowsEditing, ImagePickerCallback callback, void* pip);
extern void resetImagePicker(ImagePickerCallback callback, void* pip);
// ImagePicker: Added by Joi Yao.-END
#endif /* __Java_org_cocos2dx_lib_Cocos2dxHelper_H__ */
