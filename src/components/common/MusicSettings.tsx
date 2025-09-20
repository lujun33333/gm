// path: src/components/common/MusicSettings.tsx
import { useState } from 'react'
import { motion } from 'framer-motion'
import { Settings, Check, X, Music, Key, Wifi, Volume2, Disc3, Heart } from 'lucide-react'
import {
  Dialog,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  DialogFooter
} from '@/components/ui/dialog'
import { Button } from '@/components/ui/button'
import { Input } from '@/components/ui/input'
import { Label } from '@/components/ui/label'
import { Switch } from '@/components/ui/switch'
import { RadioGroup, RadioGroupItem } from '@/components/ui/radio-group'
import { Slider } from '@/components/ui/slider'
import { useMusicSettings, type MusicSettings } from '@/hooks/useMusicSettings'
import { musicApi } from '@/utils/musicApi'

interface MusicSettingsProps {
  open: boolean
  onOpenChange: (open: boolean) => void
}

export function MusicSettings({ open, onOpenChange }: MusicSettingsProps) {
  const {
    settings,
    musicPlatforms,
    updateSettings,
    getCurrentPlatform,
    validateApiKey
  } = useMusicSettings()

  const [localSettings, setLocalSettings] = useState<MusicSettings>(settings)
  const [isValidating, setIsValidating] = useState(false)
  const [validationResult, setValidationResult] = useState<{
    isValid: boolean
    message: string
  } | null>(null)
  const [isSaving, setIsSaving] = useState(false)

  const currentPlatform = musicPlatforms.find(p => p.id === localSettings.platform)

  const handlePlatformChange = (platform: string) => {
    setLocalSettings(prev => ({
      ...prev,
      platform: platform as 'netease' | 'qq' | 'local',
      apiKey: '', // 切换平台时清空API Key
      quality: musicPlatforms.find(p => p.id === platform)?.qualityLevels[0] || 'standard'
    }))
    setValidationResult(null)
  }

  const handleApiKeyChange = (apiKey: string) => {
    setLocalSettings(prev => ({ ...prev, apiKey }))
    setValidationResult(null)
  }

  const handleValidateApiKey = async () => {
    if (!localSettings.apiKey && localSettings.platform !== 'local') {
      setValidationResult({
        isValid: false,
        message: '请输入API密钥'
      })
      return
    }

    setIsValidating(true)
    try {
      const isValid = await validateApiKey(localSettings.platform, localSettings.apiKey)
      setValidationResult({
        isValid,
        message: isValid ? 'API密钥验证成功' : 'API密钥验证失败，请检查密钥是否正确'
      })
    } catch (error) {
      setValidationResult({
        isValid: false,
        message: '验证失败，请检查网络连接'
      })
    } finally {
      setIsValidating(false)
    }
  }

  const handleQualityChange = (quality: string) => {
    setLocalSettings(prev => ({ ...prev, quality }))
  }

  const handleVolumeChange = (volume: number[]) => {
    setLocalSettings(prev => ({ ...prev, volume: volume[0] }))
  }

  const handleCrossfadeChange = (crossfade: number[]) => {
    setLocalSettings(prev => ({ ...prev, crossfade: crossfade[0] }))
  }

  const handleSwitchChange = (field: keyof MusicSettings, value: boolean) => {
    setLocalSettings(prev => ({ ...prev, [field]: value }))
  }

  const handleSave = async () => {
    setIsSaving(true)
    try {
      // 如果是网络平台且需要API Key，先验证
      if (localSettings.platform !== 'local' && currentPlatform?.apiKeyRequired) {
        if (!localSettings.apiKey) {
          setValidationResult({
            isValid: false,
            message: '请输入API密钥'
          })
          setIsSaving(false)
          return
        }

        if (!validationResult?.isValid) {
          await handleValidateApiKey()
          if (!validationResult?.isValid) {
            setIsSaving(false)
            return
          }
        }
      }

      // 保存设置
      updateSettings(localSettings)

      // 更新音乐API设置
      musicApi.setSettings(localSettings)

      // 显示成功消息
      setValidationResult({
        isValid: true,
        message: '设置保存成功'
      })

      // 延迟关闭对话框
      setTimeout(() => {
        onOpenChange(false)
      }, 1000)
    } catch (error) {
      setValidationResult({
        isValid: false,
        message: '保存失败，请重试'
      })
    } finally {
      setIsSaving(false)
    }
  }

  const handleCancel = () => {
    setLocalSettings(settings) // 重置为原始设置
    setValidationResult(null)
    onOpenChange(false)
  }

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-2xl max-h-[80vh] overflow-y-auto">
        <DialogHeader>
          <DialogTitle className="flex items-center gap-2">
            <Settings className="w-5 h-5" />
            音乐设置
          </DialogTitle>
          <DialogDescription>
            配置音乐平台、API密钥和播放偏好设置
          </DialogDescription>
        </DialogHeader>

        <div className="space-y-6">
          {/* 平台选择 */}
          <div className="space-y-3">
            <Label className="text-base font-medium flex items-center gap-2">
              <Music className="w-4 h-4" />
              音乐平台
            </Label>
            <RadioGroup
              value={localSettings.platform}
              onValueChange={handlePlatformChange}
              className="grid grid-cols-1 gap-3"
            >
              {musicPlatforms.map((platform) => (
                <motion.div
                  key={platform.id}
                  whileHover={{ scale: 1.02 }}
                  className="flex items-center space-x-3 rounded-lg border border-border p-4 hover:bg-bg-secondary"
                >
                  <RadioGroupItem value={platform.id} id={platform.id} />
                  <div className="flex-1">
                    <Label htmlFor={platform.id} className="font-medium cursor-pointer">
                      {platform.name}
                    </Label>
                    <p className="text-sm text-text-muted mt-1">
                      {platform.description}
                    </p>
                  </div>
                  {platform.apiKeyRequired && (
                    <Key className="w-4 h-4 text-text-muted" />
                  )}
                </motion.div>
              ))}
            </RadioGroup>
          </div>

          {/* API密钥配置 */}
          {currentPlatform?.apiKeyRequired && (
            <div className="space-y-3">
              <Label className="text-base font-medium flex items-center gap-2">
                <Key className="w-4 h-4" />
                API密钥
              </Label>
              <div className="flex gap-2">
                <Input
                  type="password"
                  placeholder={`请输入${currentPlatform.name}的API密钥`}
                  value={localSettings.apiKey}
                  onChange={(e) => handleApiKeyChange(e.target.value)}
                  className="flex-1"
                />
                <Button
                  variant="outline"
                  onClick={handleValidateApiKey}
                  disabled={isValidating || !localSettings.apiKey}
                  className="px-4"
                >
                  {isValidating ? (
                    <motion.div
                      animate={{ rotate: 360 }}
                      transition={{ duration: 1, repeat: Infinity, ease: "linear" }}
                      className="w-4 h-4"
                    >
                      <Wifi className="w-4 h-4" />
                    </motion.div>
                  ) : (
                    <Check className="w-4 h-4" />
                  )}
                </Button>
              </div>
              {validationResult && (
                <motion.div
                  initial={{ opacity: 0, y: -10 }}
                  animate={{ opacity: 1, y: 0 }}
                  className={`flex items-center gap-2 text-sm ${
                    validationResult.isValid ? 'text-green-500' : 'text-red-500'
                  }`}
                >
                  {validationResult.isValid ? (
                    <Check className="w-4 h-4" />
                  ) : (
                    <X className="w-4 h-4" />
                  )}
                  {validationResult.message}
                </motion.div>
              )}
              <p className="text-xs text-text-muted">
                {localSettings.platform === 'netease' && '请访问网易云音乐开发者平台获取API密钥'}
                {localSettings.platform === 'qq' && '请访问QQ音乐开放平台获取API密钥'}
              </p>
            </div>
          )}

          {/* 音质设置 */}
          <div className="space-y-3">
            <Label className="text-base font-medium flex items-center gap-2">
              <Disc3 className="w-4 h-4" />
              音质设置
            </Label>
            <RadioGroup
              value={localSettings.quality}
              onValueChange={handleQualityChange}
              className="flex flex-wrap gap-3"
            >
              {currentPlatform?.qualityLevels.map((quality) => (
                <div key={quality} className="flex items-center space-x-2">
                  <RadioGroupItem value={quality} id={quality} />
                  <Label htmlFor={quality} className="cursor-pointer">
                    {quality === 'standard' && '标准音质'}
                    {quality === 'higher' && '较高音质'}
                    {quality === 'exhigh' && '极高音质'}
                    {quality === 'lossless' && '无损音质'}
                    {quality === 'size_128' && '128kbps'}
                    {quality === 'size_320' && '320kbps'}
                    {quality === 'size_flac' && 'FLAC'}
                    {quality === 'original' && '原始音质'}
                  </Label>
                </div>
              ))}
            </RadioGroup>
          </div>

          {/* 音量设置 */}
          <div className="space-y-3">
            <Label className="text-base font-medium flex items-center gap-2">
              <Volume2 className="w-4 h-4" />
              默认音量: {Math.round(localSettings.volume * 100)}%
            </Label>
            <Slider
              value={[localSettings.volume]}
              onValueChange={handleVolumeChange}
              max={1}
              min={0}
              step={0.1}
              className="w-full"
            />
          </div>

          {/* 渐变时间 */}
          <div className="space-y-3">
            <Label className="text-base font-medium">
              渐变时间: {localSettings.crossfade}秒
            </Label>
            <Slider
              value={[localSettings.crossfade]}
              onValueChange={handleCrossfadeChange}
              max={10}
              min={0}
              step={1}
              className="w-full"
            />
          </div>

          {/* 其他设置 */}
          <div className="space-y-4">
            <Label className="text-base font-medium">其他设置</Label>

            <div className="space-y-3">
              <div className="flex items-center justify-between">
                <div className="space-y-0.5">
                  <Label>自动播放</Label>
                  <p className="text-sm text-text-muted">页面加载时自动开始播放</p>
                </div>
                <Switch
                  checked={localSettings.autoPlay}
                  onCheckedChange={(checked) => handleSwitchChange('autoPlay', checked)}
                />
              </div>

              <div className="flex items-center justify-between">
                <div className="space-y-0.5">
                  <Label className="flex items-center gap-2">
                    <Heart className="w-4 h-4" />
                    显示歌词
                  </Label>
                  <p className="text-sm text-text-muted">在播放器中显示歌词</p>
                </div>
                <Switch
                  checked={localSettings.lyricEnabled}
                  onCheckedChange={(checked) => handleSwitchChange('lyricEnabled', checked)}
                />
              </div>

              <div className="flex items-center justify-between">
                <div className="space-y-0.5">
                  <Label>同步音乐库</Label>
                  <p className="text-sm text-text-muted">自动同步平台音乐库到本地</p>
                </div>
                <Switch
                  checked={localSettings.syncLibrary}
                  onCheckedChange={(checked) => handleSwitchChange('syncLibrary', checked)}
                />
              </div>
            </div>
          </div>
        </div>

        <DialogFooter>
          <Button variant="outline" onClick={handleCancel} disabled={isSaving}>
            取消
          </Button>
          <Button onClick={handleSave} disabled={isSaving}>
            {isSaving ? (
              <motion.div
                animate={{ rotate: 360 }}
                transition={{ duration: 1, repeat: Infinity, ease: "linear" }}
                className="w-4 h-4 mr-2"
              >
                <Settings className="w-4 h-4" />
              </motion.div>
            ) : null}
            {isSaving ? '保存中...' : '保存设置'}
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  )
}