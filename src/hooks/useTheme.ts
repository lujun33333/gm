import { useState, useEffect } from 'react'

type ThemeMode = 'light' | 'dark' | 'system' | 'scheduled'

interface ThemeSettings {
  mode: ThemeMode
  darkStart?: number // 定时深色模式开始时间（24小时制）
  darkEnd?: number   // 定时深色模式结束时间（24小时制）
}

const STORAGE_KEY = 'gm-theme-settings'

function getSystemTheme(): 'light' | 'dark' {
  return window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light'
}

function getScheduledTheme(settings: ThemeSettings): 'light' | 'dark' {
  const now = new Date()
  const hour = now.getHours()
  const darkStart = settings.darkStart || 18
  const darkEnd = settings.darkEnd || 6

  if (darkStart > darkEnd) {
    // 跨午夜，如 18:00 - 6:00
    return (hour >= darkStart || hour < darkEnd) ? 'dark' : 'light'
  } else {
    // 同一天，如 6:00 - 18:00
    return (hour >= darkStart && hour < darkEnd) ? 'dark' : 'light'
  }
}

function applyTheme(theme: 'light' | 'dark') {
  const html = document.documentElement
  html.setAttribute('data-theme', theme)
}

function getCurrentTheme(settings: ThemeSettings): 'light' | 'dark' {
  switch (settings.mode) {
    case 'light':
      return 'light'
    case 'dark':
      return 'dark'
    case 'scheduled':
      return getScheduledTheme(settings)
    default:
      return getSystemTheme()
  }
}

export function useTheme() {
  const [settings, setSettings] = useState<ThemeSettings>(() => {
    try {
      const stored = localStorage.getItem(STORAGE_KEY)
      return stored ? JSON.parse(stored) : { mode: 'system' }
    } catch {
      return { mode: 'system' as ThemeMode }
    }
  })

  const [currentTheme, setCurrentTheme] = useState<'light' | 'dark'>(() =>
    getCurrentTheme(settings)
  )

  // 监听系统主题变化
  useEffect(() => {
    const mediaQuery = window.matchMedia('(prefers-color-scheme: dark)')

    const handleChange = () => {
      if (settings.mode === 'system') {
        const newTheme = getSystemTheme()
        setCurrentTheme(newTheme)
        applyTheme(newTheme)
      }
    }

    mediaQuery.addEventListener('change', handleChange)
    return () => mediaQuery.removeEventListener('change', handleChange)
  }, [settings.mode])

  // 定时模式监听
  useEffect(() => {
    if (settings.mode !== 'scheduled') return

    const updateScheduledTheme = () => {
      const newTheme = getScheduledTheme(settings)
      setCurrentTheme(newTheme)
      applyTheme(newTheme)
    }

    // 每分钟检查一次
    const interval = setInterval(updateScheduledTheme, 60000)

    // 立即检查一次
    updateScheduledTheme()

    return () => clearInterval(interval)
  }, [settings.mode, settings.darkStart, settings.darkEnd])

  // 应用主题变化
  useEffect(() => {
    const newTheme = getCurrentTheme(settings)
    setCurrentTheme(newTheme)
    applyTheme(newTheme)
  }, [settings])

  // 保存设置到 localStorage
  useEffect(() => {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(settings))
  }, [settings])

  const updateSettings = (newSettings: Partial<ThemeSettings>) => {
    setSettings(prev => ({ ...prev, ...newSettings }))
  }

  return {
    settings,
    currentTheme,
    updateSettings,
    isLight: currentTheme === 'light',
    isDark: currentTheme === 'dark',
  }
}