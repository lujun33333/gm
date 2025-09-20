import * as React from "react"
import { usePerm, type Permission } from "@/hooks/usePerm"

interface PermitProps {
  perm: Permission | Permission[]
  mode?: 'hidden' | 'disabled'
  children: React.ReactNode
  fallback?: React.ReactNode
}

/**
 * 权限组件 - 根据用户权限控制子组件的显示和交互
 *
 * @param perm - 需要的权限，可以是单个权限或权限数组
 * @param mode - 权限控制模式：'hidden'(隐藏，默认) | 'disabled'(禁用)
 * @param children - 子组件
 * @param fallback - 无权限时的替代内容
 */
export function Permit({
  perm,
  mode = 'hidden',
  children,
  fallback
}: PermitProps) {
  const { hasPerm, hasAnyPerm } = usePerm()

  // 检查权限
  const hasPermission = React.useMemo(() => {
    if (Array.isArray(perm)) {
      return hasAnyPerm(perm)
    }
    return hasPerm(perm)
  }, [perm, hasPerm, hasAnyPerm])

  // 无权限时的处理
  if (!hasPermission) {
    if (mode === 'hidden') {
      return fallback ? <>{fallback}</> : null
    }

    if (mode === 'disabled') {
      return (
        <div className="opacity-50 pointer-events-none" title="权限不足">
          {children}
        </div>
      )
    }
  }

  return <>{children}</>
}

/**
 * 权限检查 Hook - 用于在组件逻辑中检查权限
 */
export function usePermitCheck(perm: Permission | Permission[]) {
  const { hasPerm, hasAnyPerm } = usePerm()

  return React.useMemo(() => {
    if (Array.isArray(perm)) {
      return hasAnyPerm(perm)
    }
    return hasPerm(perm)
  }, [perm, hasPerm, hasAnyPerm])
}