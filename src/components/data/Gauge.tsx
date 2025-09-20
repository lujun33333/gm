import { useEffect, useState } from 'react'
import { motion } from 'framer-motion'
import { clsx } from 'clsx'

interface GaugeProps {
  label: string
  value: number
  max?: number
  min?: number
  unit?: string
  color?: 'primary' | 'success' | 'warning' | 'error'
  size?: 'sm' | 'md' | 'lg'
  showValue?: boolean
  loading?: boolean
  className?: string
  thresholds?: Array<{
    value: number
    color: string
    label?: string
  }>
}

export function Gauge({
  label,
  value,
  max = 100,
  min = 0,
  unit = '',
  color = 'primary',
  size = 'md',
  showValue = true,
  loading = false,
  className,
  thresholds = []
}: GaugeProps) {
  const [animatedValue, setAnimatedValue] = useState(0)

  // 动画化数值变化
  useEffect(() => {
    const timer = setTimeout(() => {
      setAnimatedValue(value)
    }, 100)
    return () => clearTimeout(timer)
  }, [value])

  if (loading) {
    return <GaugeSkeleton size={size} />
  }

  // 计算百分比
  const percentage = Math.min(Math.max(((value - min) / (max - min)) * 100, 0), 100)
  const animatedPercentage = Math.min(Math.max(((animatedValue - min) / (max - min)) * 100, 0), 100)

  // 根据阈值确定颜色
  let currentColor = color
  if (thresholds.length > 0) {
    const threshold = thresholds
      .sort((a, b) => a.value - b.value)
      .find(t => value >= t.value)
    if (threshold) {
      currentColor = threshold.color as any
    }
  }

  const colorClasses = {
    primary: 'stroke-primary',
    success: 'stroke-success',
    warning: 'stroke-warning',
    error: 'stroke-error'
  }

  const sizeConfig = {
    sm: { radius: 40, strokeWidth: 6, fontSize: 'text-sm' },
    md: { radius: 60, strokeWidth: 8, fontSize: 'text-base' },
    lg: { radius: 80, strokeWidth: 10, fontSize: 'text-lg' }
  }

  const config = sizeConfig[size]
  const circumference = 2 * Math.PI * config.radius
  const strokeDasharray = circumference
  const strokeDashoffset = circumference - (animatedPercentage / 100) * circumference

  return (
    <motion.div
      initial={{ opacity: 0, scale: 0.9 }}
      animate={{ opacity: 1, scale: 1 }}
      transition={{ duration: 0.3 }}
      className={clsx('gauge flex flex-col items-center', className)}
    >
      {/* SVG 仪表盘 */}
      <div className="relative">
        <svg
          width={config.radius * 2 + config.strokeWidth * 2}
          height={config.radius * 2 + config.strokeWidth * 2}
          className="transform -rotate-90"
        >
          {/* 背景圆环 */}
          <circle
            cx={config.radius + config.strokeWidth}
            cy={config.radius + config.strokeWidth}
            r={config.radius}
            stroke="rgb(var(--border))"
            strokeWidth={config.strokeWidth}
            fill="none"
            opacity={0.3}
          />

          {/* 进度圆环 */}
          <motion.circle
            cx={config.radius + config.strokeWidth}
            cy={config.radius + config.strokeWidth}
            r={config.radius}
            stroke="currentColor"
            strokeWidth={config.strokeWidth}
            fill="none"
            strokeLinecap="round"
            strokeDasharray={strokeDasharray}
            initial={{ strokeDashoffset: circumference }}
            animate={{ strokeDashoffset }}
            transition={{
              duration: 1,
              ease: 'easeOut'
            }}
            className={colorClasses[currentColor]}
            style={{
              filter: 'drop-shadow(0 0 6px currentColor)'
            }}
          />

          {/* 阈值标记 */}
          {thresholds.map((threshold, index) => {
            const thresholdPercentage = ((threshold.value - min) / (max - min)) * 100
            const angle = (thresholdPercentage / 100) * 360 - 90
            const x1 = config.radius + config.strokeWidth + (config.radius - config.strokeWidth) * Math.cos(angle * Math.PI / 180)
            const y1 = config.radius + config.strokeWidth + (config.radius - config.strokeWidth) * Math.sin(angle * Math.PI / 180)
            const x2 = config.radius + config.strokeWidth + config.radius * Math.cos(angle * Math.PI / 180)
            const y2 = config.radius + config.strokeWidth + config.radius * Math.sin(angle * Math.PI / 180)

            return (
              <line
                key={index}
                x1={x1}
                y1={y1}
                x2={x2}
                y2={y2}
                stroke={threshold.color}
                strokeWidth={2}
                opacity={0.7}
              />
            )
          })}
        </svg>

        {/* 中心数值 */}
        {showValue && (
          <div className="absolute inset-0 flex flex-col items-center justify-center">
            <motion.div
              initial={{ scale: 0.5 }}
              animate={{ scale: 1 }}
              transition={{ delay: 0.5, type: 'spring', stiffness: 300 }}
              className={clsx(
                'font-bold text-center',
                config.fontSize,
                colorClasses[currentColor].replace('stroke-', 'text-')
              )}
            >
              {Math.round(animatedValue)}{unit}
            </motion.div>
            <div className="text-xs text-text-muted mt-1">
              {Math.round(percentage)}%
            </div>
          </div>
        )}
      </div>

      {/* 标签 */}
      <div className="mt-3 text-center">
        <h4 className={clsx('font-medium text-text', config.fontSize)}>
          {label}
        </h4>

        {/* 阈值说明 */}
        {thresholds.length > 0 && (
          <div className="flex flex-wrap justify-center gap-2 mt-2">
            {thresholds.map((threshold, index) => (
              <div
                key={index}
                className="flex items-center gap-1 text-xs text-text-muted"
              >
                <div
                  className="w-2 h-2 rounded-full"
                  style={{ backgroundColor: threshold.color }}
                />
                <span>{threshold.label || `${threshold.value}${unit}`}</span>
              </div>
            ))}
          </div>
        )}
      </div>
    </motion.div>
  )
}

