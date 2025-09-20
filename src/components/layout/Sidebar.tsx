import { useState, useEffect } from 'react'
import { motion, AnimatePresence } from 'framer-motion'
import { useNavigate, useLocation } from 'react-router-dom'
import { GlowCard } from '@/components/fx/GlowCard'
import { ThemeSwitch } from '@/components/common/ThemeSwitch'
import { Permit } from '@/components/common/Permit'
import { Button } from '@/components/ui/button'
import {
  LayoutDashboard,
  Users,
  Package,
  Send,
  Settings,
  Menu,
  X,
  Car,
  Crown,
  Calendar
} from "lucide-react"
import { clsx } from "clsx"

interface SidebarProps {
  isOpen: boolean
  onToggle: () => void
  isMobile: boolean
}

interface MenuItem {
  key: string
  label: string
  icon: React.ComponentType<any>
  path?: string
  permission?: string
  children?: MenuItem[]
}

const MENU_ITEMS: MenuItem[] = [
  { key: 'dashboard', label: '仪表盘', icon: LayoutDashboard, path: '/dashboard', permission: 'MONITOR_VIEW' },
  {
    key: 'player',
    label: '玩家管理',
    icon: Users,
    permission: 'PLAYER_VIEW',
    children: [
      { key: 'player-profile', label: '玩家资料', icon: Users, path: '/player/profile', permission: 'PLAYER_VIEW' },
      { key: 'player-inventory', label: '玩家背包', icon: Package, path: '/player/inventory', permission: 'INVENTORY_VIEW' },
      { key: 'player-cars', label: '玩家载具', icon: Car, path: '/player/cars', permission: 'CAR_EDIT' },
      { key: 'player-vip', label: '贵族等级', icon: Crown, path: '/player/vip', permission: 'NOBLE_EDIT' },
      { key: 'player-signin', label: '签到管理', icon: Calendar, path: '/player/signin', permission: 'SIGNIN_EDIT' },
    ]
  },
  { key: 'items', label: '道具管理', icon: Package, path: '/items', permission: 'ITEM_ADD' },
  { key: 'send-item', label: '发送道具', icon: Send, path: '/send-item', permission: 'ITEM_SEND' },
  { key: 'permissions', label: '权限管理', icon: Settings, path: '/permissions', permission: 'PERM_ADMIN' },
]

export function Sidebar({ isOpen, onToggle, isMobile }: SidebarProps) {
  const navigate = useNavigate()
  const location = useLocation()

  const handleItemClick = (item: MenuItem) => {
    if (item.path) {
      navigate(item.path)
      if (isMobile) {
        onToggle() // 移动端点击后关闭侧栏
      }
    }
  }

  const renderMenuItem = (item: MenuItem) => {
    const Icon = item.icon
    const isActive = item.path && location.pathname === item.path

    if (item.children) {
      const menuContent = (
        <div key={item.key} className="space-y-1">
          <div className="px-3 py-2 text-sm font-medium text-text-muted flex items-center">
            <Icon className="w-4 h-4 mr-2" />
            {item.label}
          </div>
          <div className="ml-4 space-y-1">
            {item.children.map((child) => {
              const ChildIcon = child.icon
              const isChildActive = child.path && location.pathname === child.path

              const childButton = (
                <button
                  key={child.key}
                  onClick={() => handleItemClick(child)}
                  className={clsx(
                    "w-full flex items-center px-3 py-2 text-sm rounded-md transition-colors text-left cursor-pointer",
                    isChildActive
                      ? "bg-primary text-white"
                      : "hover:bg-bg-secondary text-text-secondary"
                  )}
                >
                  <ChildIcon className="w-4 h-4 mr-2" />
                  {child.label}
                </button>
              )

              // 如果子菜单项有权限要求，使用Permit包裹
              if (child.permission) {
                return (
                  <Permit key={child.key} perm={child.permission}>
                    {childButton}
                  </Permit>
                )
              }

              return childButton
            })}
          </div>
        </div>
      )

      // 如果父菜单有权限要求，使用Permit包裹整个菜单组
      if (item.permission) {
        return (
          <Permit key={item.key} perm={item.permission}>
            {menuContent}
          </Permit>
        )
      }

      return menuContent
    }

    const menuButton = (
      <button
        key={item.key}
        onClick={() => handleItemClick(item)}
        className={clsx(
          "w-full flex items-center px-3 py-2 text-sm rounded-md transition-colors text-left cursor-pointer",
          isActive
            ? "bg-primary text-white"
            : "hover:bg-bg-secondary text-text-secondary"
        )}
      >
        <Icon className="w-4 h-4 mr-2" />
        {item.label}
      </button>
    )

    // 如果菜单项有权限要求，使用Permit包裹
    if (item.permission) {
      return (
        <Permit key={item.key} perm={item.permission}>
          {menuButton}
        </Permit>
      )
    }

    return menuButton
  }

  const sidebarContent = (
    <GlowCard className="h-full flex flex-col">
      {/* Logo 区域 */}
      <div className="p-6 border-b border-border">
        <h2 className="text-xl font-bold bg-gradient-1 bg-clip-text text-transparent">
          GM 工具
        </h2>
      </div>

      {/* 导航菜单 */}
      <nav className="flex-1 p-4 space-y-2">
        {MENU_ITEMS.map(renderMenuItem)}
      </nav>

      {/* 底部设置 */}
      <div className="p-4 border-t border-border">
        <div className="flex items-center justify-between">
          <span className="text-sm text-text-muted">主题设置</span>
          <ThemeSwitch />
        </div>
      </div>
    </GlowCard>
  )

  if (isMobile) {
    return (
      <AnimatePresence>
        {isOpen && (
          <>
            {/* 遮罩层 */}
            <motion.div
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
              exit={{ opacity: 0 }}
              className="fixed inset-0 bg-black/50 z-40"
              onClick={onToggle}
            />
            {/* 侧栏 */}
            <motion.aside
              initial={{ x: "-100%" }}
              animate={{ x: 0 }}
              exit={{ x: "-100%" }}
              transition={{ type: "tween", duration: 0.2 }}
              className="fixed left-0 top-0 h-full w-64 z-50"
            >
              {sidebarContent}
            </motion.aside>
          </>
        )}
      </AnimatePresence>
    )
  }

  return (
    <motion.aside
      animate={{
        width: isOpen ? 256 : 0,
        opacity: isOpen ? 1 : 0,
      }}
      transition={{ type: "tween", duration: 0.2 }}
      className="fixed left-0 top-0 h-full z-30 overflow-hidden"
    >
      <div className="w-64 h-full">
        {sidebarContent}
      </div>
    </motion.aside>
  )
}

// 侧栏切换按钮
interface SidebarToggleProps {
  onClick: () => void
  isOpen: boolean
}

export function SidebarToggle({ onClick, isOpen }: SidebarToggleProps) {
  return (
    <Button variant="ghost" size="icon" onClick={onClick} className="h-9 w-9">
      {isOpen ? <X className="h-4 w-4" /> : <Menu className="h-4 w-4" />}
      <span className="sr-only">切换侧栏</span>
    </Button>
  )
}