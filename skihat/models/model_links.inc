<?php
/**
 * model_links.inc 模型关系异常连接处理声明文件。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.models
 * @version     1.0
 *
 * @create      2012-10-07
 * @modify      2013-09-24
 */

/**
 * 模型关系连接对象，提供关系对象的异步加载和值的设置。
 *
 * @package skihat.models
 */
interface IModelLink extends IDataSupport {
    /**
     * 根据$model和$option异步读取模型值。
     *
     * @param Model $model  当前模型实例。
     * @param array $syntax 关系选项。
     *
     * @return mixed 异步读取的值。
     */
    function read(Model $model, array $syntax);

    /**
     * 根据$models、$relModel和$option异步设置模型值。
     *
     * @param Model $models     当前模型。
     * @param mixed $linkModel 关系模型对象。
     * @param array $syntax    选项。
     *
     * @return mixed

    function write(Model $models, $linkModel, array $syntax);    */
}

/**
 * 多对一关系模型，实现多对一关系的读写操作。
 *
 * @package skihat.models
 */
class BelongsToModelLink implements IModelLink {
    /**
     * @see IModelLink::read
     */
    public function read(Model $model, array $syntax) {
        # 获取关联模型名称和外键的值。
        $linkModelName = Skihat::using($syntax[IModelMetaSupport::META_LINKS_CLASS]);
        $linkForeign = $model->$syntax[IModelMetaSupport::META_LINKS_FOREIGN];

        # 如果外键值不为空。
        if (!empty($linkForeign)) {
            $linkModel = $linkModelName::fetch(array($linkModelName::primaryKey() => $linkForeign), $syntax);
            return $linkModel ? $linkModel : NULL;
        }

        return NULL;
    }

    /**
     * @seeIModelLink::write
     *
     * public function write(Model $models,$linkModel,array $syntax) {
     * # 获取外键列表值。
     * $foreign = $syntax[IModelMetaSupport::META_LINKS_FOREIGN];
     * if (empty($linkModel)) {
     * $models->$foreign = NULL;
     * } else {
     * $models->$foreign =$linkModel->key();
     * }
     * } */
}

/**
 * 一对一关系模型，实现一对一关系的读写操作。
 *
 * @package skihat.models
 */
class HasOneModelLink implements IModelLink {

    /**
     * @see IModelLink::read
     */
    public function read(Model $model, array $syntax) {
        # 获取关联模型名称和外键的值。
        $linkModelName = Skihat::using($syntax[IModelMetaSupport::META_LINKS_CLASS]);
        $foreign = $syntax[IModelMetaSupport::META_LINKS_FOREIGN];

        # 如果外键值不为空。
        $linkModel = $linkModelName::fetch(array($foreign => $model->key()), $syntax);
        return $linkModel ? $linkModel : NULL;
    }

    /**
     * @see IModel::write
     *
     * public function write(Model $models,$linkModel,array $syntax) {
     * throw new ModelLinkException('JTA-HasOneModelLink:Has one link not support write function.');
     * } */
}

/**
 * 一对多关系模型，实现一对多关系的读写操作。
 *
 * @package skihat.models
 */
class HasManyModelLink implements IModelLink {

    /**
     * @see IModelLink::read
     */
    public function read(Model $model, array $syntax) {
        $linkModelName = Skihat::using($syntax[IModelMetaSupport::META_LINKS_CLASS]);
        $foreign = $syntax[IModelMetaSupport::META_LINKS_FOREIGN];

        return $linkModelName::fetchAll(array($foreign => $model->key()), $syntax);
    }
}

/**
 * 多对多关系模型查找器，实现多对多关系的更多操作。
 *
 * @package skihat.models
 */
class HasAndBelongsModelFetch extends ModelFetch {
    /**
     * @var Model 关系模型相关实例。
     */
    public $model = null;

    /**
     * 根据$modelName、$syntax和$model初始化HasAndBelongsModelFetch类实例。
     *
     * @param string $modelName 模型名称。
     * @param array  $syntax    语法。
     * @param Model  $model     相关模型。
     */
    public function __construct($modelName, $syntax, $model) {
        parent::__construct($modelName, $syntax);
        $this->model = $model;
    }

