import { ReactNode, useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { clsx } from 'clsx'
import { GlowCard } from '../components/fx/GlowCard'
import { GradientText } from '../components/fx/GradientText'
import { ShimmerButton } from '../components/fx/ShimmerButton'
import { Grid, GridItem } from '../components/layout/Grid'
import { Permit } from '../components/common/Permit'
import { useConfirmDialog } from '../components/common/ConfirmDialog'
import { DataTable } from '../components/data/DataTable'
import { Statistic } from '../components/data/Statistic'
import { Gauge } from '../components/data/Gauge'
import { AreaChart, XAxis, YAxis, CartesianGrid, Tooltip, Area, ResponsiveContainer } from 'recharts'
import { usePerm } from '../hooks/usePerm'
import { useToast } from '../hooks/useToast'
import { api } from './api'

// Schema 节点类型定义
export interface SchemaNode {
  type: string
  id?: string
  title?: string
  label?: string
  name?: string
  permission?: string
  visibleWhen?: string
  className?: string
  children?: SchemaNode[]
  fields?: SchemaNode[]
  actions?: SchemaNode[]
  columns?: any[]
  data?: string
  span?: any
  [key: string]: any
}

// 动作注册表
export const actionsRegistry = {
  submit: async (formId: string, formData: any) => {
    console.log('提交表单:', formId, formData)
    const response = await api.post('/form/submit', { formId, ...formData })
    return response
  },

  confirmSubmit: async (formId: string, formData: any, summary?: any) => {
    console.log('确认提交:', formId, formData, summary)
    // 这里会触发确认对话框
    return new Promise((resolve) => {
      // 实际实现需要与 ConfirmDialog 集成
      resolve({ code: 0, msg: '提交成功' })
    })
  },

  edit: async (id: string, data: any) => {
    console.log('编辑:', id, data)
    const response = await api.put(`/data/${id}`, data)
    return response
  },

  delete: async (id: string) => {
    console.log('删除:', id)
    const response = await api.delete(`/data/${id}`)
    return response
  },

  banPlayer: async (uin: string) => {
    console.log('封禁玩家:', uin)
    const response = await api.post('/player/ban', { uin })
    return response
  },

  unbanPlayer: async (uin: string) => {
    console.log('解封玩家:', uin)
    const response = await api.post('/player/unban', { uin })
    return response
  },

  resetSecPwd: async (uin: string) => {
    console.log('重置二级密码:', uin)
    const response = await api.post('/player/reset-sec-pwd', { uin })
    return response
  }
}

// 表达式求值器（安全版）
function evaluateExpression(expression: string, context: any): boolean {
  try {
    // 简单的表达式求值，只支持白名单变量
    const safeContext = {
      data: context.data || {},
      pageState: context.pageState || {},
      user: context.user || {}
    }

    // 移除所有 {{ }} 包装
    const cleanExpr = expression.replace(/\{\{|\}\}/g, '').trim()

    // 简单的条件判断
    if (cleanExpr.includes('===')) {
      const [left, right] = cleanExpr.split('===').map(s => s.trim())
      const leftValue = getNestedValue(safeContext, left)
      const rightValue = right.replace(/['"]/g, '')
      return leftValue === rightValue
    }

    if (cleanExpr.includes('!==')) {
      const [left, right] = cleanExpr.split('!==').map(s => s.trim())
      const leftValue = getNestedValue(safeContext, left)
      const rightValue = right.replace(/['"]/g, '')
      return leftValue !== rightValue
    }

    // 直接变量求值
    return !!getNestedValue(safeContext, cleanExpr)
  } catch (error) {
    console.warn('表达式求值失败:', expression, error)
    return false
  }
}

// 获取嵌套对象值
function getNestedValue(obj: any, path: string): any {
  return path.split('.').reduce((current, key) => current?.[key], obj)
}

// 模板字符串替换
function interpolateTemplate(template: string, context: any): string {
  if (!template || typeof template !== 'string') return template

  return template.replace(/\{\{([^}]+)\}\}/g, (match, expr) => {
    try {
      const value = getNestedValue(context, expr.trim())
      return value?.toString() || ''
    } catch (error) {
      console.warn('模板插值失败:', expr, error)
      return match
    }
  })
}

// 组件映射
const ComponentMap = {
  // 布局组件
  Page: PageComponent,
  Grid: GridComponent,
  GridItem: GridItemComponent,
  Card: CardComponent,

  // 表单组件
  Form: FormComponent,
  Input: InputComponent,
  NumberInput: NumberInputComponent,
  Select: SelectComponent,
  TextArea: TextAreaComponent,
  Switch: SwitchComponent,
  Button: ButtonComponent,

  // 数据组件
  Text: TextComponent,
  Statistic: StatisticComponent,
  Gauge: GaugeComponent,
  Table: TableComponent,
  DataTable: DataTableComponent,
  AreaChart: AreaChartComponent,
  LineChart: LineChartComponent,

  // 选择器组件
  PlayerSelector: PlayerSelectorComponent,
  ItemSelector: ItemSelectorComponent,
}

// Schema 渲染器主组件
export function SchemaRenderer({
  schema,
  context = {},
  onAction
}: {
  schema: SchemaNode
  context?: any
  onAction?: (action: string, data: any) => void
}) {
  const [pageData, setPageData] = useState<any>(null)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string | null>(null)
  const { userInfo } = usePerm()
  const { toast } = useToast()

  // 加载页面数据
  useEffect(() => {
    if (schema.dataSource) {
      loadData(schema.dataSource)
    }
  }, [schema.dataSource])

  const loadData = async (dataSource: string) => {
    try {
      setLoading(true)
      setError(null)
      const interpolatedUrl = interpolateTemplate(dataSource, context)
      const response = await api.get(interpolatedUrl)
      if (response.code === 0) {
        setPageData(response.data)
      } else {
        setError(response.msg || '数据加载失败')
        toast({
          title: '数据加载失败',
          description: response.msg || '未知错误',
          variant: 'destructive'
        })
      }
    } catch (error) {
      const errorMessage = error instanceof Error ? error.message : '网络请求失败'
      setError(errorMessage)
      console.error('加载数据失败:', error)
      toast({
        title: '网络错误',
        description: errorMessage,
        variant: 'destructive'
      })
    } finally {
      setLoading(false)
    }
  }

  const renderContext = {
    ...context,
    data: pageData,
    loading,
    error,
    user: userInfo,
    onAction: onAction || handleAction
  }

  // 错误状态
  if (error && !loading) {
    return (
      <div className="schema-renderer">
        <GlowCard className="p-6 text-center">
          <div className="text-error text-lg font-medium mb-2">数据加载失败</div>
          <div className="text-text-muted">{error}</div>
          <ShimmerButton
            onClick={() => schema.dataSource && loadData(schema.dataSource)}
            className="mt-4"
          >
            重试
          </ShimmerButton>
        </GlowCard>
      </div>
    )
  }

  // 加载状态
  if (loading) {
    return (
      <div className="schema-renderer">
        <GlowCard className="p-6">
          <div className="space-y-4">
            {Array.from({ length: 3 }).map((_, i) => (
              <div key={i} className="animate-pulse">
                <div className="h-4 bg-bg-secondary rounded"></div>
              </div>
            ))}
          </div>
        </GlowCard>
      </div>
    )
  }

  return (
    <div className="schema-renderer">
      {renderNode(schema, renderContext)}
    </div>
  )
}

// 渲染节点
function renderNode(node: SchemaNode, context: any): ReactNode {
  if (!node || !node.type) return null

  // 可见性检查
  if (node.visibleWhen && !evaluateExpression(node.visibleWhen, context)) {
    return null
  }

  const Component = ComponentMap[node.type as keyof typeof ComponentMap]
  if (!Component) {
    console.warn('未知组件类型:', node.type)
    return <div>未知组件: {node.type}</div>
  }

  // 如果有权限要求，使用Permit包裹
  if (node.permission) {
    return (
      <Permit key={node.id || Math.random()} perm={node.permission}>
        <Component node={node} context={context} />
      </Permit>
    )
  }

  return <Component key={node.id || Math.random()} node={node} context={context} />
}

// 处理动作
function handleAction(action: string, data: any) {
  const handler = actionsRegistry[action as keyof typeof actionsRegistry]
  if (handler) {
    return handler(data.formId || data.id, data)
  }
  console.warn('未知动作:', action)
}

// 组件实现
function PageComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div className={clsx('page-component', node.className)}>
      {node.title && (
        <div className="mb-6">
          <GradientText className="text-3xl font-bold">
            {interpolateTemplate(node.title, context)}
          </GradientText>
        </div>
      )}
      {node.body?.map(child => renderNode(child, context))}
    </div>
  )
}

function GridComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <Grid cols={node.cols || 12} gap={node.gap || 4} className={node.className}>
      {node.children?.map(child => renderNode(child, context))}
    </Grid>
  )
}

function GridItemComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <GridItem span={node.span} className={node.className}>
      {node.children?.map(child => renderNode(child, context))}
    </GridItem>
  )
}

function CardComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <GlowCard className={node.className}>
      {node.title && (
        <GradientText className="text-xl mb-4">
          {interpolateTemplate(node.title, context)}
        </GradientText>
      )}
      {node.children?.map(child => renderNode(child, context))}
    </GlowCard>
  )
}

function FormComponent({ node, context }: { node: SchemaNode; context: any }) {
  const [formData, setFormData] = useState<any>({})
  const [submitting, setSubmitting] = useState(false)
  const { openDialog, ConfirmDialog } = useConfirmDialog()
  const { toast } = useToast()

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()

    if (submitting) return

    try {
      setSubmitting(true)

      if (node.actions) {
        for (const action of node.actions) {
          if (action.onClick === 'confirmSubmit' && action.confirm) {
            openDialog({
              title: `确认${action.text}`,
              content: (
                <div>
                  <p>请确认要执行此操作：</p>
                  <div className="mt-2 p-3 bg-bg-secondary rounded">
                    {Object.entries(formData).map(([key, value]) => (
                      <div key={key} className="flex justify-between">
                        <span>{key}:</span>
                        <span>{String(value)}</span>
                      </div>
                    ))}
                  </div>
                </div>
              ),
              requireKeyword: action.confirm.keyword,
              onConfirm: async () => {
                try {
                  const result = await context.onAction(action.onClick, { formId: node.id, ...formData })
                  if (result?.code === 0) {
                    toast({
                      title: '操作成功',
                      description: result.msg || '操作已完成',
                      variant: 'default'
                    })
                  }
                } catch (error) {
                  toast({
                    title: '操作失败',
                    description: error instanceof Error ? error.message : '未知错误',
                    variant: 'destructive'
                  })
                }
              }
            })
          } else if (action.onClick) {
            const result = await context.onAction(action.onClick, { formId: node.id, ...formData })
            if (result?.code === 0) {
              toast({
                title: '提交成功',
                description: result.msg || '操作已完成',
                variant: 'default'
              })
            } else {
              toast({
                title: '提交失败',
                description: result?.msg || '操作失败',
                variant: 'destructive'
              })
            }
          }
        }
      }
    } catch (error) {
      toast({
        title: '提交失败',
        description: error instanceof Error ? error.message : '未知错误',
        variant: 'destructive'
      })
    } finally {
      setSubmitting(false)
    }
  }

  const updateFormData = (name: string, value: any) => {
    setFormData(prev => ({ ...prev, [name]: value }))
  }

  return (
    <div className={clsx('form-component', node.className)}>
      <form onSubmit={handleSubmit} className="space-y-4">
        {node.fields?.map(field => renderNode({
          ...field,
          onChange: (value: any) => updateFormData(field.name!, value),
          disabled: submitting
        }, { ...context, formData }))}

        {node.actions && (
          <div className="flex gap-3 pt-4">
            {node.actions.map(action => renderNode({
              ...action,
              disabled: submitting,
              loading: submitting
            }, context))}
          </div>
        )}
      </form>
      <ConfirmDialog />
    </div>
  )
}

