import { ReactNode, useState } from 'react'
import { motion } from 'framer-motion'
import { clsx } from 'clsx'

interface ShimmerButtonProps {
  children: ReactNode
  onClick?: () => void
  disabled?: boolean
  loading?: boolean
  variant?: 'primary' | 'secondary' | 'success' | 'warning' | 'error'
  size?: 'sm' | 'md' | 'lg'
  className?: string
  type?: 'button' | 'submit' | 'reset'
}

export function ShimmerButton({
  children,
  onClick,
  disabled = false,
  loading = false,
  variant = 'primary',
  size = 'md',
  className,
  type = 'button',
}: ShimmerButtonProps) {
  const [isShimmering, setIsShimmering] = useState(false)

  const baseClasses = clsx(
    'relative overflow-hidden rounded-lg font-medium transition-all duration-200',
    'focus:outline-none focus:ring-2 focus:ring-offset-2',
    'disabled:opacity-50 disabled:cursor-not-allowed',
    {
      // 尺寸
      'px-3 py-2 text-sm': size === 'sm',
      'px-4 py-3 text-base': size === 'md',
      'px-6 py-4 text-lg': size === 'lg',

      // 变体
      'bg-gradient-1 text-white hover:shadow-glow focus:ring-primary': variant === 'primary',
      'bg-gradient-2 text-white hover:shadow-glow focus:ring-accent': variant === 'secondary',
      'bg-gradient-3 text-white hover:shadow-glow focus:ring-success': variant === 'success',
      'bg-gradient-to-r from-warning to-warning/80 text-white hover:shadow-glow focus:ring-warning': variant === 'warning',
      'bg-gradient-to-r from-error to-error/80 text-white hover:shadow-glow focus:ring-error': variant === 'error',
    },
    className
  )

  const handleClick = () => {
    if (disabled || loading) return

    setIsShimmering(true)
    onClick?.()

    // 重置闪光状态
    setTimeout(() => setIsShimmering(false), 600)
  }

  return (
    <motion.button
      type={type}
      onClick={handleClick}
      disabled={disabled || loading}
      whileHover={{ scale: disabled || loading ? 1 : 1.02 }}
      whileTap={{ scale: disabled || loading ? 1 : 0.98 }}
      className={baseClasses}
    >
      {/* 闪光动画层 */}
      <motion.div
        className="absolute inset-0 -top-0 -left-full w-full h-full bg-gradient-to-r from-transparent via-white/30 to-transparent skew-x-12 pointer-events-none"
        animate={{
          left: isShimmering ? '100%' : '-100%',
        }}
        transition={{
          duration: 0.6,
          ease: 'easeInOut',
        }}
      />

      {/* 周期性闪光（每3秒一次） */}
      <motion.div
        className="absolute inset-0 -top-0 -left-full w-full h-full bg-gradient-to-r from-transparent via-white/20 to-transparent skew-x-12 pointer-events-none"
        animate={{
          left: ['−100%', '100%'],
        }}
        transition={{
          duration: 0.8,
          ease: 'easeInOut',
          repeat: Infinity,
          repeatDelay: 2.2, // 3秒总间隔 - 0.8秒动画时间
        }}
      />

      {/* 内容 */}
      <span className="relative z-10 flex items-center justify-center gap-2">
        {loading && (
          <motion.div
            animate={{ rotate: 360 }}
            transition={{ duration: 1, repeat: Infinity, ease: 'linear' }}
            className="w-4 h-4 border-2 border-white/30 border-t-white rounded-full"
          />
        )}
        {children}
      </span>

      {/* 悬停光晕效果 */}
      <motion.div
        className="absolute inset-0 bg-white/10 rounded-lg pointer-events-none"
        initial={{ opacity: 0 }}
        whileHover={{ opacity: 1 }}
        transition={{ duration: 0.2 }}
      />
    </motion.button>
  )
}

// 快捷变体组件
export function ShimmerPrimaryButton(props: Omit<ShimmerButtonProps, 'variant'>) {
  return <ShimmerButton {...props} variant="primary" />
}

export function ShimmerSecondaryButton(props: Omit<ShimmerButtonProps, 'variant'>) {
  return <ShimmerButton {...props} variant="secondary" />
}

export function ShimmerSuccessButton(props: Omit<ShimmerButtonProps, 'variant'>) {
  return <ShimmerButton {...props} variant="success" />
}