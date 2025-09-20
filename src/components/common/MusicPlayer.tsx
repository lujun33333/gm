import { useState, useRef, useEffect, useCallback } from 'react'
import { motion, AnimatePresence } from 'framer-motion'
import { Search, Settings, SkipBack, SkipForward, Play, Pause, Volume2, VolumeX, Heart, Plus, X } from 'lucide-react'
import { useLocalStorage } from '@/hooks/useLocalStorage'
import { useMusicSettings } from '@/hooks/useMusicSettings'
import { musicApi, audioManager, type SearchResult } from '@/utils/musicApi'
import { MusicSettings } from './MusicSettings'
import { Button } from '@/components/ui/button'
import { Input } from '@/components/ui/input'

interface MusicPlayerProps {
  className?: string
}

interface CurrentTrack {
  id: string
  name: string
  artist: string
  album?: string
  duration: number
  url?: string
  cover?: string
  platform: 'netease' | 'qq' | 'local'
}

export function MusicPlayer({ className = '' }: MusicPlayerProps) {
  const {
    settings,
    playlist,
    favorites,
    addToPlaylist,
    removeFromPlaylist,
    addToFavorites,
    removeFromFavorites,
    isFavorite
  } = useMusicSettings()

  const [isPlaying, setIsPlaying] = useState(false)
  const [currentTime, setCurrentTime] = useState(0)
  const [duration, setDuration] = useState(0)
  const [volume, setVolume] = useLocalStorage('gm-music-volume', settings.volume)
  const [isMuted, setIsMuted] = useState(false)
  const [isExpanded, setIsExpanded] = useState(false)
  const [currentTrackIndex, setCurrentTrackIndex] = useState(0)
  const [currentTrack, setCurrentTrack] = useState<CurrentTrack | null>(null)
  const [showSettings, setShowSettings] = useState(false)
  const [showSearch, setShowSearch] = useState(false)
  const [searchQuery, setSearchQuery] = useState('')
  const [searchResults, setSearchResults] = useState<SearchResult[]>([])
  const [isSearching, setIsSearching] = useState(false)
  const [isLoading, setIsLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)

  const audioRef = useRef<HTMLAudioElement>(null)
  const searchTimeoutRef = useRef<NodeJS.Timeout>()

  // 初始化音乐API设置
  useEffect(() => {
    musicApi.setSettings(settings)
  }, [settings])

  // 加载当前播放轨道
  useEffect(() => {
    if (playlist.length > 0 && currentTrackIndex < playlist.length) {
      const track = playlist[currentTrackIndex]
      setCurrentTrack({
        id: track.id,
        name: track.name,
        artist: track.artist,
        album: track.album,
        duration: track.duration,
        url: track.url,
        cover: track.cover,
        platform: track.platform
      })
    } else {
      setCurrentTrack(null)
    }
  }, [playlist, currentTrackIndex])

  // 真实音频播放逻辑
  const loadAndPlayTrack = useCallback(async (track: CurrentTrack) => {
    if (!track) return

    setIsLoading(true)
    setError(null)

    try {
      // 获取播放URL
      let playUrl = track.url
      if (!playUrl) {
        playUrl = await musicApi.getPlayUrl(track.id, settings.quality)
      }

      if (!playUrl) {
        throw new Error('无法获取播放链接')
      }

      // 使用Web Audio API播放
      const audioInfo = await audioManager.loadAndPlay(playUrl, volume)
      setDuration(audioInfo.duration)
      setCurrentTime(0)
      setIsPlaying(true)

      // 更新轨道URL
      setCurrentTrack(prev => prev ? { ...prev, url: playUrl } : null)
    } catch (error) {
      console.error('播放失败:', error)
      setError(error instanceof Error ? error.message : '播放失败')
      setIsPlaying(false)
    } finally {
      setIsLoading(false)
    }
  }, [settings.quality, volume])

  // 播放时间更新
  useEffect(() => {
    let interval: NodeJS.Timeout

    if (isPlaying && currentTrack) {
      interval = setInterval(() => {
        setCurrentTime(prev => {
          const newTime = prev + 1
          if (newTime >= duration) {
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
  }, [isPlaying, currentTrack, duration])

  // 音量控制
  useEffect(() => {
    audioManager.setVolume(isMuted ? 0 : volume)
  }, [volume, isMuted])

  // 搜索音乐
  const handleSearch = useCallback(async (query: string) => {
    if (!query.trim()) {
      setSearchResults([])
      return
    }

    setIsSearching(true)
    try {
      const results = await musicApi.searchMusic(query.trim(), 20)
      setSearchResults(results)
    } catch (error) {
      console.error('搜索失败:', error)
      setSearchResults([])
    } finally {
      setIsSearching(false)
    }
  }, [])

  // 防抖搜索
  useEffect(() => {
    if (searchTimeoutRef.current) {
      clearTimeout(searchTimeoutRef.current)
    }

    searchTimeoutRef.current = setTimeout(() => {
      if (searchQuery && showSearch) {
        handleSearch(searchQuery)
      }
    }, 500)

    return () => {
      if (searchTimeoutRef.current) {
        clearTimeout(searchTimeoutRef.current)
      }
    }
  }, [searchQuery, showSearch, handleSearch])

  const togglePlay = async () => {
    if (!currentTrack) {
      // 如果没有当前轨道，尝试播放第一首
      if (playlist.length > 0) {
        await loadAndPlayTrack(playlist[0])
        setCurrentTrackIndex(0)
      }
      return
    }

    if (isPlaying) {
      audioManager.stop()
      setIsPlaying(false)
    } else {
      await loadAndPlayTrack(currentTrack)
    }
  }

  const handleNext = useCallback(() => {
    if (playlist.length === 0) return

    const nextIndex = (currentTrackIndex + 1) % playlist.length
    setCurrentTrackIndex(nextIndex)
    setCurrentTime(0)
    audioManager.stop()
    setIsPlaying(false)
  }, [currentTrackIndex, playlist.length])

  const handlePrevious = useCallback(() => {
    if (playlist.length === 0) return

    const prevIndex = (currentTrackIndex - 1 + playlist.length) % playlist.length
    setCurrentTrackIndex(prevIndex)
    setCurrentTime(0)
    audioManager.stop()
    setIsPlaying(false)
  }, [currentTrackIndex, playlist.length])

  const handleVolumeChange = (newVolume: number) => {
    setVolume(newVolume)
    setIsMuted(false)
  }

  const toggleMute = () => {
    setIsMuted(!isMuted)
  }

  const addToPlaylistHandler = (track: SearchResult) => {
    addToPlaylist({
      id: track.id,
      name: track.name,
      artist: track.artist,
      album: track.album,
      duration: track.duration,
      cover: track.cover,
      platform: track.platform
    })
  }

  const toggleFavorite = (track: SearchResult | CurrentTrack) => {
    if (isFavorite(track.id)) {
      removeFromFavorites(track.id)
    } else {
      addToFavorites({
        id: track.id,
        name: track.name,
        artist: track.artist,
        album: track.album,
        duration: track.duration,
        cover: track.cover,
        platform: track.platform
      })
    }
  }

  const formatTime = (seconds: number) => {
    const mins = Math.floor(seconds / 60)
    const secs = seconds % 60
    return `${mins}:${secs.toString().padStart(2, '0')}`
  }

  const progressPercentage = currentTrack && duration > 0 ? (currentTime / duration) * 100 : 0

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
              {/* 顶部控制栏 */}
              <div className="flex items-center justify-between mb-4">
                <div className="flex items-center gap-2">
                  <Button
                    variant="ghost"
                    size="sm"
                    onClick={() => setShowSearch(!showSearch)}
                    className="h-8 w-8 p-0"
                  >
                    <Search className="w-4 h-4" />
                  </Button>
                  <Button
                    variant="ghost"
                    size="sm"
                    onClick={() => setShowSettings(true)}
                    className="h-8 w-8 p-0"
                  >
                    <Settings className="w-4 h-4" />
                  </Button>
                </div>
                <div className="text-xs text-text-muted">
                  {settings.platform === 'netease' && '网易云音乐'}
                  {settings.platform === 'qq' && 'QQ音乐'}
                  {settings.platform === 'local' && '本地音乐'}
                </div>
              </div>

              {/* 搜索面板 */}
              {showSearch && (
                <div className="mb-4 p-3 bg-bg-secondary rounded-lg">
                  <div className="flex items-center gap-2 mb-3">
                    <Search className="w-4 h-4 text-text-muted" />
                    <Input
                      placeholder="搜索音乐..."
                      value={searchQuery}
                      onChange={(e) => setSearchQuery(e.target.value)}
                      className="flex-1 h-8"
                    />
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => setShowSearch(false)}
                      className="h-8 w-8 p-0"
                    >
                      <X className="w-4 h-4" />
                    </Button>
                  </div>

                  {isSearching && (
                    <div className="text-center py-2 text-sm text-text-muted">搜索中...</div>
                  )}

                  {searchResults.length > 0 && (
                    <div className="max-h-40 overflow-y-auto space-y-1">
                      {searchResults.map((result) => (
                        <div key={result.id} className="flex items-center gap-2 p-2 rounded hover:bg-bg-tertiary">
                          <div className="flex-1 min-w-0">
                            <div className="font-medium text-sm truncate">{result.name}</div>
                            <div className="text-xs text-text-muted truncate">{result.artist}</div>
                          </div>
                          <div className="flex items-center gap-1">
                            <Button
                              variant="ghost"
                              size="sm"
                              onClick={() => toggleFavorite(result)}
                              className="h-6 w-6 p-0"
                            >
                              <Heart className={`w-3 h-3 ${isFavorite(result.id) ? 'fill-red-500 text-red-500' : ''}`} />
                            </Button>
                            <Button
                              variant="ghost"
                              size="sm"
                              onClick={() => addToPlaylistHandler(result)}
                              className="h-6 w-6 p-0"
                            >
                              <Plus className="w-3 h-3" />
                            </Button>
                          </div>
                        </div>
                      ))}
                    </div>
                  )}
                </div>
              )}

              {/* 当前播放信息 */}
              <div className="text-center mb-4">
                <div className="w-16 h-16 mx-auto mb-3 bg-gradient-to-br from-primary to-accent rounded-full flex items-center justify-center overflow-hidden">
                  {currentTrack?.cover ? (
                    <img src={currentTrack.cover} alt={currentTrack.name} className="w-full h-full object-cover" />
                  ) : (
                    <span className="text-2xl">🎵</span>
                  )}
                </div>
                <div className="font-semibold text-text">
                  {currentTrack?.name || '暂无播放'}
                </div>
                <div className="text-sm text-text-muted">
                  {currentTrack?.artist || '请添加音乐到播放列表'}
                </div>
                {error && (
                  <div className="text-xs text-red-500 mt-1">{error}</div>
                )}
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
                  <span>{formatTime(duration)}</span>
                </div>
              </div>

              {/* 控制按钮 */}
              <div className="flex items-center justify-center gap-4 mb-4">
                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={handlePrevious}
                  disabled={playlist.length === 0}
                  className="w-8 h-8 rounded-full bg-bg-secondary hover:bg-bg-tertiary transition-colors flex items-center justify-center disabled:opacity-50"
                >
                  <SkipBack className="w-4 h-4" />
                </motion.button>

                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={togglePlay}
                  disabled={isLoading || (playlist.length === 0 && !currentTrack)}
                  className="w-12 h-12 rounded-full bg-gradient-to-r from-primary to-accent hover:shadow-lg transition-all flex items-center justify-center disabled:opacity-50"
                >
                  {isLoading ? (
                    <motion.div
                      animate={{ rotate: 360 }}
                      transition={{ duration: 1, repeat: Infinity, ease: "linear" }}
                      className="w-5 h-5 border-2 border-white border-t-transparent rounded-full"
                    />
                  ) : isPlaying ? (
                    <Pause className="w-5 h-5 text-white" />
                  ) : (
                    <Play className="w-5 h-5 text-white" />
                  )}
                </motion.button>

                <motion.button
                  whileHover={{ scale: 1.1 }}
                  whileTap={{ scale: 0.9 }}
                  onClick={handleNext}
                  disabled={playlist.length === 0}
                  className="w-8 h-8 rounded-full bg-bg-secondary hover:bg-bg-tertiary transition-colors flex items-center justify-center disabled:opacity-50"
                >
                  <SkipForward className="w-4 h-4" />
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
                  {isMuted ? (
                    <VolumeX className="w-4 h-4" />
                  ) : volume > 0.5 ? (
                    <Volume2 className="w-4 h-4" />
                  ) : (
                    <Volume2 className="w-4 h-4" />
                  )}
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
                <div className="text-sm font-medium text-text mb-2 flex items-center justify-between">
                  <span>播放列表 ({playlist.length})</span>
                  {currentTrack && (
                    <Button
                      variant="ghost"
                      size="sm"
                      onClick={() => toggleFavorite(currentTrack)}
                      className="h-6 w-6 p-0"
                    >
                      <Heart className={`w-3 h-3 ${isFavorite(currentTrack.id) ? 'fill-red-500 text-red-500' : ''}`} />
                    </Button>
                  )}
                </div>
                {playlist.length === 0 ? (
                  <div className="text-center py-4 text-sm text-text-muted">
                    播放列表为空<br />
                    使用搜索功能添加音乐
                  </div>
                ) : (
                  <div className="space-y-1 max-h-32 overflow-y-auto">
                    {playlist.map((track, index) => (
                      <motion.div
                        key={track.id}
                        whileHover={{ scale: 1.02 }}
                        whileTap={{ scale: 0.98 }}
                        className={`flex items-center gap-2 p-2 rounded transition-colors ${
                          index === currentTrackIndex
                            ? 'bg-primary/20 text-primary'
                            : 'hover:bg-bg-secondary'
                        }`}
                      >
                        <button
                          onClick={() => {
                            setCurrentTrackIndex(index)
                            setCurrentTime(0)
                            audioManager.stop()
                            setIsPlaying(false)
                          }}
                          className="flex-1 text-left min-w-0"
                        >
                          <div className="font-medium text-sm truncate">{track.name}</div>
                          <div className="text-xs text-text-muted truncate">{track.artist}</div>
                        </button>
                        <Button
                          variant="ghost"
                          size="sm"
                          onClick={() => removeFromPlaylist(track.id)}
                          className="h-6 w-6 p-0 opacity-60 hover:opacity-100"
                        >
                          <X className="w-3 h-3" />
                        </Button>
                      </motion.div>
                    ))}
                  </div>
                )}
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

      {/* 音乐设置对话框 */}
      <MusicSettings
        open={showSettings}
        onOpenChange={setShowSettings}
      />
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