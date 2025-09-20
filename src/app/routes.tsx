import { Routes as RouterRoutes, Route, Navigate } from 'react-router-dom'
import { AppShell } from '../components/layout/AppShell'
import { Login } from '../pages/Login'
import { Dashboard } from '../pages/Dashboard'
import { PlayerProfile } from '../pages/Player/Profile'
import { PlayerInventory } from '../pages/Player/Inventory'
import { PlayerCars } from '../pages/Player/Cars'
import { PlayerVip } from '../pages/Player/Vip'
import { PlayerSignIn } from '../pages/Player/SignIn'
import { Items } from '../pages/Items'
import { SendItem } from '../pages/SendItem'
import { Permissions } from '../pages/Permissions'
import { SchemaPage } from '../pages/Schema'

// 受保护的路由组件
function ProtectedRoute({ children }: { children: React.ReactNode }) {
  // 简单检查用户是否已登录
  const user = (() => {
    try {
      const stored = localStorage.getItem('gm-user')
      return stored ? JSON.parse(stored) : null
    } catch {
      return null
    }
  })()

  if (!user) {
    return <Navigate to="/login" replace />
  }

  return (
    <AppShell>
      {children}
    </AppShell>
  )
}

export function Routes() {
  return (
    <RouterRoutes>
      <Route path="/login" element={<Login />} />
      <Route
        path="/dashboard"
        element={
          <ProtectedRoute>
            <Dashboard />
          </ProtectedRoute>
        }
      />
      <Route
        path="/player/profile/:id"
        element={
          <ProtectedRoute>
            <PlayerProfile />
          </ProtectedRoute>
        }
      />
      <Route
        path="/player/profile"
        element={
          <ProtectedRoute>
            <PlayerProfile />
          </ProtectedRoute>
        }
      />
      <Route
        path="/player/inventory"
        element={
          <ProtectedRoute>
            <PlayerInventory />
          </ProtectedRoute>
        }
      />
      <Route
        path="/player/cars"
        element={
          <ProtectedRoute>
            <PlayerCars />
          </ProtectedRoute>
        }
      />
      <Route
        path="/player/vip"
        element={
          <ProtectedRoute>
            <PlayerVip />
          </ProtectedRoute>
        }
      />
      <Route
        path="/player/signin"
        element={
          <ProtectedRoute>
            <PlayerSignIn />
          </ProtectedRoute>
        }
      />
      <Route
        path="/items"
        element={
          <ProtectedRoute>
            <Items />
          </ProtectedRoute>
        }
      />
      <Route
        path="/send-item"
        element={
          <ProtectedRoute>
            <SendItem />
          </ProtectedRoute>
        }
      />
      <Route
        path="/permissions"
        element={
          <ProtectedRoute>
            <Permissions />
          </ProtectedRoute>
        }
      />
      <Route
        path="/s/:name"
        element={
          <ProtectedRoute>
            <SchemaPage />
          </ProtectedRoute>
        }
      />
      <Route path="/" element={<Navigate to="/login" replace />} />
    </RouterRoutes>
  )
}