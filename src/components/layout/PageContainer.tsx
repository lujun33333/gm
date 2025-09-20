import { ReactNode } from 'react'
import { motion } from 'framer-motion'

interface PageContainerProps {
  children: ReactNode
  className?: string
  maxWidth?: 'full' | 'screen' | 'screen-xl' | '7xl' | '6xl' | '5xl'
}

export function PageContainer({
  children,
  className = '',
  maxWidth = 'full'
}: PageContainerProps) {
  const maxWidthClass = {
    'full': 'w-full',
    'screen': 'min-w-full',
    'screen-xl': 'max-w-screen-xl mx-auto', // 1280px
    '7xl': 'max-w-7xl mx-auto',
    '6xl': 'max-w-6xl mx-auto',
    '5xl': 'max-w-5xl mx-auto'
  }[maxWidth]

  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.18 }}
      className={`h-full ${maxWidthClass} ${className}`}
    >
      {children}
    </motion.div>
  )
}

// 全屏页面容器
export function FullPageContainer({ children, className = '' }: { children: ReactNode, className?: string }) {
  return (
    <PageContainer maxWidth="full" className={`min-h-full ${className}`}>
      {children}
    </PageContainer>
  )
}

// 内容页面容器（带边距）
export function ContentPageContainer({ children, className = '' }: { children: ReactNode, className?: string }) {
  return (
    <PageContainer maxWidth="full" className={`p-6 ${className}`}>
      {children}
    </PageContainer>
  )
}