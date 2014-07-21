<?php
/**
 * model_test.inc 模型测试类，提供模型的单元测试服务。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.models
 * @version     1.0
 *
 * @created     2012-07-23
 * @modify      2013-09-03
 */

# 引入测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 导入被测试类。
Skihat::import('kernels.databases.database', SKIHAT_PATH_LIBRARY);
Skihat::import('models.model_meta', SKIHAT_PATH_LIBRARY);

/**
 * 模型元数据类单元测试。
 *
 * @package skihat.models
 */
class ModelMetaTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试构造器。
     */
    public function testConstructor() {
        $meta = new ModelMetaMock('ModelMetaSupportMock');

        $this->assertEquals('ModelMetaSupportMock', $meta->tableAlias);
        $this->assertEquals('model_meta_support_mock', $meta->table);

        $this->assertEquals(IModelMetaSupport::DEFAULT_PRIMARY_INCREMENT, $meta->primaryIncrement);
        $this->assertEquals(IModelMetaSupport::DEFAULT_PRIMARY_KEY, $meta->primaryKey);
        $this->assertEquals(Database::DEFAULT_ENGINE, $meta->database);
    }

    /**
     * 测试构造属性。
     */
    public function testConstructorProps() {
        $meta = new ModelMetaMock('ModelMetaSupportMockProps');

        $this->assertEquals('user', $meta->table);
        $this->assertEquals('User', $meta->tableAlias);
        $this->assertEquals('test', $meta->database);

        $this->assertFalse($meta->primaryIncrement);
        $this->assertEquals('idn', $meta->primaryKey);
    }

    /**
     * 测试构造属性，获取schemas的值。
     */
    public function testConstructorForSchemas() {
        $meta = new ModelMetaSchema('ModelMetaSupportMockProps');

        $this->assertEquals(5, count($meta->schemas));
        $this->assertEquals('id', $meta->schemas['id']['field']);
    }

    /**
     * 测试构造属性，关系连接值BelongsTo。
     */
    public function testConstructorLinksForBelongsToDefault() {
        $meta = new ModelMetaMock('ModelMetaUser');
        $group = $meta->links['Group'];

        $this->assertEquals(IModelMetaSupport::BELONGS_TO, $group[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('User.group_id = ModelMetaGroup.id', $group[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('group_id', $group[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('#ModelMetaGroup', $group[IModelMetaSupport::META_LINKS_CLASS]);

        $this->assertEquals('ModelMetaGroup', $group[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('group', $group[IModelMetaSupport::META_TABLE]);
    }

    /**
     * 测试构造属性，关系连接值BelongsTo。
     */
    public function testConstructorLinksForBelongsToAlias() {
        $meta = new ModelMetaMock('ModelMetaUser');

        $group = $meta->links['GroupAlias'];
        $this->assertEquals(IModelMetaSupport::BELONGS_TO, $group[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('User.group_id = GroupAlias.id', $group[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('group_id', $group[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('#ModelMetaGroup', $group[IModelMetaSupport::META_LINKS_CLASS]);

        $this->assertEquals('GroupAlias', $group[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('group', $group[IModelMetaSupport::META_TABLE]);
    }

    /**
     * 测试构造属性，关系连接值HAS_ONE。
     */
    public function testConstructorLinksForHasOneDefault() {
        $meta = new ModelMetaMock('ModelMetaGroup');

        $user = $meta->links['FirstUser'];
        $this->assertEquals(IModelMetaSupport::HAS_ONE, $user[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('User.group_id = ModelMetaGroup.id', $user[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('group_id', $user[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('#ModelMetaUser', $user[IModelMetaSupport::META_LINKS_CLASS]);

        $this->assertEquals('User', $user[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('user', $user[IModelMetaSupport::META_TABLE]);
    }

    /**
     * 测试构造属性，关系连接值BelongsTo。
     */
    public function testConstructorLinksForHasOneAlias() {
        $meta = new ModelMetaMock('ModelMetaGroup');

        $user = $meta->links['FirstUserAlias'];

        $this->assertEquals(IModelMetaSupport::HAS_ONE, $user[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('FirstUserAlias.group_id = ModelMetaGroup.id', $user[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('group_id', $user[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('#ModelMetaUser', $user[IModelMetaSupport::META_LINKS_CLASS]);

        $this->assertEquals('FirstUserAlias', $user[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('user', $user[IModelMetaSupport::META_TABLE]);
    }

    /**
     * 测试构造属性，关系连接值HAS_MANY。
     */
    public function testConstructorLinksForHasManyDefault() {
        $meta = new ModelMetaMock('ModelMetaGroup');

        $group = $meta->links['Users'];
        $this->assertEquals(IModelMetaSupport::HAS_MANY, $group[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('User.group_id = ModelMetaGroup.id', $group[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('group_id', $group[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('#ModelMetaUser', $group[IModelMetaSupport::META_LINKS_CLASS]);

        $this->assertEquals('User', $group[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('user', $group[IModelMetaSupport::META_TABLE]);
    }

    /**
     * 测试构造属性，关系连接值HAS_MANY。
     */
    public function testConstructorLinksForHasManyAlias() {
        $meta = new ModelMetaMock('ModelMetaGroup');
        $group = $meta->links['UsersAlias'];

        $this->assertEquals(IModelMetaSupport::HAS_MANY, $group[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('UsersAlias.group_id = ModelMetaGroup.id', $group[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('group_id', $group[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('#ModelMetaUser', $group[IModelMetaSupport::META_LINKS_CLASS]);

        $this->assertEquals('UsersAlias', $group[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('user', $group[IModelMetaSupport::META_TABLE]);
    }

    /**
     * 测试构造连接关系，关系HAS_AND_BELONGS。
     */
    public function testConstructLinksForHasAndBelongsDefault() {
        $meta = new ModelMetaMock('ModelMetaUser');
        $roles = $meta->links['Roles'];

        $this->assertEquals('#ModelMetaRole', $roles[IModelMetaSupport::META_LINKS_CLASS]);
        $this->assertEquals('hasAndBelongs', $roles[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('model_meta_role', $roles[IModelMetaSupport::META_TABLE]);
        $this->assertEquals('ModelMetaRole', $roles[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('user_id', $roles[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('ModelMetaRole.user_id = User.id', $roles[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('model_meta_role_user', $roles[IModelMetaSupport::META_LINKS_TABLE]);
        $this->assertEquals('model_meta_role_id', $roles[IModelMetaSupport::META_LINKS_TABLE_FOREIGN]);
    }

    /**
     * 测试构造函数，关系HasAndBelongs。
     */
    public function testConstructLinksForHasAndBelongs() {
        $meta = new ModelMetaMock('ModelMetaRole');
        $users = $meta->links['Users'];

        $this->assertEquals('#ModelMetaUser', $users[IModelMetaSupport::META_LINKS_CLASS]);
        $this->assertEquals('hasAndBelongs', $users[IModelMetaSupport::META_LINKS_TYPE]);
        $this->assertEquals('user', $users[IModelMetaSupport::META_TABLE]);
        $this->assertEquals('User', $users[IModelMetaSupport::META_TABLE_ALIAS]);
        $this->assertEquals('role_id', $users[IModelMetaSupport::META_LINKS_FOREIGN]);
        $this->assertEquals('User.role_id = ModelMetaRole.id', $users[IModelMetaSupport::META_LINKS_ON]);
        $this->assertEquals('user_role', $users[IModelMetaSupport::META_LINKS_TABLE]);
        $this->assertEquals('user_id', $users[IModelMetaSupport::META_LINKS_TABLE_FOREIGN]);
    }

    /**
     * 测试生成验证器。
     */
    public function testValidator() {
        $meta = new ModelMetaMock('ModelMetaValidator');

        $this->assertArrayHasKey('create', $meta->validators);
        $this->assertEquals('required', $meta->validators['create']['name']);
        $this->assertNotEmpty('age', $meta->validators['create']['age']);
        $this->assertEquals(TRUE, $meta->validators['create']['age']['required']);
        $this->assertEquals(12, $meta->validators['create']['age']['minlen']);
    }

    /**
     * 测试生成验证器消息。
     */
    public function testValidatorMessages() {
        $meta = new ModelMetaMock('ModelMetaValidator');

        $this->assertEquals('name required', $meta->validatorMessages['name']);
        $this->assertEquals('age required and must > 12', $meta->validatorMessages['age']);
    }

    /**
     * 测试实例获取值。
     */
    public function testInstance() {
        $meta = ModelMeta::instance('ModelMetaSupportMockProps');

        $this->assertEquals('user', $meta->table);
        $this->assertEquals('User', $meta->tableAlias);
        $this->assertEquals(5, count($meta->schemas));
        $this->assertEquals('id', $meta->schemas['id']['field']);

        $this->assertTrue($meta === ModelMeta::instance('ModelMetaSupportMockProps'));
    }

    /**
     * @see PHPUnit_Framework_TestCase::setUp
     */
    public function setUp() {
        # 设置数据库信息。
        Skihat::write('kernels/databases', array(
                'test' => array(
                    SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine#MySqlPdoEngine',
                    'conf' => MYSQL_CONF)
            )
        );

        Skihat::register(array(SKIHAT_PATH_LIBRARY));
        Database::initialize();
    }
}

/**
 * ModelMeta 模拟类，提供单元测试所需要的模拟服务。
 *
 * @package skihat.models
 */
class ModelMetaMock extends ModelMeta {
    public function __construct($modelName) {
        parent::__construct($modelName);
    }

    protected function _initializeSchemas() {
        return array();
    }
}

/**
 * ModelMetaSchema模拟类，提供ModelMeta单元测试所需的服务。
 *
 * @package skihat.models
 */
class ModelMetaSchema extends ModelMeta {
    public function __construct($modelName) {
        parent::__construct($modelName);
    }
}

/**
 * ModelMetaSupportMock 模拟类提供ModelMeta单元测试所需的服务。
 *
 * @package skihat.models
 */
class ModelMetaSupportMock implements IModelMetaSupport {
    public static function config() {
        return array('goods' => 'a');
    }
}

/**
 * ModelMetaSupportMockProps 模拟类提供ModelMeta单元测试所需的服务。
 *
 * @package skihat.models
 */
class ModelMetaSupportMockProps extends ModelMetaSupportMock {
    public static function config() {
        return array(
            self::META_DATABASE => 'test',
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_PRIMARY_KEY => 'idn',
            self::META_PRIMARY_INCREMENT => false
        );
    }
}

/**
 * ModelMetaUser 模拟类提供ModelMeta单元测试所需的服务。
 *
 * @package skihat.models
 */
class ModelMetaUser extends ModelMetaSupportMock {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_LINKS => array(
                'Group' => array(
                    self::META_LINKS_CLASS => '#ModelMetaGroup',
                    self::META_LINKS_TYPE => self::BELONGS_TO
                ),
                'GroupAlias' => array(
                    self::META_LINKS_CLASS => '#ModelMetaGroup',
                    self::META_LINKS_TYPE => self::BELONGS_TO,
                    self::META_TABLE_ALIAS => 'GroupAlias'
                ),
                'Roles' => array(
                    self::META_LINKS_CLASS => '#ModelMetaRole',
                    self::META_LINKS_TYPE => self::HAS_AND_BELONGS
                )
            )
        );
    }
}

/**
 * 模型元角色元数据，提供模型元数据单元测试服务。
 *
 * @package skihat.models
 */
class ModelMetaRole extends ModelMetaSupportMock {
    public static function config() {
        return array(
            self::META_LINKS => array(
                'Users' => array(
                    self::META_LINKS_CLASS => '#ModelMetaUser',
                    self::META_LINKS_TABLE => 'user_role',
                    self::META_LINKS_FOREIGN => 'role_id',
                    self::META_LINKS_TABLE_FOREIGN => 'user_id',
                    self::META_LINKS_TYPE => self::HAS_AND_BELONGS
                )
            )
        );
    }
}

/**
 * ModelMetaGroup 模拟类提供ModelMeta单元测试所需的服务。
 *
 * @package skihat.models
 */
class ModelMetaGroup extends ModelMetaSupportMock {
    public static function config() {
        return array(self::META_TABLE => 'group',
            self::META_LINKS => array(
                'FirstUser' => array(
                    self::META_LINKS_CLASS => '#ModelMetaUser',
                    self::META_LINKS_TYPE => self::HAS_ONE
                ),
                'FirstUserAlias' => array(
                    self::META_LINKS_CLASS => '#ModelMetaUser',
                    self::META_LINKS_TYPE => self::HAS_ONE,
                    self::META_TABLE_ALIAS => 'FirstUserAlias'
                ),
                'Users' => array(
                    self::META_LINKS_CLASS => '#ModelMetaUser',
                    self::META_LINKS_TYPE => self::HAS_MANY
                ),
                'UsersAlias' => array(
                    self::META_LINKS_CLASS => '#ModelMetaUser',
                    self::META_LINKS_TYPE => self::HAS_MANY,
                    self::META_TABLE_ALIAS => 'UsersAlias'
                )
            )
        );
    }
}


/**
 * ModelMetaValidator 模拟类提供ModelMeta单元测试所需的服务。
 *
 * @package skihat.models
 */
class ModelMetaValidator extends ModelMetaSupportMock {
    public static function config() {
        return array(
            self::META_RULE => array(
                'create' => array(
                    'name' => 'required',
                    'age' => array(
                        'required' => true,
                        'minlen' => 12
                    )
                )
            ),
            self::META_RULE_MESSAGE => array(
                'name' => 'name required',
                'age' => 'age required and must > 12'
            )
        );
    }
}