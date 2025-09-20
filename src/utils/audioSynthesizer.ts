// path: src/utils/audioSynthesizer.ts
/**
 * 音频合成器 - 用于生成测试音频
 * 在没有真实音频文件时，生成简单的音调作为测试
 */

interface ToneConfig {
  frequency: number
  duration: number
  volume: number
  waveType: OscillatorType
}

class AudioSynthesizer {
  private audioContext: AudioContext | null = null

  async init() {
    if (!this.audioContext) {
      this.audioContext = new (window.AudioContext || (window as any).webkitAudioContext)()
    }

    if (this.audioContext.state === 'suspended') {
      await this.audioContext.resume()
    }
  }

  // 生成音调
  async generateTone(config: ToneConfig): Promise<AudioBuffer> {
    await this.init()

    if (!this.audioContext) {
      throw new Error('音频上下文初始化失败')
    }

    const { frequency, duration, volume, waveType } = config
    const sampleRate = this.audioContext.sampleRate
    const length = sampleRate * duration
    const buffer = this.audioContext.createBuffer(1, length, sampleRate)
    const data = buffer.getChannelData(0)

    for (let i = 0; i < length; i++) {
      const angle = (frequency * 2 * Math.PI * i) / sampleRate
      let sample = 0

      switch (waveType) {
        case 'sine':
          sample = Math.sin(angle)
          break
        case 'square':
          sample = Math.sin(angle) > 0 ? 1 : -1
          break
        case 'sawtooth':
          sample = 2 * (angle / (2 * Math.PI) - Math.floor(angle / (2 * Math.PI) + 0.5))
          break
        case 'triangle':
          sample = 2 * Math.abs(2 * (angle / (2 * Math.PI) - Math.floor(angle / (2 * Math.PI) + 0.5))) - 1
          break
        default:
          sample = Math.sin(angle)
      }

      // 应用音量和淡入淡出
      const fadeLength = sampleRate * 0.1 // 0.1秒淡入淡出
      let envelope = 1

      if (i < fadeLength) {
        envelope = i / fadeLength // 淡入
      } else if (i > length - fadeLength) {
        envelope = (length - i) / fadeLength // 淡出
      }

      data[i] = sample * volume * envelope
    }

    return buffer
  }

  // 生成和弦
  async generateChord(frequencies: number[], duration: number, volume: number = 0.3): Promise<AudioBuffer> {
    await this.init()

    if (!this.audioContext) {
      throw new Error('音频上下文初始化失败')
    }

    const sampleRate = this.audioContext.sampleRate
    const length = sampleRate * duration
    const buffer = this.audioContext.createBuffer(1, length, sampleRate)
    const data = buffer.getChannelData(0)

    for (let i = 0; i < length; i++) {
      let sample = 0

      // 叠加多个频率
      for (const frequency of frequencies) {
        const angle = (frequency * 2 * Math.PI * i) / sampleRate
        sample += Math.sin(angle) / frequencies.length
      }

      // 应用音量和淡入淡出
      const fadeLength = sampleRate * 0.1
      let envelope = 1

      if (i < fadeLength) {
        envelope = i / fadeLength
      } else if (i > length - fadeLength) {
        envelope = (length - i) / fadeLength
      }

      data[i] = sample * volume * envelope
    }

    return buffer
  }

  // 生成旋律
  async generateMelody(notes: Array<{ frequency: number; duration: number }>, volume: number = 0.3): Promise<AudioBuffer> {
    await this.init()

    if (!this.audioContext) {
      throw new Error('音频上下文初始化失败')
    }

    const sampleRate = this.audioContext.sampleRate
    let totalLength = 0

    // 计算总长度
    for (const note of notes) {
      totalLength += sampleRate * note.duration
    }

    const buffer = this.audioContext.createBuffer(1, totalLength, sampleRate)
    const data = buffer.getChannelData(0)
    let currentPosition = 0

    // 生成每个音符
    for (const note of notes) {
      const noteLength = sampleRate * note.duration

      for (let i = 0; i < noteLength; i++) {
        if (currentPosition + i >= totalLength) break

        const angle = (note.frequency * 2 * Math.PI * i) / sampleRate
        let sample = Math.sin(angle)

        // 音符内的淡入淡出
        const fadeLength = Math.min(sampleRate * 0.05, noteLength / 4) // 0.05秒或音符长度的1/4
        let envelope = 1

        if (i < fadeLength) {
          envelope = i / fadeLength
        } else if (i > noteLength - fadeLength) {
          envelope = (noteLength - i) / fadeLength
        }

        data[currentPosition + i] = sample * volume * envelope
      }

      currentPosition += noteLength
    }

    return buffer
  }

  // 预定义的测试音频
  async createTestTrack(trackId: string): Promise<AudioBuffer> {
    const testTracks: Record<string, () => Promise<AudioBuffer>> = {
      'local_001': () => this.generateMelody([
        { frequency: 261.63, duration: 0.5 }, // C4
        { frequency: 293.66, duration: 0.5 }, // D4
        { frequency: 329.63, duration: 0.5 }, // E4
        { frequency: 349.23, duration: 0.5 }, // F4
        { frequency: 392.00, duration: 1.0 }, // G4
        { frequency: 392.00, duration: 0.5 }, // G4
        { frequency: 440.00, duration: 1.0 }, // A4
        { frequency: 440.00, duration: 0.5 }, // A4
        { frequency: 523.25, duration: 2.0 }  // C5
      ], 0.2),

      'local_002': () => this.generateChord([
        261.63, 329.63, 392.00 // C大调和弦
      ], 3.0, 0.15),

      'local_003': () => this.generateTone({
        frequency: 440,
        duration: 3.0,
        volume: 0.2,
        waveType: 'sine'
      }),

      'default': () => this.generateTone({
        frequency: 220,
        duration: 2.0,
        volume: 0.1,
        waveType: 'sine'
      })
    }

    const generator = testTracks[trackId] || testTracks['default']
    return generator()
  }

  // 播放生成的音频
  async playGeneratedAudio(buffer: AudioBuffer, volume: number = 1): Promise<AudioBufferSourceNode> {
    await this.init()

    if (!this.audioContext) {
      throw new Error('音频上下文初始化失败')
    }

    const source = this.audioContext.createBufferSource()
    const gainNode = this.audioContext.createGain()

    source.buffer = buffer
    gainNode.gain.value = volume

    source.connect(gainNode)
    gainNode.connect(this.audioContext.destination)

    source.start()
    return source
  }

  getContext() {
    return this.audioContext
  }
}

// 音符频率映射
export const NOTE_FREQUENCIES = {
  'C4': 261.63,
  'C#4': 277.18,
  'D4': 293.66,
  'D#4': 311.13,
  'E4': 329.63,
  'F4': 349.23,
  'F#4': 369.99,
  'G4': 392.00,
  'G#4': 415.30,
  'A4': 440.00,
  'A#4': 466.16,
  'B4': 493.88,
  'C5': 523.25
}

export const audioSynthesizer = new AudioSynthesizer()