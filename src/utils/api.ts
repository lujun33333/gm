import { mockApi } from './mock'

export interface ApiResponse<T = any> {
  code: number
  msg?: string
  data?: T
}

export interface ApiRequestConfig {
  method?: 'GET' | 'POST' | 'PUT' | 'PATCH' | 'DELETE'
  headers?: Record<string, string>
  body?: any
  timeout?: number
}

class ApiClient {
  private baseURL: string
  private mockMode: boolean

  constructor(baseURL = '/api', mockMode = true) {
    this.baseURL = baseURL
    this.mockMode = mockMode
  }

  async request<T = any>(
    endpoint: string,
    config: ApiRequestConfig = {}
  ): Promise<ApiResponse<T>> {
    const {
      method = 'GET',
      headers = {},
      body,
      timeout = 10000
    } = config

    // Mock 模式
    if (this.mockMode) {
      return mockApi.request(endpoint, config)
    }

    // 真实 API 调用
    const url = `${this.baseURL}${endpoint}`

    const requestHeaders: HeadersInit = {
      'Content-Type': 'application/json',
      ...headers
    }

    // 添加认证 token
    const auth = localStorage.getItem('gm-auth')
    if (auth) {
      const userData = JSON.parse(auth)
      requestHeaders['Authorization'] = `Bearer ${userData.token || 'mock-token'}`
    }

    const controller = new AbortController()
    const timeoutId = setTimeout(() => controller.abort(), timeout)

    try {
      const response = await fetch(url, {
        method,
        headers: requestHeaders,
        body: body ? JSON.stringify(body) : undefined,
        signal: controller.signal
      })

      clearTimeout(timeoutId)

      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`)
      }

      const result = await response.json()
      return result
    } catch (error) {
      clearTimeout(timeoutId)

      if (error instanceof Error) {
        if (error.name === 'AbortError') {
          throw new Error('请求超时')
        }
        throw error
      }

      throw new Error('网络请求失败')
    }
  }

  // 便捷方法
  get<T = any>(endpoint: string, config?: Omit<ApiRequestConfig, 'method'>) {
    return this.request<T>(endpoint, { ...config, method: 'GET' })
  }

  post<T = any>(endpoint: string, data?: any, config?: Omit<ApiRequestConfig, 'method' | 'body'>) {
    return this.request<T>(endpoint, { ...config, method: 'POST', body: data })
  }

  put<T = any>(endpoint: string, data?: any, config?: Omit<ApiRequestConfig, 'method' | 'body'>) {
    return this.request<T>(endpoint, { ...config, method: 'PUT', body: data })
  }

  patch<T = any>(endpoint: string, data?: any, config?: Omit<ApiRequestConfig, 'method' | 'body'>) {
    return this.request<T>(endpoint, { ...config, method: 'PATCH', body: data })
  }

  delete<T = any>(endpoint: string, config?: Omit<ApiRequestConfig, 'method'>) {
    return this.request<T>(endpoint, { ...config, method: 'DELETE' })
  }

  // 设置 Mock 模式
  setMockMode(enabled: boolean) {
    this.mockMode = enabled
  }

  // 获取当前模式
  isMockMode() {
    return this.mockMode
  }
}

// 导出单例实例
export const api = new ApiClient()

// 全局错误处理
let globalErrorHandler: ((error: any) => void) | null = null

export function setGlobalErrorHandler(handler: (error: any) => void) {
  globalErrorHandler = handler
}

// 错误处理 Hook
export function useApiError() {
  const handleError = (error: any) => {
    console.error('API Error:', error)

    // 调用全局错误处理器
    if (globalErrorHandler) {
      globalErrorHandler(error)
    }

    // 这里可以添加全局错误处理逻辑
    // 比如显示 toast、记录日志等
    if (error.message) {
      // 可以集成 toast 库
      console.warn('API Error:', error.message)
    }
  }

  return { handleError }
}