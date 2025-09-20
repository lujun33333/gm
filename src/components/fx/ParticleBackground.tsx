import { useEffect, useRef, useState } from 'react'
import { motion } from 'framer-motion'

interface Particle {
  x: number
  y: number
  size: number
  speedX: number
  speedY: number
  opacity: number
  color: string
}

interface ParticleBackgroundProps {
  particleCount?: number
  className?: string
  enabled?: boolean
}

export function ParticleBackground({
  particleCount = 50,
  className = '',
  enabled = true
}: ParticleBackgroundProps) {
  const canvasRef = useRef<HTMLCanvasElement>(null)
  const [particles, setParticles] = useState<Particle[]>([])
  const animationRef = useRef<number>()
  const [isPaused, setIsPaused] = useState(false)

  // 检测用户偏好和性能
  useEffect(() => {
    const prefersReducedMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches
    const isLowEnd = navigator.hardwareConcurrency && navigator.hardwareConcurrency < 4

    if (prefersReducedMotion || isLowEnd) {
      setIsPaused(true)
    }
  }, [])

  // 初始化粒子
  useEffect(() => {
    if (!enabled || isPaused) return

    const canvas = canvasRef.current
    if (!canvas) return

    const rect = canvas.getBoundingClientRect()
    const width = rect.width
    const height = rect.height

    canvas.width = width * window.devicePixelRatio
    canvas.height = height * window.devicePixelRatio

    const ctx = canvas.getContext('2d')
    if (!ctx) return

    ctx.scale(window.devicePixelRatio, window.devicePixelRatio)

    const colors = [
      'rgb(59, 130, 246)',    // primary
      'rgb(236, 72, 153)',    // accent
      'rgb(34, 197, 94)',     // success
      'rgb(147, 51, 234)',    // purple
    ]

    const newParticles: Particle[] = []
    for (let i = 0; i < particleCount; i++) {
      newParticles.push({
        x: Math.random() * width,
        y: Math.random() * height,
        size: Math.random() * 3 + 1,
        speedX: (Math.random() - 0.5) * 0.5,
        speedY: (Math.random() - 0.5) * 0.5,
        opacity: Math.random() * 0.5 + 0.2,
        color: colors[Math.floor(Math.random() * colors.length)]
      })
    }

    setParticles(newParticles)
  }, [particleCount, enabled, isPaused])

  // 动画循环
  useEffect(() => {
    if (!enabled || isPaused || particles.length === 0) return

    const canvas = canvasRef.current
    if (!canvas) return

    const ctx = canvas.getContext('2d')
    if (!ctx) return

    const rect = canvas.getBoundingClientRect()
    const width = rect.width
    const height = rect.height

    const animate = () => {
      ctx.clearRect(0, 0, width, height)

      setParticles(prevParticles => {
        return prevParticles.map(particle => {
          // 更新位置
          let newX = particle.x + particle.speedX
          let newY = particle.y + particle.speedY

          // 边界反弹
          if (newX <= 0 || newX >= width) particle.speedX *= -1
          if (newY <= 0 || newY >= height) particle.speedY *= -1

          newX = Math.max(0, Math.min(width, newX))
          newY = Math.max(0, Math.min(height, newY))

          // 绘制粒子
          ctx.save()
          ctx.globalAlpha = particle.opacity
          ctx.fillStyle = particle.color
          ctx.beginPath()
          ctx.arc(newX, newY, particle.size, 0, Math.PI * 2)
          ctx.fill()
          ctx.restore()

          return {
            ...particle,
            x: newX,
            y: newY
          }
        })
      })

      animationRef.current = requestAnimationFrame(animate)
    }

    animate()

    return () => {
      if (animationRef.current) {
        cancelAnimationFrame(animationRef.current)
      }
    }
  }, [particles.length, enabled, isPaused])

  // 窗口大小变化时重新初始化
  useEffect(() => {
    const handleResize = () => {
      const canvas = canvasRef.current
      if (!canvas) return

      const rect = canvas.getBoundingClientRect()
      canvas.width = rect.width * window.devicePixelRatio
      canvas.height = rect.height * window.devicePixelRatio

      const ctx = canvas.getContext('2d')
      if (ctx) {
        ctx.scale(window.devicePixelRatio, window.devicePixelRatio)
      }
    }

    window.addEventListener('resize', handleResize)
    return () => window.removeEventListener('resize', handleResize)
  }, [])

  if (!enabled || isPaused) {
    return null
  }

  return (
    <motion.canvas
      ref={canvasRef}
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      transition={{ duration: 1 }}
      className={`fixed inset-0 pointer-events-none z-0 ${className}`}
      style={{
        background: 'transparent',
      }}
    />
  )
}