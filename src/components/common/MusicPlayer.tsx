import { useState, useRef, useEffect } from 'react'
import { motion, AnimatePresence } from 'framer-motion'
import { useLocalStorage } from '@/hooks/useLocalStorage'

interface MusicPlayerProps {
  className?: string
}

export function MusicPlayer({ className = '' }: MusicPlayerProps) {
  const [isPlaying, setIsPlaying] = useState(false)
  const [currentTime, setCurrentTime] = useState(0)
  const [duration, setDuration] = useState(0)
  const [volume, setVolume] = useLocalStorage('gm-music-volume', 0.3)
  const [isMuted, setIsMuted] = useState(false)
  const [isExpanded, setIsExpanded] = useState(false)
  const [currentTrack, setCurrentTrack] = useState(0)

  const audioRef = useRef<HTMLAudioElement>(null)

  // 音乐播放列表 - 使用免费的背景音乐
  const tracks = [
    {
      name: '宁静夜晚',
      artist: '环境音乐',
      duration: 180, // 3分钟
      url: '' // 实际项目中应该放置音频文件URL
    },
    {
      name: '专注工作',
      artist: '轻音乐',
      duration: 240, // 4分钟
      url: ''
    },
    {
      name: '放松心情',
      artist: '纯音乐',
      duration: 200, // 3分20秒
      url: ''
    }
  ]

  // 模拟音频播放 (实际项目中会使用真实音频)
  useEffect(() => {
    let interval: NodeJS.Timeout

    if (isPlaying) {
      interval = setInterval(() => {
        setCurrentTime(prev => {
          const newTime = prev + 1
          if (newTime >= tracks[currentTrack].duration) {
            // 自动切换下一首
            handleNext()
            return 0
          }
          return newTime
        })
      }, 1000)
    }

    return () => {
      if (interval) {
        clearInterval(interval)
      }
    }
  }, [isPlaying, currentTrack])

  // 音量控制
  useEffect(() => {
    if (audioRef.current) {
      audioRef.current.volume = isMuted ? 0 : volume
    }
  }, [volume, isMuted])

  const togglePlay = () => {
    setIsPlaying(!isPlaying)
  }

  const handleNext = () => {
    setCurrentTrack((prev) => (prev + 1) % tracks.length)
    setCurrentTime(0)
  }

  const handlePrevious = () => {
    setCurrentTrack((prev) => (prev - 1 + tracks.length) % tracks.length)
    setCurrentTime(0)
  }

  const handleVolumeChange = (newVolume: number) => {
    setVolume(newVolume)
    setIsMuted(false)
  }

  const toggleMute = () => {
    setIsMuted(!isMuted)
  }

  const formatTime = (seconds: number) => {
    const mins = Math.floor(seconds / 60)
    const secs = seconds % 60
    return `${mins}:${secs.toString().padStart(2, '0')}`
  }

  const progressPercentage = (currentTime / tracks[currentTrack].duration) * 100

  return (
    <div className={`fixed bottom-4 right-4 z-50 ${className}`}>
      {/* 音频元素 (隐藏) */}
      <audio ref={audioRef} />

      {/* 主播放器 */}
      <motion.div
        initial={{ opacity: 0, scale: 0.8 }}
        animate={{ opacity: 1, scale: 1 }}
        className="relative"
      >
        {/* 展开的播放器 */}
        <AnimatePresence>
          {isExpanded && (
            <motion.div
              initial={{ opacity: 0, y: 20, scale: 0.9 }}
              animate={{ opacity: 1, y: 0, scale: 1 }}
              exit={{ opacity: 0, y: 20, scale: 0.9 }}
              transition={{ type: 'spring', duration: 0.3 }}
              className="absolute bottom-16 right-0 w-80 glass-card rounded-lg p-4 border border-border/20 shadow-xl"
            >
              {/* 当前播放信息 */}
              <div className="text-center mb-4">
                <div className="w-16 h-16 mx-auto mb-3 bg-gradient-to-br from-primary to-accent rounded-full flex items-center justify-center">
                  <span className="text-2xl">🎵</span>
                </div>
                <div className="font-semibold text-text">
                  {tracks[currentTrack].name}
                </div>
                <div className="text-sm text-text-muted">
                  {tracks[currentTrack].artist}
                </div>
              </div>

              {/* 进度条 */}
              <div className="mb-4">
                <div className="relative h-2 bg-bg-secondary rounded-full overflow-hidden">
                  <motion.div
                    className="absolute left-0 top-0 h-full bg-gradient-to-r from-primary to-accent"
                    style={{ width: `${progressPercentage}%` }}
                    transition={{ type: 'tween', duration: 0.1 }}
                  />
                </div>
                <div className="flex justify-between text-xs text-text-muted mt-1">
                  <span>{formatTime(currentTime)}</span>
                  <span>{formatTime(tracks[currentTrack].duration)}</span>
                </div>
              </div>

              {/* 控制按钮 */}
              <div className="flex items-center justify-center gap-4 mb-4">
                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={handlePrevious}
                  className="w-8 h-8 rounded-full bg-bg-secondary hover:bg-bg-tertiary transition-colors flex items-center justify-center"
                >
                  <span className="text-sm">⏮️</span>
                </motion.button>

                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={togglePlay}
                  className="w-12 h-12 rounded-full bg-gradient-to-r from-primary to-accent hover:shadow-lg transition-all flex items-center justify-center"
                >
                  <span className="text-xl text-white">
                    {isPlaying ? '⏸️' : '▶️'}
                  </span>
                </motion.button>

                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={handleNext}
                  className="w-8 h-8 rounded-full bg-bg-secondary hover:bg-bg-tertiary transition-colors flex items-center justify-center"
                >
                  <span className="text-sm">⏭️</span>
                </motion.button>
              </div>

              {/* 音量控制 */}
              <div className="flex items-center gap-3">
                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={toggleMute}
                  className="text-text-secondary hover:text-text transition-colors"
                >
                  <span className="text-sm">
                    {isMuted ? '🔇' : volume > 0.5 ? '🔊' : '🔉'}
                  </span>
                </motion.button>

                <div className="flex-1">
                  <input
                    type="range"
                    min="0"
                    max="1"
                    step="0.1"
                    value={isMuted ? 0 : volume}
                    onChange={(e) => handleVolumeChange(parseFloat(e.target.value))}
                    className="w-full h-2 bg-bg-secondary rounded-lg appearance-none cursor-pointer volume-slider"
                    style={{
                      background: `linear-gradient(to right, rgb(var(--primary)) 0%, rgb(var(--primary)) ${(isMuted ? 0 : volume) * 100}%, rgb(var(--bg-secondary)) ${(isMuted ? 0 : volume) * 100}%, rgb(var(--bg-secondary)) 100%)`
                    }}
                  />
                </div>

                <div className="text-xs text-text-muted w-8 text-right">
                  {Math.round((isMuted ? 0 : volume) * 100)}
                </div>
              </div>

              {/* 播放列表 */}
              <div className="mt-4 pt-4 border-t border-border/20">
                <div className="text-sm font-medium text-text mb-2">播放列表</div>
                <div className="space-y-1">
                  {tracks.map((track, index) => (
                    <motion.button
                      key={index}
                      whileHover={{ scale: 1.02 }}
                      whileTap={{ scale: 0.98 }}
                      onClick={() => {
                        setCurrentTrack(index)
                        setCurrentTime(0)
                      }}
                      className={`w-full text-left p-2 rounded transition-colors ${
                        index === currentTrack
                          ? 'bg-primary/20 text-primary'
                          : 'hover:bg-bg-secondary'
                      }`}
                    >
                      <div className="font-medium text-sm">{track.name}</div>
                      <div className="text-xs text-text-muted">{track.artist}</div>
                    </motion.button>
                  ))}
                </div>
              </div>
            </motion.div>
          )}
        </AnimatePresence>

        {/* 小播放器按钮 */}
        <motion.button
          whileHover={{ scale: 1.1 }}
          whileTap={{ scale: 0.9 }}
          onClick={() => setIsExpanded(!isExpanded)}
          className="w-12 h-12 rounded-full glass-card border border-border/20 shadow-lg flex items-center justify-center relative overflow-hidden"
        >
          {/* 播放状态指示器 */}
          {isPlaying && (
            <motion.div
              animate={{
                scale: [1, 1.2, 1],
                opacity: [0.5, 0.8, 0.5]
              }}
              transition={{
                duration: 2,
                repeat: Infinity,
                ease: "easeInOut"
              }}
              className="absolute inset-0 bg-gradient-to-r from-primary to-accent rounded-full"
            />
          )}

          <span className="relative z-10 text-xl">
            {isPlaying ? '🎵' : '🎵'}
          </span>

          {/* 进度环 */}
          {isPlaying && (
            <svg className="absolute inset-0 w-full h-full -rotate-90">
              <circle
                cx="24"
                cy="24"
                r="20"
                stroke="rgb(var(--primary))"
                strokeWidth="2"
                fill="none"
                strokeDasharray={`${progressPercentage * 1.256} 125.6`}
                className="opacity-60"
              />
            </svg>
          )}
        </motion.button>
      </motion.div>
    </div>
  )
}

// CSS 样式（添加到 tailwind.css 或组件样式中）
const musicPlayerStyles = `
.volume-slider::-webkit-slider-thumb {
  appearance: none;
  width: 16px;
  height: 16px;
  background: rgb(var(--primary));
  border-radius: 50%;
  cursor: pointer;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
}

.volume-slider::-moz-range-thumb {
  width: 16px;
  height: 16px;
  background: rgb(var(--primary));
  border-radius: 50%;
  cursor: pointer;
  border: none;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
}
`