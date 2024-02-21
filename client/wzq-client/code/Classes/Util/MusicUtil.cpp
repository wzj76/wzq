#include "MusicUtil.h"
#include "ReaderWriter.h"
#include "SimpleAudioEngine.h"

// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1
#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

using namespace std;

namespace util
{
    
    // 保存/读取音乐设置的key
    static const char* const kMusicKey = "kMusicKey";
    static const char* const kEffectKey = "kEffectKey";
    static const char* const kMusicVolumeKey = "kMusicVolumeKey";
    static const char* const kEffectVolumeKey = "kEffectVolumeKey";
    static const char* const kSilentModeKey = "kSilentModeKey";
    
    float   MusicUtil::music_volume = 0.6f;
    float   MusicUtil::effect_volume = 1.0f;
    bool    MusicUtil::is_bg_music_on_ = true;
    bool    MusicUtil::is_effect_music_on_ = true;
    bool    MusicUtil::is_should_replay_music_ = true;
    bool    MusicUtil::is_play_bg = false;
    bool    MusicUtil::is_on_play_scene = false;   // 是否在播放页面
    bool    MusicUtil::isSilentMode = false;
    string  MusicUtil::bg_music_name_;
    
    void MusicUtil::initialize()
    {
        // 读取音乐音效设置
        is_bg_music_on_ = ReaderWriter::LoadBooleanFromXML(kMusicKey, true);
        is_effect_music_on_ = ReaderWriter::LoadBooleanFromXML(kEffectKey, true);
        music_volume = ReaderWriter::LoadFloatFromXML(kMusicVolumeKey, 0.6);
        effect_volume = ReaderWriter::LoadFloatFromXML(kEffectVolumeKey, 1);
        isSilentMode = ReaderWriter::LoadBooleanFromXML(kSilentModeKey, false);
        SetMusicVolume(music_volume);
        SetEffectVolume(effect_volume);
    }
    
    void MusicUtil::PreloadEffect(const char* file_path)
    {
        SimpleAudioEngine::getInstance()->preloadEffect(file_path);
    }
    
    void MusicUtil::PreloadBackgroundMusic(const char* file_path)
    {
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(file_path);
    }
    
    void MusicUtil::EnterForeground()
    {
        if (is_bg_music_on_)
        {
            SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        }
    }

    void MusicUtil::PlayNoVolumeBackgroundMusic()
    {
         #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
            // xiao bai huawei 渠道双音效问题
            CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
            PHZMusicUtil::PlayBackgroundMusic("music/back.mp3", true,false);
        #endif
    }
    
    void MusicUtil::PlayBackgroundMusic(const char* music_name, bool is_loop,bool playScene)
    {
        bg_music_name_ = music_name;
        is_on_play_scene = playScene;
        if (is_bg_music_on_ && is_should_replay_music_)
        {
            is_play_bg = playScene;
            SimpleAudioEngine::getInstance()->playBackgroundMusic(music_name, is_loop);
            if (is_on_play_scene)
            {
                MusicUtil::SetMusicVolume(0.6);
            }else{
                CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
            }
        }else{
            CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
            SimpleAudioEngine::getInstance()->playBackgroundMusic(music_name, is_loop);
        }
        if (isSilentMode)
        {
            PauseBackgroundMusic();
        }
    }
    
    void MusicUtil::PauseBackgroundMusic()
    {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    
    void MusicUtil::ResumeBackgroundMusic()
    {
        if (is_bg_music_on_ && is_play_bg && !isSilentMode)
        {
            SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        }else{
            PlayNoVolumeBackgroundMusic();
        }
    }
    
    void MusicUtil::PauseEffect()
    {
        if (is_effect_music_on_)
        {
            SimpleAudioEngine::getInstance()->pauseAllEffects();
        }
    }
    
    void MusicUtil::ResumeEffect()
    {
        if (is_effect_music_on_ && !isSilentMode)
        {
            SimpleAudioEngine::getInstance()->resumeAllEffects();
        }
    }
    
    void MusicUtil::RewindBackgroundMusic()
    {
        if(is_bg_music_on_)
        {
            SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
        }
    }
    
    void MusicUtil::StopBackgroundMusic()
    {
        is_play_bg = false;
        is_on_play_scene = false;
        SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
    }
    
    void MusicUtil::StopAllEffect()
    {
        SimpleAudioEngine::getInstance()->stopAllEffects();
    }
    
    unsigned int MusicUtil::PlayEffectMusic(const char* effect_name)
    {
        if (isSilentMode)
        {
            return 0;
        }
        if (is_effect_music_on_)
        {
            return SimpleAudioEngine::getInstance()->playEffect(effect_name);
        }
        return 0;
    }
    
    void MusicUtil::StopEffect(unsigned int nSoundId)
    {
        SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
    }
    
    void MusicUtil::SetMusicVolume(float value)
    {
        music_volume = value;
        // 系统音乐播放
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
    }
    
    void MusicUtil::SetEffectVolume(float value)
    {
        
        effect_volume = value;
        SimpleAudioEngine::getInstance()->setEffectsVolume(value);
    }
    
    
    
    void MusicUtil::set_is_background_music_on(bool is_on)
    {        
        if (is_bg_music_on_ == is_on)
        {
            return;
        }
        is_bg_music_on_ = is_on;
        if (is_bg_music_on_)
        {
            is_play_bg = true;
            // 没有播放过背景音乐，开始播放背景音乐
            // 如果在音乐场景，开始播放
            if (is_on_play_scene)
            {
                MusicUtil::StopBackgroundMusic();
                util::MusicUtil::PlayBackgroundMusic(bg_music_name_.c_str(), true,true);
                MusicUtil::SetMusicVolume(0.6);
            }
        }
        else
        {
            // 暂停背景音乐
            is_play_bg =false;
             SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        }
    }
    
    void MusicUtil::set_background_music(bool is_on)
    {
        is_bg_music_on_ = is_on;
    }
    
    bool MusicUtil::is_Silent_Mode()
    {
        return isSilentMode;
    }
    
    void MusicUtil::set_Silent_Mode(bool value)
    {
        isSilentMode = value;
        if (isSilentMode) {
            set_is_background_music_on(false);
            set_is_effect_music_on(false);
        }
    }
    
    
    
    void MusicUtil::SaveSettings()
    {
        ReaderWriter::SaveBooleanToXML(kMusicKey, is_bg_music_on_);
        ReaderWriter::SaveBooleanToXML(kEffectKey, is_effect_music_on_);
        ReaderWriter::SaveBooleanToXML(kSilentModeKey,isSilentMode);
        ReaderWriter::SaveFloatFromXML(kMusicVolumeKey, music_volume);
        ReaderWriter::SaveFloatFromXML(kEffectVolumeKey, effect_volume);
        ReaderWriter::Flush();
    }
    
    void MusicUtil::End()
    {
        // 游戏结束前保存一次设置，以免忘了游戏中忘记保存了
        SaveSettings();
        SimpleAudioEngine::end();
    }
}
