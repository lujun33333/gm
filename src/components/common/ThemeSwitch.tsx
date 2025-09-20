import * as React from "react"
import { Sun, Moon, Monitor, Clock } from "lucide-react"
import { Button } from "@/components/ui/button"
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog"
import { useTheme } from "@/hooks/useTheme"

const themeOptions = [
  { value: 'light', label: '浅色', icon: Sun },
  { value: 'dark', label: '深色', icon: Moon },
  { value: 'system', label: '跟随系统', icon: Monitor },
  { value: 'scheduled', label: '定时切换', icon: Clock },
] as const

export function ThemeSwitch() {
  const { settings, currentTheme, updateSettings } = useTheme()
  const [open, setOpen] = React.useState(false)

  const currentOption = themeOptions.find(opt => opt.value === settings.mode)
  const Icon = currentOption?.icon || Sun

  return (
    <Dialog open={open} onOpenChange={setOpen}>
      <DialogTrigger asChild>
        <Button variant="ghost" size="icon" className="h-9 w-9">
          <Icon className="h-4 w-4" />
          <span className="sr-only">切换主题</span>
        </Button>
      </DialogTrigger>
      <DialogContent className="sm:max-w-md">
        <DialogHeader>
          <DialogTitle>主题设置</DialogTitle>
          <DialogDescription>
            选择你喜欢的主题模式。当前主题：{currentTheme === 'light' ? '浅色' : '深色'}
          </DialogDescription>
        </DialogHeader>
        <div className="space-y-4">
          <div className="grid grid-cols-2 gap-2">
            {themeOptions.map((option) => {
              const OptionIcon = option.icon
              return (
                <Button
                  key={option.value}
                  variant={settings.mode === option.value ? "default" : "outline"}
                  className="justify-start gap-2 h-12"
                  onClick={() => updateSettings({ mode: option.value })}
                >
                  <OptionIcon className="h-4 w-4" />
                  {option.label}
                </Button>
              )
            })}
          </div>

          {settings.mode === 'scheduled' && (
            <div className="space-y-3 pt-4 border-t border-border">
              <h4 className="text-sm font-medium">定时设置</h4>
              <div className="flex items-center gap-3">
                <div className="flex-1">
                  <label className="text-xs text-text-muted">深色开始</label>
                  <input
                    type="time"
                    className="w-full mt-1 px-3 py-2 text-sm border border-border rounded-md bg-bg"
                    value={`${settings.darkStart || 18}:00`}
                    onChange={(e) => {
                      const hour = parseInt(e.target.value.split(':')[0])
                      updateSettings({ darkStart: hour })
                    }}
                  />
                </div>
                <div className="flex-1">
                  <label className="text-xs text-text-muted">深色结束</label>
                  <input
                    type="time"
                    className="w-full mt-1 px-3 py-2 text-sm border border-border rounded-md bg-bg"
                    value={`${(settings.darkEnd || 6).toString().padStart(2, '0')}:00`}
                    onChange={(e) => {
                      const hour = parseInt(e.target.value.split(':')[0])
                      updateSettings({ darkEnd: hour })
                    }}
                  />
                </div>
              </div>
              <p className="text-xs text-text-muted">
                在 {settings.darkStart || 18}:00 - {(settings.darkEnd || 6).toString().padStart(2, '0')}:00 期间使用深色主题
              </p>
            </div>
          )}
        </div>
      </DialogContent>
    </Dialog>
  )
}