import { useState, useEffect } from 'react'
import { motion } from 'framer-motion'
import { GlowCard } from '../../components/fx/GlowCard'
import { GradientText } from '../../components/fx/GradientText'
import { ShimmerButton } from '../../components/fx/ShimmerButton'
import { DataTable } from '../../components/data/DataTable'
import { Permit } from '../../components/common/Permit'
import { useConfirmDialog } from '../../components/common/ConfirmDialog'
import { api } from '../../utils/api'

interface GMUser {
  id: string
  username: string
  email: string
  role: string
  permissions: string[]
  status: 'active' | 'disabled'
  created_at: string
  last_login?: string
}

interface Role {
  name: string
  label: string
  permissions: string[]
}

const AVAILABLE_PERMISSIONS = [
  { key: 'MONITOR_VIEW', label: '监控查看', category: '系统管理' },
  { key: 'SERVER_CONTROL', label: '服务器控制', category: '系统管理' },
  { key: 'PLAYER_VIEW', label: '玩家查看', category: '玩家管理' },
  { key: 'PLAYER_EDIT', label: '玩家编辑', category: '玩家管理' },
  { key: 'PLAYER_BAN', label: '玩家封禁', category: '玩家管理' },
  { key: 'INVENTORY_VIEW', label: '背包查看', category: '道具管理' },
  { key: 'ITEM_SEND', label: '道具发放', category: '道具管理' },
  { key: 'ITEM_ADD', label: '道具添加', category: '道具管理' },
  { key: 'ITEM_EDIT', label: '道具编辑', category: '道具管理' },
  { key: 'ITEM_DELETE', label: '道具删除', category: '道具管理' },
  { key: 'CAR_EDIT', label: '赛车编辑', category: '赛车管理' },
  { key: 'NOBLE_EDIT', label: '贵族编辑', category: '贵族管理' },
  { key: 'SIGNIN_EDIT', label: '签到编辑', category: '签到管理' },
  { key: 'PERM_ADMIN', label: '权限管理', category: '权限管理' }
]

const ROLES: Role[] = [
  {
    name: 'OWNER',
    label: '超级管理员',
    permissions: AVAILABLE_PERMISSIONS.map(p => p.key)
  },
  {
    name: 'SUB',
    label: '副管理员',
    permissions: [
      'MONITOR_VIEW',
      'PLAYER_VIEW',
      'PLAYER_EDIT',
      'INVENTORY_VIEW',
      'ITEM_SEND',
      'ITEM_ADD',
      'CAR_EDIT',
      'NOBLE_EDIT'
    ]
  },
  {
    name: 'PLAYER',
    label: '普通玩家',
    permissions: [
      'PLAYER_VIEW',
      'INVENTORY_VIEW'
    ]
  }
]

