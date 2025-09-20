import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { GlowCard } from '../../../components/fx/GlowCard'
import { GradientText } from '../../../components/fx/GradientText'
import { ShimmerButton } from '../../../components/fx/ShimmerButton'
import { DataTable } from '../../../components/data/DataTable'
import { Permit } from '../../../components/common/Permit'
import { useConfirmDialog } from '../../../components/common/ConfirmDialog'
import { api } from '../../../utils/api'

interface InventoryItem {
  id: string
  item_id: string
  item_name: string
  item_type: string
  quantity: number
  acquired_at: string
  expire_at?: string
  quality: 'common' | 'rare' | 'epic' | 'legendary'
}

export function PlayerInventory() {
  const [items, setItems] = useState<InventoryItem[]>([])
  const [loading, setLoading] = useState(true)
  const [selectedPlayer, setSelectedPlayer] = useState('')
  const [filterType, setFilterType] = useState('all')
  const { openDialog, ConfirmDialog } = useConfirmDialog()

  useEffect(() => {
    if (selectedPlayer) {
      loadInventory()
    }
  }, [selectedPlayer])

  const loadInventory = async () => {
    try {
      setLoading(true)
      const response = await api.get(`/player/${selectedPlayer}/inventory`)
      if (response.code === 0) {
        setItems(response.data.items || [])
      }
    } catch (error) {
      console.error('加载背包数据失败:', error)
    } finally {
      setLoading(false)
    }
  }

  const handleDeleteItem = async (itemId: string, itemName: string) => {
    try {
      const response = await api.delete(`/player/${selectedPlayer}/inventory/${itemId}`)
      if (response.code === 0) {
        console.log('删除成功')
        loadInventory()
      }
    } catch (error) {
      console.error('删除失败:', error)
    }
  }

  const handleAddItem = () => {
    // 这里可以打开添加道具的模态框
    console.log('添加道具')
  }

  const getQualityColor = (quality: string) => {
    switch (quality) {
      case 'legendary': return 'text-yellow-400'
      case 'epic': return 'text-purple-400'
      case 'rare': return 'text-blue-400'
      default: return 'text-text'
    }
  }

  const getQualityBadge = (quality: string) => {
    const colors = {
      legendary: 'bg-yellow-500/20 text-yellow-400 border-yellow-500/30',
      epic: 'bg-purple-500/20 text-purple-400 border-purple-500/30',
      rare: 'bg-blue-500/20 text-blue-400 border-blue-500/30',
      common: 'bg-gray-500/20 text-gray-400 border-gray-500/30'
    }

    const labels = {
      legendary: '传说',
      epic: '史诗',
      rare: '稀有',
      common: '普通'
    }

    return (
      <span className={`px-2 py-1 text-xs rounded border ${colors[quality as keyof typeof colors]}`}>
        {labels[quality as keyof typeof labels]}
      </span>
    )
  }

  const columns = [
    {
      key: 'item_name',
      title: '道具名称',
      dataIndex: 'item_name',
      render: (value: string, record: InventoryItem) => (
        <div className="flex items-center gap-2">
          <span className={getQualityColor(record.quality)}>{value}</span>
          {getQualityBadge(record.quality)}
        </div>
      )
    },
    {
      key: 'item_type',
      title: '类型',
      dataIndex: 'item_type'
    },
    {
      key: 'quantity',
      title: '数量',
      dataIndex: 'quantity',
      align: 'center' as const,
      render: (value: number) => (
        <span className="font-mono text-primary">{value.toLocaleString()}</span>
      )
    },
    {
      key: 'acquired_at',
      title: '获得时间',
      dataIndex: 'acquired_at'
    },
    {
      key: 'expire_at',
      title: '到期时间',
      dataIndex: 'expire_at',
      render: (value: string) => value || '永久'
    },
    {
      key: 'actions',
      title: '操作',
      render: (_: any, record: InventoryItem) => (
        <div className="flex gap-2">
          <Permit perm="ITEM_EDIT">
            <button
              onClick={() => console.log('编辑', record)}
              className="px-2 py-1 text-xs bg-primary/20 text-primary rounded hover:bg-primary/30 transition-colors"
            >
              编辑
            </button>
          </Permit>
          <Permit perm="ITEM_DELETE">
            <button
              onClick={() => openDialog({
                title: '确认删除道具',
                content: `确定要删除道具 ${record.item_name} (数量: ${record.quantity}) 吗？`,
                requireKeyword: 'CONFIRM',
                variant: 'error',
                onConfirm: () => handleDeleteItem(record.id, record.item_name)
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

  const filteredItems = items.filter(item => {
    if (filterType === 'all') return true
    return item.item_type === filterType
  })

  const itemTypes = [...new Set(items.map(item => item.item_type))]

  return (
    <div className="max-w-6xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          玩家背包
        </GradientText>
        <p className="text-text-muted mt-2">查看和管理玩家的道具背包</p>
      </div>

      <div className="grid gap-6">
        {/* 查询区域 */}
        <GlowCard>
          <div className="grid grid-cols-1 md:grid-cols-4 gap-4">
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
              <label className="block text-sm font-medium mb-1">道具类型</label>
              <select
                value={filterType}
                onChange={(e) => setFilterType(e.target.value)}
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              >
                <option value="all">全部类型</option>
                {itemTypes.map(type => (
                  <option key={type} value={type}>{type}</option>
                ))}
              </select>
            </div>

            <div className="flex items-end">
              <ShimmerButton
                onClick={loadInventory}
                loading={loading}
                variant="primary"
                disabled={!selectedPlayer}
              >
                查询背包
              </ShimmerButton>
            </div>

            <div className="flex items-end">
              <Permit perm="ITEM_ADD">
                <ShimmerButton
                  onClick={handleAddItem}
                  variant="secondary"
                  disabled={!selectedPlayer}
                >
                  添加道具
                </ShimmerButton>
              </Permit>
            </div>
          </div>
        </GlowCard>

        {/* 数据表格 */}
        {selectedPlayer && (
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.1 }}
          >
            <DataTable
              columns={columns}
              data={filteredItems}
              loading={loading}
              rowKey="id"
            />
          </motion.div>
        )}

        {!selectedPlayer && (
          <div className="text-center py-12">
            <div className="text-6xl mb-4">🎒</div>
            <h3 className="text-lg font-medium text-text mb-2">请选择玩家</h3>
            <p className="text-text-muted">输入玩家 UIN 来查看背包内容</p>
          </div>
        )}
      </div>

      <ConfirmDialog />
    </div>
  )
}