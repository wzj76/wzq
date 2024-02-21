#include "AnimateUtil.h"
#include "Util/SpriteUtil.h"
#include "Util/MacroUtil.h"

USING_NS_CC;

namespace util
{
    
    Animation* GetAnimationFromFile(const char* prefix_name, int frame_count, float delay)
    {
        return GetAnimationFromFile(prefix_name, frame_count, delay, 0);
    }
    
    Animation* GetAnimationFromFile(const char* prefix_name, int frame_count, float delay, int start_index)
    {
        Vector<SpriteFrame*> frames;
        for (int i = 0; i < frame_count; i++)
        {
            const char *kFrameName = str_format("%s%d.png", prefix_name, start_index + i);
            SpriteFrame *frame = GetFrameFromFile(kFrameName);
            frames.pushBack(frame);
        }
        Animation *animation = Animation::createWithSpriteFrames(frames, delay);
        //当动画播放完后，帧序是否重设为默认第一帧
        animation->setRestoreOriginalFrame(false);
        return animation;
    }
    
    Animation* GetAnimationFromPlist(const char* prefix_name, int frame_count, float delay)
    {
        return GetAnimationFromPlist(prefix_name, frame_count, delay, 0);
    }
    
    Animation* GetAnimationFromPlist(const char* prefix_name, int frame_count, float delay, int start_index)
    {
        Vector<SpriteFrame*> frames;
        for (int i = 1; i <= frame_count; i++)
        {
            const char *kFrameName = str_format("%s%d.png", prefix_name, start_index + i);
            SpriteFrame *frame = GetFrameFromPlist(kFrameName);
            frames.pushBack(frame);
        }
        Animation *animation = Animation::createWithSpriteFrames(frames, delay);
        //当动画播放完后，帧序是否重设为默认第一帧
        animation->setRestoreOriginalFrame(true);
        return animation;
    }
    
}
