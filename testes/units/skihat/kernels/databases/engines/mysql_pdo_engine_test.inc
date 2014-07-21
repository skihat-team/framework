<?php
/**
 * mysql_engine_test.inc MySql数据引擎测试声明文件。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.kernels.databases.engines
 * @version     1.0
 *
 * @create      2012-07-17
 * @modify      2013-09-05
 */

# 声明根节点。
require_once(dirname(dirname(dirname(dirname(dirname(__FILE__))))) . '/startup.inc');

# 引用被测试类
Skihat::import('kernels.databases.database', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.databases.engines.mysql_pdo_engine', SKIHAT_PATH_LIBRARY);


/**
 * MySql 数据库引擎测试类，提供对MySql数据库引擎的测试服务。
 *
 * @package skihat.kernels.databases.engines
 */
class MysqlPdoEngineTest extends PHPUnit_Framework_TestCase  {

    /**
     * 测试执行，配置错误。
     *
     * @expectedException ConfigException
     */
    public function testConfForNotSupportDsn() {
        $engine = new MysqlPdoEngine();
        $engine->query('select * from `user`');
    }

    /**
     * 测试配置，设置错误。
     *
     * @expectedException DatabaseException
     */
    public function testConfForError() {
        $engine = new MysqlPdoEngine();
        $engine->conf = 'mysql:host=127.0.0.1;dbname=skihat';
        $engine->query('select * from `user`');
    }

    /**
     * 测试执行命令。
     */
    public function testExecute() {
        $engine = $this->_engine();
        $engine->execute('DELETE FROM `user`');

        $r1 =
            $engine->execute('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);', array('skihat',
                'skihat example',
                1));
        $r2 =
            $engine->execute('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);', array('skihat2',
                'skihat example2',
                1));

        $this->assertEquals(1, $r1);
        $this->assertEquals(1, $r2);

        $rows = $this->userRows();
        $this->assertEquals(2, count($rows));
    }

    /**
     * 测试执行命令，SQL发生错误。
     *
     * @expectedException DatabaseException
     */
    public function testExecuteForCmdError() {
        $engine = $this->_engine();
        $engine->execute('INSERT INTO `user`(`name`,`description`,`group`) VALUES(?,?,?);', array('skihat',
            'skihat example',
            1));
    }

    /**
     * 测试执行命令，参数发生错误。
     *
     * @expectedException DatabaseException
     */
    public function testExecuteForArgsError() {
        $engine = $this->_engine();
        $engine->execute('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);', array('skihat',
            'skihat example'));
    }

    /**
     * 测试查询方法。
     */
    public function testQuery() {
        $engine = $this->_initEngine();
        $rows = $engine->query('SELECT * FROM `user`');

        $this->assertEquals(2, count($rows));
        $this->assertEquals('skihat', $rows[0]['name']);
        $this->assertEquals('skihat2', $rows[1]['name']);
    }

    /**
     * 测试查询方法，包含参数。
     */
    public function testQueryForArgs() {
        $engine = $this->_initEngine();
        $rows = $engine->query('SELECT * FROM `user` where `name` = ?', array('skihat2'));

        $this->assertEquals(1, count($rows));
        $this->assertEquals('skihat2', $rows[0]['name']);
        $this->assertEquals('skihat example2', $rows[0]['description']);
    }

    /**
     * 测试查询方法，包含选项。
     */
    public function testQueryForFetchStyle() {
        $engine = $this->_initEngine();
        $rows = $engine->query('SELECT group.name as gname,user.id,user.name FROM `user` left join `group` on user.group = group.id',
                array(),IDataSupport::FETCH_STYLE_GROUP | IDataSupport::FETCH_STYLE_ASSOC);

        $this->assertEquals(1, count($rows));
        $this->assertEquals(2, count($rows['system']));
    }

    /**
     * 测试查询方法，包含模型选项值。
     */
    public function testQueryForFetchModel() {
        $engine = $this->_initEngine();
        $rows = $engine->query('SELECT * FROM `user` AS `User` left join `group` AS `Group` on user.group = group.id',
                array(),IDataSupport::FETCH_STYLE_MODEL);

        $this->assertEquals(2, count($rows));
        $this->assertEquals('skihat', $rows[0]['User']['name']);
        $this->assertEquals('system', $rows[0]['Group']['name']);
    }

    /**
     * 测试查询方法，命令错误。
     *
     * @expectedException DatabaseException
     */
    public function testQueryForCmdError() {
        $engine = $this->_engine();
        $engine->query('SELECT AB FROM `user` where 1');
    }

    /**
     * 测试查询方法，参数错误。
     *
     * @expectedException DatabaseException
     */
    public function testQueryForArgsError() {
        $engine = $this->_engine();
        $engine->query('SELECT * FROM `user` where id=?', array());
    }

    /**
     * 测试检索schema信息。
     *
     * CREATE TABLE user (
     *      `id`    INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
     *      `name`  VARCHAR(32) NOT NULL,
     *      `allow` SMALLINT NOT NULL,
     *      `description`   VARCHAR(256) NOT NULL,
     *      `group` INT NOT NULL
     *  );
     */
    public function testFetchSchema() {
        $engine = $this->_engine();
        $schema = $engine->fetchSchema('user');

        $this->assertEquals(5, count($schema));

        $this->assertTrue(isset($schema['id'], $schema['name'], $schema['allow'], $schema['description'], $schema['group']));

        $this->assertEquals('id', $schema['id'][IDatabaseEngine::SCHEMA_FIELD]);
        $this->assertEquals('int', $schema['id'][IDatabaseEngine::SCHEMA_TYPE]);
        $this->assertEquals('11', $schema['id'][IDatabaseEngine::SCHEMA_SIZE]);
        $this->assertFalse($schema['id'][IDatabaseEngine::SCHEMA_NULL]);
        $this->assertNull($schema['id'][IDatabaseEngine::SCHEMA_DEFAULT]);

        $this->assertEquals('name', $schema['name'][IDatabaseEngine::SCHEMA_FIELD]);
        $this->assertEquals('varchar', $schema['name'][IDatabaseEngine::SCHEMA_TYPE]);
        $this->assertEquals('32', $schema['name'][IDatabaseEngine::SCHEMA_SIZE]);
        $this->assertFalse($schema['name'][IDatabaseEngine::SCHEMA_NULL]);
        $this->assertNull($schema['name'][IDatabaseEngine::SCHEMA_DEFAULT]);

        $this->assertEquals('allow', $schema['allow'][IDatabaseEngine::SCHEMA_FIELD]);
        $this->assertEquals('smallint', $schema['allow'][IDatabaseEngine::SCHEMA_TYPE]);
        $this->assertEquals('6', $schema['allow'][IDatabaseEngine::SCHEMA_SIZE]);
        $this->assertFalse($schema['allow'][IDatabaseEngine::SCHEMA_NULL]);
        $this->assertNull($schema['allow'][IDatabaseEngine::SCHEMA_DEFAULT]);

        $this->assertEquals('description', $schema['description'][IDatabaseEngine::SCHEMA_FIELD]);
        $this->assertEquals('varchar', $schema['description'][IDatabaseEngine::SCHEMA_TYPE]);
        $this->assertEquals('256', $schema['description'][IDatabaseEngine::SCHEMA_SIZE]);
#            $this->assertFalse($schema['description'][Database::SCHEMA_NULL]);
        # $this->assertNull($schema['description'][Database::SCHEMA_DEFAULT]);

        $this->assertEquals('group', $schema['group'][IDatabaseEngine::SCHEMA_FIELD]);
        $this->assertEquals('int', $schema['group'][IDatabaseEngine::SCHEMA_TYPE]);
        $this->assertEquals('11', $schema['group'][IDatabaseEngine::SCHEMA_SIZE]);
#            $this->assertFalse($schema['group'][Database::SCHEMA_NULL]);
        #    $this->assertEquals(1, $schema['group'][Database::SCHEMA_DEFAULT]);
    }

    /**
     * 测试检索Schema信息，表格不存在。
     *
     * @expectedException DatabaseException
     */
    public function testFetchSchemaForTableNotExists() {
        $engine = $this->_engine();
        $engine->fetchSchema('not-exists');
    }

    /**
     * 测试创建操作。
     */
    public function testCreate() {
        $engine = $this->_initEngine();
        $engine->create(array('name' => 'test',
            'allow' => 1,
            'group' => 1,
            'description' => 'des',), array(IDataSupport::SYNTAX_TABLE => 'user'));

        $rows = $this->userRows();
        $this->assertEquals(3, count($rows));
    }

    /**
     * 测试更新操作。
     */
    public function testUpdate() {
        $engine = $this->_initEngine();
        $engine->update(array('name' => 'test', 'allow' => 3), array(IDataSupport::SYNTAX_TABLE => 'user'));

        $rows = $this->userRows();
        foreach ($rows as $row) {
            $this->assertEquals('test', $row['name']);
            $this->assertEquals(3, $row['allow']);
        }
    }

    /**
     * 测试删除操作。
     */
    public function testDelete() {
        $engine = $this->_initEngine();
        $engine->delete(array(IDataSupport::SYNTAX_TABLE => 'user'));

        $rows = $this->userRows();
        $this->assertEmpty($rows);
    }


    /**
     * 测试插入空值。
     */
    public function testInsertNull() {
        $engine = $this->_initEngine();
        $engine->create(array(
            'name' => 'new test',
            'allow' => 1,
            'description' => '',
            'group' => null), array('table' => 'user'));
    }

    /**
     * 返回初始化的数据库引擎，并已初始化数据。
     *
     * @return MySqlPdoEngine
     */
    protected function _initEngine() {
        $engine = $this->_engine();

        $engine->execute('DELETE FROM `user`');
        $engine->execute('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);', array('skihat',
            'skihat example',
            1));
        $engine->execute('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);', array('skihat2',
            'skihat example2',
            1));

        return $engine;
    }

    /**
     * 返回仅仅初始化好的Mysql引擎。
     *
     * @return MySqlPdoEngine
     */
    protected function _engine() {
        $engine = new MysqlPdoEngine();
        $engine->conf = $this->_conf();

        return $engine;
    }

    /**
     * 返回用户表的二维数组记录。
     *
     * @return array
     */
    protected function userRows() {
        $link = $this->_link();
        $stmt = $link->query('SELECT * FROM `user`');

        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }

    /**
     * 返回测试数据的link连接对象。
     */
    protected function _link() {
        return new PDO(MYSQL_DSN, MYSQL_USER, MYSQL_PASS);
    }

    /**
     * 返回测试的conf配置信息。
     */
    protected function _conf() {
        return MYSQL_CONF;
    }
}

