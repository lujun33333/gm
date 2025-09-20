import { useMemo } from 'react'

// 权限点定义
export type Permission =
  | 'MONITOR_VIEW'          // 查看监控数据
  | 'SERVER_CONTROL'        // 服务器控制
  | 'PLAYER_VIEW'           // 查看玩家信息
  | 'PLAYER_EDIT'           // 编辑玩家信息
  | 'PLAYER_BAN'            // 封禁玩家
  | 'INVENTORY_VIEW'        // 查看背包
  | 'ITEM_SEND'             // 发送道具
  | 'ITEM_ADD'              // 添加道具
  | 'ITEM_EDIT'             // 编辑道具
  | 'ITEM_DELETE'           // 删除道具
  | 'CAR_EDIT'              // 编辑载具
  | 'NOBLE_EDIT'            // 编辑贵族
  | 'SIGNIN_EDIT'           // 编辑签到
  | 'PERM_ADMIN'            // 权限管理
  | '*'                     // 超级管理员权限

// 角色定义
export type Role = 'OWNER' | 'SUB' | 'PLAYER'

// 默认角色权限映射
const DEFAULT_ROLE_PERMISSIONS: Record<Role, Permission[]> = {
  OWNER: ['*'], // 超级管理员拥有所有权限
  SUB: [
    'MONITOR_VIEW',
    'PLAYER_VIEW',
    'PLAYER_EDIT',
    'INVENTORY_VIEW',
    'ITEM_SEND',
    'ITEM_EDIT',
    'CAR_EDIT',
    'NOBLE_EDIT',
    'SIGNIN_EDIT'
  ],
  PLAYER: [
    'MONITOR_VIEW',
    'PLAYER_VIEW',
    'INVENTORY_VIEW'
  ]
}

interface UserInfo {
  username: string
  role: Role
  permissions?: Permission[]
}

export function usePerm() {
  const userInfo = useMemo<UserInfo | null>(() => {
    try {
      const stored = localStorage.getItem('gm-user')
      return stored ? JSON.parse(stored) : null
    } catch {
      return null
    }
  }, [])

  const userPermissions = useMemo<Permission[]>(() => {
    if (!userInfo) return []

    // 如果用户有自定义权限，使用自定义权限
    if (userInfo.permissions && userInfo.permissions.length > 0) {
      return userInfo.permissions
    }

    // 否则使用默认角色权限
    return DEFAULT_ROLE_PERMISSIONS[userInfo.role] || []
  }, [userInfo])

  const hasPerm = (permission: Permission): boolean => {
    if (!userInfo) return false

    // 超级管理员权限
    if (userPermissions.includes('*')) return true

    // 检查具体权限
    return userPermissions.includes(permission)
  }

  const hasAnyPerm = (permissions: Permission[]): boolean => {
    return permissions.some(permission => hasPerm(permission))
  }

  const hasAllPerm = (permissions: Permission[]): boolean => {
    return permissions.every(permission => hasPerm(permission))
  }

  return {
    userInfo,
    userPermissions,
    hasPerm,
    hasAnyPerm,
    hasAllPerm,
    isLoggedIn: !!userInfo,
    isOwner: userInfo?.role === 'OWNER',
    isSub: userInfo?.role === 'SUB',
    isPlayer: userInfo?.role === 'PLAYER',
    // 向后兼容
    hasPermission: hasPerm
  }
}