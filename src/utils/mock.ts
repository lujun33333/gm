import { ApiResponse, ApiRequestConfig } from './api'

// Mock 数据类型定义
export interface Player {
  uin: string
  nickname: string
  gender: '男' | '女' | '未知'
  country: string
  exp: number
  cash_a: number
  cash_b: number
  honor: number
  charm: number
  dance: number
  sign: string
  created_at: string
  horn: number
  sec_pwd: string
  sec_pwd_status: '启用' | '禁用'
  peak_status: '参与' | '禁赛'
  status: 0 | 1 // 0: 正常, 1: 封禁
}

export interface InventoryItem {
  id: string
  uin: string
  item_id: string
  item_name: string
  count: number
  days: number
  status: '绑定' | '未绑定' | '过期'
  created_at: string
  item_type: string
  quality: 'common' | 'rare' | 'epic' | 'legendary'
}

export interface PlayerCar {
  id: string
  car_id: string
  car_name: string
  car_type: string
  level: number
  experience: number
  max_experience: number
  speed: number
  acceleration: number
  handling: number
  stability: number
  color: string
  is_favorite: boolean
  acquired_at: string
}

export interface PlayerVip {
  uin: string
  nickname: string
  current_level: number
  points: number
  expire_time?: string
  total_consumed: number
  level_history: Array<{
    level: number
    start_time: string
    end_time?: string
    consumed: number
  }>
}

export interface GameItem {
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

export interface GMUser {
  id: string
  username: string
  email: string
  role: string
  permissions: string[]
  status: 'active' | 'disabled'
  created_at: string
  last_login?: string
}

export interface SignInConfig {
  id: string
  name: string
  description: string
  rules: Array<{
    day: number
    type: 'exp' | 'cash_a' | 'cash_b' | 'item'
    amount: number
    item_id?: string
    item_name?: string
    is_special?: boolean
  }>
  is_active: boolean
  start_date: string
  end_date?: string
  created_at: string
  updated_at: string
}

export interface PlayerSignIn {
  uin: string
  nickname: string
  current_month: string
  signed_days: number[]
  last_sign_date?: string
  consecutive_days: number
  total_rewards: {
    exp: number
    cash_a: number
    cash_b: number
    items: number
  }
}

export interface DashboardData {
  online_total: number
  online_series: Array<{ time: string; count: number }>
  cpu: number
  mem: number
  uptime: string
  net_total_in: number
  net_total_out: number
  net_series: Array<{ time: string; in: number; out: number }>
  player_top_bandwidth: Array<{ uin: string; nickname: string; bps: number }>
}

class MockAPI {
  private idempotencyCache = new Map<string, any>()

  // 模拟网络延迟
  private async delay(ms = 300 + Math.random() * 700) {
    await new Promise(resolve => setTimeout(resolve, ms))
  }

  // 幂等性处理
  private handleIdempotency(key: string, data: any) {
    if (this.idempotencyCache.has(key)) {
      return this.idempotencyCache.get(key)
    }
    this.idempotencyCache.set(key, data)
    return data
  }

  async request<T = any>(
    endpoint: string,
    config: ApiRequestConfig = {}
  ): Promise<ApiResponse<T>> {
    await this.delay()

    const { method = 'GET', body, headers = {} } = config

    // 幂等性检查
    const idempotencyKey = headers['x-idempotency-key']
    if (idempotencyKey && method === 'POST') {
      const cached = this.idempotencyCache.get(idempotencyKey)
      if (cached) {
        return cached
      }
    }

    try {
      const result = await this.handleRequest(endpoint, method, body)

      // 保存幂等性结果
      if (idempotencyKey && method === 'POST') {
        this.handleIdempotency(idempotencyKey, result)
      }

      return result
    } catch (error) {
      // 模拟一些错误情况
      return {
        code: 1,
        msg: error instanceof Error ? error.message : '未知错误'
      }
    }
  }

