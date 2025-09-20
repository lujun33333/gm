// path: src/utils/musicApi.ts
import { api } from './api'
import type { PlaylistItem, MusicSettings } from '@/hooks/useMusicSettings'

export interface SearchResult {
  id: string
  name: string
  artist: string
  album?: string
  duration: number
  cover?: string
  platform: 'netease' | 'qq' | 'local'
}

export interface LyricLine {
  time: number
  text: string
}

export interface LyricData {
  lines: LyricLine[]
  offset: number
}

// 音乐API客户端类
class MusicApiClient {
  private settings: MusicSettings | null = null

  setSettings(settings: MusicSettings) {
    this.settings = settings
  }

  // 搜索音乐
  async searchMusic(keyword: string, limit = 20): Promise<SearchResult[]> {
    if (!this.settings) {
      throw new Error('音乐设置未初始化')
    }

    const platform = this.settings.platform

    if (platform === 'local') {
      // 本地音乐搜索 - 模拟数据
      return this.searchLocalMusic(keyword, limit)
    }

    try {
      const response = await api.post('/music/search', {
        platform,
        keyword,
        limit,
        apiKey: this.settings.apiKey
      })

      if (response.code === 0) {
        return response.data.songs || []
      } else {
        throw new Error(response.msg || '搜索失败')
      }
    } catch (error) {
      console.error('搜索音乐失败:', error)

      // 降级到本地模拟数据
      return this.getMockSearchResults(keyword, limit)
    }
  }

  // 获取播放URL
  async getPlayUrl(songId: string, quality?: string): Promise<string> {
    if (!this.settings) {
      throw new Error('音乐设置未初始化')
    }

    const platform = this.settings.platform
    const requestQuality = quality || this.settings.quality

    if (platform === 'local') {
      // 本地文件URL
      return this.getLocalFileUrl(songId)
    }

    try {
      const response = await api.post('/music/url', {
        platform,
        songId,
        quality: requestQuality,
        apiKey: this.settings.apiKey
      })

      if (response.code === 0) {
        return response.data.url
      } else {
        throw new Error(response.msg || '获取播放链接失败')
      }
    } catch (error) {
      console.error('获取播放链接失败:', error)

      // 降级到模拟音频URL
      return this.getMockAudioUrl(songId)
    }
  }

  // 获取歌词
  async getLyric(songId: string): Promise<LyricData | null> {
    if (!this.settings) {
      throw new Error('音乐设置未初始化')
    }

    const platform = this.settings.platform

    if (platform === 'local') {
      // 本地歌词文件
      return this.getLocalLyric(songId)
    }

    try {
      const response = await api.post('/music/lyric', {
        platform,
        songId,
        apiKey: this.settings.apiKey
      })

      if (response.code === 0) {
        return this.parseLyric(response.data.lyric)
      } else {
        return null
      }
    } catch (error) {
      console.error('获取歌词失败:', error)
      return this.getMockLyric(songId)
    }
  }

  // 获取歌单推荐
  async getRecommendations(limit = 10): Promise<SearchResult[]> {
    if (!this.settings) {
      return this.getMockRecommendations(limit)
    }

    const platform = this.settings.platform

    try {
      const response = await api.post('/music/recommend', {
        platform,
        limit,
        apiKey: this.settings.apiKey
      })

      if (response.code === 0) {
        return response.data.songs || []
      } else {
        return this.getMockRecommendations(limit)
      }
    } catch (error) {
      console.error('获取推荐失败:', error)
      return this.getMockRecommendations(limit)
    }
  }

  // 验证API密钥
  async validateApiKey(platform: string, apiKey: string): Promise<boolean> {
    if (platform === 'local') return true

    try {
      const response = await api.post('/music/validate', {
        platform,
        apiKey
      })

      return response.code === 0
    } catch (error) {
      console.error('验证API密钥失败:', error)
      return false
    }
  }

  // === 私有方法 ===

