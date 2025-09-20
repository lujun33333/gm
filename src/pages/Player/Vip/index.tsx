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

interface VipLevel {
  level: number
  name: string
  required_points: number
  monthly_fee: number
  benefits: string[]
  max_online_time: number
  experience_bonus: number
  cash_bonus: number
  icon_url?: string
}

interface PlayerVip {
  uin: string
  nickname: string
  current_level: number
  points: number
  expire_time?: string
  total_consumed: number
  level_history: Array<{
    level: number
    start_time: string
    end_time?: string
    consumed: number
  }>
}

const VIP_LEVELS: VipLevel[] = [
  {
    level: 0,
    name: '普通玩家',
    required_points: 0,
    monthly_fee: 0,
    benefits: ['基础游戏功能'],
    max_online_time: 24,
    experience_bonus: 0,
    cash_bonus: 0
  },
  {
    level: 1,
    name: '青铜贵族',
    required_points: 100,
    monthly_fee: 10,
    benefits: ['10%经验加成', '5%点卷奖励', '专属称号'],
    max_online_time: 30,
    experience_bonus: 10,
    cash_bonus: 5
  },
  {
    level: 2,
    name: '白银贵族',
    required_points: 500,
    monthly_fee: 30,
    benefits: ['20%经验加成', '10%点卷奖励', '专属聊天气泡', '优先匹配'],
    max_online_time: 36,
    experience_bonus: 20,
    cash_bonus: 10
  },
  {
    level: 3,
    name: '黄金贵族',
    required_points: 1000,
    monthly_fee: 68,
    benefits: ['30%经验加成', '15%点卷奖励', '专属座驾特效', '免费改名'],
    max_online_time: 42,
    experience_bonus: 30,
    cash_bonus: 15
  },
  {
    level: 4,
    name: '白金贵族',
    required_points: 2000,
    monthly_fee: 128,
    benefits: ['40%经验加成', '20%点卷奖励', '专属贵族房间', '每日免费抽奖'],
    max_online_time: 48,
    experience_bonus: 40,
    cash_bonus: 20
  },
  {
    level: 5,
    name: '钻石贵族',
    required_points: 5000,
    monthly_fee: 298,
    benefits: ['50%经验加成', '25%点卷奖励', '专属客服', '无限改名', '专属活动'],
    max_online_time: 0, // 无限制
    experience_bonus: 50,
    cash_bonus: 25
  }
]

