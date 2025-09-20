import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { GlowCard } from '../../components/fx/GlowCard'
import { GradientText } from '../../components/fx/GradientText'
import { ShimmerButton } from '../../components/fx/ShimmerButton'
import { DataTable } from '../../components/data/DataTable'
import { Permit } from '../../components/common/Permit'
import { useConfirmDialog } from '../../components/common/ConfirmDialog'
import { api } from '../../utils/api'

interface GameItem {
  id: string
  name: string
  type: string
  category: string
  description: string
  price: number
  max_stack: number
  tradeable: boolean
  sellable: boolean
  icon_url?: string
  rarity: 'common' | 'rare' | 'epic' | 'legendary'
  status: 'active' | 'disabled'
  created_at: string
  updated_at: string
}

export function Items() {
  const [items, setItems] = useState<GameItem[]>([])
  const [loading, setLoading] = useState(true)
  const [searchQuery, setSearchQuery] = useState('')
  const [filterCategory, setFilterCategory] = useState('all')
  const [filterRarity, setFilterRarity] = useState('all')
  const [selectedItem, setSelectedItem] = useState<GameItem | null>(null)
  const [showAddModal, setShowAddModal] = useState(false)
  const { openDialog, ConfirmDialog } = useConfirmDialog()

  useEffect(() => {
    loadItems()
  }, [])

  const loadItems = async () => {
    try {
      setLoading(true)
      const response = await api.get('/items')
      if (response.code === 0) {
        setItems(response.data.items || [])
      }
    } catch (error) {
      console.error('加载道具数据失败:', error)
    } finally {
      setLoading(false)
    }
  }

  const handleEditItem = async (item: GameItem) => {
    setSelectedItem(item)
    // 这里可以打开编辑模态框
    console.log('编辑道具:', item)
  }

  const handleDeleteItem = async (itemId: string, itemName: string) => {
    try {
      const response = await api.delete(`/items/${itemId}`)
      if (response.code === 0) {
        console.log('删除成功')
        loadItems()
      }
    } catch (error) {
      console.error('删除失败:', error)
    }
  }

  const handleToggleStatus = async (itemId: string, newStatus: string) => {
    try {
      const response = await api.patch(`/items/${itemId}`, { status: newStatus })
      if (response.code === 0) {
        console.log('状态更新成功')
        loadItems()
      }
    } catch (error) {
      console.error('状态更新失败:', error)
    }
  }

  const getRarityColor = (rarity: string) => {
    const colors = {
      legendary: 'text-yellow-400 bg-yellow-500/20 border-yellow-500/30',
      epic: 'text-purple-400 bg-purple-500/20 border-purple-500/30',
      rare: 'text-blue-400 bg-blue-500/20 border-blue-500/30',
      common: 'text-gray-400 bg-gray-500/20 border-gray-500/30'
    }
    return colors[rarity as keyof typeof colors] || colors.common
  }

  const getRarityLabel = (rarity: string) => {
    const labels = {
      legendary: '传说',
      epic: '史诗',
      rare: '稀有',
      common: '普通'
    }
    return labels[rarity as keyof typeof labels] || '普通'
  }

  const columns = [
    {
      key: 'icon',
      title: '图标',
      width: 60,
      render: (_, record: GameItem) => (
        <div className="w-10 h-10 bg-bg-tertiary rounded border border-border flex items-center justify-center">
          {record.icon_url ? (
            <img src={record.icon_url} alt={record.name} className="w-8 h-8" />
          ) : (
            <span className="text-lg">📦</span>
          )}
        </div>
      )
    },
    {
      key: 'name',
      title: '道具名称',
      dataIndex: 'name',
      sorter: true,
      render: (value: string, record: GameItem) => (
        <div>
          <div className="font-medium">{value}</div>
          <div className="text-xs text-text-muted">ID: {record.id}</div>
        </div>
      )
    },
    {
      key: 'rarity',
      title: '稀有度',
      dataIndex: 'rarity',
      align: 'center' as const,
      render: (value: string) => (
        <span className={`px-2 py-1 text-xs rounded border ${getRarityColor(value)}`}>
          {getRarityLabel(value)}
        </span>
      )
    },
    {
      key: 'category',
      title: '分类',
      dataIndex: 'category',
      render: (value: string) => (
        <span className="px-2 py-1 text-xs bg-primary/20 text-primary rounded">
          {value}
        </span>
      )
    },
    {
      key: 'type',
      title: '类型',
      dataIndex: 'type'
    },
    {
      key: 'price',
      title: '价格',
      dataIndex: 'price',
      align: 'right' as const,
      render: (value: number) => (
        <span className="font-mono text-primary">{value.toLocaleString()}</span>
      )
    },
    {
      key: 'max_stack',
      title: '最大堆叠',
      dataIndex: 'max_stack',
      align: 'center' as const,
      render: (value: number) => (
        <span className="font-mono">{value}</span>
      )
    },
    {
      key: 'properties',
      title: '属性',
      render: (_, record: GameItem) => (
        <div className="flex gap-1">
          {record.tradeable && (
            <span className="px-1 py-0.5 text-xs bg-green-500/20 text-green-400 rounded">
              可交易
            </span>
          )}
          {record.sellable && (
            <span className="px-1 py-0.5 text-xs bg-blue-500/20 text-blue-400 rounded">
              可出售
            </span>
          )}
        </div>
      )
    },
    {
      key: 'status',
      title: '状态',
      dataIndex: 'status',
      align: 'center' as const,
      render: (value: string) => (
        <span className={`px-2 py-1 text-xs rounded ${
          value === 'active'
            ? 'bg-success/20 text-success'
            : 'bg-error/20 text-error'
        }`}>
          {value === 'active' ? '启用' : '禁用'}
        </span>
      )
    },
    {
      key: 'actions',
      title: '操作',
      render: (_, record: GameItem) => (
        <div className="flex gap-1">
          <button
            onClick={() => console.log('查看详情', record)}
            className="px-2 py-1 text-xs bg-bg-secondary hover:bg-bg-tertiary rounded transition-colors"
          >
            详情
          </button>
          <Permit perm="ITEM_EDIT">
            <button
              onClick={() => handleEditItem(record)}
              className="px-2 py-1 text-xs bg-primary/20 text-primary rounded hover:bg-primary/30 transition-colors"
            >
              编辑
            </button>
          </Permit>
          <Permit perm="ITEM_EDIT">
            <button
              onClick={() => handleToggleStatus(record.id, record.status === 'active' ? 'disabled' : 'active')}
              className={`px-2 py-1 text-xs rounded transition-colors ${
                record.status === 'active'
                  ? 'bg-warning/20 text-warning hover:bg-warning/30'
                  : 'bg-success/20 text-success hover:bg-success/30'
              }`}
            >
              {record.status === 'active' ? '禁用' : '启用'}
            </button>
          </Permit>
          <Permit perm="ITEM_DELETE">
            <button
              onClick={() => openDialog({
                title: '确认删除道具',
                content: `确定要删除道具 ${record.name} 吗？此操作不可撤销！`,
                requireKeyword: 'CONFIRM',
                variant: 'error',
                onConfirm: () => handleDeleteItem(record.id, record.name)
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
    const matchesSearch = item.name.toLowerCase().includes(searchQuery.toLowerCase()) ||
                         item.description.toLowerCase().includes(searchQuery.toLowerCase())
    const matchesCategory = filterCategory === 'all' || item.category === filterCategory
    const matchesRarity = filterRarity === 'all' || item.rarity === filterRarity

    return matchesSearch && matchesCategory && matchesRarity
  })

  const categories = [...new Set(items.map(item => item.category))]
  const rarities = ['common', 'rare', 'epic', 'legendary']

  const stats = {
    total: items.length,
    active: items.filter(item => item.status === 'active').length,
    disabled: items.filter(item => item.status === 'disabled').length,
    legendary: items.filter(item => item.rarity === 'legendary').length
  }

  return (
    <div className="max-w-7xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          道具库管理
        </GradientText>
        <p className="text-text-muted mt-2">管理游戏中的所有道具和物品</p>
      </div>

      <div className="grid gap-6">
        {/* 统计卡片 */}
        <div className="grid grid-cols-1 md:grid-cols-4 gap-4">
          <GlowCard className="text-center">
            <div className="text-2xl font-bold text-primary">{stats.total}</div>
            <div className="text-sm text-text-muted">总道具数</div>
          </GlowCard>
          <GlowCard className="text-center">
            <div className="text-2xl font-bold text-success">{stats.active}</div>
            <div className="text-sm text-text-muted">启用中</div>
          </GlowCard>
          <GlowCard className="text-center">
            <div className="text-2xl font-bold text-error">{stats.disabled}</div>
            <div className="text-sm text-text-muted">已禁用</div>
          </GlowCard>
          <GlowCard className="text-center">
            <div className="text-2xl font-bold text-yellow-400">{stats.legendary}</div>
            <div className="text-sm text-text-muted">传说道具</div>
          </GlowCard>
        </div>

        {/* 筛选区域 */}
        <GlowCard>
          <div className="grid grid-cols-1 md:grid-cols-5 gap-4">
            <div className="md:col-span-2">
              <label className="block text-sm font-medium mb-1">搜索道具</label>
              <input
                type="text"
                value={searchQuery}
                onChange={(e) => setSearchQuery(e.target.value)}
                placeholder="输入道具名称或描述"
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              />
            </div>

            <div>
              <label className="block text-sm font-medium mb-1">分类</label>
              <select
                value={filterCategory}
                onChange={(e) => setFilterCategory(e.target.value)}
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              >
                <option value="all">全部分类</option>
                {categories.map(category => (
                  <option key={category} value={category}>{category}</option>
                ))}
              </select>
            </div>

            <div>
              <label className="block text-sm font-medium mb-1">稀有度</label>
              <select
                value={filterRarity}
                onChange={(e) => setFilterRarity(e.target.value)}
                className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent"
              >
                <option value="all">全部稀有度</option>
                {rarities.map(rarity => (
                  <option key={rarity} value={rarity}>{getRarityLabel(rarity)}</option>
                ))}
              </select>
            </div>

            <div className="flex items-end">
              <Permit perm="ITEM_ADD">
                <ShimmerButton
                  onClick={() => setShowAddModal(true)}
                  variant="primary"
                >
                  添加道具
                </ShimmerButton>
              </Permit>
            </div>
          </div>
        </GlowCard>

        {/* 数据表格 */}
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
            pageSize={15}
          />
        </motion.div>
      </div>

      <ConfirmDialog />
    </div>
  )
}