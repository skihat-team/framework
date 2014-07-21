<?php
/**
 * model_fetch.inc 模型检索类声明文件。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.models
 * @version     1.0
 *
 * @created     2012-07-23
 * @modify      2013-09-22
 */

/**
 * 模型检索器，提供模型的检索查找服务。
 *
 * @package skihat.models
 */
class ModelFetch implements IDataSupport {
    /**
     * @const int 默认分页大小。
     */
    const DEFAULT_PAGE_SIZE = 20;

    /**
     * 根据$model和$syntax构造ModelFetch实例。
     *
     * @param string $modelName 模型名称。
     * @param array  $syntax    查寻。
     */
    public function __construct($modelName, array $syntax) {
        $this->_syntax = $syntax;
        $this->_modelName = $modelName;
    }

    /**
     * 根据$fields设置或移除检索列表的值。
     *
     * @param string|array $fields 字段列表。
     *
     * @return ModelFetch
     */
    public function fields($fields) {
        if (empty($fields)) {
            unset($this->_syntax[self::SYNTAX_FIELDS]);
        } else {
            $this->_syntax[self::SYNTAX_FIELDS] = $fields;
        }

        return $this;
    }

    /**
     * 根据$links参数设置或移除连接表关系。
     *
     * @param mixed $joins 关系名称。
     *
     * @return ModelFetch
     */
    public function links($joins) {
        if (empty($joins)) {
            unset($this->_syntax[self::SYNTAX_LINK]);
        } else {
            $this->_syntax[self::SYNTAX_LINK] = $joins;
        }

        return $this;
    }

    /**
     * 根据$where设置或移除检索条件。
     *
     * @param mixed $where 检索条件。
     *
     * @return ModelFetch
     */
    public function where($where) {
        if (empty($where)) {
            unset($this->_syntax[self::SYNTAX_WHERE]);
        } else {
            $this->_syntax[self::SYNTAX_WHERE] = $where;
        }

        return $this;
    }

    /**
     * 根据$group设置或移除尘分组信息。
     *
     * @param mixed $group 分组参数。
     *
     * @return ModelFetch
     */
    public function group($group) {
        if (empty($group)) {
            unset($this->_syntax[self::SYNTAX_GROUP]);
        } else {
            $this->_syntax[self::SYNTAX_GROUP] = $group;
        }

        return $this;
    }

    /**
     * 根据$order设置或移除排序方式。
     *
     * @param mixed $order 排序方式。
     *
     * @return ModelFetch
     */
    public function order($order) {
        if (empty($order)) {
            unset($this->_syntax[self::SYNTAX_ORDER]);
        } else {
            $this->_syntax[self::SYNTAX_ORDER] = $order;
        }

        return $this;
    }


    /**
     * 根据$page和$size设置或移除分页值信息。
     *
     * @param int $page 当前页。
     * @param int $size 分页大小。
     *
     * @return ModelFetch
     */
    public function page($page, $size = self::DEFAULT_PAGE_SIZE) {
        if (empty($page)) {
            unset($this->_syntax[self::SYNTAX_PAGE]);
        } else {
            $this->_syntax[self::SYNTAX_PAGE] = array($page, $size);
        }

        return $this;
    }

    /**
     * 根据当前语法命令，返回模型结果。
     *
     * @return array
     */
    public function fetchModels() {
        $modelName = $this->_modelName;
        $models = $this->_fetchAll(self::FETCH_STYLE_MODEL);

        # 返回创建模型集。
        return $modelName::createModels($models, $this->_syntax);
    }

    /**
     * 根据当前语法命令，返回标准对象值。
     *
     * @return array
     */
    public function fetchObjects() {
        return $this->_fetchAll(self::FETCH_STYLE_OBJ);
    }

    /**
     * 根据当前命令语法，返回数组结果。
     *
     * @return array
     */
    public function fetchRows() {
        return $this->_fetchAll(self::FETCH_STYLE_ASSOC);
    }

    /**
     * 返回数据的json格式。
     *
     * @return string
     */
    public function fetchJson() {
        $rows = $this->_fetchAll(self::FETCH_STYLE_ASSOC);
        return json_encode($rows);
    }

    /**
     * 返回数据的xml格式。
     *
     * @return string
     */
    public function fetchXml() {
        # TODO: 暂不实现。
    }

    /**
     * 根据$field 返回索引值。
     *
     * @param bool|string $fields 字段列表。
     *
     * @return array
     */
    public function fetchList($fields = false) {
        # 如果指定有新的字段名称。
        if ($fields) $this->fields($fields);

        # 声明和获取值。
        $list = array();
        $rows = $this->_fetchAll(self::FETCH_STYLE_NUM);

        foreach ($rows as $row) {
            $list[$row[0]] = $row[1];
        }

        return $list;
    }

    /**
     * 返回结果集的数量。
     *
     * @return int
     */
    public function fetchCount() {
        $modelName = $this->_modelName;
        $count = $modelName::fetchField('COUNT(*) AS COUNT', array(), $this->_syntax);
        return $count ? $count : 0;
    }

    /**
     * 根据$name检索附加值，如果附加值不存在返回default值。
     *
     * @param string $name    附加值名称。
     * @param mixed  $default 默认值。
     *
     * @return mixed
     */
    public function extra($name, $default = false) {
        if (isset($this->_extras[$name])) {
            return $this->_extras[$name];
        }

        return $default;
    }

    /**
     * 为$name设置附加值$value，如果附加值已存在将覆盖已有值。
     *
     * @param string $name 附加值名字。
     * @param mixed  $val  附加值。
     */
    public function addExtra($name, $val) {
        $this->_extras[$name] = $val;
    }

    /**
     * 根据$name移除附加值选项。
     *
     * @param string $name 附加值名称。
     */
    public function removeExtra($name) {
        unset($this->_extras[$name]);
    }

    /**
     * 根据$fetch从数据模型中获取数据值。
     *
     * @param int $fetchStyle 检索方式。
     *
     * @return array
     */
    protected function _fetchAll($fetchStyle = self::FETCH_STYLE_ASSOC) {
        return $this->_database()->fetchAll($this->_syntax, $fetchStyle);
    }

    /**
     * 返回当前模型相关对象的数据引擎。
     *
     * @return IDatabaseEngine
     */
    protected function _database() {
        $modelName = $this->_modelName;
        return $modelName::database();
    }

    /**
     * @var array 检索语法。
     */
    protected $_syntax = null;

    /**
     * @var string 检索的相关模型名称。
     */
    protected $_modelName = null;

    /**
     * @var array 附加选项值。
     */
    protected $_extras = array();
}

