// path: src/hooks/useMusicSettings.ts
import { useLocalStorage } from './useLocalStorage'

export interface MusicPlatform {
  id: 'netease' | 'qq' | 'local'
  name: string
  description: string
  apiKeyRequired: boolean
  qualityLevels: string[]
}

export interface MusicSettings {
  platform: 'netease' | 'qq' | 'local'
  apiKey: string
  quality: string
  autoPlay: boolean
  volume: number
  lyricEnabled: boolean
  crossfade: number
  syncLibrary: boolean
}

export interface PlaylistItem {
  id: string
  name: string
  artist: string
  album?: string
  duration: number
  url?: string
  cover?: string
  platform: 'netease' | 'qq' | 'local'
  lyricUrl?: string
}

const defaultSettings: MusicSettings = {
  platform: 'local',
  apiKey: '',
  quality: 'standard',
  autoPlay: false,
  volume: 0.3,
  lyricEnabled: true,
  crossfade: 3,
  syncLibrary: false
}

export const musicPlatforms: MusicPlatform[] = [
  {
    id: 'netease',
    name: '网易云音乐',
    description: '支持网易云音乐平台的海量曲库',
    apiKeyRequired: true,
    qualityLevels: ['standard', 'higher', 'exhigh', 'lossless']
  },
  {
    id: 'qq',
    name: 'QQ音乐',
    description: '支持QQ音乐平台的丰富内容',
    apiKeyRequired: true,
    qualityLevels: ['size_128', 'size_320', 'size_flac']
  },
  {
    id: 'local',
    name: '本地音乐',
    description: '播放本地存储的音乐文件',
    apiKeyRequired: false,
    qualityLevels: ['original']
  }
]

export function useMusicSettings() {
  const [settings, setSettings] = useLocalStorage<MusicSettings>('gm-music-settings', defaultSettings)
  const [playlist, setPlaylist] = useLocalStorage<PlaylistItem[]>('gm-music-playlist', [])
  const [favorites, setFavorites] = useLocalStorage<PlaylistItem[]>('gm-music-favorites', [])

  const updateSettings = (updates: Partial<MusicSettings>) => {
    setSettings(prev => ({ ...prev, ...updates }))
  }

  const addToPlaylist = (item: PlaylistItem) => {
    setPlaylist(prev => {
      // 避免重复添加
      const exists = prev.find(track => track.id === item.id)
      if (exists) return prev
      return [...prev, item]
    })
  }

  const removeFromPlaylist = (id: string) => {
    setPlaylist(prev => prev.filter(item => item.id !== id))
  }

  const clearPlaylist = () => {
    setPlaylist([])
  }

  const addToFavorites = (item: PlaylistItem) => {
    setFavorites(prev => {
      const exists = prev.find(track => track.id === item.id)
      if (exists) return prev
      return [...prev, item]
    })
  }

  const removeFromFavorites = (id: string) => {
    setFavorites(prev => prev.filter(item => item.id !== id))
  }

  const isFavorite = (id: string) => {
    return favorites.some(item => item.id === id)
  }

  const getCurrentPlatform = () => {
    return musicPlatforms.find(p => p.id === settings.platform)
  }

  const validateApiKey = async (platform: string, apiKey: string): Promise<boolean> => {
    // 这里应该调用对应平台的验证API
    if (!apiKey && platform !== 'local') return false

    // 模拟验证延迟
    await new Promise(resolve => setTimeout(resolve, 1000))

    // 简单验证格式（实际应该调用API验证）
    if (platform === 'netease') {
      return apiKey.length >= 32 // 网易云API Key通常较长
    }
    if (platform === 'qq') {
      return apiKey.length >= 16 // QQ音乐API Key
    }
    return true // 本地音乐不需要验证
  }

  return {
    settings,
    playlist,
    favorites,
    musicPlatforms,
    updateSettings,
    addToPlaylist,
    removeFromPlaylist,
    clearPlaylist,
    addToFavorites,
    removeFromFavorites,
    isFavorite,
    getCurrentPlatform,
    validateApiKey
  }
}