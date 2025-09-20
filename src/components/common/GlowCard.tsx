import * as React from "react"
import { motion } from "framer-motion"
import { clsx } from "clsx"

interface GlowCardProps extends React.HTMLAttributes<HTMLDivElement> {
  gradient?: 1 | 2 | 3
  glow?: boolean
  children: React.ReactNode
}

export function GlowCard({
  className,
  children,
  gradient,
  glow = false,
  ...props
}: GlowCardProps) {
  const gradientClass = gradient
    ? `gradient-border bg-gradient-${gradient}`
    : ''

  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.18 }}
      className={clsx(
        "glass-card rounded-lg",
        gradientClass,
        glow && "shadow-glow",
        className
      )}
      {...props}
    >
      <div className="relative z-10 h-full w-full bg-card/90 backdrop-blur-lg rounded-lg border border-border/50">
        {children}
      </div>
    </motion.div>
  )
}