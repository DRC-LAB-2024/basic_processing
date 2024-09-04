package expo.modules.mysql

import android.util.Log
import com.facebook.react.bridge.ReadableArray
import com.facebook.react.bridge.ReadableMap
import expo.modules.kotlin.Promise
import expo.modules.kotlin.modules.Module
import expo.modules.kotlin.modules.ModuleDefinition
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.util.Timer

class ReactNativeMysqlModule : Module() {
  private val databaseManager = DatabaseManager()
  private val helper = Helper()
  override fun definition() = ModuleDefinition {
    Name("ReactNativeMysql")
    AsyncFunction("connectToDatabase") { promise: Promise ->
      CoroutineScope(Dispatchers.IO).launch {
        try {
          val result = databaseManager.connect()
          withContext(Dispatchers.Main) {
            promise.resolve(result)
          }
        } catch (e: Exception) {
          withContext(Dispatchers.Main) {
            promise.reject("CONNECT_ERROR", "Error connecting to database: ${e.message}", e)
          }
        }
      }
    }
    Function("disconnectToDatabase") {
      Log.d("NOTIFY" , "App disconected to database")
      databaseManager.disconnect()
    }
    // SELECT DATA
    AsyncFunction("select") { tableName: String, attrs : ReadableArray?, modify : ReadableMap?, promise: Promise ->
      CoroutineScope(Dispatchers.IO).launch {
        try {
          val columns = attrs?.toArrayList()?.joinToString(", ") ?: "*"
          val whereClause = modify?.let { helper.buildWhereClause(it) } ?: ""
          val query = "SELECT $columns FROM $tableName $whereClause"


          val result = databaseManager.executeSelectQuery(query)
          withContext(Dispatchers.Main) {
            promise.resolve(result)
          }
        } catch (e: Exception) {
          withContext(Dispatchers.Main) {
            promise.reject("SELECT_QUERY_ERROR", "Error executing select query: ${e.message}", e)
          }
        }
      }
    }
    // UPDATE DATA
    AsyncFunction("update"){tableName: String, attrs: ReadableArray?, values: ReadableMap, modify: ReadableMap?, promise: Promise ->
      CoroutineScope(Dispatchers.IO).launch {
        try {
          val setClause = helper.buildSetClause(attrs, values)
          val whereClause = modify?.let { helper.buildWhereClause(it) } ?: ""

          val query = "UPDATE $tableName SET $setClause $whereClause"
          Log.d("update", query)

          // Log the query for debugging

          val result = databaseManager.executeUpdateQuery(query)
          withContext(Dispatchers.Main) {
            promise.resolve(result)
          }
        } catch (e: Exception) {
          withContext(Dispatchers.Main) {
            promise.reject("UPDATE_QUERY_ERROR", "Error executing update query: ${e.message}", e)
          }
        }
      }
    }
    // INSERT DATA
    AsyncFunction("insert") { tableName: String, attrs: ReadableArray?, values: ReadableMap, promise: Promise ->
      CoroutineScope(Dispatchers.IO).launch {
        try {
          val columns = attrs?.toArrayList()?.joinToString(", "){ "`$it`" }
          val valuesList = helper.buildValuesList(values)


          val query = if (columns != null) {
            "INSERT INTO $tableName ($columns) VALUES ($valuesList);"
          } else {
            "INSERT INTO $tableName VALUES ($valuesList);"
          }

          val result = databaseManager.executeInsertQuery(query)
          withContext(Dispatchers.Main) {
            promise.resolve(result)
          }
        } catch (e: Exception) {
          withContext(Dispatchers.Main) {
            promise.reject("INSERT_QUERY_ERROR", "Error executing insert query: ${e.message}", e)
          }
        }
      }
    }
    Function("hello") {
      "Hello world! 👋"
    }
  }
}
