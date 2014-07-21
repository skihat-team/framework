<?php
/**
 * models.inc 提供模型类的声明和服务。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.models
 * @version     1.0
 *
 * @created     2012-07-23
 * @modify      2013-09-25
 */

/**
 * 模型异常，当模型发生异常情况时引发本异常。
 *
 * @package skihat.models
 */
class ModelException extends SkihatException {
}

/**
 * 未找到模型异常，当指定的模型未找到时引发异常。
 *
 * @package skihat.models
 */
class ModelNotFoundException extends ModelException {
}

/**
 * 模型关系异常，当模型关系在使用中发生错误时将引发本异常。
 *
 * @package skihat.models
 */
class ModelLinkException extends ModelException {
}

# 引用数据库实例。
Skihat::import('kernels.databases.database', SKIHAT_PATH_LIBRARY);

Skihat::import('models.model_fetch', SKIHAT_PATH_LIBRARY);
Skihat::import('models.model_links', SKIHAT_PATH_LIBRARY);
Skihat::import('models.model_meta', SKIHAT_PATH_LIBRARY);


/**
 * 根据模型和字段名称，判断字段的唯一性。
 *
 * @param Model $model 模型实例。
 * @param bool  $field 字段名称。
 *
 * @return boolean
 */
function unique_validator(Model $model, $field) {
    return $model->fetchUnique($field);
}

/**
 * MVC模型，提供使用ActiveRecord模式的模式支持。
 *
 * @package skihat.models
 */
abstract class Model implements ArrayAccess, IModelMetaSupport {
    /**
     * @const string 创建验证器。
     */
    const CREATE_RULES = 'createRules';

    /**
     * @const string 更新验证器。
     */
    const UPDATE_RULES = 'updateRules';

    /**
     * @const int 模型状态，创建。
     */
    const MODEL_NEW = 1;

    /**
     * @const int 模型状态，更新。
     */
    const MODEL_UPDATE = 2;

    /**
     * @const int 模型状态，删除。
     */
    const MODEL_DELETE = 3;

    /**
     * @const string 错误通知字段。
     */
    const ERROR_NOTICE = 'errorNotice';

    /**
     * @see IModelMetaSupport::config
     */
    public static function config() {
        return array();
    }

    /**
     * 使用$props、$forceUpdate和$forceLoad创建Model实例。
     *
     * @param mixed $props       属性值。
     * @param bool  $forceUpdate 强制采用更新。
     * @param bool  $forceLoad   强制从数据库中加载属性。
     *
     * @throws ModelException,ModelNotFoundException
     */
    public function __construct($props = array(), $forceUpdate = FALSE, $forceLoad = FALSE) {
        # 获取当前模型的数据KEY声明。
        $primaryKey = self::primaryKey();

        # 如果不是数组，则直接表示根据KEY获取当前对象；
        if (!is_array($props)) {
            $this->_initializeForceLoad($primaryKey, $props, array());
        } else if (!$forceUpdate) {
            $this->_initializeProps(array_merge($props, self::meta()->defaults), self::MODEL_NEW);
        } else if (empty($props[$primaryKey])) {
            throw new ModelException('Model:Force update not support key.');
        } else if ($forceLoad) {
            $this->_initializeForceLoad($primaryKey, $props[$primaryKey], $props);
        } else {
            $this->_initializeProps($props, self::MODEL_UPDATE);
        }
    }

    /**
     * 返回模型的主键标识信息。
     *
     * @return mixed
     */
    public function key() {
        return $this[self::primaryKey()];
    }

    /**
     * 返回一个布求值，判断当前状态是否为创建状态。
     *
     * @return bool
     */
    public function isNew() {
        return $this->_status == self::MODEL_NEW;
    }

    /**
     * 返回一个布尔值，判断当前状态是否为更新状态。
     *
     * @return bool
     */
    public function isUpdate() {
        return $this->_status == self::MODEL_UPDATE;
    }

    /**
     * 返回一个布求值，判断当前状态是否为删除状态。
     *
     * @return bool
     */
    public function isDelete() {
        return $this->_status == self::MODEL_DELETE;
    }

