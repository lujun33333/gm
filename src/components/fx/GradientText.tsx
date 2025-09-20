import { ReactNode } from 'react'
import { clsx } from 'clsx'

interface GradientTextProps {
  children: ReactNode
  className?: string
  gradient?: 1 | 2 | 3
}

export function GradientText({
  children,
  className,
  gradient = 1
}: GradientTextProps) {
  return (
    <span
      className={clsx(
        'bg-clip-text text-transparent font-bold',
        `bg-gradient-${gradient}`,
        className
      )}
    >
      {children}
    </span>
  )
}