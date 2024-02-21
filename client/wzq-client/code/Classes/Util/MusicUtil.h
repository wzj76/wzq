/*******************************************************************************
 处理音乐的类
 若用此类管理音乐，需要：
 1. 在开始游戏时（比如splash界面）调用PHZMusicUtil::DoInit()进行初始化，并且用
 PHZMusicUtil::PreloadEffect 和 PHZMusicUtil::PreloadBackgroundMusic来预加载音乐资源
 2. 在AppDelegate::applicationDidEnterBackground中调用PHZMusicUtil::PauseBackgroundMusic暂停背景音乐
 3. 在AppDelegate::applicationWillEnterForeground中调用PHZMusicUtil::EnterForeground恢复背景音乐
 4. 在AppDelegate::~AppDelegate中调用PHZMusicUtil::End结束音乐
 5. 在游戏要退出时会自动保存当前的设置。如果想要手动保存设置（比如玩家切换了音乐或音效的开启就想立即保存），
 调用PHZMusicUtil::SaveSettings。
 6. 若要从A界面进入B界面时继续A界面的背景音乐播放，在其他界面进入B界面时重新从头开始播放，可以这样设置：
 在A进入B前设置PHZMusicUtil::set_is_should_replay_music(false)，
 在B的onEnter中PlayBackgroundMusic后设置PHZMusicUtil::set_is_should_replay_music(true)
 ********************************************************************************/

#ifndef UTIL_MUSICUTIL_H_
#define UTIL_MUSICUTIL_H_

#include <string>

namespace util
{
    
    class MusicUtil
    {
    public:
        // 从文件中读取音乐设置
        static void initialize();
        
        // 背景音乐是否开了
        static bool is_background_music_on() { return is_bg_music_on_; }
        
        // 设置背景音乐开关
        static void set_is_background_music_on(bool is_on);
        
        // 设置背景音乐开关  只修改数值
        static void set_background_music(bool is_on);
        
        // 特效声音是否开了
        static bool is_effect_music_on() { return is_effect_music_on_; }
        
        // 设置背景音乐大小
        static float get_music_Volume() { return music_volume; };
        
        // 特效声音大小
        static float get_effect_Volume() { return effect_volume; }
        
        // 设置特效声音开关
        static void set_is_effect_music_on(bool is_on) { is_effect_music_on_ = is_on; }
        
        // 设置音乐是否应该重新播放
        static void set_is_should_replay_music(bool is_should_replay) { is_should_replay_music_ = is_should_replay; }
        
        // 加载音效
        static void PreloadEffect(const char* file_path);
        
        // 加载背景音乐
        static void PreloadBackgroundMusic(const char* file_path);
        
        // 播放背景音乐
        // bg_music_name: 背景音乐名字
        // is_loop: 是否循环播放
        static void PlayBackgroundMusic(const char* music_name, bool is_loop,bool playScene);
        static void PlayNoVolumeBackgroundMusic();

        // 暂停
        static void PauseBackgroundMusic();
        
        // 恢复
        static void ResumeBackgroundMusic();
        
        // 回绕
        static void RewindBackgroundMusic();
        
        // 停止
        static void StopBackgroundMusic();
        
        //停止所有音效,已记录的。
        static void StopAllEffect();
        
        static void PauseEffect();
        
        static void ResumeEffect();
        
        // 播放特效声音，music_name为特效声音名字
        static unsigned int PlayEffectMusic(const char* effect_name);
        // 停止特效声音
        static void StopEffect(unsigned int nSoundId);
        
        // 游戏进入前台
        static void EnterForeground();
        
        //设置音乐的大小
        static void SetMusicVolume(float music_vloume);
        //设置音效的大小
        static void SetEffectVolume(float effect_vloume);
        
        // 保存设置到文件
        static void SaveSettings();
        
        // 结束音乐播放（在游戏将要退出时调用）
        static void End();
        
        // 设置音乐场景
        static void SetOnMusicScene(bool IsScene) { is_on_play_scene = IsScene; }
        
        //开启 设置静音模式
        static bool is_Silent_Mode();
        
        static void set_Silent_Mode(bool value);
        
        
    private:
        static bool         is_bg_music_on_;            // 背景音乐是否开了
        static bool         is_effect_music_on_;        // 特效声音是否开了
        static bool         is_should_replay_music_;    // 控制背景音乐是否应从头重新播放
        static std::string  bg_music_name_;             // 上次播放的背景音乐的名称
        static bool         is_play_bg;
        static bool         is_on_play_scene;
        static float        music_volume;               // 音乐的音量
        static float        effect_volume;              // 音效的大小
        static bool         isSilentMode;               // 静音模式
    };
}
#endif // UTIL_MUSICUTIL_H_
