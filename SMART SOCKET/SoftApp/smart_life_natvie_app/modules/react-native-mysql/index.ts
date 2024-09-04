// Import the native module. On web, it will be resolved to ReactNativeMysql.web.ts
// and on native platforms to ReactNativeMysql.ts
import ReactNativeMysqlModule from './src/ReactNativeMysqlModule'

type Values = {
  [key: string]: string | number | boolean | null
}

type Props = {
  table: string
  attrs?: Array<string>
  values?: Values
  modify?: Values
}

export function hello(): string {
  return ReactNativeMysqlModule.hello()
}

export async function connectToDatabase() {
  try {
    const result = await ReactNativeMysqlModule.connectToDatabase()
    return JSON.parse(result)
  } catch (error) {
    console.error(error)
  }
}

export function disconnectToDatabase(): void {
  ReactNativeMysqlModule.disconnectToDatabase()
}

export async function select({ ...props }: Props) {
  try {
    const { table, attrs, modify } = props
    const result = await ReactNativeMysqlModule.select(table, attrs, modify)
    return JSON.parse(result)
  } catch (e) {
    console.log(e)
  }
}

export async function insert({ ...props }: Props) {
  try {
    const { table, attrs, values } = props
    const result = await ReactNativeMysqlModule.insert(table, attrs, values)
    return JSON.parse(result)
  } catch (e) {
    console.log(e)
  }
}
export async function update({ ...props }: Props) {
  try {
    const { table, attrs, values, modify } = props
    const result = await ReactNativeMysqlModule.update(
      table,
      attrs,
      values,
      modify
    )
    return JSON.parse(result)
  } catch (e) {
    console.log(e)
  }
}