export function PlayerVip() {
  const [playerVip, setPlayerVip] = useState<PlayerVip | null>(null)
  const [loading, setLoading] = useState(false)
  const [selectedPlayer, setSelectedPlayer] = useState('')
  const [selectedLevel, setSelectedLevel] = useState(0)
  const [duration, setDuration] = useState(30) // 天数
  const { openDialog, ConfirmDialog } = useConfirmDialog()
  const { toast } = useToast()

  useEffect(() => {
    if (selectedPlayer) {
      loadPlayerVip()
    }
  }, [selectedPlayer])

  const loadPlayerVip = async () => {
    try {
      setLoading(true)
      const response = await api.get(`/player/${selectedPlayer}/vip`)
      if (response.code === 0) {
        setPlayerVip(response.data)
      } else {
        toast({
          title: '查询失败',
          description: response.msg || '未找到玩家数据',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('加载贵族数据失败:', error)
      toast({
        title: '网络错误',
        description: '无法连接到服务器',
        variant: 'destructive'
      })
    } finally {
      setLoading(false)
    }
  }

  const handleSetVipLevel = async () => {
    try {
      const response = await api.post(`/player/${selectedPlayer}/vip`, {
        level: selectedLevel,
        duration_days: duration
      })
      if (response.code === 0) {
        toast({
          title: '设置成功',
          description: `已为玩家设置${getVipLevel(selectedLevel).name}等级`,
          variant: 'default'
        })
        loadPlayerVip()
      } else {
        toast({
          title: '设置失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('设置贵族等级失败:', error)
      toast({
        title: '操作失败',
        description: '设置贵族等级失败',
        variant: 'destructive'
      })
    }
  }

  const handleExtendVip = async (days: number) => {
    try {
      const response = await api.post(`/player/${selectedPlayer}/vip/extend`, {
        days: days
      })
      if (response.code === 0) {
        toast({
          title: '续期成功',
          description: `贵族时间已延长${days}天`,
          variant: 'default'
        })
        loadPlayerVip()
      } else {
        toast({
          title: '续期失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('续期失败:', error)
      toast({
        title: '操作失败',
        description: '续期操作失败',
        variant: 'destructive'
      })
    }
  }

  const handleCancelVip = async () => {
    try {
      const response = await api.delete(`/player/${selectedPlayer}/vip`)
      if (response.code === 0) {
        toast({
          title: '取消成功',
          description: '已取消玩家的贵族等级',
          variant: 'default'
        })
        loadPlayerVip()
      } else {
        toast({
          title: '取消失败',
          description: response.msg || '操作失败',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('取消贵族失败:', error)
      toast({
        title: '操作失败',
        description: '取消贵族失败',
        variant: 'destructive'
      })
    }
  }

  const getVipLevel = (level: number) => {
    return VIP_LEVELS.find(v => v.level === level) || VIP_LEVELS[0]
  }

  const getVipLevelColor = (level: number) => {
    const colors = {
      0: 'text-gray-400',
      1: 'text-orange-600',
      2: 'text-gray-300',
      3: 'text-yellow-400',
      4: 'text-blue-300',
      5: 'text-purple-400'
    }
    return colors[level as keyof typeof colors] || 'text-gray-400'
  }

  const getVipBadgeColor = (level: number) => {
    const colors = {
      0: 'bg-gray-500/20 text-gray-400 border-gray-500/30',
      1: 'bg-orange-600/20 text-orange-400 border-orange-600/30',
      2: 'bg-gray-400/20 text-gray-300 border-gray-400/30',
      3: 'bg-yellow-500/20 text-yellow-400 border-yellow-500/30',
      4: 'bg-blue-500/20 text-blue-300 border-blue-500/30',
      5: 'bg-purple-500/20 text-purple-400 border-purple-500/30'
    }
    return colors[level as keyof typeof colors] || colors[0]
  }

  const formatTimeRemaining = (expireTime?: string) => {
    if (!expireTime) return '永久'
    const now = new Date()
    const expire = new Date(expireTime)
    const diff = expire.getTime() - now.getTime()
    if (diff <= 0) return '已过期'
    const days = Math.ceil(diff / (1000 * 60 * 60 * 24))
    return `${days}天`
  }

  const historyColumns = [
    {
      key: 'level',
      title: '等级',
      dataIndex: 'level',
      render: (value: number) => (
        <span className={`font-semibold ${getVipLevelColor(value)}`}>
          {getVipLevel(value).name}
        </span>
      )
    },
    {
      key: 'start_time',
      title: '开始时间',
      dataIndex: 'start_time'
    },
    {
      key: 'end_time',
      title: '结束时间',
      dataIndex: 'end_time',
      render: (value: string) => value || '进行中'
    },
    {
      key: 'consumed',
      title: '消费金额',
      dataIndex: 'consumed',
      render: (value: number) => (
        <span className="font-mono text-primary">¥{value.toFixed(2)}</span>
      )
    }
  ]

  return (
    <div className="max-w-6xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          贵族等级管理
        </GradientText>
        <p className="text-text-muted mt-2">管理玩家的贵族等级和权益</p>
      </div>

      <div className="grid gap-6">
        {/* 查询区域 */}
        <GlowCard>
          <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
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
            <div className="flex items-end">
              <ShimmerButton
                onClick={loadPlayerVip}
                loading={loading}
                variant="primary"
                disabled={!selectedPlayer}
              >
                查询贵族信息
              </ShimmerButton>
            </div>
          </div>
        </GlowCard>

        {/* 贵族等级说明 */}
        <GlowCard>
          <h3 className="text-lg font-semibold mb-4">贵族等级体系</h3>
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
            {VIP_LEVELS.map(level => (
              <div key={level.level} className="border border-border rounded-lg p-4">
                <div className="flex items-center justify-between mb-2">
                  <span className={`px-2 py-1 text-sm rounded border font-semibold ${getVipBadgeColor(level.level)}`}>
                    V{level.level} {level.name}
                  </span>
                  {level.monthly_fee > 0 && (
                    <span className="text-xs text-text-muted">¥{level.monthly_fee}/月</span>
                  )}
                </div>
                <div className="space-y-1 text-xs text-text-muted">
                  <div>消费要求: {level.required_points}点</div>
                  <div>经验加成: +{level.experience_bonus}%</div>
                  <div>点卷奖励: +{level.cash_bonus}%</div>
                  {level.max_online_time > 0 && (
                    <div>在线时长: {level.max_online_time}小时</div>
                  )}
                  {level.max_online_time === 0 && level.level > 0 && (
                    <div className="text-primary">在线时长: 无限制</div>
                  )}
                </div>
                <div className="mt-2">
                  <div className="text-xs text-text-muted mb-1">特权:</div>
                  <div className="flex flex-wrap gap-1">
                    {level.benefits.slice(0, 2).map((benefit, idx) => (
                      <span key={idx} className="px-1 py-0.5 text-xs bg-primary/10 text-primary rounded">
                        {benefit}
                      </span>
                    ))}
                    {level.benefits.length > 2 && (
                      <span className="text-xs text-text-muted">+{level.benefits.length - 2}项</span>
                    )}
                  </div>
                </div>
              </div>
            ))}
          </div>
        </GlowCard>

        {/* 玩家贵族信息 */}
        {playerVip && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            className="grid grid-cols-1 lg:grid-cols-2 gap-6"
          >
            {/* 当前状态 */}
            <GlowCard>
              <h3 className="text-lg font-semibold mb-4">当前贵族状态</h3>
              <div className="space-y-4">
                <div className="flex items-center justify-between">
                  <span className="text-text-muted">玩家昵称:</span>
                  <span className="font-medium">{playerVip.nickname}</span>
                </div>
                <div className="flex items-center justify-between">
                  <span className="text-text-muted">当前等级:</span>
                  <span className={`px-2 py-1 text-sm rounded border font-semibold ${getVipBadgeColor(playerVip.current_level)}`}>
                    V{playerVip.current_level} {getVipLevel(playerVip.current_level).name}
                  </span>
                </div>
                <div className="flex items-center justify-between">
                  <span className="text-text-muted">消费积分:</span>
                  <span className="font-mono text-primary">{playerVip.points.toLocaleString()}</span>
                </div>
                <div className="flex items-center justify-between">
                  <span className="text-text-muted">剩余时间:</span>
                  <span className={`font-medium ${
                    formatTimeRemaining(playerVip.expire_time) === '已过期' ? 'text-error' : 'text-success'
                  }`}>
                    {formatTimeRemaining(playerVip.expire_time)}
                  </span>
                </div>
                <div className="flex items-center justify-between">
                  <span className="text-text-muted">累计消费:</span>
                  <span className="font-mono text-primary">¥{playerVip.total_consumed.toFixed(2)}</span>
                </div>
              </div>

              {/* 当前权益 */}
              <div className="mt-6 pt-4 border-t border-border">
                <h4 className="font-medium mb-2">当前权益</h4>
                <div className="flex flex-wrap gap-1">
                  {getVipLevel(playerVip.current_level).benefits.map((benefit, idx) => (
                    <span key={idx} className="px-2 py-1 text-xs bg-primary/10 text-primary rounded">
                      {benefit}
                    </span>
                  ))}
                </div>
              </div>
            </GlowCard>

            {/* 管理操作 */}
            <GlowCard>
              <h3 className="text-lg font-semibold mb-4">贵族管理</h3>

              <Permit perm="NOBLE_EDIT">
                <div className="space-y-4">
                  <div>
                    <label className="block text-sm font-medium mb-1">设置贵族等级</label>
                    <select
                      value={selectedLevel}
                      onChange={(e) => setSelectedLevel(Number(e.target.value))}
                      className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                    >
                      {VIP_LEVELS.map(level => (
                        <option key={level.level} value={level.level}>
                          V{level.level} {level.name}
                        </option>
                      ))}
                    </select>
                  </div>

                  <div>
                    <label className="block text-sm font-medium mb-1">持续时间 (天)</label>
                    <input
                      type="number"
                      value={duration}
                      onChange={(e) => setDuration(Number(e.target.value))}
                      min="1"
                      max="3650"
                      className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                    />
                  </div>

                  <ShimmerButton
                    onClick={() => openDialog({
                      title: '确认设置贵族等级',
                      content: `确定要为玩家 ${playerVip.nickname} 设置 ${getVipLevel(selectedLevel).name} 等级，持续${duration}天吗？`,
                      requireKeyword: 'CONFIRM',
                      variant: 'primary',
                      onConfirm: handleSetVipLevel
                    })}
                    variant="primary"
                    className="w-full"
                  >
                    设置贵族等级
                  </ShimmerButton>

                  <div className="flex gap-2">
                    <ShimmerButton
                      onClick={() => handleExtendVip(30)}
                      variant="secondary"
                      className="flex-1"
                    >
                      续期30天
                    </ShimmerButton>
                    <ShimmerButton
                      onClick={() => handleExtendVip(90)}
                      variant="secondary"
                      className="flex-1"
                    >
                      续期90天
                    </ShimmerButton>
                  </div>

                  <ShimmerButton
                    onClick={() => openDialog({
                      title: '确认取消贵族',
                      content: `确定要取消玩家 ${playerVip.nickname} 的贵族等级吗？此操作不可撤销！`,
                      requireKeyword: 'CONFIRM',
                      variant: 'error',
                      onConfirm: handleCancelVip
                    })}
                    variant="error"
                    className="w-full"
                  >
                    取消贵族等级
                  </ShimmerButton>
                </div>
              </Permit>
            </GlowCard>
          </motion.div>
        )}

        {/* 贵族历史记录 */}
        {playerVip && playerVip.level_history.length > 0 && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.2 }}
          >
            <GlowCard>
              <h3 className="text-lg font-semibold mb-4">贵族历史记录</h3>
              <DataTable
                columns={historyColumns}
                data={playerVip.level_history}
                rowKey={(record, index) => `${record.level}-${index}`}
                pageSize={10}
              />
            </GlowCard>
          </motion.div>
        )}

        {!selectedPlayer && (
          <div className="text-center py-12">
            <div className="text-6xl mb-4">👑</div>
            <h3 className="text-lg font-medium text-text mb-2">请选择玩家</h3>
            <p className="text-text-muted">输入玩家 UIN 来查看贵族信息</p>
          </div>
        )}
      </div>

      <ConfirmDialog />
    </div>
  )
}