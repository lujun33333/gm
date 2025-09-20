import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { Grid, GridItem } from '../../components/layout/Grid'
import { PageContainer } from '../../components/layout/PageContainer'
import { GlowCard } from '../../components/fx/GlowCard'
import { GradientText } from '../../components/fx/GradientText'
import { ShimmerPrimaryButton, ShimmerSecondaryButton, ShimmerSuccessButton } from '../../components/fx/ShimmerButton'
import { Permit } from '../../components/common/Permit'
import { useConfirmDialog } from '../../components/common/ConfirmDialog'
import { Statistic } from '../../components/data/Statistic'
import { Gauge } from '../../components/data/Gauge'
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
    <PageContainer maxWidth="screen-xl" className="px-6 py-8">
      {/* 页面标题区域 */}
      <div className="mb-8">
        <div className="text-center lg:text-left">
          <GradientText className="text-4xl lg:text-5xl font-bold mb-3">
            仪表盘概览
          </GradientText>
          <p className="text-text-muted text-lg">实时监控系统运行状态与玩家数据统计</p>
        </div>
      </div>

      <Grid cols={12} gap={4} className="lg:gap-6">
        {/* 核心指标行 - 3个关键统计卡片 */}
        <GridItem span={{ base: 12, md: 4 }}>
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.18, delay: 0.1 }}
          >
            <GlowCard glowColor="primary" className="h-full p-6">
              <div className="flex items-center justify-between mb-4">
                <GradientText className="text-lg font-semibold" gradient={1}>
                  在线玩家
                </GradientText>
                <div className="w-3 h-3 bg-success rounded-full animate-pulse"></div>
              </div>
              <Statistic
                label="当前在线人数"
                value={dashboardData.online_total}
                className="mb-4"
              />
              <div className="space-y-2">
                <div className="flex justify-between text-xs text-text-muted">
                  <span>活跃度</span>
                  <span>{Math.round((dashboardData.online_total / 2500) * 100)}%</span>
                </div>
                <div className="h-2 bg-bg-secondary rounded-full overflow-hidden">
                  <motion.div
                    initial={{ width: 0 }}
                    animate={{ width: `${Math.round((dashboardData.online_total / 2500) * 100)}%` }}
                    transition={{ duration: 1, delay: 0.5 }}
                    className="h-full bg-gradient-1 rounded-full"
                  />
                </div>
              </div>
            </GlowCard>
          </motion.div>
        </GridItem>

        <GridItem span={{ base: 12, md: 4 }}>
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.18, delay: 0.2 }}
          >
            <GlowCard glowColor="accent" className="h-full p-6">
              <div className="flex items-center justify-between mb-4">
                <GradientText className="text-lg font-semibold" gradient={2}>
                  系统资源
                </GradientText>
                <div className="text-xs px-2 py-1 bg-success/20 text-success rounded-full">
                  健康
                </div>
              </div>
              <div className="space-y-4">
                <Gauge label="CPU 使用率" value={dashboardData.cpu} />
                <Gauge label="内存使用率" value={dashboardData.mem} />
              </div>
            </GlowCard>
          </motion.div>
        </GridItem>

        <GridItem span={{ base: 12, md: 4 }}>
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.18, delay: 0.3 }}
          >
            <GlowCard glowColor="success" className="h-full p-6">
              <div className="flex items-center justify-between mb-4">
                <GradientText className="text-lg font-semibold" gradient={3}>
                  服务状态
                </GradientText>
                <div className="text-xs px-2 py-1 bg-success/20 text-success rounded-full">
                  运行中
                </div>
              </div>
              <div className="space-y-4">
                <div className="text-center border-b border-border/50 pb-3">
                  <div className="text-2xl font-bold text-primary mb-1">{dashboardData.uptime}</div>
                  <div className="text-sm text-text-muted">稳定运行时间</div>
                </div>
                <div className="space-y-3">
                  <div className="flex justify-between items-center">
                    <span className="text-sm text-text-secondary flex items-center">
                      <div className="w-2 h-2 bg-success rounded-full mr-2"></div>
                      网络状态
                    </span>
                    <span className="text-sm font-semibold text-success">正常</span>
                  </div>
                  <div className="flex justify-between items-center">
                    <span className="text-sm text-text-secondary flex items-center">
                      <div className="w-2 h-2 bg-success rounded-full mr-2"></div>
                      数据库
                    </span>
                    <span className="text-sm font-semibold text-success">已连接</span>
                  </div>
                </div>
              </div>
            </GlowCard>
          </motion.div>
        </GridItem>

        {/* 网络流量统计 */}
        <GridItem span={{ base: 12, lg: 8 }}>
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.18, delay: 0.4 }}
          >
            <GlowCard className="h-full p-6">
              <div className="flex items-center justify-between mb-6">
                <GradientText className="text-2xl font-bold">
                  网络流量统计
                </GradientText>
                <div className="text-sm text-text-muted">
                  {new Date().toLocaleDateString('zh-CN', {
                    year: 'numeric',
                    month: 'long',
                    day: 'numeric'
                  })}
                </div>
              </div>

              <Grid cols={2} gap={4} className="md:grid-cols-3">
                <GridItem span={1}>
                  <Statistic
                    label="总入站流量"
                    value={`${(dashboardData.net_total_in / 1024 / 1024).toFixed(1)} MB`}
                    className="text-center p-4 bg-primary/5 rounded-lg border border-primary/20"
                  />
                </GridItem>
                <GridItem span={1}>
                  <Statistic
                    label="总出站流量"
                    value={`${(dashboardData.net_total_out / 1024 / 1024).toFixed(1)} MB`}
                    className="text-center p-4 bg-accent/5 rounded-lg border border-accent/20"
                  />
                </GridItem>
                <GridItem span={1}>
                  <Statistic
                    label="总流量"
                    value={`${((dashboardData.net_total_in + dashboardData.net_total_out) / 1024 / 1024).toFixed(1)} MB`}
                    className="text-center p-4 bg-success/5 rounded-lg border border-success/20"
                  />
                </GridItem>
              </Grid>
            </GlowCard>
          </motion.div>
        </GridItem>

        {/* 服务器控制区域 */}
        <GridItem span={{ base: 12, lg: 4 }}>
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.18, delay: 0.5 }}
          >
            <GlowCard className="h-full p-6">
              <div className="mb-6">
                <GradientText className="text-2xl font-bold mb-2">
                  服务器控制
                </GradientText>
                <p className="text-sm text-text-muted">危险操作需要确认</p>
              </div>

              <div className="space-y-4">
                <Permit perm="SERVER_CONTROL">
                  <button
                    onClick={handleDangerousAction}
                    className="w-full h-12 text-base bg-gradient-to-r from-error to-warning text-white rounded-lg hover:shadow-lg transition-all duration-300 font-medium"
                  >
                    <span className="mr-2">🔄</span>
                    重启服务器 (演示)
                  </button>
                </Permit>

                <Permit perm="MONITOR_VIEW">
                  <ShimmerSecondaryButton className="w-full h-12 text-base">
                    <span className="mr-2">📊</span>
                    查看详细监控
                  </ShimmerSecondaryButton>
                </Permit>

                <Permit perm="SERVER_CONTROL">
                  <ShimmerSuccessButton className="w-full h-12 text-base">
                    <span className="mr-2">💾</span>
                    备份系统数据
                  </ShimmerSuccessButton>
                </Permit>

                <div className="pt-4 border-t border-border/50">
                  <div className="text-xs text-text-muted text-center">
                    危险操作需要输入"CONFIRM"确认
                  </div>
                </div>
              </div>
            </GlowCard>
          </motion.div>
        </GridItem>

        {/* 玩家带宽排行 */}
        <GridItem span={{ base: 12 }}>
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.18, delay: 0.6 }}
          >
            <GlowCard className="p-6">
              <GradientText className="text-xl font-bold mb-4">
                玩家带宽使用排行 TOP 5
              </GradientText>

              <div className="space-y-3">
                {dashboardData.player_top_bandwidth.map((player, index) => (
                  <div key={player.uin} className="flex items-center justify-between p-3 bg-bg-secondary rounded-lg">
                    <div className="flex items-center space-x-3">
                      <div className={`w-8 h-8 rounded-full flex items-center justify-center text-sm font-bold ${
                        index === 0 ? 'bg-warning text-white' :
                        index === 1 ? 'bg-accent text-white' :
                        index === 2 ? 'bg-success text-white' :
                        'bg-bg-tertiary text-text-secondary'
                      }`}>
                        {index + 1}
                      </div>
                      <div>
                        <div className="font-medium text-text">{player.nickname}</div>
                        <div className="text-sm text-text-muted">UIN: {player.uin}</div>
                      </div>
                    </div>
                    <div className="text-right">
                      <div className="font-semibold text-primary">
                        {(player.bps / 1024).toFixed(1)} KB/s
                      </div>
                      <div className="text-xs text-text-muted">带宽使用</div>
                    </div>
                  </div>
                ))}
              </div>
            </GlowCard>
          </motion.div>
        </GridItem>
      </Grid>

      {/* ConfirmDialog 组件 */}
      <ConfirmDialog />
    </PageContainer>
  )
}