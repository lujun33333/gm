
# GM 工具 · 完整 UI 设计规范（前端实现版）
> 目标：一次交付“炫酷、可配置、权限裁剪”的 GM 桌面端 UI（Tauri + React + TS + Tailwind）。本规范写清楚 **怎么做、怎么改**，并附 **全字段**、**接口契约**、**UI Schema 示例** 与 **验收表**。

## 0. 技术与目录（固定）
- 技术：Tauri + React18 + TypeScript + Tailwind + Framer Motion + Recharts + WebAudio（shadcn 可选）
- 返回契约：`{ code:number, msg?:string, data?:any }`
- 目录（只增不改层级名）：
```
src/
  app/{ routes.tsx, providers.tsx }
  app/theme/{ ThemeContext.tsx }
  components/
    layout/{ AppShell.tsx, Sidebar.tsx, Header.tsx, Grid.tsx }
    fx/{ GlowCard.tsx, GradientText.tsx, ParticleBackground.tsx, ShimmerButton.tsx }
    common/{ Permit.tsx, ConfirmDialog.tsx, Field.tsx }
    data/{ DataTable.tsx, Statistic.tsx, Gauge.tsx }
    media/{ AudioProvider.tsx, MiniPlayer.tsx, AudioVisualizer.tsx }
  hooks/{ useAuth.ts, usePerm.ts, useSchema.ts }
  pages/
    Login/
    Dashboard/
    Player/{ Profile/, Inventory/, Cars/, Vip/, SignIn/ }
    Items/
    SendItem/
    Permissions/
    Settings/
  schema/
  utils/{ api.ts, mock.ts, schema-runtime.tsx, storage.ts, intl.ts }
  theme/{ tailwind.css, tokens.css }
```

## 1. 视觉（怎么做成“炫酷”）
- 玻璃拟态卡片：外层渐变描边 + 内层毛玻璃（见 tokens.css & GlowCard.tsx）
- 渐变标题：`bg-clip-text text-transparent` + 渐变背景
- 动效：Framer Motion 入场 0.18s；悬停轻微放大；reduced-motion 关闭大动画
- 粒子背景：仅 Dashboard 使用；后台/低端自动暂停
- Shimmer 主按钮：3s 一次扫光

## 2. 主题（怎么改、怎么避免闪屏）
- `ThemeContext.tsx`：支持 light/dark/system/scheduled；scheduled 每15分钟检查；跨午夜正确
- `<head>` 注入内联脚本，优先设置 `<html>` 的 class 与 data-theme
- 所有颜色使用 `rgb(var(--...))`，不要写死色值

## 3. 权限（怎么裁剪 UI）
- 角色：OWNER / SUB / PLAYER / 自定义
- 权限点：MONITOR_VIEW / SERVER_CONTROL / PLAYER_VIEW / PLAYER_EDIT / PLAYER_BAN / INVENTORY_VIEW / ITEM_SEND / ITEM_ADD / ITEM_EDIT / ITEM_DELETE / CAR_EDIT / NOBLE_EDIT / SIGNIN_EDIT / PERM_ADMIN
- `<Permit perm="X">` 包裹节点；默认不渲染；可选 mode="disabled"

## 4. 页面与全字段
### 4.1 仪表盘
字段：online_total, online_series[{time,count}], cpu, mem, uptime, net_total_in, net_total_out, net_series[{time,in,out}], player_top_bandwidth[{uin,nickname,bps}]

### 4.2 玩家资料
Uin, 名称, 性别, 国家, 经验值, 点卷, 消费卷, 荣誉点, 魅力值, 舞力值, 注册时间, 签名, 喇叭, 二级密码(隐藏), 二级密码状态, 巅峰赛状态
- 操作：保存(PLAYER_EDIT)、封禁/解封(PLAYER_BAN，确认)、重置二级密码(PLAYER_EDIT，确认)

### 4.3 玩家装备的赛车
Uin, 赛车ID, 赛车名称, 宝石孔1, 宝石孔2, 宝石孔3, 改装次数
- 操作：修改宝石/改装次数(CAR_EDIT)

### 4.4 玩家背包
Uin, 名称(昵称), 物品ID, 物品名称, 数量, 天数, 状态(绑定/未绑定/过期), 获得时间, 物品类型
- 操作：新增(ITEM_ADD)、编辑(ITEM_EDIT)、删除(ITEM_DELETE)

### 4.5 玩家贵族
皇族ID, 皇族等级, 皇族经验值, 车队贵族等级, 车队贵族经验值, 情侣贵族等级, 情侣贵族经验值, 后花园贵族, 后花园贵族经验值
- 操作：等级/经验调整(NOBLE_EDIT)

### 4.6 玩家签到
Uin, 签到天数, 签到时间
- 操作：补签/清零(SIGNIN_EDIT)

### 4.7 道具管理（全部道具）
物品ID, 名称, 物品类型, 品质, 稀有度, 描述, 可交易, 有效天数, 图标
- 操作：新增/编辑/下架

### 4.8 发放道具
表单：玩家(uin/昵称) + 道具 + 数量 + 天数 + 绑定 + 备注/邮件
- 提交前摘要确认（输入 CONFIRM），权限 ITEM_SEND

### 4.9 监控/运维
实时网速、玩家带宽TopN、在线人数曲线
- 操作：重启/关闭/开启/踢人（SERVER_CONTROL，确认对话框）

### 4.10 权限管理
角色-权限矩阵（勾选）、账号绑定角色、模块可见范围勾选（即 UI 裁剪白名单）

### 4.11 设置
主题（含定时）、音乐（自动播放/音量/可视化）、偏好导入导出

## 5. API 契约（Mock 也遵守）
详见 schema/ 下各页面的 actions 名字，对应接口：
- /api/auth/login, /api/auth/me
- /api/player/*, /api/items/*, /api/monitor/*, /api/server/*, /api/perm/*
统一：成功 code=0；失败非0 + msg。

## 6. UI Schema（怎么用）
- `schema-runtime.tsx` 渲染 `schema/*.json`
- 节点：Page/Form/Table/Card/Button/Statistic/Gauge/Chart/Grid/Tabs
- 属性：permission、visibleWhen、validate、onClick
- actionsRegistry：submit/confirmSubmit/edit/delete/banPlayer/unbanPlayer/resetSecPwd/...

### 6.1 发放道具（send_item.json） —— 已提供模板
### 6.2 玩家资料（player_profile.json） —— 已提供模板
### 6.3 背包（player_inventory.json） —— 已提供模板
### 6.4 仪表盘（dashboard.json） —— 已提供模板
### 6.5 权限管理（permissions.json） —— 已提供模板

## 7. 验收表（打勾清单）
| ✓ | 项目 | 预期 |
|---|---|---|
| [ ] | 登录→仪表盘 | 可运行无错误 |
| [ ] | 玻璃+渐变 | 卡片统一风格 |
| [ ] | 主题 | 浅/深/系统/定时，刷新无闪屏 |
| [ ] | 栅格/侧栏 | 移动抽屉/桌面折叠 |
| [ ] | 动效 | 150~200ms，reduced-motion 生效 |
| [ ] | 音乐 | 播放器+可视化（可开关） |
| [ ] | 权限裁剪 | 无权限元素不渲染 |
| [ ] | 三态 | Loading/Empty/Error |
| [ ] | Mock | 主要流程通 |
| [ ] | 可访问/性能 | 焦点环、对比度、降级策略 |

