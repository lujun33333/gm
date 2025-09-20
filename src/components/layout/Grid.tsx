import { ReactNode } from 'react'
import { clsx } from 'clsx'

interface GridProps {
  children: ReactNode
  cols?: 1 | 2 | 3 | 4 | 6 | 12
  gap?: number
  className?: string
}

interface GridItemProps {
  children: ReactNode
  span?: {
    base?: number
    sm?: number
    md?: number
    lg?: number
    xl?: number
  } | number
  className?: string
}

export function Grid({ children, cols = 12, gap = 4, className }: GridProps) {
  const gridClasses = clsx(
    'grid',
    {
      'grid-cols-1': cols === 1,
      'grid-cols-2': cols === 2,
      'grid-cols-3': cols === 3,
      'grid-cols-4': cols === 4,
      'grid-cols-6': cols === 6,
      'grid-cols-12': cols === 12,
    },
    `gap-${gap}`,
    className
  )

  return (
    <div className={gridClasses}>
      {children}
    </div>
  )
}

export function GridItem({ children, span = 1, className }: GridItemProps) {
  const spanClasses = clsx(
    // 处理响应式span
    typeof span === 'object' ? [
      span.base && `col-span-${span.base}`,
      span.sm && `sm:col-span-${span.sm}`,
      span.md && `md:col-span-${span.md}`,
      span.lg && `lg:col-span-${span.lg}`,
      span.xl && `xl:col-span-${span.xl}`,
    ] : `col-span-${span}`,
    className
  )

  return (
    <div className={spanClasses}>
      {children}
    </div>
  )
}