export function Permissions() {
  const [users, setUsers] = useState<GMUser[]>([])
  const [loading, setLoading] = useState(true)
  const [selectedUser, setSelectedUser] = useState<GMUser | null>(null)
  const [showEditModal, setShowEditModal] = useState(false)
  const { openDialog, ConfirmDialog } = useConfirmDialog()

  useEffect(() => {
    loadUsers()
  }, [])

  const loadUsers = async () => {
    try {
      setLoading(true)
      const response = await api.get('/permissions/users')
      if (response.code === 0) {
        setUsers(response.data.users || [])
      }
    } catch (error) {
      console.error('加载用户数据失败:', error)
    } finally {
      setLoading(false)
    }
  }

  const handleUpdateUserPermissions = async (userId: string, newPermissions: string[]) => {
    try {
      const response = await api.put(`/permissions/users/${userId}`, {
        permissions: newPermissions
      })
      if (response.code === 0) {
        console.log('权限更新成功')
        loadUsers()
        setShowEditModal(false)
      }
    } catch (error) {
      console.error('权限更新失败:', error)
    }
  }

  const handleChangeUserRole = async (userId: string, newRole: string) => {
    const role = ROLES.find(r => r.name === newRole)
    if (!role) return

    try {
      const response = await api.put(`/permissions/users/${userId}`, {
        role: newRole,
        permissions: role.permissions
      })
      if (response.code === 0) {
        console.log('角色更新成功')
        loadUsers()
      }
    } catch (error) {
      console.error('角色更新失败:', error)
    }
  }

  const handleToggleUserStatus = async (userId: string, newStatus: string) => {
    try {
      const response = await api.patch(`/permissions/users/${userId}`, {
        status: newStatus
      })
      if (response.code === 0) {
        console.log('状态更新成功')
        loadUsers()
      }
    } catch (error) {
      console.error('状态更新失败:', error)
    }
  }

  const getRoleLabel = (roleName: string) => {
    const role = ROLES.find(r => r.name === roleName)
    return role?.label || roleName
  }

  const getRoleColor = (roleName: string) => {
    const colors = {
      OWNER: 'text-red-400 bg-red-500/20 border-red-500/30',
      SUB: 'text-orange-400 bg-orange-500/20 border-orange-500/30',
      PLAYER: 'text-blue-400 bg-blue-500/20 border-blue-500/30'
    }
    return colors[roleName as keyof typeof colors] || 'text-gray-400 bg-gray-500/20 border-gray-500/30'
  }

  const getPermissionsByCategory = () => {
    const categories: { [key: string]: typeof AVAILABLE_PERMISSIONS } = {}
    AVAILABLE_PERMISSIONS.forEach(perm => {
      if (!categories[perm.category]) {
        categories[perm.category] = []
      }
      categories[perm.category].push(perm)
    })
    return categories
  }

  const columns = [
    {
      key: 'username',
      title: '用户名',
      dataIndex: 'username',
      render: (value: string, record: GMUser) => (
        <div>
          <div className="font-medium">{value}</div>
          <div className="text-xs text-text-muted">{record.email}</div>
        </div>
      )
    },
    {
      key: 'role',
      title: '角色',
      dataIndex: 'role',
      align: 'center' as const,
      render: (value: string) => (
        <span className={`px-2 py-1 text-xs rounded border ${getRoleColor(value)}`}>
          {getRoleLabel(value)}
        </span>
      )
    },
    {
      key: 'permissions',
      title: '权限数量',
      render: (_, record: GMUser) => (
        <div className="text-center">
          <span className="text-lg font-mono text-primary">{record.permissions.length}</span>
          <div className="text-xs text-text-muted">个权限</div>
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
      key: 'last_login',
      title: '最后登录',
      dataIndex: 'last_login',
      render: (value: string) => value || '从未登录'
    },
    {
      key: 'actions',
      title: '操作',
      render: (_, record: GMUser) => (
        <div className="flex gap-1">
          <button
            onClick={() => {
              setSelectedUser(record)
              setShowEditModal(true)
            }}
            className="px-2 py-1 text-xs bg-primary/20 text-primary rounded hover:bg-primary/30 transition-colors"
          >
            编辑权限
          </button>
          <select
            value={record.role}
            onChange={(e) => openDialog({
              title: '确认更改角色',
              content: `确定要将用户 ${record.username} 的角色更改为 ${getRoleLabel(e.target.value)} 吗？`,
              requireKeyword: 'CONFIRM',
              variant: 'warning',
              onConfirm: () => handleChangeUserRole(record.id, e.target.value)
            })}
            className="px-2 py-1 text-xs bg-bg-secondary border border-border rounded focus:ring-1 focus:ring-primary"
          >
            {ROLES.map(role => (
              <option key={role.name} value={role.name}>
                {role.label}
              </option>
            ))}
          </select>
          <button
            onClick={() => openDialog({
              title: `确认${record.status === 'active' ? '禁用' : '启用'}用户`,
              content: `确定要${record.status === 'active' ? '禁用' : '启用'}用户 ${record.username} 吗？`,
              requireKeyword: 'CONFIRM',
              variant: record.status === 'active' ? 'error' : 'success',
              onConfirm: () => handleToggleUserStatus(record.id, record.status === 'active' ? 'disabled' : 'active')
            })}
            className={`px-2 py-1 text-xs rounded transition-colors ${
              record.status === 'active'
                ? 'bg-error/20 text-error hover:bg-error/30'
                : 'bg-success/20 text-success hover:bg-success/30'
            }`}
          >
            {record.status === 'active' ? '禁用' : '启用'}
          </button>
        </div>
      )
    }
  ]

  return (
    <div className="max-w-7xl mx-auto">
      <div className="mb-6">
        <GradientText className="text-3xl font-bold">
          权限管理
        </GradientText>
        <p className="text-text-muted mt-2">管理 GM 用户的角色和权限设置</p>
      </div>

      <div className="grid gap-6">
        {/* 角色说明 */}
        <GlowCard>
          <h3 className="text-lg font-semibold mb-4">系统角色说明</h3>
          <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
            {ROLES.map(role => (
              <div key={role.name} className="border border-border rounded-lg p-4">
                <div className="flex items-center gap-2 mb-2">
                  <span className={`px-2 py-1 text-xs rounded border ${getRoleColor(role.name)}`}>
                    {role.label}
                  </span>
                  <span className="text-sm text-text-muted">
                    {role.permissions.length} 个权限
                  </span>
                </div>
                <div className="text-sm text-text-muted">
                  {role.name === 'OWNER' && '拥有所有系统权限，可以管理其他用户'}
                  {role.name === 'SUB' && '拥有部分管理权限，可以进行日常运营操作'}
                  {role.name === 'PLAYER' && '只能查看基本信息，无管理权限'}
                </div>
              </div>
            ))}
          </div>
        </GlowCard>

        {/* 用户列表 */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ delay: 0.1 }}
        >
          <DataTable
            columns={columns}
            data={users}
            loading={loading}
            rowKey="id"
          />
        </motion.div>
      </div>

      {/* 权限编辑模态框 */}
      {showEditModal && selectedUser && (
        <div className="fixed inset-0 bg-black/50 backdrop-blur-sm z-50 flex items-center justify-center p-4">
          <motion.div
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
            className="bg-bg-primary border border-border rounded-lg shadow-2xl max-w-2xl w-full max-h-[80vh] overflow-auto"
          >
            <div className="p-6">
              <div className="flex items-center justify-between mb-6">
                <h3 className="text-xl font-semibold">
                  编辑用户权限 - {selectedUser.username}
                </h3>
                <button
                  onClick={() => setShowEditModal(false)}
                  className="w-8 h-8 flex items-center justify-center rounded-lg hover:bg-bg-secondary transition-colors"
                >
                  ✕
                </button>
              </div>

              <div className="space-y-6">
                {Object.entries(getPermissionsByCategory()).map(([category, permissions]) => (
                  <div key={category}>
                    <h4 className="font-medium mb-3 text-primary">{category}</h4>
                    <div className="grid grid-cols-1 md:grid-cols-2 gap-2">
                      {permissions.map(perm => (
                        <label
                          key={perm.key}
                          className="flex items-center gap-2 p-2 rounded hover:bg-bg-secondary transition-colors cursor-pointer"
                        >
                          <input
                            type="checkbox"
                            checked={selectedUser.permissions.includes(perm.key)}
                            onChange={(e) => {
                              const newPermissions = e.target.checked
                                ? [...selectedUser.permissions, perm.key]
                                : selectedUser.permissions.filter(p => p !== perm.key)
                              setSelectedUser({
                                ...selectedUser,
                                permissions: newPermissions
                              })
                            }}
                            className="w-4 h-4 text-primary bg-bg-secondary border-border rounded focus:ring-primary"
                          />
                          <span className="text-sm">{perm.label}</span>
                        </label>
                      ))}
                    </div>
                  </div>
                ))}
              </div>

              <div className="flex gap-3 mt-6 pt-6 border-t border-border">
                <ShimmerButton
                  onClick={() => handleUpdateUserPermissions(selectedUser.id, selectedUser.permissions)}
                  variant="primary"
                >
                  保存权限
                </ShimmerButton>
                <ShimmerButton
                  onClick={() => setShowEditModal(false)}
                  variant="secondary"
                >
                  取消
                </ShimmerButton>
              </div>
            </div>
          </motion.div>
        </div>
      )}

      <ConfirmDialog />
    </div>
  )
}