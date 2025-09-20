import * as React from "react"
import { useNavigate } from "react-router-dom"
import { LogOut, User, Settings, ChevronDown } from "lucide-react"
import { Button } from "@/components/ui/button"
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog"
import { GlowCard } from '@/components/fx/GlowCard'
import { ThemeSwitch } from '@/components/common/ThemeSwitch'
import { useToast } from "@/hooks/useToast"

export function Header() {
  const navigate = useNavigate()
  const { toast } = useToast()
  const [logoutOpen, setLogoutOpen] = React.useState(false)
  const [userMenuOpen, setUserMenuOpen] = React.useState(false)

  // 获取用户信息
  const userInfo = React.useMemo(() => {
    try {
      const stored = localStorage.getItem('gm-user')
      return stored ? JSON.parse(stored) : null
    } catch {
      return null
    }
  }, [])

  const handleLogout = () => {
    localStorage.removeItem('gm-user')
    toast({
      title: "已退出登录",
      description: "您已成功退出系统",
    })
    navigate('/login')
    setLogoutOpen(false)
  }

  return (
    <GlowCard className="flex items-center justify-between px-6 py-4 backdrop-blur-xl bg-bg/90 border-b border-border/50">
      <div className="flex items-center space-x-6">
        <div className="flex items-center space-x-3">
          {/* Logo图标 */}
          <div className="flex items-center justify-center w-8 h-8 rounded-lg bg-gradient-to-br from-primary to-accent shadow-lg">
            <span className="text-white font-bold text-sm">GM</span>
          </div>
          <h1 className="text-xl font-bold bg-gradient-to-r from-primary to-accent bg-clip-text text-transparent">
            管理控制台
          </h1>
        </div>
      </div>

      <div className="flex items-center space-x-4">
        {/* 主题切换按钮 */}
        <ThemeSwitch />

        {/* 用户头像按钮 */}
        {userInfo && (
          <Dialog open={userMenuOpen} onOpenChange={setUserMenuOpen}>
            <DialogTrigger asChild>
              <Button variant="ghost" className="relative p-0 hover:bg-transparent group">
                <div className="relative">
                  {/* 外层光环 */}
                  <div className="absolute inset-0 rounded-full bg-gradient-to-r from-primary/30 to-accent/30 blur-sm group-hover:blur-md transition-all duration-300 group-hover:scale-110"></div>

                  {/* 主头像 */}
                  <div className="relative flex h-11 w-11 items-center justify-center rounded-full bg-gradient-to-br from-primary via-accent to-primary shadow-lg group-hover:shadow-xl transition-all duration-300 group-hover:scale-105 border-2 border-white/10 group-hover:border-white/20">
                    <span className="text-white font-bold text-base tracking-wide">
                      {userInfo.username?.charAt(0)?.toUpperCase() || 'U'}
                    </span>

                    {/* 顶部高光 */}
                    <div className="absolute top-1 left-1/2 transform -translate-x-1/2 w-3 h-1.5 bg-white/30 rounded-full blur-sm"></div>
                  </div>

                  {/* 在线状态指示器 */}
                  <div className="absolute -bottom-0.5 -right-0.5 w-3 h-3 bg-success rounded-full border-2 border-bg shadow-sm">
                    <div className="w-full h-full bg-success rounded-full animate-pulse"></div>
                  </div>
                </div>
              </Button>
            </DialogTrigger>
            <DialogContent className="w-96 border-0 bg-bg/95 backdrop-blur-xl shadow-2xl">
              <DialogHeader>
                <DialogTitle className="flex items-center space-x-4 pb-4">
                  {/* 对话框中的头像 */}
                  <div className="relative">
                    <div className="flex h-16 w-16 items-center justify-center rounded-full bg-gradient-to-br from-primary via-accent to-primary shadow-xl border-4 border-white/10">
                      <span className="text-white font-bold text-xl tracking-wide">
                        {userInfo.username?.charAt(0)?.toUpperCase() || 'U'}
                      </span>
                      {/* 顶部高光 */}
                      <div className="absolute top-2 left-1/2 transform -translate-x-1/2 w-4 h-2 bg-white/30 rounded-full blur-sm"></div>
                    </div>

                    {/* 在线状态 */}
                    <div className="absolute -bottom-1 -right-1 w-5 h-5 bg-success rounded-full border-3 border-bg shadow-lg">
                      <div className="w-full h-full bg-success rounded-full animate-pulse"></div>
                    </div>
                  </div>

                  <div className="flex-1">
                    <p className="text-xl font-bold text-text mb-1">{userInfo.username}</p>
                    <p className="text-sm text-text-muted mb-2">
                      {userInfo.role === 'OWNER' ? '🔱 超级管理员' :
                       userInfo.role === 'SUB' ? '⚡ 子管理员' : '👤 用户'}
                    </p>
                    <div className="flex items-center text-xs text-success">
                      <div className="w-2 h-2 bg-success rounded-full mr-2 animate-pulse"></div>
                      在线中
                    </div>
                  </div>
                </DialogTitle>
              </DialogHeader>
              <div className="space-y-3 py-6">
                <Button
                  variant="ghost"
                  className="w-full justify-start h-12 px-4 rounded-xl hover:bg-bg-secondary/50 transition-all duration-200 group"
                >
                  <div className="flex items-center justify-center w-8 h-8 rounded-lg bg-primary/10 group-hover:bg-primary/20 transition-colors mr-3">
                    <Settings className="h-4 w-4 text-primary" />
                  </div>
                  <span className="font-medium">个人设置</span>
                </Button>

                <Button
                  variant="ghost"
                  className="w-full justify-start h-12 px-4 rounded-xl text-error hover:text-error hover:bg-error/5 transition-all duration-200 group"
                  onClick={() => {
                    setUserMenuOpen(false)
                    setLogoutOpen(true)
                  }}
                >
                  <div className="flex items-center justify-center w-8 h-8 rounded-lg bg-error/10 group-hover:bg-error/20 transition-colors mr-3">
                    <LogOut className="h-4 w-4 text-error" />
                  </div>
                  <span className="font-medium">退出登录</span>
                </Button>
              </div>
            </DialogContent>
          </Dialog>
        )}

        {/* 退出登录确认对话框 */}
        <Dialog open={logoutOpen} onOpenChange={setLogoutOpen}>
          <DialogContent>
            <DialogHeader>
              <DialogTitle>确认退出</DialogTitle>
              <DialogDescription>
                您确定要退出登录吗？退出后需要重新登录才能使用系统。
              </DialogDescription>
            </DialogHeader>
            <DialogFooter>
              <Button variant="outline" onClick={() => setLogoutOpen(false)}>
                取消
              </Button>
              <Button variant="destructive" onClick={handleLogout}>
                确认退出
              </Button>
            </DialogFooter>
          </DialogContent>
        </Dialog>
      </div>
    </GlowCard>
  )
}