  private async searchLocalMusic(keyword: string, limit: number): Promise<SearchResult[]> {
    // 模拟本地音乐搜索
    const localTracks = [
      {
        id: 'local_001',
        name: '宁静夜晚',
        artist: '环境音乐',
        album: '放松时光',
        duration: 180,
        cover: 'https://via.placeholder.com/300x300/4A90E2/FFFFFF?text=宁静夜晚',
        platform: 'local' as const
      },
      {
        id: 'local_002',
        name: '专注工作',
        artist: '轻音乐',
        album: '工作伴侣',
        duration: 240,
        cover: 'https://via.placeholder.com/300x300/7ED321/FFFFFF?text=专注工作',
        platform: 'local' as const
      },
      {
        id: 'local_003',
        name: '放松心情',
        artist: '纯音乐',
        album: '心灵治愈',
        duration: 200,
        cover: 'https://via.placeholder.com/300x300/F5A623/FFFFFF?text=放松心情',
        platform: 'local' as const
      }
    ]

    return localTracks.filter(track =>
      track.name.includes(keyword) ||
      track.artist.includes(keyword) ||
      track.album?.includes(keyword)
    ).slice(0, limit)
  }

  private getLocalFileUrl(songId: string): string {
    // 本地文件路径映射
    const localFiles: Record<string, string> = {
      'local_001': '/audio/calm_night.mp3',
      'local_002': '/audio/focus_work.mp3',
      'local_003': '/audio/relax_mood.mp3'
    }

    return localFiles[songId] || ''
  }

  private async getLocalLyric(songId: string): Promise<LyricData | null> {
    // 本地歌词数据
    const localLyrics: Record<string, string> = {
      'local_001': '[00:00.00]宁静夜晚\n[00:05.00]远离喧嚣\n[00:10.00]享受这份安详',
      'local_002': '[00:00.00]专注的力量\n[00:05.00]让思绪清晰\n[00:10.00]工作更高效',
      'local_003': '[00:00.00]放松心情\n[00:05.00]释放压力\n[00:10.00]重获内心平静'
    }

    const lyricText = localLyrics[songId]
    return lyricText ? this.parseLyric(lyricText) : null
  }

  private getMockSearchResults(keyword: string, limit: number): SearchResult[] {
    // 模拟搜索结果
    const mockResults: SearchResult[] = [
      {
        id: 'mock_001',
        name: `${keyword} - 热门歌曲`,
        artist: '热门歌手',
        album: '最新专辑',
        duration: 210,
        cover: 'https://via.placeholder.com/300x300/E74C3C/FFFFFF?text=热门',
        platform: this.settings?.platform || 'netease'
      },
      {
        id: 'mock_002',
        name: `经典 ${keyword}`,
        artist: '经典歌手',
        album: '经典合集',
        duration: 195,
        cover: 'https://via.placeholder.com/300x300/9B59B6/FFFFFF?text=经典',
        platform: this.settings?.platform || 'netease'
      },
      {
        id: 'mock_003',
        name: `${keyword} 纯音乐版`,
        artist: '纯音乐',
        album: '轻音乐',
        duration: 180,
        cover: 'https://via.placeholder.com/300x300/3498DB/FFFFFF?text=纯音乐',
        platform: this.settings?.platform || 'netease'
      }
    ]

    return mockResults.slice(0, limit)
  }

  private getMockAudioUrl(songId: string): string {
    // 使用免费的测试音频URL
    const testAudioUrls = [
      'https://www.soundjay.com/misc/sounds/bell-ringing-05.mp3',
      'https://www.soundjay.com/misc/sounds/clock-chimes-01.mp3',
      'https://www.soundjay.com/misc/sounds/fail-buzzer-02.mp3'
    ]

    // 根据songId选择不同的测试音频
    const index = songId.split('').reduce((acc, char) => acc + char.charCodeAt(0), 0) % testAudioUrls.length
    return testAudioUrls[index]
  }

  private getMockLyric(songId: string): LyricData {
    return {
      lines: [
        { time: 0, text: '这是模拟歌词' },
        { time: 5000, text: '展示歌词同步功能' },
        { time: 10000, text: '音乐让生活更美好' }
      ],
      offset: 0
    }
  }

