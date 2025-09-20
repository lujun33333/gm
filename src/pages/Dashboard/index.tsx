import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { Grid, GridItem } from '../../components/layout/Grid'
import { PageContainer } from '../../components/layout/PageContainer'
import { GlowCard } from '../../components/fx/GlowCard'
import { GradientText } from '../../components/fx/GradientText'
import { ShimmerPrimaryButton, ShimmerSecondaryButton, ShimmerSuccessButton } from '../../components/fx/ShimmerButton'
import { Permit } from '../../components/common/Permit'
import { useConfirmDialog } from '../../components/common/ConfirmDialog'
import { Statistic, StatisticCard } from '../../components/data/Statistic'
import { Gauge, MiniGauge } from '../../components/data/Gauge'
import { TrendChart, MultiTrendChart, MiniTrendChart } from '../../components/charts/TrendChart'
import { useToast } from '../../hooks/useToast'
import { api } from '../../utils/api'
import type { DashboardData } from '../../utils/mock'

export function Dashboard() {
  const [dashboardData, setDashboardData] = useState<DashboardData | null>(null)
  const [loading, setLoading] = useState(true)
  const { openDialog, ConfirmDialog } = useConfirmDialog()
  const { toast } = useToast()

  useEffect(() => {
    loadDashboardData()
  }, [])

  // 生成模拟趋势数据
  const generateTrendData = (hours: number, baseValue: number, variance: number) => {
    const data = []
    const now = new Date()

    for (let i = hours; i >= 0; i--) {
      const time = new Date(now.getTime() - i * 60 * 60 * 1000)
      const randomVariance = (Math.random() - 0.5) * variance
      const value = Math.max(0, baseValue + randomVariance)

      data.push({
        time: time.getHours().toString().padStart(2, '0') + ':00',
        value: Math.round(value)
      })
    }

    return data
  }

  // 生成服务器资源趋势数据
  const getServerTrendData = () => {
    if (!dashboardData) return []

    return [
      {
        time: '12小时前',
        cpu: dashboardData.cpu * 0.8,
        memory: dashboardData.mem * 0.9,
        network: 45
      },
      {
        time: '6小时前',
        cpu: dashboardData.cpu * 0.9,
        memory: dashboardData.mem * 0.95,
        network: 60
      },
      {
        time: '现在',
        cpu: dashboardData.cpu,
        memory: dashboardData.mem,
        network: 78
      }
    ]
  }

  const loadDashboardData = async () => {
    try {
      setLoading(true)
      const response = await api.get<DashboardData>('/dashboard')
      if (response.code === 0) {
        setDashboardData(response.data!)
      } else {
        toast({
          title: '数据加载失败',
          description: response.msg || '未知错误',
          variant: 'destructive'
        })
      }
    } catch (error) {
      console.error('加载仪表盘数据失败:', error)
      toast({
        title: '网络错误',
        description: '无法连接到服务器',
        variant: 'destructive'
      })
    } finally {
      setLoading(false)
    }
  }

  // 演示重启服务器操作
  const handleRestartServer = async () => {
    try {
      await new Promise(resolve => setTimeout(resolve, 2000)) // 模拟操作延时
      toast({
        title: '服务器重启成功',
        description: '服务器已安全重启，所有服务正常运行',
        variant: 'default'
      })
    } catch (error) {
      toast({
        title: '重启失败',
        description: '服务器重启过程中出现错误',
        variant: 'destructive'
      })
    }
  }

  // 演示危险操作
  const handleDangerousAction = () => {
    openDialog({
      title: '警告：危险操作',
      content: (
        <div className="space-y-3">
          <p className="text-text-secondary">
            您即将执行一个危险操作，这可能会影响系统稳定性。
          </p>
          <div className="p-3 bg-error/10 border border-error/20 rounded-lg">
            <p className="text-sm text-error font-medium">
              ⚠️ 此操作不可逆转！请确认您了解风险。
            </p>
          </div>
        </div>
      ),
      requireKeyword: 'CONFIRM',
      confirmText: '确认执行',
      variant: 'error',
      onConfirm: handleRestartServer
    })
  }

  if (loading) {
    return (
      <PageContainer maxWidth="screen-xl" className="px-6 py-8">
        <div className="space-y-6">
          {Array.from({ length: 3 }).map((_, i) => (
            <div key={i} className="animate-pulse">
              <div className="h-32 bg-bg-secondary rounded-lg"></div>
            </div>
          ))}
        </div>
      </PageContainer>
    )
  }

  if (!dashboardData) {
    return (
      <PageContainer maxWidth="screen-xl" className="px-6 py-8">
        <div className="text-center py-12">
          <div className="text-text-muted text-lg">数据加载失败</div>
        </div>
      </PageContainer>
    )
  }

  return (
    <PageContainer maxWidth="screen-2xl" className="px-4 sm:px-6 lg:px-8 py-6 lg:py-8">
      {/* 页面标题区域 */}
      <motion.div
        initial={{ opacity: 0, y: -20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: 0.4 }}
        className="mb-8"
      >
        <div className="text-center lg:text-left">
          <GradientText className="text-3xl sm:text-4xl lg:text-5xl font-bold mb-3">
            仪表盘概览
          </GradientText>
          <p className="text-text-muted text-base lg:text-lg">
            实时监控系统运行状态与玩家数据统计
          </p>
        </div>
      </motion.div>

      <div className="space-y-6 lg:space-y-8">
        {/* 第一行：关键指标卡片 - 4列响应式布局 */}
        <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4 lg:gap-6">
          {/* 在线玩家 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.1 }}
          >
            <StatisticCard
              icon="👥"
              label="在线玩家"
              value={dashboardData.online_total}
              suffix="人"
              trend={{
                value: 8.5,
                isPositive: true
              }}
              description="24小时内平均在线人数增长"
              color="primary"
              size="lg"
              className="h-full"
            />
          </motion.div>

          {/* CPU使用率 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.2 }}
          >
            <GlowCard
              glowColor={dashboardData.cpu > 80 ? "error" : dashboardData.cpu > 60 ? "warning" : "success"}
              className="h-full p-4 lg:p-6"
            >
              <div className="flex items-center justify-between mb-4">
                <div className="flex items-center gap-2">
                  <span className="text-2xl">🖥️</span>
                  <span className="font-semibold text-text">CPU</span>
                </div>
                <MiniGauge
                  value={dashboardData.cpu}
                  color={dashboardData.cpu > 80 ? "rgb(var(--error))" : dashboardData.cpu > 60 ? "rgb(var(--warning))" : "rgb(var(--success))"}
                />
              </div>
              <div className="text-center">
                <div className="text-2xl lg:text-3xl font-bold text-text mb-1">
                  {dashboardData.cpu}%
                </div>
                <div className="text-sm text-text-muted">当前使用率</div>
                <div className="mt-3">
                  <MiniTrendChart
                    data={generateTrendData(6, dashboardData.cpu, 10)}
                    color={dashboardData.cpu > 80 ? "rgb(var(--error))" : dashboardData.cpu > 60 ? "rgb(var(--warning))" : "rgb(var(--success))"}
                    height={40}
                  />
                </div>
              </div>
            </GlowCard>
          </motion.div>

          {/* 内存使用率 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.3 }}
          >
            <GlowCard
              glowColor={dashboardData.mem > 80 ? "error" : dashboardData.mem > 60 ? "warning" : "success"}
              className="h-full p-4 lg:p-6"
            >
              <div className="flex items-center justify-between mb-4">
                <div className="flex items-center gap-2">
                  <span className="text-2xl">💾</span>
                  <span className="font-semibold text-text">内存</span>
                </div>
                <MiniGauge
                  value={dashboardData.mem}
                  color={dashboardData.mem > 80 ? "rgb(var(--error))" : dashboardData.mem > 60 ? "rgb(var(--warning))" : "rgb(var(--success))"}
                />
              </div>
              <div className="text-center">
                <div className="text-2xl lg:text-3xl font-bold text-text mb-1">
                  {dashboardData.mem}%
                </div>
                <div className="text-sm text-text-muted">当前使用率</div>
                <div className="mt-3">
                  <MiniTrendChart
                    data={generateTrendData(6, dashboardData.mem, 8)}
                    color={dashboardData.mem > 80 ? "rgb(var(--error))" : dashboardData.mem > 60 ? "rgb(var(--warning))" : "rgb(var(--success))"}
                    height={40}
                  />
                </div>
              </div>
            </GlowCard>
          </motion.div>

          {/* 网络状态 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.4 }}
          >
            <StatisticCard
              icon="🌐"
              label="网络流量"
              value={`${((dashboardData.net_total_in + dashboardData.net_total_out) / 1024 / 1024).toFixed(1)}`}
              suffix="MB"
              trend={{
                value: 12.3,
                isPositive: true
              }}
              description="今日总流量使用量"
              color="success"
              size="lg"
              className="h-full"
            />
          </motion.div>
        </div>

        {/* 第二行：图表区域 */}
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          {/* 服务器资源趋势 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.5 }}
            className="lg:col-span-2"
          >
            <GlowCard className="h-full p-6">
              <div className="flex items-center justify-between mb-6">
                <GradientText className="text-xl font-bold">
                  🔍 系统资源监控
                </GradientText>
                <div className="text-sm text-text-muted">
                  实时状态
                </div>
              </div>

              <MultiTrendChart
                data={getServerTrendData()}
                metrics={[
                  {
                    key: 'cpu',
                    name: 'CPU',
                    color: 'rgb(var(--primary))',
                    formatter: (value) => `${value.toFixed(1)}%`
                  },
                  {
                    key: 'memory',
                    name: '内存',
                    color: 'rgb(var(--accent))',
                    formatter: (value) => `${value.toFixed(1)}%`
                  },
                  {
                    key: 'network',
                    name: '网络',
                    color: 'rgb(var(--success))',
                    formatter: (value) => `${value.toFixed(1)}MB/s`
                  }
                ]}
                height={300}
              />
            </GlowCard>
          </motion.div>

          {/* 服务器状态和控制 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.6 }}
          >
            <GlowCard className="h-full p-6">
              <div className="mb-6">
                <GradientText className="text-xl font-bold mb-2">
                  🎛️ 服务器控制
                </GradientText>
                <div className="flex items-center gap-2 mb-4">
                  <div className="w-3 h-3 bg-success rounded-full animate-pulse"></div>
                  <span className="text-sm text-text-muted">运行状态正常</span>
                </div>
              </div>

              {/* 服务器信息 */}
              <div className="space-y-4 mb-6">
                <div className="text-center p-4 bg-bg-secondary/50 rounded-lg">
                  <div className="text-2xl font-bold text-primary mb-1">
                    {dashboardData.uptime}
                  </div>
                  <div className="text-sm text-text-muted">运行时间</div>
                </div>

                <div className="grid grid-cols-2 gap-3 text-sm">
                  <div className="flex items-center justify-between p-2 bg-success/5 rounded">
                    <span className="text-text-secondary">数据库</span>
                    <span className="text-success font-medium">已连接</span>
                  </div>
                  <div className="flex items-center justify-between p-2 bg-success/5 rounded">
                    <span className="text-text-secondary">API</span>
                    <span className="text-success font-medium">正常</span>
                  </div>
                </div>
              </div>

              {/* 控制按钮 */}
              <div className="space-y-3">
                <Permit perm="SERVER_CONTROL">
                  <button
                    onClick={handleDangerousAction}
                    className="w-full h-10 text-sm bg-gradient-to-r from-error to-warning text-white rounded-lg hover:shadow-lg transition-all duration-300 font-medium"
                  >
                    🔄 重启服务器
                  </button>
                </Permit>

                <Permit perm="MONITOR_VIEW">
                  <ShimmerSecondaryButton className="w-full h-10 text-sm">
                    📊 详细监控
                  </ShimmerSecondaryButton>
                </Permit>

                <Permit perm="SERVER_CONTROL">
                  <ShimmerSuccessButton className="w-full h-10 text-sm">
                    💾 备份数据
                  </ShimmerSuccessButton>
                </Permit>
              </div>
            </GlowCard>
          </motion.div>
        </div>

        {/* 第三行：玩家统计和活动 */}
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          {/* 在线玩家趋势 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.7 }}
          >
            <GlowCard className="h-full p-6">
              <div className="mb-6">
                <GradientText className="text-xl font-bold mb-2">
                  📈 在线玩家趋势
                </GradientText>
                <p className="text-sm text-text-muted">24小时内玩家活跃度变化</p>
              </div>

              <TrendChart
                data={generateTrendData(12, dashboardData.online_total, 300)}
                color="rgb(var(--primary))"
                height={240}
                valueFormatter={(value) => `${value}人`}
                timeFormatter={(time) => time}
              />
            </GlowCard>
          </motion.div>

          {/* 玩家带宽排行 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.8 }}
          >
            <GlowCard className="h-full p-6">
              <GradientText className="text-xl font-bold mb-6">
                🏆 带宽使用排行 TOP 5
              </GradientText>

              <div className="space-y-3">
                {dashboardData.player_top_bandwidth.map((player, index) => (
                  <motion.div
                    key={player.uin}
                    initial={{ opacity: 0, x: -20 }}
                    animate={{ opacity: 1, x: 0 }}
                    transition={{ duration: 0.3, delay: 0.1 * index }}
                    className="flex items-center justify-between p-3 bg-bg-secondary/50 rounded-lg hover:bg-bg-secondary transition-colors"
                  >
                    <div className="flex items-center space-x-3">
                      <div className={`w-8 h-8 rounded-full flex items-center justify-center text-sm font-bold ${
                        index === 0 ? 'bg-gradient-to-r from-yellow-400 to-yellow-600 text-white' :
                        index === 1 ? 'bg-gradient-to-r from-gray-300 to-gray-500 text-white' :
                        index === 2 ? 'bg-gradient-to-r from-amber-600 to-amber-800 text-white' :
                        'bg-bg-tertiary text-text-secondary border border-border'
                      }`}>
                        {index + 1}
                      </div>
                      <div>
                        <div className="font-medium text-text">{player.nickname}</div>
                        <div className="text-xs text-text-muted">UIN: {player.uin}</div>
                      </div>
                    </div>
                    <div className="text-right">
                      <div className="font-bold text-primary">
                        {(player.bps / 1024).toFixed(1)} KB/s
                      </div>
                      <div className="text-xs text-text-muted">带宽使用</div>
                    </div>
                  </motion.div>
                ))}
              </div>
            </GlowCard>
          </motion.div>
        </div>

        {/* 第四行：网络流量详细统计 */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.4, delay: 0.9 }}
        >
          <GlowCard className="p-6">
            <div className="flex flex-col sm:flex-row sm:items-center sm:justify-between mb-6">
              <div>
                <GradientText className="text-xl font-bold mb-2">
                  📊 网络流量统计
                </GradientText>
                <p className="text-sm text-text-muted">实时网络流量监控和统计</p>
              </div>
              <div className="text-sm text-text-muted mt-4 sm:mt-0">
                {new Date().toLocaleDateString('zh-CN', {
                  year: 'numeric',
                  month: 'long',
                  day: 'numeric'
                })}
              </div>
            </div>

            <div className="grid grid-cols-1 sm:grid-cols-3 gap-4 lg:gap-6">
              <div className="text-center p-4 lg:p-6 bg-gradient-to-br from-primary/5 to-primary/10 rounded-lg border border-primary/20">
                <div className="text-xs text-primary font-medium mb-2">入站流量</div>
                <div className="text-2xl lg:text-3xl font-bold text-primary">
                  {(dashboardData.net_total_in / 1024 / 1024).toFixed(1)}
                </div>
                <div className="text-xs text-text-muted mt-1">MB</div>
              </div>

              <div className="text-center p-4 lg:p-6 bg-gradient-to-br from-accent/5 to-accent/10 rounded-lg border border-accent/20">
                <div className="text-xs text-accent font-medium mb-2">出站流量</div>
                <div className="text-2xl lg:text-3xl font-bold text-accent">
                  {(dashboardData.net_total_out / 1024 / 1024).toFixed(1)}
                </div>
                <div className="text-xs text-text-muted mt-1">MB</div>
              </div>

              <div className="text-center p-4 lg:p-6 bg-gradient-to-br from-success/5 to-success/10 rounded-lg border border-success/20">
                <div className="text-xs text-success font-medium mb-2">总流量</div>
                <div className="text-2xl lg:text-3xl font-bold text-success">
                  {((dashboardData.net_total_in + dashboardData.net_total_out) / 1024 / 1024).toFixed(1)}
                </div>
                <div className="text-xs text-text-muted mt-1">MB</div>
              </div>
            </div>
          </GlowCard>
        </motion.div>
      </div>

      {/* ConfirmDialog 组件 */}
      <ConfirmDialog />
    </PageContainer>
  )
}