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
  LineChart,
  Line
} from 'recharts'
import { clsx } from 'clsx'

interface TrendDataPoint {
  time: string
  value: number
  label?: string
}

interface TrendChartProps {
  data: TrendDataPoint[]
  title?: string
  color?: string
  gradientColor?: string
  height?: number
  showGrid?: boolean
  showTooltip?: boolean
  type?: 'area' | 'line'
  loading?: boolean
  className?: string
  valueFormatter?: (value: number) => string
  timeFormatter?: (time: string) => string
}

export function TrendChart({
  data,
  title,
  color = 'rgb(var(--primary))',
  gradientColor,
  height = 200,
  showGrid = true,
  showTooltip = true,
  type = 'area',
  loading = false,
  className,
  valueFormatter = (value) => value.toString(),
  timeFormatter = (time) => time
}: TrendChartProps) {
  const [animatedData, setAnimatedData] = useState<TrendDataPoint[]>([])

  // 动画加载数据
  useEffect(() => {
    if (data.length === 0) return

    // 逐步添加数据点创建动画效果
    const timer = setTimeout(() => {
      setAnimatedData(data)
    }, 300)

    return () => clearTimeout(timer)
  }, [data])

  if (loading) {
    return <TrendChartSkeleton height={height} />
  }

  if (data.length === 0) {
    return (
      <div
        className={clsx('flex items-center justify-center text-text-muted', className)}
        style={{ height }}
      >
        暂无数据
      </div>
    )
  }

  const gradientId = `gradient-${Math.random().toString(36).substr(2, 9)}`
  const finalGradientColor = gradientColor || color

  // 自定义Tooltip
  const CustomTooltip = ({ active, payload, label }: any) => {
    if (active && payload && payload.length) {
      return (
        <motion.div
          initial={{ opacity: 0, scale: 0.9 }}
          animate={{ opacity: 1, scale: 1 }}
          className="bg-bg-secondary/95 backdrop-blur-sm border border-border rounded-lg px-3 py-2 shadow-lg"
        >
          <p className="text-text-muted text-sm mb-1">
            {timeFormatter(label)}
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

  const ChartComponent = type === 'area' ? AreaChart : LineChart

  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.4 }}
      className={clsx('trend-chart', className)}
    >
      {title && (
        <h3 className="text-text font-medium mb-4 text-center">{title}</h3>
      )}

      <ResponsiveContainer width="100%" height={height}>
        <ChartComponent data={animatedData}>
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
            dataKey="time"
            axisLine={false}
            tickLine={false}
            tick={{ fontSize: 12, fill: 'rgb(var(--text-muted))' }}
            tickFormatter={timeFormatter}
          />

          <YAxis
            axisLine={false}
            tickLine={false}
            tick={{ fontSize: 12, fill: 'rgb(var(--text-muted))' }}
            tickFormatter={valueFormatter}
          />

          {showTooltip && <Tooltip content={<CustomTooltip />} />}

          {type === 'area' ? (
            <Area
              type="monotone"
              dataKey="value"
              stroke={color}
              strokeWidth={2}
              fill={`url(#${gradientId})`}
              animationDuration={1000}
              animationBegin={0}
            />
          ) : (
            <Line
              type="monotone"
              dataKey="value"
              stroke={color}
              strokeWidth={2}
              dot={{ fill: color, strokeWidth: 2, r: 4 }}
              activeDot={{ r: 6, stroke: color, strokeWidth: 2 }}
              animationDuration={1000}
              animationBegin={0}
            />
          )}
        </ChartComponent>
      </ResponsiveContainer>
    </motion.div>
  )
}

// 加载骨架屏
function TrendChartSkeleton({ height }: { height: number }) {
  return (
    <div className="animate-pulse">
      <div className="h-4 bg-bg-secondary rounded w-32 mx-auto mb-4" />
      <div
        className="bg-bg-secondary rounded-lg"
        style={{ height }}
      />
    </div>
  )
}

// 多指标趋势图
interface MultiTrendData {
  time: string
  [key: string]: string | number
}