  private async handleRequest(endpoint: string, method: string, body?: any): Promise<ApiResponse> {
    // 路由处理
    if (endpoint.startsWith('/auth/login')) {
      return this.handleLogin(body)
    }

    if (endpoint.startsWith('/auth/me')) {
      return this.handleMe()
    }

    if (endpoint.startsWith('/dashboard')) {
      return this.handleDashboard()
    }

    if (endpoint.startsWith('/player/')) {
      return this.handlePlayer(endpoint, method, body)
    }

    if (endpoint.startsWith('/items')) {
      return this.handleItems(endpoint, method, body)
    }

    if (endpoint.startsWith('/send-item')) {
      return this.handleSendItem(body)
    }

    if (endpoint.startsWith('/permissions')) {
      return this.handlePermissions(endpoint, method, body)
    }

    if (endpoint.startsWith('/signin')) {
      return this.handleSignIn(endpoint, method, body)
    }

    if (endpoint.startsWith('/form/submit')) {
      return this.handleFormSubmit(body)
    }

    if (endpoint.startsWith('/server/restart')) {
      return this.handleServerRestart(body)
    }

    if (endpoint.startsWith('/music/')) {
      return this.handleMusic(endpoint, method, body)
    }

    // 默认响应
    return {
      code: 0,
      msg: 'success',
      data: null
    }
  }

  private handleLogin(body: any): ApiResponse {
    const { username, password } = body || {}

    if (!username) {
      return { code: 1, msg: '用户名不能为空' }
    }

    // 模拟用户验证
    const users = ['admin', 'manager', 'player']
    if (!users.includes(username)) {
      return { code: 1, msg: '用户名或密码错误' }
    }

    return {
      code: 0,
      msg: '登录成功',
      data: {
        token: `mock-token-${username}-${Date.now()}`,
        user: username
      }
    }
  }

  private handleMe(): ApiResponse {
    const auth = localStorage.getItem('gm-auth')
    if (!auth) {
      return { code: 401, msg: '未授权' }
    }

    const userData = JSON.parse(auth)
    return {
      code: 0,
      data: userData
    }
  }

  private handleDashboard(): ApiResponse<DashboardData> {
    const now = new Date()
    const hours = Array.from({ length: 24 }, (_, i) => {
      const time = new Date(now.getTime() - (23 - i) * 60 * 60 * 1000)
      return time.toISOString().slice(11, 16)
    })

    return {
      code: 0,
      data: {
        online_total: Math.floor(Math.random() * 2000) + 500,
        online_series: hours.map(time => ({
          time,
          count: Math.floor(Math.random() * 1000) + 200
        })),
        cpu: Math.floor(Math.random() * 30) + 10,
        mem: Math.floor(Math.random() * 40) + 30,
        uptime: '72天 14小时 23分钟',
        net_total_in: Math.floor(Math.random() * 1000000),
        net_total_out: Math.floor(Math.random() * 800000),
        net_series: hours.slice(-12).map(time => ({
          time,
          in: Math.floor(Math.random() * 1000),
          out: Math.floor(Math.random() * 800)
        })),
        player_top_bandwidth: [
          { uin: '10001', nickname: '速度之王', bps: 1024000 },
          { uin: '10002', nickname: '网络达人', bps: 896000 },
          { uin: '10003', nickname: '下载狂魔', bps: 743000 },
          { uin: '10004', nickname: '流量大户', bps: 652000 },
          { uin: '10005', nickname: '宽带用户', bps: 589000 },
        ]
      }
    }
  }

