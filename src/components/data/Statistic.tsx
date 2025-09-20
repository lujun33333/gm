import { ReactNode } from 'react'
import { motion } from 'framer-motion'
import { clsx } from 'clsx'

interface StatisticProps {
  label: string
  value: string | number
  prefix?: ReactNode
  suffix?: ReactNode
  trend?: {
    value: number
    isPositive: boolean
  }
  loading?: boolean
  className?: string
  size?: 'sm' | 'md' | 'lg'
  color?: 'default' | 'primary' | 'success' | 'warning' | 'error'
}

export function Statistic({
  label,
  value,
  prefix,
  suffix,
  trend,
  loading = false,
  className,
  size = 'md',
  color = 'default'
}: StatisticProps) {
  if (loading) {
    return <StatisticSkeleton size={size} />
  }

  const sizeClasses = {
    sm: 'text-lg',
    md: 'text-2xl',
    lg: 'text-3xl'
  }

  const colorClasses = {
    default: 'text-text',
    primary: 'text-primary',
    success: 'text-success',
    warning: 'text-warning',
    error: 'text-error'
  }

  return (
    <motion.div
      initial={{ opacity: 0, y: 10 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.3 }}
      className={clsx('statistic', className)}
    >
      {/* 标签 */}
      <div className="text-sm text-text-muted mb-1">
        {label}
      </div>

      {/* 数值 */}
      <div className={clsx(
        'font-bold flex items-baseline gap-1',
        sizeClasses[size],
        colorClasses[color]
      )}>
        {prefix && <span className="text-base">{prefix}</span>}

        <motion.span
          initial={{ scale: 0.8 }}
          animate={{ scale: 1 }}
          transition={{ type: 'spring', stiffness: 300, damping: 20 }}
        >
          {typeof value === 'number' ? value.toLocaleString('zh-CN') : value}
        </motion.span>

        {suffix && <span className="text-base">{suffix}</span>}

        {/* 趋势指示器 */}
        {trend && (
          <motion.span
            initial={{ opacity: 0, x: -10 }}
            animate={{ opacity: 1, x: 0 }}
            transition={{ delay: 0.2 }}
            className={clsx(
              'text-sm flex items-center gap-1 ml-2',
              trend.isPositive ? 'text-success' : 'text-error'
            )}
          >
            <span className="text-xs">
              {trend.isPositive ? '↗' : '↘'}
            </span>
            {Math.abs(trend.value)}%
          </motion.span>
        )}
      </div>
    </motion.div>
  )
}

// 加载骨架屏
function StatisticSkeleton({ size }: { size: 'sm' | 'md' | 'lg' }) {
  const heightClasses = {
    sm: 'h-6',
    md: 'h-8',
    lg: 'h-10'
  }

  return (
    <div className="space-y-2">
      <div className="h-4 bg-bg-secondary rounded w-16 animate-pulse" />
      <div className={clsx('bg-bg-secondary rounded w-20 animate-pulse', heightClasses[size])} />
    </div>
  )
}

// 统计卡片组件
interface StatisticCardProps extends StatisticProps {
  icon?: ReactNode
  description?: string
  onClick?: () => void
}

export function StatisticCard({
  icon,
  description,
  onClick,
  ...statisticProps
}: StatisticCardProps) {
  return (
    <motion.div
      whileHover={onClick ? { scale: 1.02 } : undefined}
      whileTap={onClick ? { scale: 0.98 } : undefined}
      onClick={onClick}
      className={clsx(
        'glass-card rounded-lg p-4 transition-all',
        {
          'cursor-pointer hover:shadow-glow': onClick
        }
      )}
    >
      <div className="flex items-start justify-between">
        <div className="flex-1">
          <Statistic {...statisticProps} />
          {description && (
            <p className="text-xs text-text-muted mt-2">{description}</p>
          )}
        </div>

        {icon && (
          <div className="ml-4 text-2xl opacity-60">
            {icon}
          </div>
        )}
      </div>
    </motion.div>
  )
}

// 统计组合组件
interface StatisticGroupProps {
  title?: string
  statistics: Array<StatisticProps & { key: string }>
  columns?: 2 | 3 | 4
  className?: string
}

export function StatisticGroup({
  title,
  statistics,
  columns = 3,
  className
}: StatisticGroupProps) {
  const gridClasses = {
    2: 'grid-cols-2',
    3: 'grid-cols-3',
    4: 'grid-cols-4'
  }

  return (
    <div className={clsx('statistic-group', className)}>
      {title && (
        <h3 className="text-lg font-semibold text-text mb-4">{title}</h3>
      )}

      <div className={clsx(
        'grid gap-4',
        `grid-cols-1 sm:grid-cols-2 lg:${gridClasses[columns]}`
      )}>
        {statistics.map((stat) => (
          <StatisticCard key={stat.key} {...stat} />
        ))}
      </div>
    </div>
  )
}