    /**
     * 魔法方法，读取属性的值。
     *
     * @param string $field 属性名称。
     *
     * @return mixed
     */
    public function __get($field) {
        return $this->_readProp($field);
    }

    /**
     * 魔法方法，设置属性值。
     *
     * @param string $field 属性名称。
     * @param mixed  $value 属性值。
     */
    public function __set($field, $value) {
        $this->_writeProp($field, $value);
    }

    /**
     * @see ArrayAccess::offsetExists
     */
    public function offsetExists($prop) {
        return isset($this->_currentProps[$prop]);
    }

    /**
     * @see ArrayAccess::offsetGet
     */
    public function offsetGet($prop) {
        return $this->_readProp($prop);
    }

    /**
     * @see ArrayAccess::offsetSet
     */
    public function offsetSet($prop, $value) {
        $this->_writeProp($prop, $value);
    }

    /**
     * @see ArrayAccess::offsetUnset
     */
    public function offsetUnset($prop) {
        unset($this->_currentProps[$prop]);
    }

    /**
     * 使用$props批量更新属性值。
     *
     * @param array $props 属性值。
     */
    public function changeProps(array $props) {
        $this->_currentProps = array_merge($this->_currentProps, $props);
    }

    /**
     * 获取$prop属性的初始值，如果$prop指定属性不存将返回null值。
     *
     * @param string $prop 属性名称。
     *
     * @return mixed
     */
    public function originalProp($prop) {
        return isset($this->_originalProps[$prop]) ? $this->_originalProps[$prop] : NULL;
    }

    /**
     * 返回全部属性的初始化值。
     *
     * @return array
     */
    public function originalProps() {
        return $this->_originalProps;
    }

    /**
     * 返回全部属性的当前值。
     *
     * @return array
     */
    public function currentProps() {
        return $this->_currentProps;
    }

    /**
     * 返回当前对象的json格式数据。
     *
     * @return string
     */
    public function json() {
        return json_encode($this->_currentProps);
    }

    /**
     * 返回当前对象属性值的xml格式数据。
     *
     * @return string

    public function xml() {
     * #
     * }*/


    /**
     * 验证当前值是否满足保存需求，成功返回true，失败返回false。
     *
     * @param mixed $onRule 验证规则。
     * @param array $syntax 执行语法。
     *
     * @return bool
     */
    public function validate($onRule = false, array &$syntax = array()) {
        # 根据状态获取验证方法
        if (!$onRule) {
            $onRule = $this->isNew() ? self::CREATE_RULES : self::UPDATE_RULES;
        }
        # 执行验证回调方法。
        if ($this->_validateBefore($onRule, $syntax)) {
            if ($this->_validateInvoke($onRule, $syntax)) {
                $this->_validateAfter($onRule, $syntax);
                $this->clearErrors();

                return true;
            }
        }

        return false;
    }

    /**
     * 将对象的更新保存到数据库保存方案中。
     *
     * @param array $syntax 附加选项。
     *
     * @return int 响应的行。
     *
     * @throws ModelException 如果保存对象为删除状态时将引发本异常。
     */
    public function save(array $syntax = array()) {
        # 如果当前对象已经删除。
        if ($this->isDelete()) {
            throw new ModelException('Model:Can \'t save deleted models.');
        }

        # 根据当前对象状态，判断执行的操作。
        if ($this->isNew()) {
            return $this->_create($syntax);
        } else {
            return $this->_update($syntax);
        }
    }

    /**
     * 返回到创建或上一次save时的状态。
     */
    public function cancel() {
        $this->_currentProps = $this->_originalProps;
    }

    /**
     * 根据$syntax删除对象实例，成功返回true，返回返回false。
     *
     * @param array $syntax 删除语法。
     *
     * @return bool
     *
     * @throws ModelException 如果删除模型为非更新模型。
     */
    public function delete(array $syntax = array()) {
        # 如果当前状态不是更新，则直接引发异常。
        if (!$this->isUpdate()) {
            throw new ModelException('Model:Delete not update models.');
        }

        # 执行删除方法。
        if ($this->_deleteBefore($syntax) !== FALSE) {
            if ($this->_deleteInvoke($syntax) !== FALSE) {
                $this->_deleteAfter($syntax);

                $this->_status = self::MODEL_DELETE;
                return TRUE;
            }
        }

        return FALSE;
    }