  private handlePlayer(endpoint: string, method: string, body?: any): ApiResponse {
    const match = endpoint.match(/\/player\/(\d+)/)
    const playerId = match?.[1]

    // 玩家基本信息
    if (method === 'GET' && playerId && !endpoint.includes('/')) {
      return {
        code: 0,
        data: this.generateMockPlayer(playerId)
      }
    }

    // 玩家资料更新
    if (method === 'POST' && endpoint.includes('/profile')) {
      return {
        code: 0,
        msg: '玩家资料更新成功',
        data: { ...body, updated_at: new Date().toISOString() }
      }
    }

    // 玩家背包
    if (endpoint.includes('/inventory')) {
      return {
        code: 0,
        data: {
          items: this.generateMockInventory(playerId || '10001'),
          total: 156,
          page: 1,
          size: 20
        }
      }
    }

    // 玩家赛车
    if (endpoint.includes('/cars')) {
      if (method === 'GET') {
        return {
          code: 0,
          data: {
            cars: this.generateMockCars(playerId || '10001'),
            total: 25
          }
        }
      }
      if (method === 'PUT' || method === 'POST') {
        return {
          code: 0,
          msg: '赛车信息更新成功',
          data: body
        }
      }
      if (method === 'DELETE') {
        return {
          code: 0,
          msg: '赛车删除成功'
        }
      }
    }

    // 玩家贵族
    if (endpoint.includes('/vip')) {
      if (method === 'GET') {
        return {
          code: 0,
          data: this.generateMockVip(playerId || '10001')
        }
      }
      if (method === 'POST') {
        return {
          code: 0,
          msg: '贵族等级设置成功',
          data: body
        }
      }
      if (method === 'DELETE') {
        return {
          code: 0,
          msg: '贵族等级取消成功'
        }
      }
    }

    // 玩家签到
    if (endpoint.includes('/signin')) {
      if (method === 'GET') {
        return {
          code: 0,
          data: this.generateMockSignIn(playerId || '10001')
        }
      }
      if (method === 'POST') {
        return {
          code: 0,
          msg: '签到成功',
          data: body
        }
      }
      if (method === 'DELETE') {
        return {
          code: 0,
          msg: '签到数据重置成功'
        }
      }
    }

    // 玩家封禁/解封
    if (endpoint.includes('/ban')) {
      return {
        code: 0,
        msg: method === 'POST' ? '玩家封禁成功' : '玩家解封成功'
      }
    }

    if (endpoint.includes('/unban')) {
      return {
        code: 0,
        msg: '玩家解封成功'
      }
    }

    // 重置二级密码
    if (endpoint.includes('/reset-sec-pwd')) {
      return {
        code: 0,
        msg: '二级密码重置成功'
      }
    }

    return { code: 0, data: null }
  }

  private handleItems(endpoint: string, method: string, body?: any): ApiResponse {
    if (method === 'GET') {
      return {
        code: 0,
        data: {
          items: this.generateMockGameItems(),
          total: 523,
          page: 1,
          size: 20
        }
      }
    }

    if (method === 'POST') {
      return {
        code: 0,
        msg: '道具创建成功',
        data: { ...body, id: `item_${Date.now()}`, created_at: new Date().toISOString() }
      }
    }

    if (method === 'PUT') {
      return {
        code: 0,
        msg: '道具更新成功',
        data: body
      }
    }

    if (method === 'DELETE') {
      return {
        code: 0,
        msg: '道具删除成功'
      }
    }

    if (method === 'PATCH') {
      return {
        code: 0,
        msg: '道具状态更新成功',
        data: body
      }
    }

    return { code: 0, msg: '操作成功' }
  }

  private handleSendItem(body: any): ApiResponse {
    const { uin, item_id, count, days, bound, mail } = body || {}

    if (!uin || !item_id || !count) {
      return { code: 1, msg: '参数不完整' }
    }

    return {
      code: 0,
      msg: '道具发放成功',
      data: {
        id: `send_${Date.now()}`,
        uin,
        item_id,
        count,
        days: days || 0,
        bound: bound || false,
        mail: mail || '',
        created_at: new Date().toISOString()
      }
    }
  }

  private handlePermissions(endpoint: string, method: string, body?: any): ApiResponse {
    if (endpoint.includes('/users')) {
      if (method === 'GET') {
        return {
          code: 0,
          data: {
            users: this.generateMockUsers()
          }
        }
      }
      if (method === 'PUT') {
        return {
          code: 0,
          msg: '用户权限更新成功',
          data: body
        }
      }
      if (method === 'PATCH') {
        return {
          code: 0,
          msg: '用户状态更新成功',
          data: body
        }
      }
    }

    return { code: 0, msg: '权限操作成功' }
  }

  private handleSignIn(endpoint: string, method: string, body?: any): ApiResponse {
    if (endpoint.includes('/configs')) {
      if (method === 'GET') {
        return {
          code: 0,
          data: {
            configs: this.generateMockSignInConfigs()
          }
        }
      }
      if (method === 'POST') {
        return {
          code: 0,
          msg: '签到配置创建成功',
          data: { ...body, id: `config_${Date.now()}`, created_at: new Date().toISOString() }
        }
      }
      if (method === 'PUT') {
        return {
          code: 0,
          msg: '签到配置更新成功',
          data: body
        }
      }
      if (endpoint.includes('/activate')) {
        return {
          code: 0,
          msg: '签到配置激活成功'
        }
      }
    }

    return { code: 0, msg: '签到操作成功' }
  }

