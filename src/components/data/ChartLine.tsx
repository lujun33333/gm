import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import {
  LineChart,
  Line,
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
  [key: string]: any
}

interface MetricConfig {
  key: string
  name: string
  color: string
  strokeWidth?: number
  showDots?: boolean
  formatter?: (value: number) => string
}

interface ChartLineProps {
  data: ChartDataPoint[]
  title?: string
  metrics?: MetricConfig[]
  // 单线图属性（向后兼容）
  dataKey?: string
  color?: string
  showDots?: boolean
  //
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
  unit?: string
}

export function ChartLine({
  data,
  title,
  metrics,
  // 单线图向后兼容
  dataKey = 'value',
  color = 'rgb(var(--primary))',
  showDots: singleShowDots = true,
  //
  height = 300,
  showGrid = true,
  showTooltip = true,
  showLegend = false,
  loading = false,
  empty = false,
  error,
  className,
  valueFormatter = (value) => value.toLocaleString('zh-CN'),
  nameFormatter = (name) => name,
  unit = ''
}: ChartLineProps) {
  const [animatedData, setAnimatedData] = useState<ChartDataPoint[]>([])

  useEffect(() => {
    if (loading || error || empty) return

    const timer = setTimeout(() => {
      setAnimatedData(data)
    }, 300)

    return () => clearTimeout(timer)
  }, [data, loading, error, empty])

  // 处理单线图和多线图配置
  const finalMetrics: MetricConfig[] = metrics || [
    {
      key: dataKey,
      name: dataKey,
      color,
      showDots: singleShowDots,
      formatter: valueFormatter
    }
  ]

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
            <div className="text-2xl mb-2">📈</div>
            <div className="text-sm">暂无数据</div>
          </div>
        </div>
      </div>
    )
  }

  const CustomTooltip = ({ active, payload, label }: any) => {
    if (active && payload && payload.length) {
      return (
        <motion.div
          initial={{ opacity: 0, scale: 0.9 }}
          animate={{ opacity: 1, scale: 1 }}
          className="bg-bg-secondary/95 backdrop-blur-sm border border-border rounded-lg px-3 py-2 shadow-lg z-50"
        >
          <p className="text-text-muted text-sm mb-2">
            {nameFormatter(label)}
          </p>
          <div className="space-y-1">
            {payload.map((entry: any, index: number) => {
              const metric = finalMetrics.find(m => m.key === entry.dataKey)
              const formatter = metric?.formatter || valueFormatter
              return (
                <p key={index} className="text-text font-medium">
                  <span
                    className="inline-block w-2 h-2 rounded-full mr-2"
                    style={{ backgroundColor: entry.color }}
                  />
                  <span className="text-text-muted mr-1">{metric?.name || entry.dataKey}:</span>
                  {formatter(entry.value)}{unit && ` ${unit}`}
                </p>
              )
            })}
          </div>
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
      className={clsx('chart-line', className)}
    >
      {title && (
        <h3 className="text-lg font-medium text-text mb-4">{title}</h3>
      )}

      <ResponsiveContainer width="100%" height={height}>
        <LineChart data={animatedData}>
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
            tickFormatter={(value) => `${valueFormatter(value)}${unit ? ` ${unit}` : ''}`}
          />

          {showTooltip && <Tooltip content={<CustomTooltip />} />}
          {showLegend && <Legend />}

          {finalMetrics.map((metric, index) => (
            <Line
              key={metric.key}
              type="monotone"
              dataKey={metric.key}
              stroke={metric.color}
              strokeWidth={metric.strokeWidth || 2}
              dot={metric.showDots !== false ? {
                fill: metric.color,
                strokeWidth: 2,
                r: 4
              } : false}
              activeDot={{
                r: 6,
                stroke: metric.color,
                strokeWidth: 2,
                fill: 'rgb(var(--bg))'
              }}
              animationDuration={1200}
              animationBegin={100 + index * 100}
            />
          ))}
        </LineChart>
      </ResponsiveContainer>
    </motion.div>
  )
}