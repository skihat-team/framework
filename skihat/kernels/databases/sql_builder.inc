<?php
/**
 * sql_builder.inc SQL生成器类声明文件。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.kernels.databases
 * @version     1.0
 *
 * @create      2012-07-17
 * @modify      2013-09-11
 */

/**
 * SQL 生成器类，提供SQL的生成服务。
 *
 * @package skihat.kernels.databases
 */
abstract class SqlBuilder implements IDataSupport {
    /**
     * 根据$fields和$syntax生成创建查询所需的命令、参数。
     *
     * @param array $fields 创建所需的数据值。
     * @param array $syntax 创建所需的语法值。
     *
     * @return array
     */
    public function create(array $fields, array $syntax) {
        # 获取插入表的名称。
        $tableStmt = $this->safeName($syntax[self::SYNTAX_TABLE]);
        # 根据数据和语法过滤有效数据并生成查询命令。
        $fields = $this->_fieldsFilter($fields, $syntax);
        $values = array_values($fields);

        # 生成字段集和参数集。
        $fieldStmt = $this->_stmtListFields(array_keys($fields));
        $valueStmt = $this->_stmtListArgs(count($fields));

        # 返回结果，命令、参数
        return array($this->_stmtInsert($tableStmt, $fieldStmt, $valueStmt), $values);
    }

    /**
     * 根据$fields、$values和$syntax生成创建多条记录所需要命令和参数。
     *
     * @param array $fields 字段名称。
     * @param array $values 字段值。
     * @param array $syntax 命令语法。
     *
     * @return mixed
     */
    public function createMultiple(array $fields, array $values, array $syntax) {
        $tableStmt = $this->safeName($syntax[self::SYNTAX_TABLE]);
        $fieldStmt = $this->_stmtListFields($fields);
        $fieldCount = count($fields);
        $valueCount = count($values);

        $valueStmt = '';
        for ($i = 0; $i < $valueCount; ++$i) {
            $valueStmt .= $this->_stmtListArgs($fieldCount) . '),(';
        }

        $valueStmt = substr($valueStmt, 0, -3);
        $values = call_user_func_array('array_merge', $values);

        # 返回结果命令、参数
        return array($this->_stmtInsert($tableStmt, $fieldStmt, $valueStmt), $values);
    }

    /**
     * 使用$data、$syntax和$options生成更新所需的命令、参数和选项值。
     *
     * @param mixed $fields 更新字段集。
     * @param array $syntax 更新语法值。
     *
     * @return array
     */
    public function update($fields, array $syntax) {
        # 如果$fields是数组值，则使用updateFields方法生成，否则表示直接生成。
        if (is_array($fields)) {
            if (!($fieldStmt = array_key_pop($fields, 0))) {
                $fields = $this->_fieldsFilter($fields, $syntax);
                $fieldStmt = $this->_stmtUpdateFields($fields);
            }
        } else {
            $fieldStmt = $fields;
            $fields = array();
        }

        $whereStmt = $this->_stmtWhere($fields, $syntax);
        $tableStmt = $this->_stmtFullTable($syntax);

        # 返回结果，命令、参数、选项。
        return array($this->_stmtUpdate($tableStmt, $fieldStmt, $whereStmt), array_values($fields));
    }

    /**
     * 根据$syntax和$options生成删除命令。
     *
     * @param array $syntax 删除语法。
     *
     * @return array
     */
    public function delete(array $syntax) {
        $args = array();

        # 生成条件，表命令。
        $whereStmt = $this->_stmtWhere($args, $syntax);
        $aliasStmt = $this->_stmtListTableAlias($syntax);
        $tableStmt = $this->_stmtFullTable($syntax);

        # 返回信息：命令、参数。
        return array($this->_deleteTable($aliasStmt, $tableStmt, $whereStmt), $args, array());
    }

    /**
     * 根据$syntax和$options生成查询命令。
     *
     * @param array $syntax  查询语法。
     * @param array $options 查询选项。
     *
     * @return array
     */
    public function fetch(array $syntax, $options = array()) {
        $args = array();

        # 获取字段，条件，排序，表和分页语法命令。
        $whereStmt = $this->_stmtWhere($args, $syntax);
        $fieldsSmt = $this->_stmtFetchFields($syntax);
        $groupSmt = $this->_stmtGroup($syntax);
        $orderStmt = $this->_stmtOrder($syntax);
        $tableStmt = $this->_stmtFullTable($syntax);
        $pageStmt = $this->_stmtPage($syntax);

        # 生成并返回命令及参数。
        return array($this->_stmtFetch($tableStmt, $fieldsSmt, $whereStmt, $groupSmt, $orderStmt, $pageStmt), $args);
    }

    /**
     * 根据$name生成符合安全规则的命名。
     *
     * @param string $name 需命名的参数。
     *
     * @return mixed
     */
    public abstract function safeName($name);

