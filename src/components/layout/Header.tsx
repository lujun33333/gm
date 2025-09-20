import * as React from "react"
import { useNavigate } from "react-router-dom"
import { LogOut, User } from "lucide-react"
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
import { GlowCard } from "@/components/common/GlowCard"
import { useToast } from "@/hooks/useToast"

export function Header() {
  const navigate = useNavigate()
  const { toast } = useToast()
  const [logoutOpen, setLogoutOpen] = React.useState(false)

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
    <GlowCard className="flex items-center justify-between p-4">
      <div className="flex items-center space-x-4">
        <h1 className="text-lg font-semibold">GM 管理控制台</h1>
      </div>

      <div className="flex items-center space-x-3">
        {/* 用户信息 */}
        {userInfo && (
          <div className="flex items-center space-x-2 text-sm">
            <User className="w-4 h-4" />
            <span className="text-text-secondary">
              {userInfo.username} ({userInfo.role})
            </span>
          </div>
        )}

        {/* 退出登录 */}
        <Dialog open={logoutOpen} onOpenChange={setLogoutOpen}>
          <DialogTrigger asChild>
            <Button variant="ghost" size="icon" className="h-9 w-9">
              <LogOut className="h-4 w-4" />
              <span className="sr-only">退出登录</span>
            </Button>
          </DialogTrigger>
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