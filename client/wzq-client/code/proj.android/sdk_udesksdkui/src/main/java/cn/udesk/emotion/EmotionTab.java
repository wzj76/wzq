package cn.udesk.emotion;

import android.content.Context;
import android.net.Uri;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;

import com.facebook.drawee.view.SimpleDraweeView;

import java.io.File;

import cn.udesk.R;
import cn.udesk.UdeskUtil;

/**
 * 表情底部tab
 */
public class EmotionTab extends RelativeLayout {

    private SimpleDraweeView mIvIcon;
    private String mStickerCoverImgPath;
    private int mIconSrc = R.drawable.udesk_001;

    public EmotionTab(Context context, int iconSrc) {
        super(context);
        mIconSrc = iconSrc;
        init(context);
    }

    public EmotionTab(Context context, String stickerCoverImgPath) {
        super(context);
        mStickerCoverImgPath = stickerCoverImgPath;
        init(context);
    }


    private void init(Context context) {
        LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        inflater.inflate(R.layout.udesk_emotion_tab, this);

        mIvIcon = (SimpleDraweeView) findViewById(R.id.ivIcon);

        if (TextUtils.isEmpty(mStickerCoverImgPath)) {
            mIvIcon.setImageResource(mIconSrc);
        } else {
//            mIvIcon.setImageURI(Uri.fromFile(new File(mStickerCoverImgPath)));
            UdeskUtil.loadNoChangeView(context, mIvIcon, Uri.fromFile(new File(mStickerCoverImgPath)));
        }
    }

}