  private handleFormSubmit(body: any): ApiResponse {
    const { formId, ...formData } = body || {}

    if (!formId) {
      return { code: 1, msg: '表单ID不能为空' }
    }

    switch (formId) {
      case 'send_item':
        return this.handleSendItem(formData)

      case 'player_profile':
        return {
          code: 0,
          msg: '玩家资料更新成功',
          data: { ...formData, updated_at: new Date().toISOString() }
        }

      default:
        return {
          code: 0,
          msg: '表单提交成功',
          data: { formId, ...formData, submitted_at: new Date().toISOString() }
        }
    }
  }

  private handleServerRestart(body: any): ApiResponse {
    return {
      code: 0,
      msg: '服务器重启指令已发送',
      data: {
        operation: 'restart',
        initiated_at: new Date().toISOString(),
        estimated_downtime: '30秒'
      }
    }
  }

  private handleMusic(endpoint: string, method: string, body?: any): ApiResponse {
    // 音乐搜索
    if (endpoint.includes('/search')) {
      const { keyword, limit = 20, platform } = body || {}
      if (!keyword) {
        return { code: 1, msg: '搜索关键词不能为空' }
      }

      return {
        code: 0,
        data: {
          songs: this.generateMockSearchResults(keyword, platform, limit)
        }
      }
    }

    // 获取播放URL
    if (endpoint.includes('/url')) {
      const { songId, platform, quality } = body || {}
      if (!songId) {
        return { code: 1, msg: '歌曲ID不能为空' }
      }

      // 模拟不同平台的播放URL
      const baseUrls = {
        netease: 'https://music.163.com/song/media/outer/url',
        qq: 'https://stream.qqmusic.qq.com',
        local: '/audio'
      }

      const baseUrl = baseUrls[platform as keyof typeof baseUrls] || baseUrls.local
      const mockUrl = `${baseUrl}/${songId}.mp3?quality=${quality}`

      return {
        code: 0,
        data: {
          url: mockUrl
        }
      }
    }

    // 获取歌词
    if (endpoint.includes('/lyric')) {
      const { songId, platform } = body || {}
      if (!songId) {
        return { code: 1, msg: '歌曲ID不能为空' }
      }

      return {
        code: 0,
        data: {
          lyric: this.generateMockLyric(songId)
        }
      }
    }

    // 获取推荐
    if (endpoint.includes('/recommend')) {
      const { limit = 10, platform } = body || {}

      return {
        code: 0,
        data: {
          songs: this.generateMockRecommendations(platform, limit)
        }
      }
    }

    // 验证API密钥
    if (endpoint.includes('/validate')) {
      const { platform, apiKey } = body || {}

      if (platform === 'local') {
        return { code: 0, msg: '本地音乐无需验证' }
      }

      if (!apiKey) {
        return { code: 1, msg: 'API密钥不能为空' }
      }

      // 模拟验证逻辑
      const validKeys = {
        netease: ['test_netease_key_123456', 'demo_key_abcdef'],
        qq: ['test_qq_key_789012', 'demo_qq_key_ghijkl']
      }

      const platformKeys = validKeys[platform as keyof typeof validKeys]
      if (platformKeys && platformKeys.includes(apiKey)) {
        return { code: 0, msg: 'API密钥验证成功' }
      }

      return { code: 1, msg: 'API密钥验证失败' }
    }

    return { code: 0, msg: '音乐操作成功' }
  }

  // Mock数据生成方法
  private generateMockPlayer(uin: string): Player {
    return {
      uin,
      nickname: `玩家${uin}`,
      gender: ['男', '女', '未知'][Math.floor(Math.random() * 3)] as '男' | '女' | '未知',
      country: '中国',
      exp: Math.floor(Math.random() * 100000),
      cash_a: Math.floor(Math.random() * 50000),
      cash_b: Math.floor(Math.random() * 20000),
      honor: Math.floor(Math.random() * 10000),
      charm: Math.floor(Math.random() * 5000),
      dance: Math.floor(Math.random() * 8000),
      sign: '这是我的个性签名',
      created_at: '2023-01-15 10:30:00',
      horn: Math.floor(Math.random() * 100),
      sec_pwd: '******',
      sec_pwd_status: Math.random() > 0.5 ? '启用' : '禁用',
      peak_status: Math.random() > 0.8 ? '禁赛' : '参与',
      status: Math.random() > 0.9 ? 1 : 0
    }
  }

