import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { PageContainer } from '../../components/layout/PageContainer'
import { GlowCard } from '../../components/fx/GlowCard'
import { GradientText } from '../../components/fx/GradientText'
import { ParticleBackground } from '../../components/fx/ParticleBackground'
import { ShimmerSecondaryButton, ShimmerSuccessButton } from '../../components/fx/ShimmerButton'
import { Permit } from '../../components/common/Permit'
import { useConfirmDialog } from '../../components/common/ConfirmDialog'
import { MetricGrid } from '../../components/data/MetricCard'
import { MiniGauge } from '../../components/data/Gauge'
import { ChartArea } from '../../components/data/ChartArea'
import { ChartLine } from '../../components/data/ChartLine'
import { useToast } from '../../hooks/useToast'
import { usePerm } from '../../hooks/usePerm'
import { api } from '../../utils/api'
import type { DashboardData } from '../../utils/mock'

export function Dashboard() {
  const [dashboardData, setDashboardData] = useState<DashboardData | null>(null)
  const [loading, setLoading] = useState(true)
  const { openDialog, ConfirmDialog } = useConfirmDialog()
  const { toast } = useToast()
  const { userInfo, userPermissions, isOwner, isSub } = usePerm()

  useEffect(() => {
    loadDashboardData()
  }, [])

  // 生成24小时趋势数据
  const generate24HourData = (baseValue: number, variance: number) => {
    const data = []
    const now = new Date()

    for (let i = 23; i >= 0; i--) {
      const time = new Date(now.getTime() - i * 60 * 60 * 1000)
      const randomVariance = (Math.random() - 0.5) * variance
      const value = Math.max(0, baseValue + randomVariance)

      data.push({
        name: `${time.getHours().toString().padStart(2, '0')}:00`,
        value: Math.round(value)
      })
    }

    return data
  }

  // 生成服务器资源多线图数据
  const getServerResourceData = () => {
    if (!dashboardData) return []

    const data = []
    const now = new Date()

    for (let i = 11; i >= 0; i--) {
      const time = new Date(now.getTime() - i * 2 * 60 * 60 * 1000) // 每2小时一个点
      data.push({
        name: `${time.getHours().toString().padStart(2, '0')}:00`,
        cpu: Math.max(20, dashboardData.cpu + (Math.random() - 0.5) * 20),
        memory: Math.max(30, dashboardData.mem + (Math.random() - 0.5) * 15),
        network: Math.max(10, 50 + (Math.random() - 0.5) * 30)
      })
    }

    return data
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

  // 重启服务器操作
  const handleRestartServer = async () => {
    try {
      await new Promise(resolve => setTimeout(resolve, 2000))
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

  // 危险操作确认
  const handleDangerousAction = () => {
    openDialog({
      title: '警告：危险操作',
      content: (
        <div className="space-y-3">
          <p className="text-text-secondary">
            您即将执行服务器重启操作，这可能会影响在线玩家的游戏体验。
          </p>
          <div className="p-3 bg-error/10 border border-error/20 rounded-lg">
            <p className="text-sm text-error font-medium">
              ⚠️ 此操作不可逆转！请确认您了解风险。
            </p>
          </div>
        </div>
      ),
      requireKeyword: 'CONFIRM',
      confirmText: '确认重启',
      variant: 'error',
      onConfirm: handleRestartServer
    })
  }

  // Loading 状态
  if (loading) {
    return (
      <PageContainer maxWidth="screen-xl" className="px-4 sm:px-6 lg:px-8 py-6 lg:py-8">
        <div className="space-y-6">
          <div className="h-12 bg-bg-secondary rounded-lg animate-pulse" />
          <div className="grid grid-cols-1 sm:grid-cols-2 gap-4">
            {Array.from({ length: 4 }).map((_, i) => (
              <div key={i} className="h-32 bg-bg-secondary rounded-lg animate-pulse" />
            ))}
          </div>
          <div className="h-96 bg-bg-secondary rounded-lg animate-pulse" />
        </div>
      </PageContainer>
    )
  }

  // Error 状态
  if (!dashboardData) {
    return (
      <PageContainer maxWidth="screen-xl" className="px-4 sm:px-6 lg:px-8 py-6 lg:py-8">
        <div className="text-center py-12">
          <div className="text-text-muted text-lg">数据加载失败</div>
          <button
            onClick={loadDashboardData}
            className="mt-4 px-4 py-2 bg-primary text-white rounded-lg hover:bg-primary/90 transition-colors"
          >
            重新加载
          </button>
        </div>
      </PageContainer>
    )
  }

  // 准备统计卡片数据
  const metricsData = [
    {
      key: 'online-players',
      icon: '👥',
      title: '在线玩家',
      value: dashboardData.online_total,
      unit: '人',
      trend: {
        value: 8.5,
        isPositive: true,
        label: '24小时内平均增长'
      },
      color: 'primary' as const
    },
    {
      key: 'cpu-usage',
      icon: '🖥️',
      title: 'CPU使用率',
      value: dashboardData.cpu,
      unit: '%',
      color: (dashboardData.cpu > 80 ? 'error' : dashboardData.cpu > 60 ? 'warning' : 'success') as 'error' | 'warning' | 'success',
      children: (
        <div className="mt-2">
          <MiniGauge
            value={dashboardData.cpu}
            color={dashboardData.cpu > 80 ? 'rgb(var(--error))' : dashboardData.cpu > 60 ? 'rgb(var(--warning))' : 'rgb(var(--success))'}
          />
        </div>
      )
    },
    {
      key: 'memory-usage',
      icon: '💾',
      title: '内存使用',
      value: dashboardData.mem,
      unit: '%',
      color: (dashboardData.mem > 80 ? 'error' : dashboardData.mem > 60 ? 'warning' : 'success') as 'error' | 'warning' | 'success',
      children: (
        <div className="mt-2">
          <MiniGauge
            value={dashboardData.mem}
            color={dashboardData.mem > 80 ? 'rgb(var(--error))' : dashboardData.mem > 60 ? 'rgb(var(--warning))' : 'rgb(var(--success))'}
          />
        </div>
      )
    },
    {
      key: 'network-traffic',
      icon: '🌐',
      title: '网络流量',
      value: ((dashboardData.net_total_in + dashboardData.net_total_out) / 1024 / 1024).toFixed(1),
      unit: 'MB',
      trend: {
        value: 12.3,
        isPositive: true,
        label: '今日总流量'
      },
      color: 'success' as const
    }
  ]

  return (
    <div className="relative min-h-screen">
      {/* 粒子背景 - 仅在Dashboard显示 */}
      <ParticleBackground />

      <PageContainer maxWidth="screen-xl" className="relative z-10 px-4 sm:px-6 lg:px-8 py-6 lg:py-8">
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
          {/* 第一行：关键指标卡片 - 2×2 统计区 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.1 }}
          >
            <MetricGrid
              metrics={metricsData}
              columns={2}
              className="mb-8"
            />
          </motion.div>

          {/* 第二行：图表区域 */}
          <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
            {/* 24小时在线玩家趋势 */}
            <motion.div
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ duration: 0.4, delay: 0.2 }}
              className="lg:col-span-2"
            >
              <GlowCard className="h-full p-6">
                <ChartArea
                  title="📈 24小时在线玩家趋势"
                  data={generate24HourData(dashboardData.online_total, 300)}
                  color="rgb(var(--primary))"
                  height={320}
                  unit="人"
                  valueFormatter={(value) => value.toLocaleString('zh-CN')}
                  nameFormatter={(name) => name}
                />
              </GlowCard>
            </motion.div>

            {/* 快速操作面板 */}
            <motion.div
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ duration: 0.4, delay: 0.3 }}
            >
              <GlowCard className="h-full p-6">
                <div className="mb-6">
                  <GradientText className="text-xl font-bold mb-2">
                    🎛️ 快速操作
                  </GradientText>
                  <div className="flex items-center gap-2 mb-4">
                    <div className="w-3 h-3 bg-success rounded-full animate-pulse"></div>
                    <span className="text-sm text-text-muted">服务器运行正常</span>
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

                  <div className="grid grid-cols-1 gap-3 text-sm">
                    <div className="flex items-center justify-between p-2 bg-success/5 rounded">
                      <span className="text-text-secondary">数据库</span>
                      <span className="text-success font-medium">已连接</span>
                    </div>
                    <div className="flex items-center justify-between p-2 bg-success/5 rounded">
                      <span className="text-text-secondary">API服务</span>
                      <span className="text-success font-medium">正常</span>
                    </div>
                  </div>
                </div>

                {/* 操作按钮组 */}
                <div className="space-y-3">
                  <Permit perm="SERVER_CONTROL">
                    <button
                      onClick={handleDangerousAction}
                      className="w-full h-10 text-sm bg-gradient-to-r from-error to-warning text-white rounded-lg hover:shadow-lg transition-all duration-300 font-medium hover:scale-105 active:scale-95"
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
                      💾 数据备份
                    </ShimmerSuccessButton>
                  </Permit>
                </div>
              </GlowCard>
            </motion.div>
          </div>

          {/* 第三行：系统资源监控 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.4 }}
          >
            <GlowCard className="p-6">
              <ChartLine
                title="🔍 系统资源监控（12小时）"
                data={getServerResourceData()}
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
                height={280}
                showLegend={true}
                nameFormatter={(name) => name}
              />
            </GlowCard>
          </motion.div>

          {/* 第四行：玩家排行榜 */}
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.5 }}
          >
            <GlowCard className="p-6">
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

          {/* 权限演示区域 - 仅管理员可见 */}
          <Permit perm={['*', 'PERM_ADMIN']}>
            <motion.div
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ duration: 0.4, delay: 0.6 }}
            >
              <GlowCard glowColor="warning" className="p-6">
                <div className="flex items-center justify-between mb-6">
                  <GradientText className="text-xl font-bold">
                    🔐 权限管理演示
                  </GradientText>
                  <div className="text-sm text-text-muted">
                    当前权限: {userPermissions.join(', ')}
                  </div>
                </div>

                <div className="grid grid-cols-1 lg:grid-cols-3 gap-4">
                  {/* 用户信息 */}
                  <div className="p-4 bg-bg-secondary/50 rounded-lg">
                    <h4 className="font-semibold text-text mb-3">用户信息</h4>
                    <div className="space-y-2 text-sm">
                      <div>用户名: {userInfo?.username}</div>
                      <div>角色: {userInfo?.role}</div>
                      <div className={`px-2 py-1 rounded text-xs ${
                        isOwner ? 'bg-error/20 text-error' :
                        isSub ? 'bg-warning/20 text-warning' :
                        'bg-success/20 text-success'
                      }`}>
                        {isOwner ? '超级管理员' : isSub ? '子管理员' : '普通用户'}
                      </div>
                    </div>
                  </div>

                  {/* 权限测试按钮 */}
                  <div className="p-4 bg-bg-secondary/50 rounded-lg">
                    <h4 className="font-semibold text-text mb-3">权限测试</h4>
                    <div className="space-y-2">
                      <Permit perm="SERVER_CONTROL">
                        <button className="w-full text-sm bg-error/20 text-error p-2 rounded hover:bg-error/30 transition-colors">
                          服务器控制权限 ✓
                        </button>
                      </Permit>

                      <Permit perm="PLAYER_BAN">
                        <button className="w-full text-sm bg-warning/20 text-warning p-2 rounded hover:bg-warning/30 transition-colors">
                          封禁玩家权限 ✓
                        </button>
                      </Permit>

                      <Permit perm="ITEM_ADD">
                        <button className="w-full text-sm bg-success/20 text-success p-2 rounded hover:bg-success/30 transition-colors">
                          添加道具权限 ✓
                        </button>
                      </Permit>
                    </div>
                  </div>

                  {/* 危险操作演示 */}
                  <div className="p-4 bg-bg-secondary/50 rounded-lg">
                    <h4 className="font-semibold text-text mb-3">危险操作</h4>
                    <div className="space-y-2">
                      <Permit perm="*">
                        <button
                          onClick={() => openDialog({
                            title: '超级危险操作',
                            content: (
                              <div>
                                <p className="text-error mb-3">
                                  此操作将清空所有数据！
                                </p>
                                <div className="p-3 bg-error/10 border border-error/20 rounded">
                                  <p className="text-sm text-error">
                                    ⚠️ 需要输入 CONFIRM 确认
                                  </p>
                                </div>
                              </div>
                            ),
                            requireKeyword: 'CONFIRM',
                            variant: 'error',
                            onConfirm: async () => {
                              toast({
                                title: '操作已确认',
                                description: '演示：危险操作已执行'
                              })
                            }
                          })}
                          className="w-full text-sm bg-error text-white p-2 rounded hover:bg-error/90 transition-colors"
                        >
                          清空数据库 (需CONFIRM)
                        </button>
                      </Permit>

                      <button className="w-full text-sm bg-bg-tertiary text-text-muted p-2 rounded cursor-not-allowed">
                        无权限操作 (隐藏)
                      </button>
                    </div>
                  </div>
                </div>
              </GlowCard>
            </motion.div>
          </Permit>
        </div>

        {/* ConfirmDialog 组件 */}
        <ConfirmDialog />
      </PageContainer>
    </div>
  )
}