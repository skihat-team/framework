<?php
/**
 * model_link_test.inc 模型测试类，提供模型的单元测试服务。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.models
 * @version     1.0
 *
 * @created     2012-07-23
 * @modify      2013-09-24
 */

# 引入测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 导入被测试类。
Skihat::import('models.model', SKIHAT_PATH_LIBRARY);
Skihat::import('models.model_meta', SKIHAT_PATH_LIBRARY);
Skihat::import('models.model_fetch', SKIHAT_PATH_LIBRARY);
Skihat::import('models.model_links', SKIHAT_PATH_LIBRARY);

/**
 * 模型关系读取测试，提供对ModelLinks相关类的测试服务。
 *
 * @package skihat.models
 */
class ModelLinksTest extends PHPUnit_Framework_TestCase {


    /**
     * 测试BelongsTo异步读取。
     */
    public function testBelongsToRead() {
        $this->_initLink();
        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $link = new BelongsToModelLink();

        $group = $link->read($user, ModelLinkUser::meta()->links['Group']);

        $this->assertNotEmpty($group);
        $this->assertEquals('guest', $group->name);
    }

    /**
     * 测试BelongsTo异步读取，外键为空。
     */
    public function testBelongsToReadForForeignEmpty() {
        $this->_initLink();
        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $link = new BelongsToModelLink();
        $user->group = NULL;

        $group = $link->read($user, ModelLinkUser::meta()->links['Group']);
        $this->assertEmpty($group);
    }

    /**
     * 测试BelongsTo异步读取，外键对象不存在。
     */
    public function testBelongsToReadForNotExists() {
        $this->_initLink();
        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $link = new BelongsToModelLink();
        $user->group = 1000;

        $group = $link->read($user, ModelLinkUser::meta()->links['Group']);
        $this->assertEmpty($group);
    }

    /**
     * 测试BelongsTo写入。
     *
     * public function testBelongsToWrite() {
     * $this->_initLink();
     * $user = ModelLinkUser::fetch(array('name' => 'skihat'));
     * $group = ModelLinkGroup::fetch(array('name' => 'system'));
     * $link = new BelongsToModelLink();
     * $link->write($user,$group,ModelLinkUser::meta()->links['Group']);
     *
     * $this->assertEquals(1,$user->group);
     * }
     *
     * /**
     * 测试BelongsTo写入空值。
     *
     * public function testBelongsToWriteEmpty() {
     * $this->_initLink();
     * $user = ModelLinkUser::fetch(array('name' => 'skihat'));
     * $link = new BelongsToModelLink();
     * $link->write($user,NULL,ModelLinkUser::meta()->links['Group']);
     *
     * $this->assertNull($user->group);
     * }  */

    /**
     * 测试hasOne，读取值。
     */
    public function testHasOneForRead() {
        $this->_initLink();
        $group = ModelLinkGroup::fetch(array('name' => 'system'));
        $link = new HasOneModelLink();

        $user = $link->read($group, ModelLinkGroup::meta()->links['FirstUser']);
        $this->assertEquals('skihat3', $user->name);
    }

    /**
     * 测试hasOne，读取值不存在。
     */
    public function testHasOneForReadNotExists() {
        $this->_initLink();
        $group = ModelLinkGroup::fetch(array('name' => 'emp'));
        $link = new HasOneModelLink();

        $user = $link->read($group, ModelLinkGroup::meta()->links['FirstUser']);
        $this->assertEmpty($user);
    }

    /**
     * 测试hasOne，写入值。
     *
     * @expectedException ModelLinkException

    public function testHasOneWrite() {
     * $this->_initLink();
     * $group = ModelLinkGroup::fetch(array('name' => 'system'));
     * $link = new HasOneModelLink();
     *
     * $link->write($group,NULL, ModelLinkGroup::meta()->links['FirstUser']);
     * }     */

    /**
     * 测试读取一对多关系。
     */
    public function testHasManyForRead() {
        $this->_initLink();
        $link = new HasManyModelLink();
        $group = ModelLinkGroup::fetch(1);
        $users = $link->read($group, ModelLinkGroup::meta()->links['Users']);

        $this->assertInstanceOf('ModelFetch', $users);
        $rows = $users->fetchRows();

        $this->assertEquals(2, count($rows));
    }

    /**
     * 测试读取一对多关系。
     * public function testHasManyForWrite() {
     *
     * }*/

    /**
     * 测试多对多关系数据读取。
     */
    public function testHasAndBelongsReadEmpty() {
        $this->_initLink();
        $link = new HasAndBelongsModelLink();
        $role = ModelLinkRole::fetch(1);
        $roles = $link->read($role, ModelLinkRole::meta()->links['Users']);

        $this->assertEquals(0, $roles->fetchCount());
    }

    /**
     * 测试多对多数据读取。
     */
    public function testHasAndBelongsRead() {
        $this->_initLink();
        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $role = ModelLinkRole::fetch(1);

        ModelLinkUser::execute("INSERT INTO user_role(`user_id`,`role_id`) VALUES({$user->id},1);");
        $users = $role->Users->fetchObjects();

        $this->assertEquals(1, count($users));
        $this->assertEquals('skihat', $users[0]->name);
    }