    /**
     * 使用$prop和$error增加字段的错误消息。
     *
     * @param string $prop  属性名字。
     * @param string $error 错误消息。
     */
    public function addError($prop, $error) {
        # 如果指定属性未已经包含错误信息；
        if (!isset($this->_errors[$prop])) {
            $this->_errors[$prop] = (array)$error;
        } else {
            $this->_errors[$prop] = array_merge($this->_errors[$prop], $error);
        }
    }

    /**
     * 返回指定$prop属性的错误消息。
     *
     * @param string $prop 属性名字。
     *
     * @return string|mixed
     */
    public function error($prop) {
        return isset($this->_errors[$prop]) ? $this->_errors[$prop] : false;
    }

    /**
     * 返回模型的全部错误消息。
     *
     * @return array
     */
    public function allErrors() {
        return $this->_errors;
    }

    /**
     * 清除全部错误，并且使用$errors初始化新错误消息。
     *
     * @param array $errors 错误消息。
     *
     * @return $this 返回当前模型。
     */
    public function clearErrors($errors = array()) {
        # 如果初始化错误是为数组；
        if (is_array($errors)) {
            $this->_errors = $errors;
        } else {
            $this->_errors = array(self::ERROR_NOTICE => array($errors));
        }

        return $this;
    }

    /**
     * 根据$syntax执行创建方法，并返回执行结果,true表示执行成功false表示失败。
     *
     * @param array $syntax 命令语法。
     *
     * @return bool
     */
    protected function _create(array &$syntax) {
        # 如果执行创建前回调方法成功，则继续执行。
        if ($this->_createBefore($syntax) !== FALSE) {
            if (!$this->validate(false, $syntax)) return false;
            if ($this->_createInvoke($syntax) !== FALSE) {
                $this->_createAfter($syntax);

                # 更新状态信息和原始值。
                $this->_status = self::MODEL_UPDATE;
                $this->_originalProps = $this->_currentProps;

                return TRUE;
            }
        }

        return FALSE;
    }

    /**
     * 创建回调方法，如果返回值为false，则表示中断创建，默认返回为true。
     *
     * @param array $syntax
     *
     * @return bool
     */
    protected function _createBefore(array &$syntax) {
        return TRUE;
    }

    /**
     * 根据$syntax执行创建工作，并返回结果，如果创建成功返回true，失败返回false。
     *
     * @param array $syntax 语法命令。
     *
     * @return bool
     */
    protected function _createInvoke(array &$syntax) {
        # 只读属性
        $meta = & self::meta();
        if (isset($meta->schemas['created'])) {
            $this->_currentProps['created'] = time();
        }
        if (isset($meta->schemas['modified'])) {
            $this->_currentProps['modified'] = time();
        }

        # 从当前属性中获取表的属性集合。
        $currProps = array_intersect_key($this->_currentProps, $meta->schemas);

        # 执行数据库命令。
        $database = self::database();
        $database->create($currProps, self::defaultSyntax($syntax));

        # 判断是否为自动更新主键。
        if (self::meta()->primaryIncrement) {
            $this->_currentProps[self::primaryKey()] = $database->fetchLastID();
        }

        return TRUE;
    }

    /**
     * 当执行创建完成后，执行回调方法。
     *
     * @param array $syntax 命令语法。
     *
     * @return void
     */
    protected function _createAfter(array &$syntax) {
    }

    /**
     * 根据$syntax执行更新操作。
     *
     * @param array $syntax 命令语法。
     *
     * @return bool
     */
    protected function _update(array &$syntax) {
        # 执行更新回调用方法。
        if ($this->_updateBefore($syntax) !== FALSE) {
            if (!$this->validate(false, $syntax)) return false;
            if ($this->_updateInvoke($syntax) !== FALSE) {
                $this->_updateAfter($syntax);

                # 更新原始值，并返回结果。
                $this->_originalProps = $this->_currentProps;
                return TRUE;
            }
        }

        return FALSE;
    }