  private generateMockInventory(uin: string): InventoryItem[] {
    const items = []
    const itemTypes = ['武器', '防具', '道具', '消耗品', '材料']
    const qualities: ('common' | 'rare' | 'epic' | 'legendary')[] = ['common', 'rare', 'epic', 'legendary']
    const statusList = ['绑定', '未绑定', '过期'] as const

    for (let i = 1; i <= 20; i++) {
      items.push({
        id: `inv_${uin}_${i}`,
        uin,
        item_id: `item_${1000 + i}`,
        item_name: `道具${i}`,
        count: Math.floor(Math.random() * 99) + 1,
        days: Math.floor(Math.random() * 365),
        status: statusList[Math.floor(Math.random() * statusList.length)],
        created_at: new Date(Date.now() - Math.random() * 365 * 24 * 60 * 60 * 1000).toISOString(),
        item_type: itemTypes[Math.floor(Math.random() * itemTypes.length)],
        quality: qualities[Math.floor(Math.random() * qualities.length)]
      })
    }

    return items
  }

  private generateMockCars(uin: string): PlayerCar[] {
    const cars = []
    const carTypes = ['S', 'A', 'B', 'C', 'D']
    const carNames = ['疾风赛车', '雷电跑车', '火焰战车', '冰霜赛车', '光速飞车']
    const colors = ['#FF0000', '#00FF00', '#0000FF', '#FFFF00', '#FF00FF']

    for (let i = 1; i <= 12; i++) {
      const level = Math.floor(Math.random() * 15) + 1
      const maxExp = level * 1000
      cars.push({
        id: `car_${uin}_${i}`,
        car_id: `car_${1000 + i}`,
        car_name: carNames[Math.floor(Math.random() * carNames.length)] + i,
        car_type: carTypes[Math.floor(Math.random() * carTypes.length)],
        level,
        experience: Math.floor(Math.random() * maxExp),
        max_experience: maxExp,
        speed: 80 + Math.floor(Math.random() * 40),
        acceleration: 70 + Math.floor(Math.random() * 50),
        handling: 60 + Math.floor(Math.random() * 40),
        stability: 75 + Math.floor(Math.random() * 35),
        color: colors[Math.floor(Math.random() * colors.length)],
        is_favorite: Math.random() > 0.7,
        acquired_at: new Date(Date.now() - Math.random() * 365 * 24 * 60 * 60 * 1000).toISOString().split('T')[0]
      })
    }

    return cars
  }

  private generateMockVip(uin: string): PlayerVip {
    const level = Math.floor(Math.random() * 6)
    const points = Math.floor(Math.random() * 10000)
    const expireTime = level > 0 ? new Date(Date.now() + Math.random() * 90 * 24 * 60 * 60 * 1000).toISOString() : undefined

    return {
      uin,
      nickname: `玩家${uin}`,
      current_level: level,
      points,
      expire_time: expireTime,
      total_consumed: Math.floor(Math.random() * 5000),
      level_history: [
        {
          level: 1,
          start_time: '2023-01-01 00:00:00',
          end_time: '2023-02-01 00:00:00',
          consumed: 100
        },
        {
          level: 2,
          start_time: '2023-02-01 00:00:00',
          end_time: '2023-03-01 00:00:00',
          consumed: 300
        },
        {
          level: level,
          start_time: '2023-03-01 00:00:00',
          consumed: Math.floor(Math.random() * 1000)
        }
      ]
    }
  }

  private generateMockSignIn(uin: string): PlayerSignIn {
    const today = new Date().getDate()
    const signedDays = Array.from({ length: Math.floor(Math.random() * today) }, (_, i) => i + 1)

    return {
      uin,
      nickname: `玩家${uin}`,
      current_month: new Date().toISOString().slice(0, 7),
      signed_days: signedDays,
      last_sign_date: signedDays.length > 0 ? `${new Date().toISOString().split('T')[0]} 09:30:00` : undefined,
      consecutive_days: Math.floor(Math.random() * 7) + 1,
      total_rewards: {
        exp: Math.floor(Math.random() * 50000),
        cash_a: Math.floor(Math.random() * 5000),
        cash_b: Math.floor(Math.random() * 3000),
        items: Math.floor(Math.random() * 20)
      }
    }
  }

