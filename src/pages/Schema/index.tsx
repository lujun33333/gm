import { useState, useEffect } from 'react'
import { useParams, useNavigate } from 'react-router-dom'
import { motion } from 'framer-motion'
import { GlowCard } from '@/components/fx/GlowCard'
import { GradientText } from '@/components/fx/GradientText'
import { SchemaRenderer } from '@/utils/schema-runtime'
import { useToast } from '@/hooks/useToast'
import { ArrowLeft } from 'lucide-react'

interface SchemaPageData {
  title: string
  description?: string
  schema: any
  data?: any
}

export function SchemaPage() {
  const { name } = useParams<{ name: string }>()
  const navigate = useNavigate()
  const { toast } = useToast()
  const [pageData, setPageData] = useState<SchemaPageData | null>(null)
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    loadSchemaPage()
  }, [name])

  const loadSchemaPage = async () => {
    if (!name) {
      setError('Schema 名称无效')
      setLoading(false)
      return
    }

    try {
      setLoading(true)
      setError(null)

      // 尝试加载 schema 文件
      const schemaResponse = await fetch(`/src/schema/${name}.json`)

      if (!schemaResponse.ok) {
        throw new Error(`Schema 文件 ${name}.json 不存在`)
      }

      const schema = await schemaResponse.json()

      setPageData({
        title: schema.title || name,
        description: schema.description,
        schema,
        data: {} // 初始数据为空
      })
    } catch (err) {
      console.error('加载 Schema 失败:', err)
      setError(err instanceof Error ? err.message : '加载失败')
    } finally {
      setLoading(false)
    }
  }

  const handleBack = () => {
    navigate(-1)
  }

  // Loading 状态
  if (loading) {
    return (
      <div className="min-h-screen p-6">
        <div className="max-w-4xl mx-auto space-y-6">
          <div className="flex items-center gap-4 mb-8">
            <div className="w-8 h-8 bg-bg-secondary rounded animate-pulse" />
            <div className="h-8 bg-bg-secondary rounded w-48 animate-pulse" />
          </div>
          <div className="h-96 bg-bg-secondary rounded-lg animate-pulse" />
        </div>
      </div>
    )
  }

  // Error 状态
  if (error) {
    return (
      <div className="min-h-screen p-6">
        <div className="max-w-4xl mx-auto">
          <div className="flex items-center gap-4 mb-8">
            <button
              onClick={handleBack}
              className="p-2 text-text-secondary hover:text-text transition-colors"
            >
              <ArrowLeft className="w-5 h-5" />
            </button>
            <GradientText className="text-2xl font-bold">
              Schema 页面
            </GradientText>
          </div>

          <GlowCard className="p-8 text-center">
            <div className="text-6xl mb-4">⚠️</div>
            <h2 className="text-xl font-semibold text-error mb-2">
              加载失败
            </h2>
            <p className="text-text-muted mb-6">{error}</p>
            <button
              onClick={loadSchemaPage}
              className="px-4 py-2 bg-primary text-white rounded-lg hover:bg-primary/90 transition-colors"
            >
              重试
            </button>
          </GlowCard>
        </div>
      </div>
    )
  }

  // 正常渲染
  if (!pageData) {
    return null
  }

  return (
    <div className="min-h-screen p-6">
      <div className="max-w-4xl mx-auto">
        {/* 页面头部 */}
        <motion.div
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.4 }}
          className="flex items-center gap-4 mb-8"
        >
          <button
            onClick={handleBack}
            className="p-2 text-text-secondary hover:text-text transition-colors"
          >
            <ArrowLeft className="w-5 h-5" />
          </button>
          <div>
            <GradientText className="text-2xl font-bold">
              {pageData.title}
            </GradientText>
            {pageData.description && (
              <p className="text-text-muted mt-1">
                {pageData.description}
              </p>
            )}
          </div>
        </motion.div>

        {/* Schema 渲染区域 */}
        <motion.div
          initial={{ opacity: 0, y: 20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.4, delay: 0.1 }}
        >
          <SchemaRenderer
            schema={pageData.schema}
            data={pageData.data}
            onSubmit={async (formData) => {
              // 处理表单提交
              try {
                console.log('Schema 表单提交:', formData)
                toast({
                  title: '操作成功',
                  description: '数据已成功提交'
                })
              } catch (error) {
                toast({
                  title: '操作失败',
                  description: '提交数据时发生错误',
                  variant: 'destructive'
                })
              }
            }}
            onError={(error) => {
              toast({
                title: '渲染错误',
                description: error,
                variant: 'destructive'
              })
            }}
          />
        </motion.div>
      </div>
    </div>
  )
}