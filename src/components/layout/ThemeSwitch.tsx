import { useState } from 'react'
import { motion, AnimatePresence } from 'framer-motion'
import { useTheme } from '../../app/theme/ThemeContext'

const THEME_OPTIONS = [
  { key: 'light' as const, label: '浅色', icon: '☀️' },
  { key: 'dark' as const, label: '深色', icon: '🌙' },
  { key: 'system' as const, label: '跟随系统', icon: '🖥️' },
  { key: 'scheduled' as const, label: '定时', icon: '⏰' },
]

export function ThemeSwitch() {
  const { settings, setMode, setSchedule } = useTheme()
  const [showMenu, setShowMenu] = useState(false)
  const [showSchedule, setShowSchedule] = useState(false)

  const currentOption = THEME_OPTIONS.find(opt => opt.key === settings.mode) || THEME_OPTIONS[0]

  const handleModeChange = (mode: typeof settings.mode) => {
    setMode(mode)
    if (mode !== 'scheduled') {
      setShowSchedule(false)
    } else {
      setShowSchedule(true)
    }
    setShowMenu(false)
  }

  const handleScheduleSubmit = (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault()
    const formData = new FormData(e.currentTarget)
    const darkStart = parseInt(formData.get('darkStart') as string)
    const darkEnd = parseInt(formData.get('darkEnd') as string)
    setSchedule(darkStart, darkEnd)
    setShowSchedule(false)
  }

  return (
    <div className="relative">
      <motion.button
        whileHover={{ scale: 1.05 }}
        whileTap={{ scale: 0.95 }}
        onClick={() => setShowMenu(!showMenu)}
        className="flex items-center gap-2 px-3 py-2 rounded-lg bg-bg-secondary border border-border hover:bg-bg-tertiary transition-colors"
      >
        <span>{currentOption.icon}</span>
        <span className="text-sm">{currentOption.label}</span>
        <motion.span
          animate={{ rotate: showMenu ? 180 : 0 }}
          transition={{ duration: 0.18 }}
        >
          ⌄
        </motion.span>
      </motion.button>

      <AnimatePresence>
        {showMenu && (
          <motion.div
            initial={{ opacity: 0, scale: 0.9, y: -10 }}
            animate={{ opacity: 1, scale: 1, y: 0 }}
            exit={{ opacity: 0, scale: 0.9, y: -10 }}
            transition={{ duration: 0.15 }}
            className="absolute top-full right-0 mt-2 w-48 glass-card rounded-lg p-2 z-50"
          >
            {THEME_OPTIONS.map((option) => (
              <motion.button
                key={option.key}
                whileHover={{ backgroundColor: 'rgb(var(--bg-tertiary))' }}
                onClick={() => handleModeChange(option.key)}
                className={`w-full flex items-center gap-3 px-3 py-2 rounded text-left transition-colors ${
                  option.key === settings.mode ? 'bg-primary text-white' : ''
                }`}
              >
                <span>{option.icon}</span>
                <span className="text-sm">{option.label}</span>
              </motion.button>
            ))}
          </motion.div>
        )}
      </AnimatePresence>

      <AnimatePresence>
        {showSchedule && (
          <motion.div
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
            exit={{ opacity: 0, scale: 0.9 }}
            className="fixed inset-0 bg-bg/50 backdrop-blur-sm flex items-center justify-center z-50"
            onClick={(e) => e.target === e.currentTarget && setShowSchedule(false)}
          >
            <motion.form
              onSubmit={handleScheduleSubmit}
              className="glass-card rounded-lg p-6 w-80"
              onClick={(e) => e.stopPropagation()}
            >
              <h3 className="text-lg font-semibold mb-4">设置定时主题</h3>

              <div className="space-y-4">
                <div>
                  <label className="block text-sm font-medium mb-1">深色开始时间</label>
                  <select
                    name="darkStart"
                    defaultValue={settings.darkStart}
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  >
                    {Array.from({ length: 24 }, (_, i) => (
                      <option key={i} value={i}>
                        {i.toString().padStart(2, '0')}:00
                      </option>
                    ))}
                  </select>
                </div>

                <div>
                  <label className="block text-sm font-medium mb-1">深色结束时间</label>
                  <select
                    name="darkEnd"
                    defaultValue={settings.darkEnd}
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  >
                    {Array.from({ length: 24 }, (_, i) => (
                      <option key={i} value={i}>
                        {i.toString().padStart(2, '0')}:00
                      </option>
                    ))}
                  </select>
                </div>
              </div>

              <div className="flex gap-3 mt-6">
                <motion.button
                  type="button"
                  whileHover={{ scale: 1.02 }}
                  whileTap={{ scale: 0.98 }}
                  onClick={() => setShowSchedule(false)}
                  className="flex-1 px-4 py-2 bg-bg-secondary border border-border rounded-lg hover:bg-bg-tertiary transition-colors"
                >
                  取消
                </motion.button>
                <motion.button
                  type="submit"
                  whileHover={{ scale: 1.02 }}
                  whileTap={{ scale: 0.98 }}
                  className="flex-1 px-4 py-2 bg-primary text-white rounded-lg hover:bg-primary-hover transition-colors"
                >
                  确定
                </motion.button>
              </div>
            </motion.form>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  )
}