    /**
     * 附加新的关系模型实例。
     */
    public function add($models) {
        if (empty($models)) return 0;

        $syntax = $this->_syntax;
        $fields = array(
            $syntax[IModelMetaSupport::META_LINKS_FOREIGN],
            $syntax[IModelMetaSupport::META_LINKS_TABLE_FOREIGN]
        );

        $values = array();
        $key = $this->model->key();
        foreach ((array)$models as $model) {
            $values[] = array($key, $model);
        }

        return $this->_database()->createMultiple($fields, $values, array(
            self::SYNTAX_TABLE => $syntax[IModelMetaSupport::META_LINKS_TABLE]
        ));
    }

    /**
     * 移除关系模型实例。
     */
    public function remove($models) {
        $syntax = $this->_syntax;
        return $this->_database()->delete(array(self::SYNTAX_TABLE => $syntax[IModelMetaSupport::META_LINKS_TABLE],
            self::SYNTAX_WHERE => array($syntax[IModelMetaSupport::META_LINKS_TABLE_FOREIGN] => (array)$models)));
    }

    /**
     * 替换关系模型实例。
     */
    public function replace($models) {
        $syntax = $this->_syntax;
        $database = $this->_database();

        # 执行移除角色和分配角色。
        if (empty($models)) {
            return $database->delete(array(self::SYNTAX_TABLE => $syntax[IModelMetaSupport::META_LINKS_TABLE],
                self::SYNTAX_WHERE => array($syntax[IModelMetaSupport::META_LINKS_FOREIGN] => $this->model->key())));
        } else {
            $rows = $database->delete(array(self::SYNTAX_TABLE => $syntax[IModelMetaSupport::META_LINKS_TABLE],
                self::SYNTAX_WHERE => array($syntax[IModelMetaSupport::META_LINKS_FOREIGN] => $this->model->key(),
                    $syntax[IModelMetaSupport::META_LINKS_TABLE_FOREIGN] . ' NOT IN ' => new NormalParameter('(' . implode(',', (array)$models) . ')'))));

            $dbKeys = $database->fetchAll(array(self::SYNTAX_TABLE => $syntax[IModelMetaSupport::META_LINKS_TABLE],
                self::SYNTAX_FIELDS => array($syntax[IModelMetaSupport::META_LINKS_FOREIGN])), self::FETCH_STYLE_COLUMN);

            $models = array_diff((array)$models, array_keys($dbKeys));
            return $rows + $this->add($models);
        }
    }
}

/**
 * 多对多关系模型，实现多对多关系的读写操作。
 */
class HasAndBelongsModelLink implements IModelLink {
    /**
     * @see IModelLink::read
     */
    public function read(Model $model, array $syntax) {
        $linkModelName = Skihat::using($syntax[IModelMetaSupport::META_LINKS_CLASS]);
        $field = sprintf('%s.%s IN', $syntax[IModelMetaSupport::META_TABLE_ALIAS], $linkModelName::primaryKey());
        $value =
            new NormalParameter(sprintf('(SELECT %s FROM %s WHERE %s = %s)', $syntax[IModelMetaSupport::META_LINKS_TABLE_FOREIGN], $syntax[IModelMetaSupport::META_LINKS_TABLE], $syntax[IModelMetaSupport::META_LINKS_FOREIGN], $model->key()));

        return new HasAndBelongsModelFetch($linkModelName, $linkModelName::defaultSyntax($syntax, array($field => $value)), $model);
    }
}

/**
 * 模型关系工厂，提供通过模型属性异步读取关系对象的服务。
 *
 * @package skihat.models
 */
class ModelLinkFactory {

    /**
     * @var array 模型关系处理实例对象。
     */
    protected static $_links = array();

    /**
     * 根据$name和$link注册模型关系实例。
     *
     * @param string     $name 关系名称。
     * @param IModelLink $link 处理实例。
     *
     * @return void
     */
    public static function register($name, IModelLink $link) {
        self::$_links[$name] = $link;
    }

    /**
     * 根据$syntax和$model读取模型的关联对象。
     *
     * @param array $syntax 模型关系声明。
     * @param Model $model  模型实例。
     *
     * @return mixed
     *
     * @throws ModelLinkException 如果指定模型类型不存在则引发异常。
     */
    public static function read(Model $model, array $syntax) {
        $type = array_key_value($syntax, IModelMetaSupport::META_LINKS_TYPE, Model::HAS_ONE);

        if (isset(self::$_links[$type])) {
            return self::$_links[$type]->read($model, $syntax);
        }

        throw new ModelLinkException('ModelLinkFactory:Not support models link type ' . $type);
    }

}