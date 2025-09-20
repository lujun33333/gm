import { useState, ReactNode } from 'react'
import { motion, AnimatePresence } from 'framer-motion'
import { ShimmerButton } from '../fx/ShimmerButton'

interface ConfirmDialogProps {
  isOpen: boolean
  onClose: () => void
  onConfirm: () => void | Promise<void>
  title?: string
  content?: ReactNode
  requireKeyword?: string
  confirmText?: string
  cancelText?: string
  variant?: 'warning' | 'error' | 'info'
}

export function ConfirmDialog({
  isOpen,
  onClose,
  onConfirm,
  title = '确认操作',
  content,
  requireKeyword,
  confirmText = '确认',
  cancelText = '取消',
  variant = 'warning'
}: ConfirmDialogProps) {
  const [keyword, setKeyword] = useState('')
  const [isLoading, setIsLoading] = useState(false)

  const canConfirm = requireKeyword ? keyword === requireKeyword : true

  const handleConfirm = async () => {
    if (!canConfirm || isLoading) return

    try {
      setIsLoading(true)
      await onConfirm()
      onClose()
      setKeyword('')
    } catch (error) {
      console.error('确认操作失败:', error)
    } finally {
      setIsLoading(false)
    }
  }

  const handleClose = () => {
    if (isLoading) return
    onClose()
    setKeyword('')
  }

  const variantStyles = {
    warning: 'border-warning/50 bg-warning/5',
    error: 'border-error/50 bg-error/5',
    info: 'border-primary/50 bg-primary/5'
  }

  return (
    <AnimatePresence>
      {isOpen && (
        <motion.div
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          exit={{ opacity: 0 }}
          className="fixed inset-0 bg-black/50 backdrop-blur-sm flex items-center justify-center z-50 p-4"
          onClick={handleClose}
        >
          <motion.div
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
            exit={{ opacity: 0, scale: 0.9 }}
            transition={{ type: 'spring', duration: 0.3 }}
            className={`glass-card rounded-lg p-6 w-full max-w-md border-2 ${variantStyles[variant]}`}
            onClick={(e) => e.stopPropagation()}
          >
            {/* 标题 */}
            <div className="mb-4">
              <h3 className="text-lg font-semibold text-text flex items-center gap-2">
                {variant === 'warning' && <span className="text-warning text-xl">⚠️</span>}
                {variant === 'error' && <span className="text-error text-xl">🚨</span>}
                {variant === 'info' && <span className="text-primary text-xl">ℹ️</span>}
                {title}
              </h3>
            </div>

            {/* 内容 */}
            {content && (
              <div className="mb-6 text-text-secondary">
                {content}
              </div>
            )}

            {/* 关键词输入 */}
            {requireKeyword && (
              <div className="mb-6">
                <label className="block text-sm font-medium text-text-secondary mb-2">
                  请输入 <code className="bg-bg-tertiary px-1 py-0.5 rounded text-error font-bold">{requireKeyword}</code> 确认操作
                </label>
                <input
                  type="text"
                  value={keyword}
                  onChange={(e) => setKeyword(e.target.value)}
                  className="w-full px-3 py-2 bg-bg-secondary border border-border rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent transition-colors"
                  placeholder={`输入 ${requireKeyword}`}
                  disabled={isLoading}
                />
              </div>
            )}

            {/* 按钮 */}
            <div className="flex gap-3">
              <motion.button
                whileHover={{ scale: 1.02 }}
                whileTap={{ scale: 0.98 }}
                onClick={handleClose}
                disabled={isLoading}
                className="flex-1 px-4 py-2 bg-bg-secondary border border-border rounded-lg hover:bg-bg-tertiary transition-colors disabled:opacity-50"
              >
                {cancelText}
              </motion.button>

              <ShimmerButton
                onClick={handleConfirm}
                disabled={!canConfirm}
                loading={isLoading}
                variant={variant === 'error' ? 'error' : 'warning'}
                className="flex-1"
              >
                {confirmText}
              </ShimmerButton>
            </div>
          </motion.div>
        </motion.div>
      )}
    </AnimatePresence>
  )
}

// Hook 用法
export function useConfirmDialog() {
  const [isOpen, setIsOpen] = useState(false)
  const [config, setConfig] = useState<Partial<ConfirmDialogProps>>({})

  const openDialog = (dialogConfig: Partial<ConfirmDialogProps>) => {
    setConfig(dialogConfig)
    setIsOpen(true)
  }

  const closeDialog = () => {
    setIsOpen(false)
  }

  const ConfirmDialogComponent = () => (
    <ConfirmDialog
      isOpen={isOpen}
      onClose={closeDialog}
      onConfirm={config.onConfirm || (() => {})}
      {...config}
    />
  )

  return {
    openDialog,
    closeDialog,
    ConfirmDialog: ConfirmDialogComponent
  }
}