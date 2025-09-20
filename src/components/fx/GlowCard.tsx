import { motion } from 'framer-motion'
import { ReactNode } from 'react'
import { clsx } from 'clsx'

interface GlowCardProps {
  children: ReactNode
  className?: string
  glowColor?: string
  hover?: boolean
}

export function GlowCard({
  children,
  className,
  glowColor = 'primary',
  hover = true
}: GlowCardProps) {
  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.18 }}
      whileHover={hover ? {
        scale: 1.02,
        boxShadow: `0 0 20px rgb(var(--${glowColor}) / 0.3)`
      } : undefined}
      className={clsx(
        'glass-card gradient-border rounded-xl p-6',
        'transition-all duration-200',
        className
      )}
    >
      {children}
    </motion.div>
  )
}