    /**
     * 测试多对多数据读取，增加新的关系。
     */
    public function testHasAndBelongsAdd() {
        $this->_initLink();
        ModelLinkUser::execute('DELETE FROM user_role');

        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $role = ModelLinkRole::fetch(1);

        $role->Users->add($user->key());
        $users = $role->Users->fetchObjects();

        $this->assertEquals(1, count($users));
        $this->assertEquals('skihat', $users[0]->name);
    }

    /**
     * 测试多对多关系，移除实例。
     */
    public function testHasAndBelongsRemove() {
        $this->_initLink();
        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $role = ModelLinkRole::fetch(1);

        ModelLinkUser::execute("INSERT INTO user_role(`user_id`,`role_id`) VALUES({$user->id},1);");
        $this->assertEquals(1, $role->Users->fetchCount());

        $this->assertEquals(1, $role->Users->remove($user->key()));
        $this->assertEquals(0, $role->Users->fetchCount());
    }

    /**
     * 测试多对多关系，移除实例。
     */
    public function testHasAndBelongsRemoveArray() {
        $this->_initLink();
        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        $role = ModelLinkRole::fetch(1);

        ModelLinkUser::execute("INSERT INTO user_role(`user_id`,`role_id`) VALUES({$user->id},1);");
        $this->assertEquals(1, $role->Users->fetchCount());

        $this->assertEquals(1, $role->Users->remove(array($user->key())));
        $this->assertEquals(0, $role->Users->fetchCount());
    }

    /**
     * 测试多对多关系，替换实例。
     */
    public function testHasAndBelongsReplace() {
        $this->_initLink();
        ModelLinkUser::execute('DELETE FROM user_role');

        $user = ModelLinkUser::fetch(array('name' => 'skihat'));
        ModelLinkUser::execute("INSERT INTO user_role(`user_id`,`role_id`) VALUES({$user->id},1);");

        $role = ModelLinkRole::fetch(1);
        $user1 = ModelLinkUser::fetch(array('name' => 'skihat2'));

        $role->Users->replace($user1->id);
        $users = $role->Users->fetchObjects();

        $this->assertEquals(1, count($users));
        $this->assertEquals('skihat2', $users[0]->name);
        $this->assertNotEquals($user->id, $users[0]->id);
    }

    /**
     * 返回初始化的数据库引擎，并已初始化数据。
     *
     * @return PDO
     */
    protected function _initLink() {
        $conf = array(
            'dsn' => MYSQL_DSN,
            'user' => MYSQL_USER,
            'pass' => MYSQL_PASS,
            'charset' => 'utf8'
        );

        $link = new PDO($conf['dsn'], $conf['user'], $conf['pass']);
        $link->exec('DELETE FROM `user`');
        $stmt = $link->prepare('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);');
        $stmt->execute(array('skihat', 'skihat example', 2));
        $stmt->execute(array('skihat2', 'skihat example 2', 1));
        $stmt->execute(array('skihat3', 'skihat example 2', 1));

        return $link;
    }

    public function setUp() {
        # 设置数据库信息。
        Skihat::write('kernels/databases', array(
                'default' => array(
                    SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine#MySqlPdoEngine',
                    'conf' => MYSQL_CONF
                )
            )
        );

        Skihat::register(array(SKIHAT_PATH_LIBRARY));
        Database::initialize();
    }
}

class ModelLinkUser extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_LINKS => array(
                'Group' => array(
                    self::META_LINKS_CLASS => '#ModelLinkGroup',
                    self::META_LINKS_TYPE => self::BELONGS_TO,
                    self::META_LINKS_FOREIGN => 'group',
                    self::META_LINKS_ON => 'User.group = Group.id')
            )
        );
    }
}

class ModelLinkRole extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'role',
            self::META_TABLE_ALIAS => 'Role',
            self::META_LINKS => array(
                'Users' => array(
                    self::META_LINKS_TYPE => self::HAS_AND_BELONGS,
                    self::META_LINKS_CLASS => '#ModelLinkUser',
                    self::META_LINKS_TABLE => 'user_role')
            )
        );
    }
}

class ModelLinkGroup extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'group',
            self::META_TABLE_ALIAS => 'Group',
            self::META_LINKS => array(
                'FirstUser' => array(
                    self::META_LINKS_CLASS => '#ModelLinkUser',
                    self::META_LINKS_FOREIGN => 'group',
                    self::META_LINKS_TYPE => self::HAS_ONE,
                    Model::SYNTAX_ORDER => array('id' => 'desc')
                ),
                'Users' => array(
                    self::META_LINKS_CLASS => '#ModelLinkUser',
                    self::META_LINKS_TYPE => self::HAS_MANY,
                    self::META_LINKS_FOREIGN => 'group')
            )
        );
    }
}

