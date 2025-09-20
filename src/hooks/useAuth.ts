import { useState, useEffect, createContext, useContext, ReactNode, createElement } from 'react'

export interface User {
  id: string
  username: string
  role: string
  permissions: string[]
  avatar?: string
  email?: string
}

interface AuthContextType {
  user: User | null
  isLoading: boolean
  login: (username: string, password: string) => Promise<void>
  logout: () => void
  hasPermission: (permission: string) => boolean
}

const AuthContext = createContext<AuthContextType | undefined>(undefined)

export function useAuth() {
  const context = useContext(AuthContext)
  if (context === undefined) {
    throw new Error('useAuth must be used within an AuthProvider')
  }
  return context
}

interface AuthProviderProps {
  children: ReactNode
}

export function AuthProvider({ children }: AuthProviderProps) {
  const [user, setUser] = useState<User | null>(null)
  const [isLoading, setIsLoading] = useState(true)

  // 模拟用户数据
  const MOCK_USERS = {
    admin: {
      id: '1',
      username: 'admin',
      role: 'OWNER',
      permissions: [
        'MONITOR_VIEW',
        'SERVER_CONTROL',
        'PLAYER_VIEW',
        'PLAYER_EDIT',
        'PLAYER_BAN',
        'INVENTORY_VIEW',
        'ITEM_SEND',
        'ITEM_ADD',
        'ITEM_EDIT',
        'ITEM_DELETE',
        'CAR_EDIT',
        'NOBLE_EDIT',
        'SIGNIN_EDIT',
        'PERM_ADMIN'
      ],
      avatar: 'https://api.dicebear.com/7.x/avataaars/svg?seed=admin',
      email: 'admin@example.com'
    },
    manager: {
      id: '2',
      username: 'manager',
      role: 'SUB',
      permissions: [
        'MONITOR_VIEW',
        'PLAYER_VIEW',
        'PLAYER_EDIT',
        'INVENTORY_VIEW',
        'ITEM_SEND',
        'ITEM_ADD',
        'CAR_EDIT',
        'NOBLE_EDIT'
      ],
      avatar: 'https://api.dicebear.com/7.x/avataaars/svg?seed=manager',
      email: 'manager@example.com'
    },
    player: {
      id: '3',
      username: 'player',
      role: 'PLAYER',
      permissions: [
        'PLAYER_VIEW',
        'INVENTORY_VIEW'
      ],
      avatar: 'https://api.dicebear.com/7.x/avataaars/svg?seed=player',
      email: 'player@example.com'
    }
  }

  // 初始化检查登录状态
  useEffect(() => {
    const checkAuthStatus = async () => {
      try {
        const storedAuth = localStorage.getItem('gm-auth')
        if (storedAuth) {
          const userData = JSON.parse(storedAuth)
          setUser(userData)
        }
      } catch (error) {
        console.error('Failed to restore auth:', error)
        localStorage.removeItem('gm-auth')
      } finally {
        setIsLoading(false)
      }
    }

    checkAuthStatus()
  }, [])

  const login = async (username: string, password: string) => {
    setIsLoading(true)

    try {
      // 模拟网络延迟
      await new Promise(resolve => setTimeout(resolve, 800))

      // 查找用户（接受任意密码）
      const userData = MOCK_USERS[username as keyof typeof MOCK_USERS]

      if (!userData) {
        throw new Error('用户名或密码错误')
      }

      // 保存到本地存储
      localStorage.setItem('gm-auth', JSON.stringify(userData))
      setUser(userData)
    } catch (error) {
      throw error
    } finally {
      setIsLoading(false)
    }
  }

  const logout = () => {
    localStorage.removeItem('gm-auth')
    setUser(null)
  }

  const hasPermission = (permission: string): boolean => {
    if (!user) return false
    return user.permissions.includes(permission)
  }

  const value: AuthContextType = {
    user,
    isLoading,
    login,
    logout,
    hasPermission
  }

  return createElement(AuthContext.Provider, { value }, children)
}