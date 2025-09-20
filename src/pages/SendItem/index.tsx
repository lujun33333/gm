import { useEffect, useState } from 'react'
import { SchemaRenderer } from '../../utils/schema-runtime'
import { usePerm } from '../../hooks/usePerm'
import { useToast } from '../../hooks/useToast'
import { PageContainer } from '../../components/layout/PageContainer'
import { api } from '../../utils/api'
import sendItemSchema from '../../schema/send_item.json'

export function SendItem() {
  const { userInfo } = usePerm()
  const { toast } = useToast()
  const [context, setContext] = useState({
    user: userInfo,
    pageState: {},
    params: {}
  })

  useEffect(() => {
    setContext(prev => ({ ...prev, user: userInfo }))
  }, [userInfo])

  const handleAction = async (action: string, data: any) => {
    console.log('SendItem Action:', action, data)

    try {
      if (action === 'confirmSubmit') {
        // 调用API发送道具
        const response = await api.post('/send-item', data)

        if (response.code === 0) {
          toast({
            title: '道具发送成功',
            description: response.msg || '道具已成功发送给玩家',
            variant: 'default'
          })
          return response
        } else {
          toast({
            title: '道具发送失败',
            description: response.msg || '发送失败，请重试',
            variant: 'destructive'
          })
          throw new Error(response.msg || '发送失败')
        }
      }

      // 其他动作的默认处理
      const response = await api.post('/form/submit', { action, ...data })
      return response
    } catch (error) {
      console.error('SendItem Action Error:', error)
      toast({
        title: '操作失败',
        description: error instanceof Error ? error.message : '未知错误',
        variant: 'destructive'
      })
      throw error
    }
  }

  return (
    <PageContainer maxWidth="screen-lg" className="px-6 py-8">
      <SchemaRenderer
        schema={sendItemSchema}
        context={context}
        onAction={handleAction}
      />
    </PageContainer>
  )
}