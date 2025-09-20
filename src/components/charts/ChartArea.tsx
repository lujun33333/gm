import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import {
  AreaChart,
  Area,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  Legend
} from 'recharts'
import { clsx } from 'clsx'

interface ChartDataPoint {
  name: string
  value: number
  [key: string]: any
}

interface ChartAreaProps {
  data: ChartDataPoint[]
  title?: string
  dataKey?: string
  color?: string
  gradientColor?: string
  height?: number
  showGrid?: boolean
  showTooltip?: boolean
  showLegend?: boolean
  loading?: boolean
  empty?: boolean
  error?: string
  className?: string
  valueFormatter?: (value: number) => string
  nameFormatter?: (name: string) => string
}

export function ChartArea({
  data,
  title,
  dataKey = 'value',
  color = 'rgb(var(--primary))',
  gradientColor,
  height = 300,
  showGrid = true,
  showTooltip = true,
  showLegend = false,
  loading = false,
  empty = false,
  error,
  className,
  valueFormatter = (value) => value.toString(),
  nameFormatter = (name) => name
}: ChartAreaProps) {
  const [animatedData, setAnimatedData] = useState<ChartDataPoint[]>([])

  useEffect(() => {
    if (loading || error || empty) return

    const timer = setTimeout(() => {
      setAnimatedData(data)
    }, 300)

    return () => clearTimeout(timer)
  }, [data, loading, error, empty])

  // Loading 状态
  if (loading) {
    return (
      <div className={clsx('space-y-4', className)}>
        {title && (
          <div className="h-6 bg-bg-secondary rounded w-48 animate-pulse" />
        )}
        <div
          className="bg-bg-secondary rounded-lg animate-pulse"
          style={{ height }}
        />
      </div>
    )
  }

  // Error 状态
  if (error) {
    return (
      <div className={clsx('space-y-4', className)}>
        {title && (
          <h3 className="text-lg font-medium text-text">{title}</h3>
        )}
        <div
          className="flex items-center justify-center text-error bg-error/10 border border-error/20 rounded-lg"
          style={{ height }}
        >
          <div className="text-center">
            <div className="text-2xl mb-2">⚠️</div>
            <div className="text-sm">{error}</div>
          </div>
        </div>
      </div>
    )
  }

  // Empty 状态
  if (empty || data.length === 0) {
    return (
      <div className={clsx('space-y-4', className)}>
        {title && (
          <h3 className="text-lg font-medium text-text">{title}</h3>
        )}
        <div
          className="flex items-center justify-center text-text-muted bg-bg-secondary/50 rounded-lg"
          style={{ height }}
        >
          <div className="text-center">
            <div className="text-2xl mb-2">📊</div>
            <div className="text-sm">暂无数据</div>
          </div>
        </div>
      </div>
    )
  }

  const gradientId = `area-gradient-${Math.random().toString(36).substr(2, 9)}`
  const finalGradientColor = gradientColor || color

  const CustomTooltip = ({ active, payload, label }: any) => {
    if (active && payload && payload.length) {
      return (
        <motion.div
          initial={{ opacity: 0, scale: 0.9 }}
          animate={{ opacity: 1, scale: 1 }}
          className="bg-bg-secondary/95 backdrop-blur-sm border border-border rounded-lg px-3 py-2 shadow-lg"
        >
          <p className="text-text-muted text-sm mb-1">
            {nameFormatter(label)}
          </p>
          <p className="text-text font-medium">
            <span
              className="inline-block w-2 h-2 rounded-full mr-2"
              style={{ backgroundColor: color }}
            />
            {valueFormatter(payload[0].value)}
          </p>
        </motion.div>
      )
    }
    return null
  }

  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.4 }}
      className={clsx('chart-area', className)}
    >
      {title && (
        <h3 className="text-lg font-medium text-text mb-4">{title}</h3>
      )}

      <ResponsiveContainer width="100%" height={height}>
        <AreaChart data={animatedData}>
          <defs>
            <linearGradient id={gradientId} x1="0" y1="0" x2="0" y2="1">
              <stop offset="5%" stopColor={color} stopOpacity={0.3} />
              <stop offset="95%" stopColor={finalGradientColor} stopOpacity={0.05} />
            </linearGradient>
          </defs>

          {showGrid && (
            <CartesianGrid
              strokeDasharray="3 3"
              stroke="rgb(var(--border))"
              opacity={0.3}
            />
          )}

          <XAxis
            dataKey="name"
            axisLine={false}
            tickLine={false}
            tick={{ fontSize: 12, fill: 'rgb(var(--text-muted))' }}
            tickFormatter={nameFormatter}
          />

          <YAxis
            axisLine={false}
            tickLine={false}
            tick={{ fontSize: 12, fill: 'rgb(var(--text-muted))' }}
            tickFormatter={valueFormatter}
          />

          {showTooltip && <Tooltip content={<CustomTooltip />} />}
          {showLegend && <Legend />}

          <Area
            type="monotone"
            dataKey={dataKey}
            stroke={color}
            strokeWidth={2}
            fill={`url(#${gradientId})`}
            animationDuration={1000}
            animationBegin={0}
          />
        </AreaChart>
      </ResponsiveContainer>
    </motion.div>
  )
}