    /**
     * 根据$data和$syntax过滤出有效的列字段值。
     *
     * @param array $fields 数据值。
     * @param array $syntax 语法。
     *
     * @return array
     */
    protected function _fieldsFilter(array &$fields, array &$syntax) {
        # 如果设置有字段，则去除设置字段以外的值。
        if (isset($syntax[self::SYNTAX_FIELDS])) {
            $fields = array_intersect_key($fields, array_flip($syntax[self::SYNTAX_FIELDS]));
        }

        return $fields;
    }

    /**
     * 根据$syntax获取表名称声明。
     *
     * @param array $syntax
     *
     * @return string
     *
     * @throws DBSyntaxException 如果语法发生错误将引发本异常。
     */
    protected function _stmtFullTable(array &$syntax) {
        # 如果在语法中指定有自己的FROM结构，则直接返回v1.1。
        if (isset($syntax[self::SYNTAX_FROM])) {
            return $syntax[self::SYNTAX_FROM];
        }

        # 获取基本表的声明信息。
        $stmt = $this->_stmtTable($syntax);

        if (isset($syntax[self::SYNTAX_LINK])) {
            # 合并生成关系 连接表的信息。
            foreach ((array)$syntax[self::SYNTAX_LINK] AS $name) {
                # 如果指定关系没有配设置，引发语法异常。
                if (!isset($syntax[self::SYNTAX_JOIN][$name])) {
                    throw new DBSyntaxException('SqlBuilder: Not support join ON syntax in "' . $name . '"');
                }

                # 获取配置值并生成连接语法。
                $cfg = $syntax[self::SYNTAX_JOIN][$name];
                $type = array_key_value($cfg, self::SYNTAX_JOIN_TYPE, 'LEFT JOIN');
                $stmt .= sprintf(' %s %s ON %s ', $type, $this->_stmtTable($cfg), $cfg[self::SYNTAX_JOIN_ON]);
            }
        }

        return $stmt;
    }

    /**
     * 根据$syntax返回全部需求表的别名列表。
     *
     * @param array $syntax 语法参数。
     *
     * @return string
     *
     * @throws DbSyntaxException
     */
    protected function _stmtListTableAlias(array &$syntax) {
        # 获取基本表的声明信息。
        $stmt = $this->_stmtTableAlias($syntax);
        if (isset($syntax[self::SYNTAX_LINK])) {
            foreach ((array)$syntax[self::SYNTAX_LINK] AS $name) {
                # 如果指定关系没有配设置，引发语法异常。
                if (!isset($syntax[self::SYNTAX_JOIN][$name])) {
                    throw new DBSyntaxException('SqlBuilder: Not support join ON syntax in "' . $name . '"');
                }

                # 获取配置值并生成连接语法。
                $cfg = $syntax[self::SYNTAX_JOIN][$name];
                $stmt .= ',' . $this->_stmtTableAlias($cfg);
            }
        }

        return $stmt;
    }

    /**
     * 获取语法表别名信息。
     *
     * @param array $syntax 命令语法。
     *
     * @return string
     */
    protected function _stmtTableAlias(array &$syntax) {
        # 如果语法中存在表别名信息。
        if (isset($syntax[self::SYNTAX_TABLE_ALIAS])) {
            return $this->safeName($syntax[self::SYNTAX_TABLE_ALIAS]);
        }

        return $this->safeName($syntax[self::SYNTAX_TABLE]);
    }

    /**
     * 根据$syntax返回表的声明信息。
     *
     * @param array $syntax 语法参数。
     *
     * @return mixed|string
     *
     * @throws DBSyntaxException
     */
    protected function _stmtTable(array &$syntax) {
        # 如果没有设置table语法。
        if (!isset($syntax[self::SYNTAX_TABLE])) {
            throw new DBSyntaxException('SqlBuilder:Not support table syntax');
        }

        # 如果包含有表别名。
        if (isset($syntax[self::SYNTAX_TABLE_ALIAS])) {
            return $this->safeName($syntax[self::SYNTAX_TABLE]) . ' AS ' . $this->safeName($syntax[self::SYNTAX_TABLE_ALIAS]);
        } else {
            return $this->safeName($syntax[self::SYNTAX_TABLE]);
        }
    }

    /**
     * 根据$fields和$syntax生成where条件语句。
     *
     * @param array $args   更新参数集。
     * @param array $syntax 语法。
     *
     * @return string
     */
    protected function _stmtWhere(array &$args, array &$syntax) {
        # 如果没有设置条件，直接返回。
        if ($where = array_key_value($syntax, self::SYNTAX_WHERE)) {
            # 如果字符串条件，直接合并返回。
            if (!is_array($where)) return ' WHERE ' . $where;

            # 如果采用参数条件命令。
            if ($stmt = array_key_pop($where, 0)) {
                array_append($args, $where);
                return ' WHERE ' . $stmt;
            }

            $stmt = ' WHERE ';
            foreach ($where as $field => $value) {
                if (is_array($value)) {
                    array_append($args, $value);
                    $stmt .= ($field . ' IN (' . $this->_stmtListArgs(count($value)) . ') AND ');
                } else if ($value instanceof NormalParameter) {
                    $stmt .= (strpos($field, ' ') ? ($field . ' ' . $value . ' AND ') :
                        ($this->safeName($field) . ' = ' . $value . ' AND '));
                } else {
                    array_push($args, $value);
                    $stmt .= (strpos($field, ' ') ? ($field . ' ? AND ') : ($this->safeName($field) . ' = ? AND '));
                }
            }

            return substr($stmt, 0, -5);
        }

        return '';
    }

