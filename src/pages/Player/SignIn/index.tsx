import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { GlowCard } from '../../../components/fx/GlowCard'
import { GradientText } from '../../../components/fx/GradientText'
import { ShimmerButton } from '../../../components/fx/ShimmerButton'
import { DataTable } from '../../../components/data/DataTable'
import { Permit } from '../../../components/common/Permit'
import { useConfirmDialog } from '../../../components/common/ConfirmDialog'
import { useToast } from '../../../hooks/useToast'
import { api } from '../../../utils/api'

interface SignInRule {
  day: number
  type: 'exp' | 'cash_a' | 'cash_b' | 'item'
  amount: number
  item_id?: string
  item_name?: string
  is_special?: boolean // 特殊奖励（如第7天）
}

interface SignInConfig {
  id: string
  name: string
  description: string
  rules: SignInRule[]
  is_active: boolean
  start_date: string
  end_date?: string
  created_at: string
  updated_at: string
}

interface PlayerSignIn {
  uin: string
  nickname: string
  current_month: string
  signed_days: number[]
  last_sign_date?: string
  consecutive_days: number
  total_rewards: {
    exp: number
    cash_a: number
    cash_b: number
    items: number
  }
}

const DEFAULT_SIGNIN_RULES: SignInRule[] = [
  { day: 1, type: 'exp', amount: 1000 },
  { day: 2, type: 'cash_a', amount: 50 },
  { day: 3, type: 'exp', amount: 1500 },
  { day: 4, type: 'cash_b', amount: 100 },
  { day: 5, type: 'exp', amount: 2000 },
  { day: 6, type: 'item', amount: 1, item_id: '100001', item_name: '经验药水' },
  { day: 7, type: 'cash_a', amount: 200, is_special: true }
]

