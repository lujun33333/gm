import { useState, useEffect } from 'react'
import { useParams } from 'react-router-dom'
import { motion } from 'framer-motion'
import { GlowCard } from '../../../components/fx/GlowCard'
import { GradientText } from '../../../components/fx/GradientText'
import { ShimmerButton } from '../../../components/fx/ShimmerButton'
import { Permit } from '../../../components/common/Permit'
import { useConfirmDialog } from '../../../components/common/ConfirmDialog'
import { api } from '../../../utils/api'
import type { Player } from '../../../utils/mock'

export function PlayerProfile() {
  const { id } = useParams<{ id: string }>()
  const [player, setPlayer] = useState<Player | null>(null)
  const [loading, setLoading] = useState(true)
  const [saving, setSaving] = useState(false)
  const { openDialog, ConfirmDialog } = useConfirmDialog()

  useEffect(() => {
    loadPlayer()
  }, [id])

  const loadPlayer = async () => {
    try {
      setLoading(true)
      const response = await api.get(`/player/${id}`)
      if (response.code === 0) {
        setPlayer(response.data)
      }
    } catch (error) {
      console.error('加载玩家数据失败:', error)
    } finally {
      setLoading(false)
    }
  }

  const handleSave = async (formData: FormData) => {
    try {
      setSaving(true)
      const data = Object.fromEntries(formData)
      const response = await api.post('/player/profile', { ...data, uin: id })
      if (response.code === 0) {
        console.log('保存成功')
        loadPlayer() // 重新加载数据
      }
    } catch (error) {
      console.error('保存失败:', error)
    } finally {
      setSaving(false)
    }
  }

  const handleBan = async () => {
    try {
      const response = await api.post('/player/ban', { uin: id })
      if (response.code === 0) {
        console.log('封禁成功')
        loadPlayer()
      }
    } catch (error) {
      console.error('封禁失败:', error)
    }
  }

  const handleUnban = async () => {
    try {
      const response = await api.post('/player/unban', { uin: id })
      if (response.code === 0) {
        console.log('解封成功')
        loadPlayer()
      }
    } catch (error) {
      console.error('解封失败:', error)
    }
  }

  const handleResetSecPwd = async () => {
    try {
      const response = await api.post('/player/reset-sec-pwd', { uin: id })
      if (response.code === 0) {
        console.log('重置密码成功')
        loadPlayer()
      }
    } catch (error) {
      console.error('重置密码失败:', error)
    }
  }

  if (loading) {
    return <ProfileSkeleton />
  }

  if (!player) {
    return (
      <div className="text-center py-12">
        <div className="text-6xl mb-4">😕</div>
        <h3 className="text-lg font-medium text-text mb-2">玩家不存在</h3>
        <p className="text-text-muted">找不到 UIN 为 {id} 的玩家</p>
      </div>
    )
  }

  return (
    <div className="max-w-4xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          玩家资料
        </GradientText>
        <p className="text-text-muted mt-2">查看和编辑玩家基础信息</p>
      </div>

      <GlowCard>
        <form onSubmit={(e) => {
          e.preventDefault()
          handleSave(new FormData(e.currentTarget))
        }}>
          <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
            {/* 基础信息 */}
            <div className="space-y-4">
              <h3 className="text-lg font-semibold text-text border-b border-border pb-2">
                基础信息
              </h3>

              <div>
                <label className="block text-sm font-medium mb-1">UIN</label>
                <input
                  type="text"
                  value={player.uin}
                  readOnly
                  className="w-full px-3 py-2 bg-bg-tertiary border border-border rounded-lg text-text-muted"
                />
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">昵称</label>
                <input
                  name="nickname"
                  type="text"
                  defaultValue={player.nickname}
                  required
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                />
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">性别</label>
                <select
                  name="gender"
                  defaultValue={player.gender}
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                >
                  <option value="男">男</option>
                  <option value="女">女</option>
                  <option value="未知">未知</option>
                </select>
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">国家</label>
                <input
                  name="country"
                  type="text"
                  defaultValue={player.country}
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                />
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">注册时间</label>
                <input
                  type="text"
                  value={player.created_at}
                  readOnly
                  className="w-full px-3 py-2 bg-bg-tertiary border border-border rounded-lg text-text-muted"
                />
              </div>
            </div>

            {/* 游戏数据 */}
            <div className="space-y-4">
              <h3 className="text-lg font-semibold text-text border-b border-border pb-2">
                游戏数据
              </h3>

              <div className="grid grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium mb-1">经验值</label>
                  <input
                    name="exp"
                    type="number"
                    defaultValue={player.exp}
                    min="0"
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium mb-1">点卷</label>
                  <input
                    name="cash_a"
                    type="number"
                    defaultValue={player.cash_a}
                    min="0"
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium mb-1">消费卷</label>
                  <input
                    name="cash_b"
                    type="number"
                    defaultValue={player.cash_b}
                    min="0"
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium mb-1">荣誉点</label>
                  <input
                    name="honor"
                    type="number"
                    defaultValue={player.honor}
                    min="0"
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium mb-1">魅力值</label>
                  <input
                    name="charm"
                    type="number"
                    defaultValue={player.charm}
                    min="0"
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  />
                </div>

                <div>
                  <label className="block text-sm font-medium mb-1">舞力值</label>
                  <input
                    name="dance"
                    type="number"
                    defaultValue={player.dance}
                    min="0"
                    className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                  />
                </div>
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">个性签名</label>
                <textarea
                  name="sign"
                  defaultValue={player.sign}
                  maxLength={60}
                  rows={3}
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                />
              </div>
            </div>
          </div>

          {/* 状态信息 */}
          <div className="mt-6 pt-6 border-t border-border">
            <h3 className="text-lg font-semibold text-text mb-4">账号状态</h3>

            <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
              <div>
                <label className="block text-sm font-medium mb-1">二级密码状态</label>
                <select
                  name="sec_pwd_status"
                  defaultValue={player.sec_pwd_status}
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                >
                  <option value="启用">启用</option>
                  <option value="禁用">禁用</option>
                </select>
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">巅峰赛状态</label>
                <select
                  name="peak_status"
                  defaultValue={player.peak_status}
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
                >
                  <option value="参与">参与</option>
                  <option value="禁赛">禁赛</option>
                </select>
              </div>

              <div>
                <label className="block text-sm font-medium mb-1">账号状态</label>
                <div className={`px-3 py-2 rounded-lg text-center font-medium ${
                  player.status === 0 ? 'bg-success/20 text-success' : 'bg-error/20 text-error'
                }`}>
                  {player.status === 0 ? '正常' : '已封禁'}
                </div>
              </div>
            </div>
          </div>

          {/* 操作按钮 */}
          <div className="flex flex-wrap gap-3 mt-6 pt-6 border-t border-border">
            <Permit perm="PLAYER_EDIT">
              <ShimmerButton
                type="submit"
                loading={saving}
                variant="primary"
              >
                保存资料
              </ShimmerButton>
            </Permit>

            <Permit perm="PLAYER_BAN">
              {player.status === 0 ? (
                <ShimmerButton
                  onClick={() => openDialog({
                    title: '确认封禁玩家',
                    content: `确定要封禁玩家 ${player.nickname} (${player.uin}) 吗？`,
                    requireKeyword: 'CONFIRM',
                    variant: 'error',
                    onConfirm: handleBan
                  })}
                  variant="error"
                >
                  封禁玩家
                </ShimmerButton>
              ) : (
                <ShimmerButton
                  onClick={() => openDialog({
                    title: '确认解封玩家',
                    content: `确定要解封玩家 ${player.nickname} (${player.uin}) 吗？`,
                    requireKeyword: 'CONFIRM',
                    variant: 'success',
                    onConfirm: handleUnban
                  })}
                  variant="success"
                >
                  解封玩家
                </ShimmerButton>
              )}
            </Permit>

            <Permit perm="PLAYER_EDIT">
              <ShimmerButton
                onClick={() => openDialog({
                  title: '确认重置二级密码',
                  content: `确定要重置玩家 ${player.nickname} (${player.uin}) 的二级密码吗？`,
                  requireKeyword: 'CONFIRM',
                  variant: 'warning',
                  onConfirm: handleResetSecPwd
                })}
                variant="warning"
              >
                重置二级密码
              </ShimmerButton>
            </Permit>
          </div>
        </form>
      </GlowCard>

      <ConfirmDialog />
    </div>
  )
}

function ProfileSkeleton() {
  return (
    <div className="max-w-4xl mx-auto">
      <div className="mb-6">
        <div className="h-8 bg-bg-secondary rounded w-32 animate-pulse mb-2" />
        <div className="h-4 bg-bg-secondary rounded w-48 animate-pulse" />
      </div>

      <div className="glass-card rounded-lg p-6">
        <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
          {Array.from({ length: 10 }).map((_, i) => (
            <div key={i} className="space-y-2">
              <div className="h-4 bg-bg-secondary rounded w-20 animate-pulse" />
              <div className="h-10 bg-bg-secondary rounded animate-pulse" />
            </div>
          ))}
        </div>
      </div>
    </div>
  )
}