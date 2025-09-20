import { useState, useMemo } from 'react'
import { motion } from 'framer-motion'
import { clsx } from 'clsx'

export interface Column {
  title: string
  dataIndex: string
  key?: string
  width?: number | string
  render?: (value: any, record: any, index: number) => React.ReactNode
  sorter?: boolean
  filters?: Array<{ text: string; value: any }>
  align?: 'left' | 'center' | 'right'
}

interface DataTableProps {
  columns: Column[]
  data: any[]
  loading?: boolean
  pagination?: {
    current?: number
    pageSize?: number
    total?: number
    showSizeChanger?: boolean
    onChange?: (page: number, pageSize: number) => void
  }
  rowSelection?: {
    selectedRowKeys?: any[]
    onChange?: (selectedRowKeys: any[], selectedRows: any[]) => void
  }
  className?: string
  onRow?: (record: any, index: number) => {
    onClick?: () => void
    onDoubleClick?: () => void
    className?: string
  }
}

export function DataTable({
  columns,
  data,
  loading = false,
  pagination,
  rowSelection,
  className,
  onRow
}: DataTableProps) {
  const [sortField, setSortField] = useState<string>('')
  const [sortOrder, setSortOrder] = useState<'asc' | 'desc'>('asc')
  const [filters, setFilters] = useState<Record<string, any[]>>({})
  const [selectedRows, setSelectedRows] = useState<any[]>([])

  // 处理排序
  const handleSort = (field: string) => {
    if (sortField === field) {
      setSortOrder(sortOrder === 'asc' ? 'desc' : 'asc')
    } else {
      setSortField(field)
      setSortOrder('asc')
    }
  }

  // 处理筛选
  const handleFilter = (field: string, values: any[]) => {
    setFilters(prev => ({ ...prev, [field]: values }))
  }

  // 排序和筛选后的数据
  const processedData = useMemo(() => {
    let result = [...data]

    // 应用筛选
    Object.entries(filters).forEach(([field, values]) => {
      if (values.length > 0) {
        result = result.filter(item => values.includes(item[field]))
      }
    })

    // 应用排序
    if (sortField) {
      result.sort((a, b) => {
        const aVal = a[sortField]
        const bVal = b[sortField]

        if (typeof aVal === 'number' && typeof bVal === 'number') {
          return sortOrder === 'asc' ? aVal - bVal : bVal - aVal
        }

        const aStr = String(aVal).toLowerCase()
        const bStr = String(bVal).toLowerCase()
        return sortOrder === 'asc'
          ? aStr.localeCompare(bStr)
          : bStr.localeCompare(aStr)
      })
    }

    return result
  }, [data, sortField, sortOrder, filters])

  // 分页数据
  const paginatedData = useMemo(() => {
    if (!pagination) return processedData

    const { current = 1, pageSize = 10 } = pagination
    const start = (current - 1) * pageSize
    const end = start + pageSize
    return processedData.slice(start, end)
  }, [processedData, pagination])

  // 处理行选择
  const handleRowSelect = (record: any, selected: boolean) => {
    const key = record.id || record.key || JSON.stringify(record)

    if (selected) {
      setSelectedRows(prev => [...prev, record])
      rowSelection?.onChange?.([...selectedRows.map(r => r.id || r.key), key], [...selectedRows, record])
    } else {
      const newRows = selectedRows.filter(r => (r.id || r.key) !== key)
      setSelectedRows(newRows)
      rowSelection?.onChange?.(newRows.map(r => r.id || r.key), newRows)
    }
  }

  // 全选
  const handleSelectAll = (selected: boolean) => {
    if (selected) {
      setSelectedRows(paginatedData)
      rowSelection?.onChange?.(
        paginatedData.map(r => r.id || r.key || JSON.stringify(r)),
        paginatedData
      )
    } else {
      setSelectedRows([])
      rowSelection?.onChange?.([], [])
    }
  }

  const isAllSelected = selectedRows.length === paginatedData.length && paginatedData.length > 0
  const isIndeterminate = selectedRows.length > 0 && selectedRows.length < paginatedData.length

  if (loading) {
    return <TableSkeleton />
  }

  if (data.length === 0) {
    return <EmptyTable />
  }

  return (
    <div className={clsx('data-table', className)}>
      {/* 表格容器 */}
      <div className="overflow-hidden rounded-lg border border-border bg-bg-secondary">
        <div className="overflow-x-auto">
          <table className="w-full">
            {/* 表头 */}
            <thead className="bg-bg-tertiary">
              <tr>
                {/* 选择列 */}
                {rowSelection && (
                  <th className="w-12 px-4 py-3 text-left">
                    <input
                      type="checkbox"
                      checked={isAllSelected}
                      ref={(input) => {
                        if (input) input.indeterminate = isIndeterminate
                      }}
                      onChange={(e) => handleSelectAll(e.target.checked)}
                      className="w-4 h-4 text-primary bg-bg-secondary border-border rounded focus:ring-primary"
                    />
                  </th>
                )}

                {/* 数据列 */}
                {columns.map((column, index) => (
                  <th
                    key={column.key || column.dataIndex || index}
                    className={clsx(
                      'px-4 py-3 text-left text-sm font-medium text-text-secondary',
                      {
                        'text-center': column.align === 'center',
                        'text-right': column.align === 'right',
                        'cursor-pointer hover:bg-bg-secondary': column.sorter
                      }
                    )}
                    style={{ width: column.width }}
                    onClick={() => column.sorter && handleSort(column.dataIndex)}
                  >
                    <div className="flex items-center gap-2">
                      <span>{column.title}</span>
                      {column.sorter && (
                        <div className="flex flex-col">
                          <span className={clsx('text-xs', {
                            'text-primary': sortField === column.dataIndex && sortOrder === 'asc'
                          })}>▲</span>
                          <span className={clsx('text-xs', {
                            'text-primary': sortField === column.dataIndex && sortOrder === 'desc'
                          })}>▼</span>
                        </div>
                      )}
                    </div>
                  </th>
                ))}
              </tr>
            </thead>

            {/* 表体 */}
            <tbody className="divide-y divide-border">
              {paginatedData.map((record, rowIndex) => {
                const rowProps = onRow?.(record, rowIndex) || {}
                const isSelected = selectedRows.some(r =>
                  (r.id || r.key || JSON.stringify(r)) === (record.id || record.key || JSON.stringify(record))
                )

                return (
                  <motion.tr
                    key={record.id || record.key || rowIndex}
                    initial={{ opacity: 0, y: 10 }}
                    animate={{ opacity: 1, y: 0 }}
                    transition={{ duration: 0.15, delay: rowIndex * 0.02 }}
                    className={clsx(
                      'hover:bg-bg-tertiary transition-colors',
                      {
                        'bg-primary/10': isSelected,
                        'cursor-pointer': rowProps.onClick
                      },
                      rowProps.className
                    )}
                    onClick={rowProps.onClick}
                    onDoubleClick={rowProps.onDoubleClick}
                  >
                    {/* 选择列 */}
                    {rowSelection && (
                      <td className="px-4 py-3">
                        <input
                          type="checkbox"
                          checked={isSelected}
                          onChange={(e) => handleRowSelect(record, e.target.checked)}
                          className="w-4 h-4 text-primary bg-bg-secondary border-border rounded focus:ring-primary"
                        />
                      </td>
                    )}

                    {/* 数据列 */}
                    {columns.map((column, colIndex) => {
                      const value = record[column.dataIndex]
                      const displayValue = column.render
                        ? column.render(value, record, rowIndex)
                        : value

                      return (
                        <td
                          key={column.key || column.dataIndex || colIndex}
                          className={clsx(
                            'px-4 py-3 text-sm text-text',
                            {
                              'text-center': column.align === 'center',
                              'text-right': column.align === 'right'
                            }
                          )}
                        >
                          {displayValue}
                        </td>
                      )
                    })}
                  </motion.tr>
                )
              })}
            </tbody>
          </table>
        </div>
      </div>

      {/* 分页 */}
      {pagination && (
        <div className="flex items-center justify-between mt-4">
          <div className="text-sm text-text-muted">
            共 {processedData.length} 条记录
          </div>

          <div className="flex items-center gap-2">
            <button
              onClick={() => pagination.onChange?.(1, pagination.pageSize || 10)}
              disabled={pagination.current === 1}
              className="px-3 py-1 text-sm border border-border rounded hover:bg-bg-tertiary disabled:opacity-50"
            >
              首页
            </button>
            <button
              onClick={() => pagination.onChange?.((pagination.current || 1) - 1, pagination.pageSize || 10)}
              disabled={pagination.current === 1}
              className="px-3 py-1 text-sm border border-border rounded hover:bg-bg-tertiary disabled:opacity-50"
            >
              上一页
            </button>

            <span className="px-3 py-1 text-sm">
              {pagination.current} / {Math.ceil((pagination.total || processedData.length) / (pagination.pageSize || 10))}
            </span>

            <button
              onClick={() => pagination.onChange?.((pagination.current || 1) + 1, pagination.pageSize || 10)}
              disabled={pagination.current === Math.ceil((pagination.total || processedData.length) / (pagination.pageSize || 10))}
              className="px-3 py-1 text-sm border border-border rounded hover:bg-bg-tertiary disabled:opacity-50"
            >
              下一页
            </button>
            <button
              onClick={() => pagination.onChange?.(
                Math.ceil((pagination.total || processedData.length) / (pagination.pageSize || 10)),
                pagination.pageSize || 10
              )}
              disabled={pagination.current === Math.ceil((pagination.total || processedData.length) / (pagination.pageSize || 10))}
              className="px-3 py-1 text-sm border border-border rounded hover:bg-bg-tertiary disabled:opacity-50"
            >
              末页
            </button>
          </div>
        </div>
      )}
    </div>
  )
}

// 加载骨架屏
function TableSkeleton() {
  return (
    <div className="space-y-4">
      <div className="h-4 bg-bg-secondary rounded w-1/4 animate-pulse" />
      <div className="space-y-2">
        {Array.from({ length: 5 }).map((_, i) => (
          <div key={i} className="h-8 bg-bg-secondary rounded animate-pulse" />
        ))}
      </div>
    </div>
  )
}

// 空状态
function EmptyTable() {
  return (
    <div className="text-center py-12">
      <div className="text-6xl mb-4">📄</div>
      <h3 className="text-lg font-medium text-text mb-2">暂无数据</h3>
      <p className="text-text-muted">还没有任何记录</p>
    </div>
  )
}