  private generateMockGameItems(): GameItem[] {
    const items = []
    const categories = ['武器', '防具', '道具', '消耗品', '材料', '装饰']
    const types = ['近战武器', '远程武器', '头盔', '护甲', '恢复道具', '强化道具']
    const rarities: ('common' | 'rare' | 'epic' | 'legendary')[] = ['common', 'rare', 'epic', 'legendary']

    for (let i = 1; i <= 30; i++) {
      const category = categories[Math.floor(Math.random() * categories.length)]
      const rarity = rarities[Math.floor(Math.random() * rarities.length)]

      items.push({
        id: `item_${1000 + i}`,
        name: `${category}${i}`,
        type: types[Math.floor(Math.random() * types.length)],
        category,
        description: `这是一个${rarity}品质的${category}，具有特殊属性。`,
        price: Math.floor(Math.random() * 10000) + 100,
        max_stack: Math.floor(Math.random() * 100) + 1,
        tradeable: Math.random() > 0.3,
        sellable: Math.random() > 0.2,
        icon_url: `https://api.dicebear.com/7.x/shapes/svg?seed=item${i}`,
        rarity,
        status: Math.random() > 0.1 ? 'active' : 'disabled',
        created_at: new Date(Date.now() - Math.random() * 365 * 24 * 60 * 60 * 1000).toISOString(),
        updated_at: new Date(Date.now() - Math.random() * 30 * 24 * 60 * 60 * 1000).toISOString()
      })
    }

    return items
  }

  private generateMockUsers(): GMUser[] {
    const roles = ['OWNER', 'SUB', 'PLAYER']
    const permissions = {
      OWNER: ['MONITOR_VIEW', 'SERVER_CONTROL', 'PLAYER_VIEW', 'PLAYER_EDIT', 'PLAYER_BAN', 'INVENTORY_VIEW', 'ITEM_SEND', 'ITEM_ADD', 'ITEM_EDIT', 'ITEM_DELETE', 'CAR_EDIT', 'NOBLE_EDIT', 'SIGNIN_EDIT', 'PERM_ADMIN'],
      SUB: ['MONITOR_VIEW', 'PLAYER_VIEW', 'PLAYER_EDIT', 'INVENTORY_VIEW', 'ITEM_SEND', 'ITEM_ADD', 'CAR_EDIT', 'NOBLE_EDIT'],
      PLAYER: ['PLAYER_VIEW', 'INVENTORY_VIEW']
    }

    return [
      {
        id: '1',
        username: 'admin',
        email: 'admin@example.com',
        role: 'OWNER',
        permissions: permissions.OWNER,
        status: 'active',
        created_at: '2023-01-01 00:00:00',
        last_login: '2024-01-15 14:30:00'
      },
      {
        id: '2',
        username: 'manager',
        email: 'manager@example.com',
        role: 'SUB',
        permissions: permissions.SUB,
        status: 'active',
        created_at: '2023-02-01 00:00:00',
        last_login: '2024-01-14 16:45:00'
      },
      {
        id: '3',
        username: 'player1',
        email: 'player1@example.com',
        role: 'PLAYER',
        permissions: permissions.PLAYER,
        status: 'active',
        created_at: '2023-03-01 00:00:00',
        last_login: '2024-01-10 10:20:00'
      },
      {
        id: '4',
        username: 'test_user',
        email: 'test@example.com',
        role: 'PLAYER',
        permissions: permissions.PLAYER,
        status: 'disabled',
        created_at: '2023-04-01 00:00:00'
      }
    ]
  }

  private generateMockSignInConfigs(): SignInConfig[] {
    return [
      {
        id: 'config_1',
        name: '每日签到奖励',
        description: '标准的每日签到奖励配置',
        rules: [
          { day: 1, type: 'exp', amount: 1000 },
          { day: 2, type: 'cash_a', amount: 50 },
          { day: 3, type: 'exp', amount: 1500 },
          { day: 4, type: 'cash_b', amount: 100 },
          { day: 5, type: 'exp', amount: 2000 },
          { day: 6, type: 'item', amount: 1, item_id: '100001', item_name: '经验药水' },
          { day: 7, type: 'cash_a', amount: 200, is_special: true }
        ],
        is_active: true,
        start_date: '2024-01-01',
        created_at: '2024-01-01 00:00:00',
        updated_at: '2024-01-01 00:00:00'
      },
      {
        id: 'config_2',
        name: '新手签到奖励',
        description: '适合新手玩家的签到奖励',
        rules: [
          { day: 1, type: 'exp', amount: 500 },
          { day: 2, type: 'cash_a', amount: 30 },
          { day: 3, type: 'exp', amount: 800 },
          { day: 4, type: 'cash_b', amount: 60 },
          { day: 5, type: 'exp', amount: 1200 },
          { day: 6, type: 'item', amount: 1, item_id: '100002', item_name: '新手礼包' },
          { day: 7, type: 'cash_a', amount: 100, is_special: true }
        ],
        is_active: false,
        start_date: '2024-02-01',
        end_date: '2024-02-29',
        created_at: '2024-02-01 00:00:00',
        updated_at: '2024-02-01 00:00:00'
      }
    ]
  }

