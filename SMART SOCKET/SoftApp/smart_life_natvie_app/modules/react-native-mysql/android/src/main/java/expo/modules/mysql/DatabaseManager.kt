package expo.modules.mysql

import org.json.JSONArray
import org.json.JSONObject
import java.sql.Connection
import java.sql.DriverManager
import java.sql.PreparedStatement
import java.sql.ResultSet

class DatabaseManager {
    private var connection : Connection? = null
    fun connect(): String {
            Class.forName("com.mysql.jdbc.Driver")
            val url = "jdbc:mysql://103.200.23.149:3306/optimaro_hoang_dev?useUnicode=true&characterEncoding=UTF-8&autoReconnect=true"
            this.connection= DriverManager.getConnection(url, "optimaro_hoang_dev_esp32", "Hoang2468@")
            return JSONObject().put("success", true).toString()
    }
    fun isConnected () : Boolean {
        return connection != null && !connection!!.isClosed
    }
    fun disconnect(){
        if (isConnected()){
            connection!!.close()
            connection = null
        }
    }
    fun executeSelectQuery(query: String): String {
        val result = JSONArray()
        try {
            val stmt : PreparedStatement? = connection?.prepareStatement(query);
            val rs : ResultSet?= stmt?.executeQuery()
            rs?.let {
                val metaData = rs.metaData
                val columnCount = metaData.columnCount

                while (rs.next()) {
                    val row = JSONObject()
                    for (i in 1..columnCount) {
                        row.put(metaData.getColumnName(i), rs.getString(i))
                    }
                    result.put(row)
                }
                rs.close()
                stmt.close()
            }
        } catch (e: Exception) {
            return JSONObject().put("error", e.message ?: "Error").toString()
        }
        return result.toString()
    }
    fun executeInsertQuery(query: String): String {
        return try {
            val stmt: PreparedStatement? = connection?.prepareStatement(query)
            val result = stmt?.executeUpdate()
            stmt?.close()

            if (result != null && result > 0) {
                JSONObject().put("success", true).toString()
            } else {
                JSONObject().put("success", false).put("error", "Insert failed").toString()
            }
        } catch (e: Exception) {
            JSONObject().put("error", e.message ?: "Error").toString()
        }
    }
    fun executeUpdateQuery(query: String): String {
        return try {
            val stmt: PreparedStatement? = connection?.prepareStatement(query)
            val result = stmt?.executeUpdate()
            stmt?.close()

            if (result != null && result > 0) {
                JSONObject().put("success", true).toString()
            } else {
                JSONObject().put("success", false).put("error", "Update failed").toString()
            }
        } catch (e: Exception) {
            e.printStackTrace() // Print stack trace for debugging
            JSONObject().put("error", e.message ?: "Error").toString()
        }
    }

}