// 表单字段组件
function InputComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div>
      {node.label && <label className="block text-sm font-medium mb-1">{node.label}</label>}
      <input
        type="text"
        name={node.name}
        placeholder={node.placeholder}
        required={node.required}
        readOnly={node.readOnly}
        className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
        onChange={(e) => node.onChange?.(e.target.value)}
      />
    </div>
  )
}

function NumberInputComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div>
      {node.label && <label className="block text-sm font-medium mb-1">{node.label}</label>}
      <input
        type="number"
        name={node.name}
        min={node.min}
        max={node.max}
        required={node.required}
        className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
        onChange={(e) => node.onChange?.(Number(e.target.value))}
      />
    </div>
  )
}

function SelectComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div>
      {node.label && <label className="block text-sm font-medium mb-1">{node.label}</label>}
      <select
        name={node.name}
        required={node.required}
        className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
        onChange={(e) => node.onChange?.(e.target.value)}
      >
        {node.options?.map((option: string) => (
          <option key={option} value={option}>{option}</option>
        ))}
      </select>
    </div>
  )
}

function TextAreaComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div>
      {node.label && <label className="block text-sm font-medium mb-1">{node.label}</label>}
      <textarea
        name={node.name}
        maxLength={node.maxLength}
        required={node.required}
        className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
        rows={3}
        onChange={(e) => node.onChange?.(e.target.value)}
      />
    </div>
  )
}

function SwitchComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div className="flex items-center gap-2">
      <input
        type="checkbox"
        name={node.name}
        className="w-4 h-4 text-primary bg-bg-secondary border-border rounded focus:ring-primary"
        onChange={(e) => node.onChange?.(e.target.checked)}
      />
      {node.label && <label className="text-sm font-medium">{node.label}</label>}
    </div>
  )
}

function ButtonComponent({ node, context }: { node: SchemaNode; context: any }) {
  const handleClick = () => {
    if (node.onClick && context.onAction) {
      context.onAction(node.onClick, { id: node.id })
    }
  }

  if (node.permission) {
    return (
      <Permit perm={node.permission}>
        <ShimmerButton onClick={handleClick} variant={node.style === 'primary' ? 'primary' : 'secondary'}>
          {node.text}
        </ShimmerButton>
      </Permit>
    )
  }

  return (
    <ShimmerButton onClick={handleClick} variant={node.style === 'primary' ? 'primary' : 'secondary'}>
      {node.text}
    </ShimmerButton>
  )
}

// 数据展示组件
function TextComponent({ node, context }: { node: SchemaNode; context: any }) {
  const value = interpolateTemplate(node.value || '', context)
  return (
    <div className={node.className}>
      {node.label && <span className="text-text-secondary">{node.label}: </span>}
      <span>{value}</span>
    </div>
  )
}

function StatisticComponent({ node, context }: { node: SchemaNode; context: any }) {
  const value = interpolateTemplate(node.value || '', context)
  return (
    <Statistic
      label={node.label}
      value={value}
      className={node.className}
    />
  )
}

function GaugeComponent({ node, context }: { node: SchemaNode; context: any }) {
  const value = parseFloat(interpolateTemplate(node.value || '0', context))
  return (
    <Gauge
      label={node.label}
      value={value}
      className={node.className}
    />
  )
}

function TableComponent({ node, context }: { node: SchemaNode; context: any }) {
  const data = getNestedValue(context, node.data || 'data') || []

  return (
    <DataTable
      columns={node.columns || []}
      data={data}
      className={node.className}
    />
  )
}

function DataTableComponent({ node, context }: { node: SchemaNode; context: any }) {
  return <TableComponent node={node} context={context} />
}

function AreaChartComponent({ node, context }: { node: SchemaNode; context: any }) {
  const data = getNestedValue(context, node.data || 'data') || []

  return (
    <ResponsiveContainer width="100%" height={200}>
      <AreaChart data={data}>
        <CartesianGrid strokeDasharray="3 3" />
        <XAxis dataKey={node.x} />
        <YAxis />
        <Tooltip />
        <Area type="monotone" dataKey={node.y} stroke="rgb(var(--primary))" fill="rgb(var(--primary) / 0.3)" />
      </AreaChart>
    </ResponsiveContainer>
  )
}

function LineChartComponent({ node, context }: { node: SchemaNode; context: any }) {
  return <AreaChartComponent node={node} context={context} />
}

// 业务选择器组件
function PlayerSelectorComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div>
      {node.label && <label className="block text-sm font-medium mb-1">{node.label}</label>}
      <input
        type="text"
        name={node.name}
        placeholder="输入 UIN 或昵称"
        required={node.required}
        className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
        onChange={(e) => node.onChange?.(e.target.value)}
      />
    </div>
  )
}

function ItemSelectorComponent({ node, context }: { node: SchemaNode; context: any }) {
  return (
    <div>
      {node.label && <label className="block text-sm font-medium mb-1">{node.label}</label>}
      <select
        name={node.name}
        required={node.required}
        className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
        onChange={(e) => node.onChange?.(e.target.value)}
      >
        <option value="">选择道具</option>
        <option value="item_1001">强化石</option>
        <option value="item_1002">经验药水</option>
        <option value="item_1003">金币</option>
      </select>
    </div>
  )
}