import { createContext, useContext, useEffect, useState, ReactNode } from 'react'

type ThemeMode = 'light' | 'dark' | 'system' | 'scheduled'
type Theme = 'light' | 'dark'

interface ThemeSettings {
  mode: ThemeMode
  darkStart: number // 深色模式开始时间（小时）
  darkEnd: number   // 深色模式结束时间（小时）
}

interface ThemeContextType {
  theme: Theme
  settings: ThemeSettings
  setMode: (mode: ThemeMode) => void
  setSchedule: (darkStart: number, darkEnd: number) => void
}

const ThemeContext = createContext<ThemeContextType | undefined>(undefined)

export function useTheme() {
  const context = useContext(ThemeContext)
  if (context === undefined) {
    throw new Error('useTheme must be used within a ThemeProvider')
  }
  return context
}

const DEFAULT_SETTINGS: ThemeSettings = {
  mode: 'system',
  darkStart: 18, // 18:00
  darkEnd: 6     // 6:00
}

function getStoredSettings(): ThemeSettings {
  try {
    const stored = localStorage.getItem('gm-theme-settings')
    return stored ? { ...DEFAULT_SETTINGS, ...JSON.parse(stored) } : DEFAULT_SETTINGS
  } catch {
    return DEFAULT_SETTINGS
  }
}

function getSystemTheme(): Theme {
  return window.matchMedia('(prefers-color-scheme: dark)').matches ? 'dark' : 'light'
}

function getScheduledTheme(settings: ThemeSettings): Theme {
  const now = new Date()
  const hour = now.getHours()
  const { darkStart, darkEnd } = settings

  if (darkStart > darkEnd) {
    // 跨午夜，如 18:00 - 6:00
    return (hour >= darkStart || hour < darkEnd) ? 'dark' : 'light'
  } else {
    // 同一天，如 6:00 - 18:00
    return (hour >= darkStart && hour < darkEnd) ? 'dark' : 'light'
  }
}

function calculateTheme(settings: ThemeSettings): Theme {
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

function applyTheme(theme: Theme) {
  const html = document.documentElement
  html.className = html.className.replace(/theme-\w+/g, '')
  html.classList.add(`theme-${theme}`)
  html.setAttribute('data-theme', theme)
}

interface ThemeProviderProps {
  children: ReactNode
}

export function ThemeProvider({ children }: ThemeProviderProps) {
  const [settings, setSettings] = useState<ThemeSettings>(getStoredSettings)
  const [theme, setTheme] = useState<Theme>(() => calculateTheme(settings))

  // 保存设置到 localStorage
  useEffect(() => {
    localStorage.setItem('gm-theme-settings', JSON.stringify(settings))
  }, [settings])

  // 计算并应用主题
  useEffect(() => {
    const newTheme = calculateTheme(settings)
    setTheme(newTheme)
    applyTheme(newTheme)
  }, [settings])

  // 监听系统主题变化
  useEffect(() => {
    if (settings.mode !== 'system') return

    const mediaQuery = window.matchMedia('(prefers-color-scheme: dark)')
    const handleChange = () => {
      const newTheme = calculateTheme(settings)
      setTheme(newTheme)
      applyTheme(newTheme)
    }

    mediaQuery.addEventListener('change', handleChange)
    return () => mediaQuery.removeEventListener('change', handleChange)
  }, [settings])

  // 定时检查（仅 scheduled 模式）
  useEffect(() => {
    if (settings.mode !== 'scheduled') return

    const interval = setInterval(() => {
      const newTheme = calculateTheme(settings)
      if (newTheme !== theme) {
        setTheme(newTheme)
        applyTheme(newTheme)
      }
    }, 15 * 60 * 1000) // 15分钟检查一次

    return () => clearInterval(interval)
  }, [settings, theme])

  const setMode = (mode: ThemeMode) => {
    setSettings(prev => ({ ...prev, mode }))
  }

  const setSchedule = (darkStart: number, darkEnd: number) => {
    setSettings(prev => ({ ...prev, darkStart, darkEnd }))
  }

  const value: ThemeContextType = {
    theme,
    settings,
    setMode,
    setSchedule
  }

  return (
    <ThemeContext.Provider value={value}>
      {children}
    </ThemeContext.Provider>
  )
}