    /**
     * 根据$table、$fields和$values生成INSERT命令。
     *
     * @param string $table  表名称。
     * @param string $fields 字段。
     * @param string $values 值。
     *
     * @return string
     */
    protected function _stmtInsert($table, $fields, $values) {
        return "INSERT INTO $table($fields) VALUES($values)";
    }

    /**
     * 根据$fields生成字段列的值。
     *
     * @param array $fields 字段集。
     *
     * @return string
     */
    protected function _stmtListFields(array $fields) {
        return implode(',', array_map(array($this, 'safeName'), $fields));
    }

    /**
     * 根据$count生成参数数组集。
     *
     * @param int $fieldCount 字段数量。
     *
     * @return string
     */
    protected function _stmtListArgs($fieldCount) {
        return substr(str_repeat('?,', $fieldCount), 0, -1);
    }

    /**
     * 根据$table、$fields生成更新UPDATE命令。
     *
     * @param string $table  表名称。
     * @param string $fields 字段集。
     * @param string $where  条件。
     *
     * @return string
     */
    protected function _stmtUpdate($table, $fields, $where) {
        return "UPDATE $table SET $fields $where";
    }

    /**
     * 根据$fields生成更新字段值声明并返回。
     *
     * @param array $fields 字段集。
     *
     * @return string
     */
    protected function _stmtUpdateFields(array &$fields) {
        # 使用更新参数：array('group = ?,allow = ?', 1, 0)。
        if ($stmt = array_key_pop($fields, 0)) return $stmt;

        $stmt = '';
        foreach (array_keys($fields) as $field) {
            $stmt .= ($this->safeName($field) . ' = ? ,');
        }

        return substr($stmt, 0, -1);
    }

    /**
     * 根据$table和$where生成删除命令。
     *
     * @param string $table     表格名称。
     * @param string $fullTable 完整表名。
     * @param string $where     条件声明。
     *
     * @return string
     */
    protected function _deleteTable($table, $fullTable, $where) {
        return "DELETE $table FROM $fullTable $where";
    }

    /**
     * 根据$table、$fields、$where生成查询条件。
     *
     * @param string $table  表格名称。
     * @param string $fields 字段名称。
     * @param string $where  条件。
     * @param string $group  分组
     * @param string $order  排序。
     * @param string $page   分页。
     *
     * @return string
     */
    protected function _stmtFetch($table, $fields, $where = '', $group = '', $order = '', $page = '') {
        return "SELECT $fields FROM {$table}{$where}{$group}{$order}{$page}";
    }

    /**
     * 根据$fields生成查询的字段集。
     *
     * @param array $syntax 查询语法。
     *
     * @return string
     */
    protected function _stmtFetchFields(array &$syntax) {
        # 如果没有设置查询字段或排除字段，直接返回。
        if ($fields = array_key_value($syntax, self::SYNTAX_FIELDS)) {
            # 如果列为字符串，则直接返回值。
            if (is_string($fields)) return $fields;

            return implode(',', array_map(array($this, 'safeName'), $fields));
        }

        return '*';
    }

    /**
     * 根据$syntax生成分组信息。
     *
     * @param array $syntax 语法参数。
     *
     * @return string
     */
    protected function _stmtGroup(array &$syntax) {
        # 从语法中取出分组信息。
        if ($group = array_key_value($syntax, self::SYNTAX_GROUP)) {
            if (is_string($group)) return ' GROUP BY ' . $group;

            $stmt = ' GROUP BY ';
            foreach ($group as $field) {
                $stmt .= ($this->safeName($field) . ',');
            }

            echo $stmt;

            return substr($stmt, 0, -1);
        }

        return '';
    }

    /**
     * 根据$syntax生成排序信息。
     *
     * @param array $syntax 语法。
     *
     * @return string
     */
    protected function _stmtOrder(array &$syntax) {
        # 从语法数据中取出排序方式。
        if ($order = array_key_value($syntax, self::SYNTAX_ORDER)) {
            # 如果指定排序方式为字符串，则直接返回排序声明。
            if (is_string($order)) return ' ORDER BY ' . $order;

            $stmt = ' ORDER BY ';
            foreach ($order as $field => $dir) {
                $stmt .= ($this->safeName($field) . ' ' . $dir . ',');
            }

            return substr($stmt, 0, -1);
        }

        return '';
    }

    /**
     * 根据$syntax生成分页信息。
     *
     * @param array $syntax 语法。
     *
     * @return string
     */
    protected function _stmtPage(array &$syntax) {
        # 如果语法中包含有分页信息，设置limit值。
        if ($page = array_key_value($syntax, self::SYNTAX_PAGE)) {
            list($page, $size) = $page;
            return sprintf(' LIMIT %d,%d', ($page - 1) * $size, $size);
        }

        return '';
    }
}