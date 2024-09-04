package expo.modules.mysql

import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap
import com.facebook.react.bridge.ReadableType

class Helper {
    fun buildSetClause(attrs: ReadableArray?, values: ReadableMap): String {
        val setClause = mutableListOf<String>()
        attrs?.toArrayList()?.forEach { attr ->
            val attrName = attr as String
            when (values.getType(attrName)) {
                ReadableType.String -> setClause.add("`$attrName` = '${values.getString(attrName)}'")
                ReadableType.Number -> setClause.add("`$attrName` = ${values.getInt(attrName)}")
                ReadableType.Boolean -> setClause.add("`$attrName` = ${values.getBoolean(attrName)}")
                else -> {
                    // Handle other types if needed
                }
            }
        }
        return setClause.joinToString(", ")
    }
    fun buildValuesList(values: ReadableMap): String {
        val valuesList = mutableListOf<String>()
        val iterator = values.keySetIterator()

        while (iterator.hasNextKey()) {
            val key = iterator.nextKey()
            when (values.getType(key)) {
                ReadableType.String -> valuesList.add("'${values.getString(key)}'")
                ReadableType.Number -> valuesList.add("${values.getInt(key)}")
                ReadableType.Boolean -> valuesList.add("${values.getBoolean(key)}")
                else -> {
                    // Handle other types if needed
                }
            }
        }

        return valuesList.joinToString(", ")
    }
    fun buildWhereClause(modify: ReadableMap): String {
        val conditions = mutableListOf<String>()
        val iterator = modify.keySetIterator()

        while (iterator.hasNextKey()) {
            val key = iterator.nextKey()
            when (modify.getType(key)) {
                ReadableType.String -> conditions.add("$key = '${modify.getString(key)}'")
                ReadableType.Number -> conditions.add("$key = ${modify.getInt(key)}")
                ReadableType.Boolean -> conditions.add("$key = ${modify.getBoolean(key)}")
                else -> {
                    // Handle other types if needed
                }
            }
        }

        return if (conditions.isNotEmpty()) "WHERE ${conditions.joinToString(" AND ")}" else ""
    }
}