    /**
     * 根据$syntax执行更新前回调，返回true继续执行,false中断更新操作。
     *
     * @param array $syntax 更新语法。
     *
     * @return bool
     */
    protected function _updateBefore(array &$syntax) {
        return TRUE;
    }

    /**
     * 根据$syntax执行数据库更新操作，更新成功返回true，失败返回false。
     *
     * @param array $syntax 更新语法。
     *
     * @return bool
     */
    protected function _updateInvoke(array &$syntax) {
        # 获取当前属性，并去除更新主键值。
        $currProps = $this->_currentProps;
        unset($currProps[self::primaryKey()]);

        # 只读属性
        $meta = & self::meta();
        if (isset($meta->schemas['modified'])) {
            $currProps['modified'] = time();
        }

        $currProps = array_intersect_key($currProps, $meta->schemas);
        if (!empty($meta->readonly)) {
            $currProps = array_diff_key($currProps, array_flip($meta->readonly));
        }

        # 执行数据库命令。
        return self::database()->update($currProps, self::defaultSyntax($syntax, array($meta->tableAlias . '.' . self::primaryKey() => $this->key())));
    }

    /**
     * 根据$syntax执行更新完成后回调方法。
     *
     * @param array $syntax 更新语法。
     *
     * @return bool
     */
    protected function _updateAfter(array &$syntax) {
    }

    /**
     * 根据$syntax执行删除回调方法，返回true表示继续执行删除，返回false中断删除。
     *
     * @param array $syntax 删除命令语法。
     *
     * @return bool
     */
    protected function _deleteBefore(array &$syntax) {
        return TRUE;
    }

    /**
     * 根据$syntax执行删除方法，返回非false值表示删除成功。
     *
     * @param array $syntax 删除命令语法。
     *
     * @return bool
     */
    protected function _deleteInvoke(array &$syntax) {
        return self::database()->delete(self::defaultSyntax($syntax, array(self::primaryKey() => $this->key())));
    }

    /**
     * 根据$syntax执行验证前回调，返回false表示验证前失败。
     *
     * @param mixed $onRule 验证规则。
     * @param array $syntax 执行语法。
     *
     * @return bool
     */
    protected function _validateBefore($onRule, array &$syntax) {
        return true;
    }

    /**
     * 根据$syntax执行验证方法，返回false表示验证失败。
     *
     * @param mixed $onRule 验证规则。
     * @param array $syntax 执行语法。
     *
     * @return bool
     */
    protected function _validateInvoke($onRule, array &$syntax) {
        # 如果验证类还未引用，则引用类。
        if (!function_exists('validate_validators')) {
            Skihat::import('kernels.validators', SKIHAT_PATH_LIBRARY);
        }

        # 执行当前对象的验证方法
        if ($errors = self::validateRule($onRule, $this)) {
            $this->clearErrors($errors);
            return false;
        }

        return true;
    }

    /**
     * 根据$syntax执行验证后回调。
     *
     * @param mixed $onRule 验证规则。
     * @param array $syntax 验证语法。
     *
     * @return void
     */
    protected function _validateAfter($onRule, array &$syntax) {
    }

    /**
     * 根据$syntax执行删除完成后回调方法。
     *
     * @param array $syntax 删除命令语法。
     */
    protected function _deleteAfter(array &$syntax) {
    }

    /**
     * 获取模型的完整元数据声明。
     *
     * @return ModelMeta
     */
    public static function & meta() {
        return ModelMeta::instance(get_called_class());
    }

    /**
     * 返回当前模型的主键名称。
     *
     * @return string
     */
    public static function primaryKey() {
        return self::meta()->primaryKey;
    }

