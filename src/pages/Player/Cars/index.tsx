import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { GlowCard } from '../../../components/fx/GlowCard'
import { GradientText } from '../../../components/fx/GradientText'
import { ShimmerButton } from '../../../components/fx/ShimmerButton'
import { DataTable } from '../../../components/data/DataTable'
import { Permit } from '../../../components/common/Permit'
import { useConfirmDialog } from '../../../components/common/ConfirmDialog'
import { api } from '../../../utils/api'

interface PlayerCar {
  id: string
  car_id: string
  car_name: string
  car_type: string
  level: number
  experience: number
  max_experience: number
  speed: number
  acceleration: number
  handling: number
  stability: number
  color: string
  is_favorite: boolean
  acquired_at: string
}

export function PlayerCars() {
  const [cars, setCars] = useState<PlayerCar[]>([])
  const [loading, setLoading] = useState(true)
  const [selectedPlayer, setSelectedPlayer] = useState('')
  const [filterType, setFilterType] = useState('all')
  const { openDialog, ConfirmDialog } = useConfirmDialog()

  useEffect(() => {
    if (selectedPlayer) {
      loadCars()
    }
  }, [selectedPlayer])

  const loadCars = async () => {
    try {
      setLoading(true)
      const response = await api.get(`/player/${selectedPlayer}/cars`)
      if (response.code === 0) {
        setCars(response.data.cars || [])
      }
    } catch (error) {
      console.error('加载赛车数据失败:', error)
    } finally {
      setLoading(false)
    }
  }

  const handleEditCar = async (carId: string, updates: Partial<PlayerCar>) => {
    try {
      const response = await api.put(`/player/${selectedPlayer}/cars/${carId}`, updates)
      if (response.code === 0) {
        console.log('修改成功')
        loadCars()
      }
    } catch (error) {
      console.error('修改失败:', error)
    }
  }

  const handleDeleteCar = async (carId: string, carName: string) => {
    try {
      const response = await api.delete(`/player/${selectedPlayer}/cars/${carId}`)
      if (response.code === 0) {
        console.log('删除成功')
        loadCars()
      }
    } catch (error) {
      console.error('删除失败:', error)
    }
  }

  const handleLevelUp = async (carId: string, carName: string) => {
    try {
      const response = await api.post(`/player/${selectedPlayer}/cars/${carId}/levelup`)
      if (response.code === 0) {
        console.log('升级成功')
        loadCars()
      }
    } catch (error) {
      console.error('升级失败:', error)
    }
  }

  const getCarTypeColor = (type: string) => {
    const colors = {
      'S': 'text-red-400',
      'A': 'text-orange-400',
      'B': 'text-yellow-400',
      'C': 'text-green-400',
      'D': 'text-blue-400'
    }
    return colors[type as keyof typeof colors] || 'text-text'
  }

  const getProgressPercent = (exp: number, maxExp: number) => {
    return Math.round((exp / maxExp) * 100)
  }

  const columns = [
    {
      key: 'car_name',
      title: '赛车名称',
      dataIndex: 'car_name',
      render: (value: string, record: PlayerCar) => (
        <div className="flex items-center gap-2">
          <span className={getCarTypeColor(record.car_type)}>{value}</span>
          {record.is_favorite && <span className="text-yellow-400">⭐</span>}
        </div>
      )
    },
    {
      key: 'car_type',
      title: '等级',
      dataIndex: 'car_type',
      align: 'center' as const,
      render: (value: string) => (
        <span className={`font-bold text-lg ${getCarTypeColor(value)}`}>{value}</span>
      )
    },
    {
      key: 'level',
      title: '强化等级',
      dataIndex: 'level',
      align: 'center' as const,
      render: (value: number) => (
        <span className="font-mono text-primary">+{value}</span>
      )
    },
    {
      key: 'experience',
      title: '经验值',
      render: (_, record: PlayerCar) => (
        <div className="space-y-1">
          <div className="flex justify-between text-xs">
            <span>{record.experience.toLocaleString()}</span>
            <span>{record.max_experience.toLocaleString()}</span>
          </div>
          <div className="w-full bg-bg-tertiary rounded-full h-2">
            <div
              className="bg-gradient-to-r from-primary to-primary-light h-2 rounded-full transition-all"
              style={{ width: `${getProgressPercent(record.experience, record.max_experience)}%` }}
            />
          </div>
          <div className="text-xs text-text-muted text-center">
            {getProgressPercent(record.experience, record.max_experience)}%
          </div>
        </div>
      )
    },
    {
      key: 'stats',
      title: '属性',
      render: (_, record: PlayerCar) => (
        <div className="grid grid-cols-2 gap-1 text-xs">
          <div>速度: <span className="text-primary">{record.speed}</span></div>
          <div>加速: <span className="text-primary">{record.acceleration}</span></div>
          <div>操控: <span className="text-primary">{record.handling}</span></div>
          <div>稳定: <span className="text-primary">{record.stability}</span></div>
        </div>
      )
    },
    {
      key: 'color',
      title: '颜色',
      dataIndex: 'color',
      render: (value: string) => (
        <div className="flex items-center gap-2">
          <div
            className="w-4 h-4 rounded border border-border"
            style={{ backgroundColor: value }}
          />
          <span className="text-xs">{value}</span>
        </div>
      )
    },
    {
      key: 'acquired_at',
      title: '获得时间',
      dataIndex: 'acquired_at'
    },
    {
      key: 'actions',
      title: '操作',
      render: (_, record: PlayerCar) => (
        <div className="flex gap-1">
          <Permit perm="CAR_EDIT">
            <button
              onClick={() => openDialog({
                title: '确认升级赛车',
                content: `确定要升级赛车 ${record.car_name} 吗？`,
                requireKeyword: 'CONFIRM',
                variant: 'primary',
                onConfirm: () => handleLevelUp(record.id, record.car_name)
              })}
              className="px-2 py-1 text-xs bg-primary/20 text-primary rounded hover:bg-primary/30 transition-colors"
            >
              升级
            </button>
          </Permit>
          <Permit perm="CAR_EDIT">
            <button
              onClick={() => handleEditCar(record.id, { is_favorite: !record.is_favorite })}
              className={`px-2 py-1 text-xs rounded transition-colors ${
                record.is_favorite
                  ? 'bg-yellow-500/20 text-yellow-400 hover:bg-yellow-500/30'
                  : 'bg-bg-secondary text-text-muted hover:bg-bg-tertiary'
              }`}
            >
              {record.is_favorite ? '取消收藏' : '收藏'}
            </button>
          </Permit>
          <Permit perm="CAR_EDIT">
            <button
              onClick={() => openDialog({
                title: '确认删除赛车',
                content: `确定要删除赛车 ${record.car_name} 吗？此操作不可撤销！`,
                requireKeyword: 'CONFIRM',
                variant: 'error',
                onConfirm: () => handleDeleteCar(record.id, record.car_name)
              })}
              className="px-2 py-1 text-xs bg-error/20 text-error rounded hover:bg-error/30 transition-colors"
            >
              删除
            </button>
          </Permit>
        </div>
      )
    }
  ]

  const filteredCars = cars.filter(car => {
    if (filterType === 'all') return true
    return car.car_type === filterType
  })

  const carTypes = [...new Set(cars.map(car => car.car_type))]

  return (
    <div className="max-w-7xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          玩家赛车
        </GradientText>
        <p className="text-text-muted mt-2">查看和管理玩家的赛车收藏</p>
      </div>

      <div className="grid gap-6">
        {/* 查询区域 */}
        <GlowCard>
          <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
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

            <div>
              <label className="block text-sm font-medium mb-1">赛车等级</label>
              <select
                value={filterType}
                onChange={(e) => setFilterType(e.target.value)}
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              >
                <option value="all">全部等级</option>
                {carTypes.map(type => (
                  <option key={type} value={type}>{type} 级</option>
                ))}
              </select>
            </div>

            <div className="flex items-end">
              <ShimmerButton
                onClick={loadCars}
                loading={loading}
                variant="primary"
                disabled={!selectedPlayer}
              >
                查询赛车
              </ShimmerButton>
            </div>
          </div>
        </GlowCard>

        {/* 统计卡片 */}
        {selectedPlayer && cars.length > 0 && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            className="grid grid-cols-1 md:grid-cols-4 gap-4"
          >
            <GlowCard className="text-center">
              <div className="text-2xl font-bold text-primary">{cars.length}</div>
              <div className="text-sm text-text-muted">总赛车数</div>
            </GlowCard>
            <GlowCard className="text-center">
              <div className="text-2xl font-bold text-yellow-400">
                {cars.filter(car => car.is_favorite).length}
              </div>
              <div className="text-sm text-text-muted">收藏数量</div>
            </GlowCard>
            <GlowCard className="text-center">
              <div className="text-2xl font-bold text-green-400">
                {Math.round(cars.reduce((sum, car) => sum + car.level, 0) / cars.length)}
              </div>
              <div className="text-sm text-text-muted">平均等级</div>
            </GlowCard>
            <GlowCard className="text-center">
              <div className="text-2xl font-bold text-purple-400">
                {cars.filter(car => car.car_type === 'S').length}
              </div>
              <div className="text-sm text-text-muted">S级赛车</div>
            </GlowCard>
          </motion.div>
        )}

        {/* 数据表格 */}
        {selectedPlayer && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.1 }}
          >
            <DataTable
              columns={columns}
              data={filteredCars}
              loading={loading}
              rowKey="id"
            />
          </motion.div>
        )}

        {!selectedPlayer && (
          <div className="text-center py-12">
            <div className="text-6xl mb-4">🏎️</div>
            <h3 className="text-lg font-medium text-text mb-2">请选择玩家</h3>
            <p className="text-text-muted">输入玩家 UIN 来查看赛车收藏</p>
          </div>
        )}
      </div>

      <ConfirmDialog />
    </div>
  )
}