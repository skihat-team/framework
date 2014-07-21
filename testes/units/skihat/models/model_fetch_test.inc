<?php
/**
 * model_fetch_test.inc 模型检索测试类，提供模型的单元测试服务。
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
Skihat::import('models.model', SKIHAT_PATH_LIBRARY);

/**
 * 模型检索单元测试服务，提供模型检索类的单元测试服务。
 *
 * @package skihat.models
 */
class ModelFetchTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试附加组件值。
     */
    public function testExtra() {
        $modelFetch = new ModelFetch('User', array());

        $modelFetch->addExtra('paginate', 'hello');
        $this->assertEquals('hello', $modelFetch->extra('paginate'));
        $this->assertEquals('default', $modelFetch->extra('not-exists', 'default'));

        $modelFetch->removeExtra('paginate');
        $this->assertFalse($modelFetch->extra('paginate'));
    }

    /**
     * 测试获取二维数组值。
     */
    public function testFetchRows() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();
        $rows = $fetch->fetchRows();

        $this->assertEquals(3, count($rows));
        $this->assertEquals('skihat', $rows[0]['name']);
        $this->assertEquals('skihat2', $rows[1]['name']);
        $this->assertEquals('skihat3', $rows[2]['name']);
    }

    /**
     * 获取对象值。
     */
    public function testFetchObjects() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();
        $rows = $fetch->fetchObjects();

        $this->assertEquals(3, count($rows));
        $this->assertEquals('skihat', $rows[0]->name);
        $this->assertEquals('skihat2', $rows[1]->name);
        $this->assertEquals('skihat3', $rows[2]->name);
    }

    /**
     * 测试获取值，模型对象。
     */
    public function testFetchModels() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();
        $rows = $fetch->fetchModels();

        $this->assertEquals(3, count($rows));
        $this->assertEquals('skihat', $rows[0]->name);
        $this->assertEquals('skihat2', $rows[1]->name);
        $this->assertEquals('skihat3', $rows[2]->name);
    }

    /**
     * 测试字段值。
     */
    public function testFields() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();

        $fetch->fields('name,description');
        $rows = $fetch->fetchRows();

        $this->assertEquals(3, count($rows));
        $this->assertEquals(2, count($rows[0]));
        $this->assertEquals(2, count($rows[1]));
    }

    /**
     * 测试排序。
     */
    public function testOrder() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();
        $fetch->order('id desc');
        $rows = $fetch->fetchRows();

        $this->assertEquals(3, count($rows));
        $this->assertEquals('skihat3', $rows[0]['name']);
        $this->assertEquals('skihat2', $rows[1]['name']);
        $this->assertEquals('skihat', $rows[2]['name']);
    }

    /**
     * 测试条件。
     */
    public function testWhere() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();
        $fetch->where('name = \'skihat\'');
        $rows = $fetch->fetchRows();

        $this->assertEquals(1, count($rows));
        $this->assertEquals('skihat', $rows[0]['name']);
    }

    /**
     * 测试分页。
     */
    public function testPage() {
        $link = $this->_initLink();
        $fetch = $this->_fetchObj();
        $fetch->page(1, 1);

        $rows = $fetch->fetchRows();

        $this->assertEquals(1, count($rows));
        $this->assertEquals('skihat', $rows[0]['name']);
    }

    /**
     * 测试获取生成json格式数据。
     */
    public function testFetchJson() {
        $link = $this->_initLink();
        $rows = $this->_fetchObj()->fields('name,`group`')->fetchJson();

        $this->assertEquals('[{"name":"skihat","group":"1"},{"name":"skihat2","group":"1"},{"name":"skihat3","group":"1"}]', $rows);
    }

    /**
     * 测试关系，返回对象数组。
     */
    public function testFetchLinksObjects() {
        $this->_initLink();
        $objects =
            $this->_fetchObj()->links('Group')->fields('User.id,User.name,Group.name as gname,group.id as gid')->fetchObjects();

        $this->assertEquals(3, count($objects));
        $this->assertEquals('skihat', $objects[0]->name);
        $this->assertEquals('skihat2', $objects[1]->name);
        $this->assertEquals('skihat3', $objects[2]->name);
        $this->assertEquals('system', $objects[0]->gname);
        $this->assertEquals('system', $objects[1]->gname);
        $this->assertEquals('system', $objects[2]->gname);
    }

    /**
     * 测试关系，返回二维数据。
     */
    public function testFetchLinksRows() {
        $this->_initLink();
        $objects =
            $this->_fetchObj()->links('Group')->fields('User.id,User.name,Group.name as gname,group.id as gid')->fetchRows();

        $this->assertEquals(3, count($objects));
        $this->assertEquals('skihat', $objects[0]['name']);
        $this->assertEquals('skihat2', $objects[1]['name']);
        $this->assertEquals('skihat3', $objects[2]['name']);
        $this->assertEquals('system', $objects[0]['gname']);
        $this->assertEquals('system', $objects[1]['gname']);
        $this->assertEquals('system', $objects[2]['gname']);
    }

    /**
     * 测试关系，返回对象模型。
     */
    public function testFetchLinksModels() {
        $this->_initLink();
        $objects = $this->_fetchObj()->links('Group')->fetchModels();

        $this->assertEquals(3, count($objects));
        $this->assertEquals('skihat', $objects[0]->name);
        $this->assertEquals('skihat2', $objects[1]->name);
        $this->assertEquals('skihat3', $objects[2]->name);
        $this->assertEquals('system', $objects[0]->Group->name);
        $this->assertEquals('system', $objects[1]->Group->name);
        $this->assertEquals('system', $objects[2]->Group->name);
    }

    /**
     * 初始化连接对象。
     */
    protected function _initLink() {
        $link = new PDO(MYSQL_DSN, MYSQL_USER, MYSQL_PASS);
        $link->exec('DELETE FROM `user`');

        $stmt = $link->prepare('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);');
        $stmt->execute(array('skihat', 'skihat example', 1));
        $stmt->execute(array('skihat2', 'skihat example 2', 1));
        $stmt->execute(array('skihat3', 'skihat example 3', 1));

        return $link;
    }

    /**
     * @see PHPUnit_Framework_TestCase::setUp
     */
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

    /**
     * @return ModelFetch
     */
    public function _fetchObj() {
        return new ModelFetch('ModelFetchUser', ModelFetchUser::defaultSyntax());
    }
}

class ModelFetchUser extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_LINKS => array(
                'Group' => array(
                    self::META_LINKS_CLASS => '#ModelFetchGroup',
                    self::META_LINKS_TYPE => self::BELONGS_TO,
                    self::META_LINKS_ON => 'User.group = Group.id',
                    self::META_LINKS_FOREIGN => 'group')
            )
        );
    }
}

class ModelFetchGroup extends Model {
    public static function config() {
        return array(self::META_TABLE => 'group', self::META_TABLE_ALIAS => 'Group');
    }
}