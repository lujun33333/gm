import { ReactNode, useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { Sidebar, SidebarToggle } from './Sidebar'
import { Header } from './Header'
import { useLocalStorage } from '@/hooks/useLocalStorage'

interface AppShellProps {
  children: ReactNode
}

function useMediaQuery(query: string): boolean {
  const [matches, setMatches] = useState(false)

  useEffect(() => {
    const media = window.matchMedia(query)
    if (media.matches !== matches) {
      setMatches(media.matches)
    }
    const listener = () => setMatches(media.matches)
    media.addEventListener('change', listener)
    return () => media.removeEventListener('change', listener)
  }, [matches, query])

  return matches
}

export function AppShell({ children }: AppShellProps) {
  const isMobile = useMediaQuery('(max-width: 1023px)') // lg断点以下都视为移动端
  const [sidebarOpen, setSidebarOpen] = useLocalStorage('gm-sidebar-open', true)

  // 移动端默认关闭侧栏
  useEffect(() => {
    if (isMobile && sidebarOpen) {
      setSidebarOpen(false)
    }
  }, [isMobile, sidebarOpen, setSidebarOpen])

  const toggleSidebar = () => {
    setSidebarOpen(!sidebarOpen)
  }

  return (
    <div className="flex layout-main bg-gradient-to-br from-bg to-bg-secondary">
      {/* 侧栏 */}
      <Sidebar
        isOpen={sidebarOpen}
        onToggle={toggleSidebar}
        isMobile={isMobile}
      />

      {/* 主内容区域 */}
      <motion.main
        animate={{
          marginLeft: isMobile ? 0 : (sidebarOpen ? 256 : 0),
        }}
        transition={{ type: 'tween', duration: 0.2 }}
        className="flex-1 flex flex-col layout-content transition-all duration-200"
      >
        {/* 顶部导航 */}
        <div className="sticky top-0 z-20 p-4 pb-0 flex-shrink-0">
          <div className="flex items-center gap-4">
            <SidebarToggle onClick={toggleSidebar} isOpen={sidebarOpen} />
            <div className="flex-1">
              <Header />
            </div>
          </div>
        </div>

        {/* 页面内容 */}
        <div className="flex-1 p-4 pt-0 overflow-auto">
          {children}
        </div>
      </motion.main>
    </div>
  )
}