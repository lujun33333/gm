import { ReactNode } from 'react'
import { HashRouter } from 'react-router-dom'
import { Toaster } from '@/components/ui/toaster'

interface ProvidersProps {
  children: ReactNode
}

export function Providers({ children }: ProvidersProps) {
  return (
    <HashRouter>
      {children}
      <Toaster />
    </HashRouter>
  )
}