    /**
     * 根据$modelRows和$syntax创建模型对象集。
     *
     * @param array $modelRows 模型分行集。
     * @param array $syntax    模型语法。
     *
     * @return array
     */
    public static function createModels(array $modelRows, array $syntax) {
        $modelAlias = $syntax[self::SYNTAX_TABLE_ALIAS];
        $modelLinks = self::_createModelLinks($syntax);
        $modelName = get_called_class();
        $models = array();

        foreach ($modelRows as $row) {
            $model = new $modelName($row[$modelAlias], TRUE);

            # 处理同步加载实例对象。
            foreach ($modelLinks as $name => $link) {
                # 如果记录集中包含有这个列的记录值，则生成关系模型对象。
                $relAlias = $link[self::SYNTAX_TABLE_ALIAS];

                if (isset($row[$relAlias])) {
                    $relationModel = $link['relModel'];
                    $model->{$name} = new $relationModel($row[$relAlias], TRUE);
                }
            }

            array_push($models, $model);
        }

        return $models;
    }

    /**
     * 根据$modelRows和$syntax创建单个模型实例。
     *
     * @param array $modelRows 模型二维数组。
     * @param array $syntax    命令语法。
     *
     * @return mixed
     */
    public static function createModel(array $modelRows, array $syntax) {
        $models = self::createModels($modelRows, $syntax);
        return $models[0];
    }

    /**
     * 根据$fields和$syntax创建模型记录值，并返回创建的数量。
     *
     * @param array $fields 模型字段值，二维数组。
     * @param array $syntax 附加语法。
     *
     * @return int
     */
    public static function createAll(array $fields, array $syntax = array()) {
        return self::database()->create($fields, self::defaultSyntax($syntax));
    }

    /**
     * 根据$fields、$values和$syntax创建多条模型记录，并返回创建的行的数量。
     *
     * @param array $fields 字段名称。
     * @param array $values 字段值，注意必须为二维数据。
     * @param array $syntax 创建语法；
     *
     * @return mixed
     */
    public static function createMultiple(array $fields, array $values, $syntax = array()) {
        return self::database()->createMultiple($fields, $values, self::defaultSyntax($syntax));
    }

    /**
     * 根据$fields、$conditions和$syntax更新多条记录，并返回更新的数量。
     *
     * @param mixed $fields     更新的字段值。
     * @param mixed $conditions 更新条件。
     * @param array $syntax     附加语法。
     *
     * @return int 影响的记录数。
     */
    public static function updateAll($fields, $conditions, array $syntax = array()) {
        return self::database()->update($fields, self::defaultSyntax($syntax, $conditions));
    }

    /**
     * 根据$conditions和$syntax删除多条记录，并返回影响的行。
     *
     * @param mixed $conditions 删除条件。
     * @param array $syntax     附加语法。
     *
     * @return int 响应的记录数。
     */
    public static function deleteAll($conditions, array $syntax = array()) {
        return self::database()->delete(self::defaultSyntax($syntax, $conditions));
    }

    /**
     * 使用$conditions和$syntax查询多条记录，并返回模型查询对象。
     *
     * @param mixed $conditions 查询条件。
     * @param array $syntax     附加语法支持。
     *
     * @return ModelFetch
     */
    public static function fetchAll($conditions = array(), array $syntax = array()) {
        return new ModelFetch(get_called_class(), self::defaultSyntax($syntax, $conditions));
    }

    /**
     * 根据$field、$conditions和$syntax查询单个字段的值。
     *
     * @param  string $field      字段名称。
     * @param  mixed  $conditions 查询条件。
     * @param array   $syntax     查询语法。
     *
     * @return mixed
     */
    public static function fetchField($field, $conditions, array $syntax = array()) {
        $syntax = self::defaultSyntax($syntax, $conditions);
        $syntax[self::SYNTAX_FIELDS] = $field;

        $rows = self::database()->fetchAll($syntax, self::FETCH_STYLE_NUM);
        return empty($rows) ? false : $rows[0][0];
    }

    /**
     * 返回当前$field的字段判断，是否是唯一值。
     *
     * @param string $field 字段名称；
     *
     * @return boolean
     */
    public function fetchUnique($field) {
        # 如果字段的值为空，直接返回true。
        if (empty($this->_currentProps[$field])) return true;

        if ($this->isNew()) {
            $row = self::fetchField($field, array($field => $this[$field]), array(self::SYNTAX_PAGE => 1));
        } else {
            $row = self::fetchField($field, array(
                self::primaryKey() . ' <> ' => $this->key(),
                $field => $this[$field]), array(self::SYNTAX_PAGE => 1));
        }

        return empty($row);
    }