  private getMockRecommendations(limit: number): SearchResult[] {
    const recommendations: SearchResult[] = [
      {
        id: 'rec_001',
        name: '今日推荐',
        artist: '推荐歌手',
        album: '热门榜单',
        duration: 220,
        cover: 'https://via.placeholder.com/300x300/E67E22/FFFFFF?text=推荐',
        platform: this.settings?.platform || 'netease'
      },
      {
        id: 'rec_002',
        name: '流行金曲',
        artist: '流行歌手',
        album: '流行专辑',
        duration: 200,
        cover: 'https://via.placeholder.com/300x300/1ABC9C/FFFFFF?text=流行',
        platform: this.settings?.platform || 'netease'
      }
    ]

    return recommendations.slice(0, limit)
  }

  private parseLyric(lyricText: string): LyricData {
    const lines: LyricLine[] = []
    const lyricLines = lyricText.split('\n')

    for (const line of lyricLines) {
      const match = line.match(/\[(\d{2}):(\d{2})\.(\d{2})\](.*)/)
      if (match) {
        const [, minutes, seconds, milliseconds, text] = match
        const time = parseInt(minutes) * 60000 +
                    parseInt(seconds) * 1000 +
                    parseInt(milliseconds) * 10

        if (text.trim()) {
          lines.push({ time, text: text.trim() })
        }
      }
    }

    return { lines, offset: 0 }
  }
}

// 导出单例实例
export const musicApi = new MusicApiClient()

// 音频上下文管理
class AudioManager {
  private audioContext: AudioContext | null = null
  private currentSource: AudioBufferSourceNode | null = null
  private gainNode: GainNode | null = null

  async init() {
    if (!this.audioContext) {
      this.audioContext = new (window.AudioContext || (window as any).webkitAudioContext)()
      this.gainNode = this.audioContext.createGain()
      this.gainNode.connect(this.audioContext.destination)
    }

    // 恢复音频上下文（某些浏览器需要用户交互后才能播放）
    if (this.audioContext.state === 'suspended') {
      await this.audioContext.resume()
    }
  }

  async loadAndPlay(url: string, volume: number = 1) {
    await this.init()

    if (!this.audioContext || !this.gainNode) {
      throw new Error('音频上下文初始化失败')
    }

    // 停止当前播放
    this.stop()

    try {
      let audioBuffer: AudioBuffer

      // 检查是否是本地生成的音频
      if (url.includes('local_') || url.includes('mock_')) {
        // 使用音频合成器生成测试音频
        const songId = url.split('/').pop()?.split('.')[0] || 'default'
        audioBuffer = await audioSynthesizer.createTestTrack(songId)
      } else {
        // 尝试加载真实音频文件
        try {
          const response = await fetch(url)
          const arrayBuffer = await response.arrayBuffer()
          audioBuffer = await this.audioContext.decodeAudioData(arrayBuffer)
        } catch (error) {
          console.warn('加载真实音频失败，使用合成音频:', error)
          // 如果加载失败，也使用合成音频
          audioBuffer = await audioSynthesizer.createTestTrack('default')
        }
      }

      // 创建音频源
      this.currentSource = this.audioContext.createBufferSource()
      this.currentSource.buffer = audioBuffer
      this.currentSource.connect(this.gainNode)

      // 设置音量
      this.gainNode.gain.value = volume

      // 开始播放
      this.currentSource.start()

      return {
        duration: audioBuffer.duration,
        sampleRate: audioBuffer.sampleRate
      }
    } catch (error) {
      console.error('音频播放失败:', error)
      throw new Error('音频播放失败')
    }
  }

  stop() {
    if (this.currentSource) {
      try {
        this.currentSource.stop()
      } catch (error) {
        // 忽略已经停止的错误
      }
      this.currentSource = null
    }
  }

  setVolume(volume: number) {
    if (this.gainNode) {
      this.gainNode.gain.value = Math.max(0, Math.min(1, volume))
    }
  }

  getContext() {
    return this.audioContext
  }
}

// 导出音频管理器单例
export const audioManager = new AudioManager()

// 导入音频合成器
import { audioSynthesizer } from './audioSynthesizer'