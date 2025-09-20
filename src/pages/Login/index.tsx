import { useState } from 'react'
import { motion } from 'framer-motion'
import { useNavigate } from 'react-router-dom'
import { GlowCard } from '@/components/common/GlowCard'
import { Button } from '@/components/ui/button'
import { Input } from '@/components/ui/input'
import { useToast } from '@/hooks/useToast'

export function Login() {
  const navigate = useNavigate()
  const [loading, setLoading] = useState(false)
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')
  const { toast } = useToast()

  const handleLogin = async (e: React.FormEvent) => {
    e.preventDefault()
    setLoading(true)

    try {
      // 模拟登录逻辑
      await new Promise(resolve => setTimeout(resolve, 1000))

      if (username && password) {
        // 存储用户信息到 localStorage
        localStorage.setItem('gm-user', JSON.stringify({
          username,
          role: username === 'admin' ? 'OWNER' : username === 'manager' ? 'SUB' : 'PLAYER',
          permissions: username === 'admin' ? ['*'] : []
        }))

        toast({
          title: "登录成功",
          description: `欢迎回来，${username}！`,
        })

        navigate('/dashboard')
      } else {
        throw new Error('请输入用户名和密码')
      }
    } catch (err) {
      toast({
        title: "登录失败",
        description: err instanceof Error ? err.message : '登录失败',
        variant: "destructive",
      })
    } finally {
      setLoading(false)
    }
  }

  return (
    <div className="min-h-screen flex items-center justify-center p-4 bg-gradient-to-br from-bg to-bg-secondary">
      <motion.div
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.3 }}
        className="w-full max-w-md"
      >
        <GlowCard className="p-8">
          <div className="text-center mb-8">
            <h1 className="text-3xl font-bold bg-gradient-1 bg-clip-text text-transparent">
              GM 工具桌面端
            </h1>
            <p className="text-text-muted mt-2">管理员工具登录</p>
          </div>

          <form onSubmit={handleLogin} className="space-y-4">
            <div className="space-y-2">
              <label className="text-sm font-medium">用户名</label>
              <Input
                type="text"
                placeholder="请输入用户名"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
                required
              />
            </div>

            <div className="space-y-2">
              <label className="text-sm font-medium">密码</label>
              <Input
                type="password"
                placeholder="请输入密码"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                required
              />
            </div>

            <Button
              type="submit"
              className="w-full"
              disabled={loading}
              variant="shimmer"
            >
              {loading ? '登录中...' : '登录'}
            </Button>
          </form>

          <div className="mt-6 pt-6 border-t border-border">
            <p className="text-text-muted text-sm text-center">
              测试账号：admin/manager/player，任意密码
            </p>
          </div>
        </GlowCard>
      </motion.div>
    </div>
  )
}