    /**
     * 使用$conditions和$syntax查询单个模型记录，如果有多个记录将什么满足条件的第一个记录。
     *
     * @param mixed $conditions 查询条件。
     * @param array $syntax     查询语法。
     * @param int   $fetchStyle 查询数据格式。
     *
     * @return mixed
     */
    public static function fetch($conditions = array(), array $syntax = array(), $fetchStyle = self::FETCH_STYLE_MODEL) {
        # 处理分页和条件命令。
        $syntax = self::defaultSyntax($syntax, $conditions);
        $syntax[self::SYNTAX_PAGE] = array(1, 1);

        # 读取数据库的值，并返回。
        $rows = self::database()->fetchAll($syntax, $fetchStyle);

        # 返回数据
        if (empty($rows)) return false;
        if ($fetchStyle != self::FETCH_STYLE_MODEL) return $rows[0];
        else return self::createModel($rows, $syntax);
    }


    /**
     * 使用$cmd和$args进行查询操作，结果结果为二维数组。
     *
     * @param string $cmd        查询命令。
     * @param array  $args       查询参数。
     * @param int    $fetchStyle 检索数据的格式。
     *
     * @return array
     */
    public static function query($cmd, array $args = array(), $fetchStyle = self::FETCH_STYLE_ASSOC) {
        return self::database()->query($cmd, $args, $fetchStyle);
    }

    /**
     * 使用$cmd、$args和$options执行命令，并返回影响到的行。
     *
     * @param string $cmd  查询命令。
     * @param array  $args 查询命令。
     *
     * @return int
     */
    public static function execute($cmd, array $args = array()) {
        return self::database()->execute($cmd, $args);
    }

    /**
     * 根据$on和$model验证$model实例是否满足当前模型验证规则。
     *
     * @param string $onRule 验证规则。
     * @param Model  $model  验证实例对象。
     *
     * @return bool|array
     */
    public static function validateRule($onRule, Model $model) {
        # 如果指定$onRule包含验证器，则调用验证组件进行验证。
        if ($validators = array_key_value(self::meta()->validators, $onRule)) {
            return validate_validators($model->_currentProps, $validators, self::meta()->validatorMessages);
        }

        return array();
    }

    /**
     * 返回当前模型对象的数据库引擎信息。
     *
     * @return IDatabaseEngine
     */
    public static function database() {
        return Database::engine(self::meta()->database);
    }

    /**
     * 根据$syntax和$conditions合并语法默认值。
     *
     * @param array $syntax     指定语法结构。
     * @param mixed $conditions 条件内容。
     *
     * @return array
     */
    public static function defaultSyntax(array $syntax = array(), $conditions = FALSE) {
        # 获取表名称和别名。
        $meta = self::meta();
        $syntax[self::SYNTAX_JOIN] = $meta->links;

        # 如果没有设置表名和别名，则设置默认值。
        if (isset($syntax[self::SYNTAX_TABLE]) == FALSE) {
            $syntax[self::SYNTAX_TABLE] = $meta->table;
        }
        if (isset($syntax[self::SYNTAX_TABLE_ALIAS]) == FALSE) {
            $syntax[self::SYNTAX_TABLE_ALIAS] = $meta->tableAlias;
        }

        # 如果包含有指定条件。
        if ($conditions) {
            # 如果条件为数字，则直接使用ID条件。
            if (is_numeric($conditions)) {
                $conditions = array($meta->primaryKey => intval($conditions));
            }

            # 如果语法中已指定条件参数，并且条件为数组参数。
            if (isset($syntax[self::SYNTAX_WHERE]) && is_array($syntax[self::SYNTAX_WHERE])) {
                $conditions = array_merge($syntax[self::SYNTAX_WHERE], $conditions);
            }

            $syntax[self::SYNTAX_WHERE] = $conditions;
        }

        return $syntax;
    }

