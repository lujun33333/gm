import { ReactNode } from 'react'
import { motion } from 'framer-motion'
import { clsx } from 'clsx'
import { GlowCard } from '../fx/GlowCard'

interface MetricCardProps {
  icon?: ReactNode | string
  title: string
  value: string | number
  unit?: string
  trend?: {
    value: number
    isPositive: boolean
    label?: string
  }
  description?: string
  color?: 'primary' | 'success' | 'warning' | 'error' | 'default'
  size?: 'sm' | 'md' | 'lg'
  loading?: boolean
  onClick?: () => void
  className?: string
  children?: ReactNode
}

export function MetricCard({
  icon,
  title,
  value,
  unit,
  trend,
  description,
  color = 'default',
  size = 'md',
  loading = false,
  onClick,
  className,
  children
}: MetricCardProps) {
  if (loading) {
    return <MetricCardSkeleton size={size} />
  }

  const sizeClasses = {
    sm: { value: 'text-xl', title: 'text-sm', padding: 'p-3' },
    md: { value: 'text-2xl', title: 'text-sm', padding: 'p-4' },
    lg: { value: 'text-3xl', title: 'text-base', padding: 'p-6' }
  }

  const colorClasses = {
    default: 'text-text',
    primary: 'text-primary',
    success: 'text-success',
    warning: 'text-warning',
    error: 'text-error'
  }

  const glowColor = color === 'default' ? undefined : color

  const formatValue = (val: string | number) => {
    if (typeof val === 'number') {
      return val.toLocaleString('zh-CN')
    }
    return val
  }

  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.4 }}
      whileHover={onClick ? { scale: 1.02 } : undefined}
      whileTap={onClick ? { scale: 0.98 } : undefined}
      className={className}
    >
      <div
        className={clsx(
          'h-full transition-all duration-300 cursor-pointer',
          sizeClasses[size].padding,
          {
            'hover:shadow-glow': onClick
          }
        )}
        onClick={onClick}
      >
        <GlowCard
          glowColor={glowColor}
          className="h-full p-0"
        >
        <div className="flex items-start justify-between">
          <div className="flex-1">
            {/* 标题和图标 */}
            <div className="flex items-center gap-2 mb-3">
              {icon && (
                <div className="text-lg opacity-70">
                  {typeof icon === 'string' ? icon : icon}
                </div>
              )}
              <h3 className={clsx(
                'font-medium text-text-secondary',
                sizeClasses[size].title
              )}>
                {title}
              </h3>
            </div>

            {/* 数值 */}
            <div className="flex items-baseline gap-2 mb-2">
              <motion.div
                initial={{ scale: 0.8 }}
                animate={{ scale: 1 }}
                transition={{ type: 'spring', stiffness: 300, damping: 20 }}
                className={clsx(
                  'font-bold',
                  sizeClasses[size].value,
                  colorClasses[color]
                )}
              >
                {formatValue(value)}
              </motion.div>

              {unit && (
                <span className="text-text-muted text-sm font-normal">
                  {unit}
                </span>
              )}

              {/* 趋势指示器 */}
              {trend && (
                <motion.div
                  initial={{ opacity: 0, x: -10 }}
                  animate={{ opacity: 1, x: 0 }}
                  transition={{ delay: 0.2 }}
                  className={clsx(
                    'text-sm flex items-center gap-1',
                    trend.isPositive ? 'text-success' : 'text-error'
                  )}
                >
                  <span className="text-xs">
                    {trend.isPositive ? '↗' : '↘'}
                  </span>
                  <span className="font-medium">
                    {Math.abs(trend.value)}%
                  </span>
                </motion.div>
              )}
            </div>

            {/* 描述文本 */}
            {description && (
              <p className="text-xs text-text-muted leading-relaxed">
                {description}
              </p>
            )}

            {/* 趋势标签 */}
            {trend?.label && (
              <p className="text-xs text-text-muted mt-1">
                {trend.label}
              </p>
            )}

            {/* 自定义内容 */}
            {children && (
              <div className="mt-3">
                {children}
              </div>
            )}
          </div>
        </div>
        </GlowCard>
      </div>
    </motion.div>
  )
}

// 加载骨架屏
function MetricCardSkeleton({ size }: { size: 'sm' | 'md' | 'lg' }) {
  const sizeClasses = {
    sm: { padding: 'p-3', valueHeight: 'h-6' },
    md: { padding: 'p-4', valueHeight: 'h-8' },
    lg: { padding: 'p-6', valueHeight: 'h-10' }
  }

  return (
    <div className={clsx(
      'glass-card rounded-lg animate-pulse',
      sizeClasses[size].padding
    )}>
      <div className="space-y-3">
        <div className="h-4 bg-bg-secondary rounded w-24" />
        <div className={clsx('bg-bg-secondary rounded w-16', sizeClasses[size].valueHeight)} />
        <div className="h-3 bg-bg-secondary rounded w-32" />
      </div>
    </div>
  )
}

// 统计组合组件
interface MetricGridProps {
  title?: string
  metrics: Array<MetricCardProps & { key: string }>
  columns?: 2 | 3 | 4
  className?: string
}

export function MetricGrid({
  title,
  metrics,
  columns = 2,
  className
}: MetricGridProps) {
  const gridClasses = {
    2: 'grid-cols-1 sm:grid-cols-2',
    3: 'grid-cols-1 sm:grid-cols-2 lg:grid-cols-3',
    4: 'grid-cols-1 sm:grid-cols-2 lg:grid-cols-4'
  }

  return (
    <div className={clsx('metric-grid', className)}>
      {title && (
        <h3 className="text-lg font-semibold text-text mb-4">{title}</h3>
      )}

      <div className={clsx(
        'grid gap-4',
        gridClasses[columns]
      )}>
        {metrics.map((metric) => (
          <MetricCard key={metric.key} {...metric} />
        ))}
      </div>
    </div>
  )
}