  private generateMockSearchResults(keyword: string, platform: string, limit: number): any[] {
    const artists = ['周杰伦', '邓紫棋', '林俊杰', '张学友', '王菲', '陈奕迅', '李荣浩', '毛不易', '薛之谦', '汪苏泷']
    const albumPrefixes = ['专辑', '新歌', '精选', '合集', '最新']
    const songs = []

    const platformNames = {
      netease: '网易云音乐',
      qq: 'QQ音乐',
      local: '本地音乐'
    }

    for (let i = 1; i <= Math.min(limit, 20); i++) {
      const artist = artists[Math.floor(Math.random() * artists.length)]
      const albumPrefix = albumPrefixes[Math.floor(Math.random() * albumPrefixes.length)]

      songs.push({
        id: `${platform}_${keyword}_${i}`,
        name: `${keyword}相关歌曲${i}`,
        artist: artist,
        album: `${albumPrefix}·${artist}`,
        duration: 180 + Math.floor(Math.random() * 120), // 3-5分钟
        cover: `https://via.placeholder.com/300x300/${this.getRandomColor()}/FFFFFF?text=${encodeURIComponent(keyword + i)}`,
        platform: platform
      })
    }

    return songs
  }

  private generateMockRecommendations(platform: string, limit: number): any[] {
    const recommendations = [
      { name: '今日推荐', artist: '推荐歌手1', album: '热门榜单' },
      { name: '热门金曲', artist: '流行歌手', album: '经典合集' },
      { name: '网络热歌', artist: '网络歌手', album: '网络音乐' },
      { name: '抒情慢歌', artist: '情歌王子', album: '抒情专辑' },
      { name: '动感节拍', artist: 'DJ大师', album: '电音合集' },
      { name: '古风雅韵', artist: '古风歌手', album: '古风专辑' },
      { name: '摇滚经典', artist: '摇滚乐队', album: '摇滚精选' },
      { name: '民谣时光', artist: '民谣歌手', album: '民谣集' },
      { name: '说唱新声', artist: '说唱歌手', album: 'Hip-Hop' },
      { name: '轻音乐', artist: '轻音乐团', album: '纯音乐' }
    ]

    return recommendations.slice(0, limit).map((rec, index) => ({
      id: `${platform}_rec_${index + 1}`,
      name: rec.name,
      artist: rec.artist,
      album: rec.album,
      duration: 200 + Math.floor(Math.random() * 100),
      cover: `https://via.placeholder.com/300x300/${this.getRandomColor()}/FFFFFF?text=${encodeURIComponent(rec.name)}`,
      platform: platform
    }))
  }

  private generateMockLyric(songId: string): string {
    const lyrics = [
      '[00:00.00]这是一首美妙的歌曲\n[00:05.00]让我们一起聆听\n[00:10.00]音乐的力量无穷\n[00:15.00]带来心灵的慰藉',
      '[00:00.00]歌词同步显示功能\n[00:04.00]让音乐更有感染力\n[00:08.00]每一个音符都有意义\n[00:12.00]每一句歌词都有故事',
      '[00:00.00]在这个美好的时光里\n[00:06.00]音乐伴随着我们\n[00:12.00]创造美好的回忆\n[00:18.00]让生活充满色彩'
    ]

    // 根据songId选择不同的歌词
    const index = songId.split('').reduce((acc, char) => acc + char.charCodeAt(0), 0) % lyrics.length
    return lyrics[index]
  }

  private getRandomColor(): string {
    const colors = ['FF6B6B', '4ECDC4', '45B7D1', '96CEB4', 'FECA57', 'FF9FF3', 'A8E6CF', 'FFD93D', 'C7CEEA', 'FF8B94']
    return colors[Math.floor(Math.random() * colors.length)]
  }
}

export const mockApi = new MockAPI()