    /**
     * 根据$keyName、$key和$props初始化当前实例为强制更新状态。
     *
     * @param string $keyName 主键名称。
     * @param mixed  $key     主键值。
     * @param array  $props   初始化属性。
     *
     * @throws ModelNotFoundException
     */
    protected function _initializeForceLoad($keyName, $key, array $props = array()) {
        # 根据主键条件生成查询语法，并并指定返回值。
        $syntax = self::defaultSyntax(array(), array($keyName => $key));
        $syntax[self::SYNTAX_PAGE] = array(1, 1);

        $rows = self::database()->fetchAll($syntax, IDataSupport::FETCH_STYLE_ASSOC);
        if ($rows) {
            $this->_originalProps = $rows[0];
            $this->_status = self::MODEL_UPDATE;
            $this->_currentProps = array_merge($this->_originalProps, $props);
        } else {
            throw new ModelNotFoundException('Model:Not found ' . $syntax[self::SYNTAX_TABLE_ALIAS] . ' models:' . $key);
        }
    }

    /**
     * 根据$props初始化当前状态。
     *
     * @param array $props  当前属性。
     * @param int   $status 当前状态。
     */
    public function _initializeProps(array $props, $status = self::MODEL_NEW) {
        $this->_originalProps = $props;
        $this->_currentProps = $props;

        $this->_status = $status;
    }

    /**
     * 根据$prop读取属性的值。
     *
     * @param string $prop 属性名称。
     *
     * @return mixed
     */
    protected function _readProp($prop) {
        # 如果当前属性中设置有相关值。
        if (isset($this->_currentProps[$prop])) {
            return $this->_currentProps[$prop];
        }

        $meta = self::meta();
        # 如果当前连接关系中存在值。
        if (isset($meta->links[$prop])) {
            return $this->_currentProps[$prop] = ModelLinkFactory::read($this, $meta->links[$prop]);
        }
        # 如果当前字段为虚拟字段。
        if (in_array($prop, $meta->virtual)) {
            return $this->{$prop}();
        }

        return NULL;
    }

    /**
     * 根据$prop和$value设置属性的值。
     *
     * @param string $prop  属性名称。
     * @param mixed  $value 属性值。
     */
    protected function _writeProp($prop, $value) {
        $meta = self::meta();

        if (in_array($prop, $meta->virtual)) {
            $this->{$prop}($value);
        }

        $this->_currentProps[$prop] = $value;
    }

    /**
     * 获取模型创建对象中指定的关系配置信息。
     *
     * @param array $syntax 语法命令。
     *
     * @return array
     */
    protected static function _createModelLinks(array &$syntax) {
        # 初始化已有的连接关系。
        $links = array();

        if (!empty($syntax[self::SYNTAX_LINK])) {
            foreach ((array)$syntax[self::SYNTAX_LINK] as $name) {
                if ($link = array_key_value($syntax[self::SYNTAX_JOIN], $name, FALSE)) {
                    if ($link[self::META_LINKS_TYPE] == self::HAS_ONE || $link[self::META_LINKS_TYPE] == self::BELONGS_TO) {
                        $link['relModel'] = Skihat::using($link[self::META_LINKS_CLASS]);
                        $links[$name] = $link;
                    }
                }
            }
        }

        return $links;
    }

    /**
     * @var array 当前属性值。
     */
    protected $_currentProps = array();

    /**
     * @var array 原始属性值。
     */
    protected $_originalProps = array();

    /**
     * @var int 模型状态信息。
     */
    protected $_status = self::MODEL_NEW;

    /**
     * @var array 错误消息列表。
     */
    protected $_errors = array();
}

ModelLinkFactory::register(Model::HAS_ONE, new HasOneModelLink());
ModelLinkFactory::register(Model::HAS_MANY, new HasManyModelLink());
ModelLinkFactory::register(Model::BELONGS_TO, new BelongsToModelLink());
ModelLinkFactory::register(Model::HAS_AND_BELONGS, new HasAndBelongsModelLink());