export function PlayerSignIn() {
  const [configs, setConfigs] = useState<SignInConfig[]>([])
  const [loading, setLoading] = useState(true)
  const [selectedConfig, setSelectedConfig] = useState<SignInConfig | null>(null)
  const [editingConfig, setEditingConfig] = useState<SignInConfig | null>(null)
  const [showConfigModal, setShowConfigModal] = useState(false)
  const [playerSignIn, setPlayerSignIn] = useState<PlayerSignIn | null>(null)
  const [selectedPlayer, setSelectedPlayer] = useState('')
  const { openDialog, ConfirmDialog } = useConfirmDialog()
  const { toast } = useToast()

  useEffect(() => {
    loadConfigs()
  }, [])

  const loadConfigs = async () => {
    try {
      setLoading(true)
      const response = await api.get('/signin/configs')
      if (response.code === 0) {
        setConfigs(response.data.configs || [])
        // 设置默认选中活跃的配置
        const activeConfig = response.data.configs?.find((c: SignInConfig) => c.is_active)
        if (activeConfig) {
          setSelectedConfig(activeConfig)
        }
      }
    } catch (error) {
      console.error('加载签到配置失败:', error)
    } finally {
      setLoading(false)
    }
  }

  const loadPlayerSignIn = async () => {
    if (!selectedPlayer) return

    try {
      const response = await api.get(`/player/${selectedPlayer}/signin`)
      if (response.code === 0) {
        setPlayerSignIn(response.data)
      } else {
        toast({
          title: '查询失败',
          description: response.msg || '未找到玩家签到数据',
          variant: 'destructive'
        })
        setPlayerSignIn(null)
      }
    } catch (error) {
      console.error('加载玩家签到数据失败:', error)
      toast({
        title: '网络错误',
        description: '无法连接到服务器',
        variant: 'destructive'
      })
    }
  }

  const handleSaveConfig = async (config: Partial<SignInConfig>) => {
    try {
      let response
      if (editingConfig?.id) {
        response = await api.put(`/signin/configs/${editingConfig.id}`, config)
      } else {
        response = await api.post('/signin/configs', config)
      }

      if (response.code === 0) {
        toast({
          title: '保存成功',
          description: editingConfig ? '签到配置已更新' : '新签到配置已创建',
          variant: 'default'
        })
        loadConfigs()
        setShowConfigModal(false)
        setEditingConfig(null)
      } else {
        toast({
          title: '保存失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('保存配置失败:', error)
      toast({
        title: '操作失败',
        description: '保存配置失败',
        variant: 'destructive'
      })
    }
  }

  const handleActivateConfig = async (configId: string) => {
    try {
      const response = await api.post(`/signin/configs/${configId}/activate`)
      if (response.code === 0) {
        toast({
          title: '激活成功',
          description: '签到配置已激活',
          variant: 'default'
        })
        loadConfigs()
      } else {
        toast({
          title: '激活失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('激活配置失败:', error)
      toast({
        title: '操作失败',
        description: '激活配置失败',
        variant: 'destructive'
      })
    }
  }

  const handleManualSignIn = async (day: number) => {
    if (!selectedPlayer) return

    try {
      const response = await api.post(`/player/${selectedPlayer}/signin/manual`, { day })
      if (response.code === 0) {
        toast({
          title: '签到成功',
          description: `已为玩家补签第${day}天`,
          variant: 'default'
        })
        loadPlayerSignIn()
      } else {
        toast({
          title: '签到失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('手动签到失败:', error)
      toast({
        title: '操作失败',
        description: '手动签到失败',
        variant: 'destructive'
      })
    }
  }

  const handleResetPlayerSignIn = async () => {
    if (!selectedPlayer) return

    try {
      const response = await api.delete(`/player/${selectedPlayer}/signin`)
      if (response.code === 0) {
        toast({
          title: '重置成功',
          description: '玩家签到数据已重置',
          variant: 'default'
        })
        loadPlayerSignIn()
      } else {
        toast({
          title: '重置失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('重置签到失败:', error)
      toast({
        title: '操作失败',
        description: '重置签到失败',
        variant: 'destructive'
      })
    }
  }

  const getRewardTypeLabel = (type: string) => {
    const types = {
      exp: '经验值',
      cash_a: '点卷',
      cash_b: '消费卷',
      item: '道具'
    }
    return types[type as keyof typeof types] || type
  }

  const getRewardTypeColor = (type: string) => {
    const colors = {
      exp: 'text-green-400',
      cash_a: 'text-blue-400',
      cash_b: 'text-yellow-400',
      item: 'text-purple-400'
    }
    return colors[type as keyof typeof colors] || 'text-text'
  }

  const formatReward = (rule: SignInRule) => {
    if (rule.type === 'item') {
      return `${rule.item_name || '未知道具'} x${rule.amount}`
    }
    return `${rule.amount.toLocaleString()}`
  }

  const configColumns = [
    {
      key: 'name',
      title: '配置名称',
      dataIndex: 'name',
      render: (value: string, record: SignInConfig) => (
        <div>
          <div className="font-medium">{value}</div>
          <div className="text-xs text-text-muted">{record.description}</div>
        </div>
      )
    },
    {
      key: 'is_active',
      title: '状态',
      dataIndex: 'is_active',
      align: 'center' as const,
      render: (value: boolean) => (
        <span className={`px-2 py-1 text-xs rounded ${
          value
            ? 'bg-success/20 text-success'
            : 'bg-error/20 text-error'
        }`}>
          {value ? '激活' : '停用'}
        </span>
      )
    },
    {
      key: 'rules_count',
      title: '规则数量',
      render: (_, record: SignInConfig) => (
        <span className="font-mono text-primary">{record.rules.length}</span>
      )
    },
    {
      key: 'date_range',
      title: '时间范围',
      render: (_, record: SignInConfig) => (
        <div className="text-xs">
          <div>开始: {record.start_date}</div>
          {record.end_date && <div>结束: {record.end_date}</div>}
        </div>
      )
    },
    {
      key: 'actions',
      title: '操作',
      render: (_, record: SignInConfig) => (
        <div className="flex gap-1">
          <button
            onClick={() => setSelectedConfig(record)}
            className="px-2 py-1 text-xs bg-bg-secondary hover:bg-bg-tertiary rounded transition-colors"
          >
            查看
          </button>
          <Permit perm="SIGNIN_EDIT">
            <button
              onClick={() => {
                setEditingConfig(record)
                setShowConfigModal(true)
              }}
              className="px-2 py-1 text-xs bg-primary/20 text-primary rounded hover:bg-primary/30 transition-colors"
            >
              编辑
            </button>
          </Permit>
          <Permit perm="SIGNIN_EDIT">
            {!record.is_active && (
              <button
                onClick={() => handleActivateConfig(record.id)}
                className="px-2 py-1 text-xs bg-success/20 text-success rounded hover:bg-success/30 transition-colors"
              >
                激活
              </button>
            )}
          </Permit>
        </div>
      )
    }
  ]

  const renderCalendar = () => {
    if (!selectedConfig) return null

    const daysInMonth = 31 // 简化为31天
    const calendar = []

    for (let day = 1; day <= daysInMonth; day++) {
      const rule = selectedConfig.rules.find(r => r.day === day)
      const isSigned = playerSignIn?.signed_days.includes(day) || false
      const isToday = day === new Date().getDate()

      calendar.push(
        <div
          key={day}
          className={`
            relative p-2 border rounded-lg text-center cursor-pointer transition-all
            ${isSigned
              ? 'bg-success/20 border-success/30 text-success'
              : rule
                ? 'bg-primary/10 border-primary/30 hover:bg-primary/20'
                : 'bg-bg-secondary border-border'
            }
            ${isToday ? 'ring-2 ring-accent' : ''}
            ${rule?.is_special ? 'bg-gradient-to-br from-yellow-500/20 to-orange-500/20' : ''}
          `}
          onClick={() => {
            if (rule && !isSigned && playerSignIn) {
              openDialog({
                title: '确认补签',
                content: `确定要为玩家 ${playerSignIn.nickname} 补签第${day}天吗？将获得奖励：${formatReward(rule)}`,
                requireKeyword: 'CONFIRM',
                variant: 'primary',
                onConfirm: () => handleManualSignIn(day)
              })
            }
          }}
        >
          <div className="font-bold">{day}</div>
          {isSigned && (
            <div className="absolute top-0 right-0 w-2 h-2 bg-success rounded-full transform translate-x-1 -translate-y-1" />
          )}
          {rule && (
            <div className="text-xs mt-1">
              <div className={getRewardTypeColor(rule.type)}>
                {getRewardTypeLabel(rule.type)}
              </div>
              <div className="font-mono text-xs">
                {formatReward(rule)}
              </div>
            </div>
          )}
          {rule?.is_special && (
            <div className="absolute -top-1 -left-1 text-yellow-400">⭐</div>
          )}
        </div>
      )
    }

    return (
      <div className="grid grid-cols-7 gap-2">
        {['周一', '周二', '周三', '周四', '周五', '周六', '周日'].map(day => (
          <div key={day} className="text-center text-sm font-medium text-text-muted p-2">
            {day}
          </div>
        ))}
        {calendar}
      </div>
    )
  }

  return (
    <div className="max-w-7xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          签到管理
        </GradientText>
        <p className="text-text-muted mt-2">配置每日签到规则和查看玩家签到状态</p>
      </div>

      <div className="grid gap-6">
        {/* 配置管理 */}
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          <GlowCard>
            <div className="flex items-center justify-between mb-4">
              <h3 className="text-lg font-semibold">签到配置列表</h3>
              <Permit perm="SIGNIN_EDIT">
                <ShimmerButton
                  onClick={() => {
                    setEditingConfig(null)
                    setShowConfigModal(true)
                  }}
                  variant="primary"
                  size="sm"
                >
                  新建配置
                </ShimmerButton>
              </Permit>
            </div>
            <DataTable
              columns={configColumns}
              data={configs}
              loading={loading}
              rowKey="id"
              pageSize={5}
            />
          </GlowCard>

          <GlowCard>
            <h3 className="text-lg font-semibold mb-4">玩家签到查询</h3>
            <div className="space-y-4">
              <div>
                <label className="block text-sm font-medium mb-1">玩家 UIN</label>
                <input
                  type="text"
                  value={selectedPlayer}
                  onChange={(e) => setSelectedPlayer(e.target.value)}
                  placeholder="输入玩家 UIN"
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                />
              </div>
              <ShimmerButton
                onClick={loadPlayerSignIn}
                variant="primary"
                className="w-full"
                disabled={!selectedPlayer}
              >
                查询签到记录
              </ShimmerButton>

              {playerSignIn && (
                <motion.div
                  initial={{ opacity: 0, y: 10 }}
                  animate={{ opacity: 1, y: 0 }}
                  className="space-y-3 pt-4 border-t border-border"
                >
                  <div className="flex justify-between">
                    <span className="text-text-muted">玩家昵称:</span>
                    <span className="font-medium">{playerSignIn.nickname}</span>
                  </div>
                  <div className="flex justify-between">
                    <span className="text-text-muted">本月签到:</span>
                    <span className="font-mono text-primary">{playerSignIn.signed_days.length}天</span>
                  </div>
                  <div className="flex justify-between">
                    <span className="text-text-muted">连续签到:</span>
                    <span className="font-mono text-success">{playerSignIn.consecutive_days}天</span>
                  </div>
                  <div className="flex justify-between">
                    <span className="text-text-muted">最后签到:</span>
                    <span className="text-sm">{playerSignIn.last_sign_date || '从未签到'}</span>
                  </div>

                  <Permit perm="SIGNIN_EDIT">
                    <ShimmerButton
                      onClick={() => openDialog({
                        title: '确认重置签到',
                        content: `确定要重置玩家 ${playerSignIn.nickname} 的签到数据吗？此操作不可撤销！`,
                        requireKeyword: 'CONFIRM',
                        variant: 'error',
                        onConfirm: handleResetPlayerSignIn
                      })}
                      variant="error"
                      className="w-full"
                    >
                      重置签到数据
                    </ShimmerButton>
                  </Permit>
                </motion.div>
              )}
            </div>
          </GlowCard>
        </div>

        {/* 签到日历 */}
        {selectedConfig && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
          >
            <GlowCard>
              <div className="flex items-center justify-between mb-6">
                <div>
                  <h3 className="text-lg font-semibold">
                    签到日历 - {selectedConfig.name}
                  </h3>
                  <p className="text-sm text-text-muted mt-1">
                    {selectedConfig.description}
                  </p>
                </div>
                <div className="text-right">
                  <div className="text-sm text-text-muted">当前月份</div>
                  <div className="font-semibold">
                    {new Date().toLocaleDateString('zh-CN', { year: 'numeric', month: 'long' })}
                  </div>
                </div>
              </div>

              {renderCalendar()}

              <div className="mt-6 pt-4 border-t border-border">
                <div className="flex items-center gap-4 text-xs text-text-muted">
                  <div className="flex items-center gap-1">
                    <div className="w-3 h-3 bg-success/20 border border-success/30 rounded" />
                    <span>已签到</span>
                  </div>
                  <div className="flex items-center gap-1">
                    <div className="w-3 h-3 bg-primary/10 border border-primary/30 rounded" />
                    <span>可签到</span>
                  </div>
                  <div className="flex items-center gap-1">
                    <div className="w-3 h-3 bg-gradient-to-br from-yellow-500/20 to-orange-500/20 border border-yellow-500/30 rounded" />
                    <span>特殊奖励</span>
                  </div>
                  <div className="flex items-center gap-1">
                    <div className="w-3 h-3 bg-bg-secondary border border-border rounded" />
                    <span>无奖励</span>
                  </div>
                </div>
              </div>
            </GlowCard>
          </motion.div>
        )}

        {!selectedConfig && configs.length === 0 && !loading && (
          <div className="text-center py-12">
            <div className="text-6xl mb-4">📅</div>
            <h3 className="text-lg font-medium text-text mb-2">暂无签到配置</h3>
            <p className="text-text-muted mb-4">创建第一个签到配置来开始管理每日签到</p>
            <Permit perm="SIGNIN_EDIT">
              <ShimmerButton
                onClick={() => {
                  setEditingConfig(null)
                  setShowConfigModal(true)
                }}
                variant="primary"
              >
                创建签到配置
              </ShimmerButton>
            </Permit>
          </div>
        )}
      </div>

      {/* 配置编辑模态框 */}
      {showConfigModal && (
        <SignInConfigModal
          config={editingConfig}
          onSave={handleSaveConfig}
          onClose={() => {
            setShowConfigModal(false)
            setEditingConfig(null)
          }}
        />
      )}

      <ConfirmDialog />
    </div>
  )
}

// 签到配置编辑模态框组件
function SignInConfigModal({
  config,
  onSave,
  onClose
}: {
  config: SignInConfig | null
  onSave: (config: Partial<SignInConfig>) => void
  onClose: () => void
}) {
  const [formData, setFormData] = useState({
    name: config?.name || '',
    description: config?.description || '',
    start_date: config?.start_date || new Date().toISOString().split('T')[0],
    end_date: config?.end_date || '',
    rules: config?.rules || [...DEFAULT_SIGNIN_RULES]
  })

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault()
    onSave(formData)
  }

  const updateRule = (index: number, updates: Partial<SignInRule>) => {
    const newRules = [...formData.rules]
    newRules[index] = { ...newRules[index], ...updates }
    setFormData({ ...formData, rules: newRules })
  }

  return (
    <div className="fixed inset-0 bg-black/50 backdrop-blur-sm z-50 flex items-center justify-center p-4">
      <motion.div
        initial={{ opacity: 0, scale: 0.9 }}
        animate={{ opacity: 1, scale: 1 }}
        className="bg-bg-primary border border-border rounded-lg shadow-2xl max-w-4xl w-full max-h-[90vh] overflow-auto"
      >
        <form onSubmit={handleSubmit} className="p-6">
          <div className="flex items-center justify-between mb-6">
            <h3 className="text-xl font-semibold">
              {config ? '编辑签到配置' : '新建签到配置'}
            </h3>
            <button
              type="button"
              onClick={onClose}
              className="w-8 h-8 flex items-center justify-center rounded-lg hover:bg-bg-secondary transition-colors"
            >
              ✕
            </button>
          </div>

          <div className="grid grid-cols-1 md:grid-cols-2 gap-6 mb-6">
            <div>
              <label className="block text-sm font-medium mb-1">配置名称</label>
              <input
                type="text"
                value={formData.name}
                onChange={(e) => setFormData({ ...formData, name: e.target.value })}
                required
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              />
            </div>
            <div>
              <label className="block text-sm font-medium mb-1">配置描述</label>
              <input
                type="text"
                value={formData.description}
                onChange={(e) => setFormData({ ...formData, description: e.target.value })}
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              />
            </div>
            <div>
              <label className="block text-sm font-medium mb-1">开始日期</label>
              <input
                type="date"
                value={formData.start_date}
                onChange={(e) => setFormData({ ...formData, start_date: e.target.value })}
                required
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              />
            </div>
            <div>
              <label className="block text-sm font-medium mb-1">结束日期（可选）</label>
              <input
                type="date"
                value={formData.end_date}
                onChange={(e) => setFormData({ ...formData, end_date: e.target.value })}
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              />
            </div>
          </div>

          <div className="mb-6">
            <h4 className="text-lg font-semibold mb-4">签到规则配置</h4>
            <div className="space-y-3 max-h-60 overflow-y-auto">
              {formData.rules.map((rule, index) => (
                <div key={index} className="grid grid-cols-6 gap-2 p-3 bg-bg-secondary rounded-lg">
                  <div>
                    <label className="block text-xs text-text-muted mb-1">天数</label>
                    <input
                      type="number"
                      value={rule.day}
                      onChange={(e) => updateRule(index, { day: Number(e.target.value) })}
                      min="1"
                      max="31"
                      className="w-full px-2 py-1 text-sm bg-bg-tertiary border border-border rounded focus:ring-1 focus:ring-primary"
                    />
                  </div>
                  <div>
                    <label className="block text-xs text-text-muted mb-1">类型</label>
                    <select
                      value={rule.type}
                      onChange={(e) => updateRule(index, { type: e.target.value as any })}
                      className="w-full px-2 py-1 text-sm bg-bg-tertiary border border-border rounded focus:ring-1 focus:ring-primary"
                    >
                      <option value="exp">经验值</option>
                      <option value="cash_a">点卷</option>
                      <option value="cash_b">消费卷</option>
                      <option value="item">道具</option>
                    </select>
                  </div>
                  <div>
                    <label className="block text-xs text-text-muted mb-1">数量</label>
                    <input
                      type="number"
                      value={rule.amount}
                      onChange={(e) => updateRule(index, { amount: Number(e.target.value) })}
                      min="1"
                      className="w-full px-2 py-1 text-sm bg-bg-tertiary border border-border rounded focus:ring-1 focus:ring-primary"
                    />
                  </div>
                  {rule.type === 'item' && (
                    <>
                      <div>
                        <label className="block text-xs text-text-muted mb-1">道具ID</label>
                        <input
                          type="text"
                          value={rule.item_id || ''}
                          onChange={(e) => updateRule(index, { item_id: e.target.value })}
                          className="w-full px-2 py-1 text-sm bg-bg-tertiary border border-border rounded focus:ring-1 focus:ring-primary"
                        />
                      </div>
                      <div>
                        <label className="block text-xs text-text-muted mb-1">道具名称</label>
                        <input
                          type="text"
                          value={rule.item_name || ''}
                          onChange={(e) => updateRule(index, { item_name: e.target.value })}
                          className="w-full px-2 py-1 text-sm bg-bg-tertiary border border-border rounded focus:ring-1 focus:ring-primary"
                        />
                      </div>
                    </>
                  )}
                  <div className="flex items-end">
                    <label className="flex items-center gap-1 text-xs">
                      <input
                        type="checkbox"
                        checked={rule.is_special || false}
                        onChange={(e) => updateRule(index, { is_special: e.target.checked })}
                        className="w-3 h-3"
                      />
                      特殊
                    </label>
                  </div>
                </div>
              ))}
            </div>
          </div>

          <div className="flex gap-3">
            <ShimmerButton type="submit" variant="primary">
              保存配置
            </ShimmerButton>
            <ShimmerButton type="button" onClick={onClose} variant="secondary">
              取消
            </ShimmerButton>
          </div>
        </form>
      </motion.div>
    </div>
  )
}