interface MultiTrendChartProps {
  data: MultiTrendData[]
  metrics: Array<{
    key: string
    name: string
    color: string
    formatter?: (value: number) => string
  }>
  title?: string
  height?: number
  loading?: boolean
  className?: string
}

export function MultiTrendChart({
  data,
  metrics,
  title,
  height = 250,
  loading = false,
  className
}: MultiTrendChartProps) {
  if (loading) {
    return <TrendChartSkeleton height={height} />
  }

  return (
    <motion.div
      initial={{ opacity: 0, y: 20 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.4 }}
      className={clsx('multi-trend-chart', className)}
    >
      {title && (
        <h3 className="text-text font-medium mb-4 text-center">{title}</h3>
      )}

      {/* 图例 */}
      <div className="flex flex-wrap justify-center gap-4 mb-4">
        {metrics.map((metric) => (
          <div key={metric.key} className="flex items-center gap-2 text-sm">
            <div
              className="w-3 h-3 rounded-full"
              style={{ backgroundColor: metric.color }}
            />
            <span className="text-text-secondary">{metric.name}</span>
          </div>
        ))}
      </div>

      <ResponsiveContainer width="100%" height={height}>
        <LineChart data={data}>
          <CartesianGrid
            strokeDasharray="3 3"
            stroke="rgb(var(--border))"
            opacity={0.3}
          />

          <XAxis
            dataKey="time"
            axisLine={false}
            tickLine={false}
            tick={{ fontSize: 12, fill: 'rgb(var(--text-muted))' }}
          />

          <YAxis
            axisLine={false}
            tickLine={false}
            tick={{ fontSize: 12, fill: 'rgb(var(--text-muted))' }}
          />

          <Tooltip
            content={({ active, payload, label }) => {
              if (active && payload && payload.length) {
                return (
                  <div className="bg-bg-secondary/95 backdrop-blur-sm border border-border rounded-lg px-3 py-2 shadow-lg">
                    <p className="text-text-muted text-sm mb-2">{label}</p>
                    {payload.map((entry) => {
                      const metric = metrics.find(m => m.key === entry.dataKey)
                      return (
                        <p key={entry.dataKey} className="text-text text-sm">
                          <span
                            className="inline-block w-2 h-2 rounded-full mr-2"
                            style={{ backgroundColor: entry.color }}
                          />
                          {metric?.name}: {metric?.formatter
                            ? metric.formatter(entry.value as number)
                            : entry.value
                          }
                        </p>
                      )
                    })}
                  </div>
                )
              }
              return null
            }}
          />

          {metrics.map((metric) => (
            <Line
              key={metric.key}
              type="monotone"
              dataKey={metric.key}
              stroke={metric.color}
              strokeWidth={2}
              dot={false}
              activeDot={{ r: 4, stroke: metric.color, strokeWidth: 2 }}
              animationDuration={1000}
            />
          ))}
        </LineChart>
      </ResponsiveContainer>
    </motion.div>
  )
}

// 小型趋势图 (用于卡片中)
interface MiniTrendChartProps {
  data: TrendDataPoint[]
  color?: string
  height?: number
  className?: string
}

export function MiniTrendChart({
  data,
  color = 'rgb(var(--primary))',
  height = 40,
  className
}: MiniTrendChartProps) {
  if (data.length === 0) {
    return (
      <div
        className={clsx('bg-bg-secondary rounded', className)}
        style={{ height }}
      />
    )
  }

  return (
    <ResponsiveContainer width="100%" height={height} className={className}>
      <AreaChart data={data}>
        <defs>
          <linearGradient id="miniGradient" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor={color} stopOpacity={0.4} />
            <stop offset="95%" stopColor={color} stopOpacity={0.1} />
          </linearGradient>
        </defs>
        <Area
          type="monotone"
          dataKey="value"
          stroke={color}
          strokeWidth={1.5}
          fill="url(#miniGradient)"
          animationDuration={800}
        />
      </AreaChart>
    </ResponsiveContainer>
  )
}