// 加载骨架屏
function GaugeSkeleton({ size }: { size: 'sm' | 'md' | 'lg' }) {
  const sizeClasses = {
    sm: 'w-20 h-20',
    md: 'w-32 h-32',
    lg: 'w-40 h-40'
  }

  return (
    <div className="flex flex-col items-center">
      <div className={clsx('bg-bg-secondary rounded-full animate-pulse', sizeClasses[size])} />
      <div className="h-4 bg-bg-secondary rounded w-16 mt-3 animate-pulse" />
    </div>
  )
}

// 仪表盘组合组件
interface GaugeGroupProps {
  title?: string
  gauges: Array<GaugeProps & { key: string }>
  columns?: 2 | 3 | 4
  className?: string
}

export function GaugeGroup({
  title,
  gauges,
  columns = 3,
  className
}: GaugeGroupProps) {
  const gridClasses = {
    2: 'grid-cols-2',
    3: 'grid-cols-3',
    4: 'grid-cols-4'
  }

  return (
    <div className={clsx('gauge-group', className)}>
      {title && (
        <h3 className="text-lg font-semibold text-text mb-4">{title}</h3>
      )}

      <div className={clsx(
        'grid gap-6',
        `grid-cols-1 sm:grid-cols-2 lg:${gridClasses[columns]}`
      )}>
        {gauges.map((gauge) => (
          <Gauge key={gauge.key} {...gauge} />
        ))}
      </div>
    </div>
  )
}

// 简化的小仪表盘
interface MiniGaugeProps {
  value: number
  max?: number
  color?: string
  size?: number
}

export function MiniGauge({
  value,
  max = 100,
  color = 'rgb(var(--primary))',
  size = 24
}: MiniGaugeProps) {
  const percentage = Math.min(Math.max((value / max) * 100, 0), 100)
  const circumference = 2 * Math.PI * 8
  const strokeDashoffset = circumference - (percentage / 100) * circumference

  return (
    <div className="inline-block">
      <svg width={size} height={size} className="transform -rotate-90">
        <circle
          cx={size / 2}
          cy={size / 2}
          r={8}
          stroke="rgb(var(--border))"
          strokeWidth={2}
          fill="none"
          opacity={0.3}
        />
        <circle
          cx={size / 2}
          cy={size / 2}
          r={8}
          stroke={color}
          strokeWidth={2}
          fill="none"
          strokeLinecap="round"
          strokeDasharray={circumference}
          strokeDashoffset={strokeDashoffset}
          className="transition-all duration-500"
        />
      </svg>
    </div>
  )
}