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

# 引入测试启动文件。
require_once(dirname(dirname(dirname(__FILE__))) . '/startup.inc');

# 导入被测试类。
Skihat::import('models.model', SKIHAT_PATH_LIBRARY);

/**
 * 模型测试类，提供模型的单元测试服务。
 *
 * @package skihat.models
 */
class ModelTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试初始化构造器。
     */
    public function testConstructorForOriginalAndCurrentProps() {
        $chen = new ModelExample(array('name' => 'chen', 'pass' => '123321'));
        $boss = new ModelExample(array('name' => 'boss', 'pass' => '123321'));

        $props = $chen->originalProps();
        $this->assertEquals('chen', $props['name']);
        $this->assertEquals('123321', $props['pass']);

        $props = $boss->currentProps();
        $this->assertEquals('boss', $props['name']);
        $this->assertEquals('123321', $props['pass']);
    }

    /**
     * 测试初始化构造器，默认值。
     */
    public function testConstructorForDefaults() {
        $user = new ModelUser();

        $this->assertEquals('description', $user->description);
        $this->assertEquals(1, $user->group);
    }

    /**
     * 测试获取原始属性值。
     */
    public function testOriginalProp() {
        $chen = new ModelExample(array('name' => 'chen', 'age' => 30));

        $this->assertEquals('chen', $chen->originalProp('name'));
        $this->assertEquals(30, $chen->originalProp('age'));
    }

    /**
     * 测试获取原始属性值，属性不存在。
     */
    public function testOriginalPropForPropNotExists() {
        $chen = new ModelExample(array('name' => 'chen'));

        $this->assertNull($chen->originalProp('age'));
        $this->assertNull($chen->originalProp('pass'));
    }

    /**
     * 测试Get方法。
     */
    public function testGet() {
        $chen = new ModelExample(array('name' => 'chen'));
        $jack = new ModelExample(array('name' => 'jack'));

        $this->assertEquals('chen', $chen->name);
        $this->assertEquals('jack', $jack->name);
    }

    /**
     * 测试Get方法，指定属性值不存在。
     */
    public function testGetForPropNotExists() {
        $chen = new ModelExample(array('name' => 'chen'));

        $this->assertNull($chen->age);
        $this->assertNull($chen->pass);
    }

    /**
     * 测试Set方法。
     */
    public function testSet() {
        $chen = new ModelExample(array('name' => 'chen'));
        $boss = new ModelExample(array('name' => 'boss'));

        $chen->name = 'boss';
        $boss->name = 'chen';

        $this->assertEquals('boss', $chen->name);
        $this->assertEquals('chen', $boss->name);
    }

    /**
     * 测试Set方法，不存在的属性。
     */
    public function testSetForNotExists() {
        $chen = new ModelExample(array('name' => 'chen'));
        $boss = new ModelExample(array('name' => 'boss'));

        $chen->age = 27;
        $boss->age = 31;

        $this->assertEquals(27, $chen->age);
        $this->assertEquals(31, $boss->age);

        $this->assertEquals('chen', $chen->originalProp('name'));
        $this->assertNull($chen->originalProp('age'));
    }

    /**
     * 测试更新属性值。
     */
    public function testChangeProps() {
        $chen = new ModelExample(array('name' => 'chen'));
        $chen->changeProps(array('age' => 27, 'name' => 'boss'));

        $this->assertEquals(27, $chen->age);
        $this->assertEquals('boss', $chen->name);

        $this->assertEquals('chen', $chen->originalProp('name'));
    }

    /**
     * 测试ArrayAccess，判断索引是否存在。
     */
    public function testOffsetExists() {
        $chen = new ModelExample(array('name' => 'chen'));

        $this->assertTrue(isset($chen['name']));
        $this->assertFalse(isset($chen['age']));
    }

    /**
     * 测试ArrayAccess，get方法。
     */
    public function testOffsetGet() {
        $chan = new ModelExample(array('name' => 'chen'));

        $this->assertEquals('chen', $chan['name']);
        $this->assertNull($chan['age']);
    }

    /**
     * 测试ArrayAccess，Set方法。
     */
    public function testOffsetSet() {
        $chan = new ModelExample(array('name' => 'chan'));

        $chan['name'] = 'boss';
        $chan['age'] = 27;

        $this->assertEquals('boss', $chan['name']);
        $this->assertEquals(27, $chan['age']);

        $this->assertEquals('boss', $chan->name);
        $this->assertEquals(27, $chan->age);
    }

    /**
     * 测试ArrayAccess，Unset方法。
     */
    public function testOffsetUnset() {
        $chan = new ModelExample(array('name' => 'chen'));

        unset($chan['name']);

        $this->assertNull($chan['name']);
    }

    /**
     * 测试返回json格式数据。
     */
    public function testJson() {
        $chan = new ModelExample(array('name' => 'chen', 'age' => 20));

        $this->assertEquals('{"name":"chen","age":20}', $chan->json());
    }

    /**
     * 测试返回xml数据。
     *
     * public function testXml() {
     * #
     * }*/

    /**
     * 测试返回默认DataKey值。
     */
    public function testDefaultDataKey() {
        $this->assertEquals(ModelUser::DEFAULT_PRIMARY_KEY, ModelUser::meta()->primaryKey);
    }

    /**
     * 测试执行查询服务。
     */
    public function testQuery() {
        $this->_initLink();

        $rows = ModelUser::query('SELECT * FROM `user` WHERE `name` = ?', array('skihat'));
        $this->assertEquals(1, count($rows));
    }

    /**
     * 测试执行方法。
     *
     * @depends testQuery
     */
    public function testExecute() {
        $this->_initLink();

        ModelUser::execute('DELETE FROM `user` WHERE `name` = ?', array('skihat'));
        $rows = ModelUser::query('SELECT * FROM `user` WHERE `name` = ?', array('skihat'));

        $this->assertEmpty($rows);
    }

    /**
     * 测试创建多个对象。
     */
    public function testCreateAll() {
        ModelUser::execute('DELETE FROM `user`');

        $rows = ModelUser::createAll(array('name' => 'skihat',
            'description' => 'skihat example',
            'allow' => 2,
            'group' => 3));

        $this->assertEquals(1, $rows);
        $users = ModelUser::query('SELECT * FROM `user` WHERE `name` = ?', array('skihat'));

        $this->assertEquals(1, count($users));
    }

    /**
     * 测试创建多个对象，暂不实现。
     */
    # public function testCreateAllForMultiple() {

    # }

    /**
     * 测试更新内容。
     */
    public function testUpdateAll() {
        $this->_initLink();
        ModelUser::updateAll(array('name' => 'new skihat'), array());

        $users = ModelUser::query('SELECT * FROM `user`');
        foreach ($users as $user) {
            $this->assertEquals('new skihat', $user['name']);
        }
    }

    /**
     * 测试更新内容，指定条件字符串。
     */
    public function testUpdateAllForConditionString() {
        $this->_initLink();
        ModelUser::updateAll(array('name' => 'new skihat'), array('name' => 'skihat'));

        $users = ModelUser::query('SELECT * FROM `user` WHERE name = \'new skihat\'');
        $this->assertNotEmpty($users);
    }

    /**
     * 测试更新内容，字符串条件。
     */
    public function testUpdateAllForConditionStringArgs() {
        $this->_initLink();
        ModelUser::updateAll(array('name' => 'new skihat'), array('name = ?', 'skihat'));

        $users = ModelUser::query('SELECT * FROM `user` WHERE name = \'new skihat\'');
        $this->assertNotEmpty($users);
    }

    /**
     * 测试更新内容，数组条件。
     */
    public function testUpdateForConditionArray() {
        $this->_initLink();
        ModelUser::updateAll(array('name' => 'new skihat'), array('name' => 'skihat'));

        $users = ModelUser::query('SELECT * FROM `user` WHERE name=\'new skihat\'');
        $this->assertNotEmpty($users);
    }

    /**
     * 测试更新内容，数据条件空格KEY值。
     */
    public function testUpdateForConditionArraySpaceKey() {
        $this->_initLink();
        ModelUser::updateAll(array('name' => 'new skihat'), array('name like' => '%skihat%'));

        $users = ModelUser::query('SELECT * FROM `user` WHERE name=\'new skihat\'');
        $this->assertNotEmpty($users);
        $this->assertEquals(2, count($users));
    }

    /**
     * 测试更新，数字条件。
     */
    public function testUpdateForConditionNumeric() {
        $link = $this->_initLink();
        $stmt = $link->query('SELECT * FROM `user`');
        $rows = $stmt->fetchAll();

        ModelUser::updateAll(array('name' => 'new skihat'), $rows[0]['id']);

        $users = ModelUser::query('SELECT * FROM `user` WHERE name=\'new skihat\'');
        $this->assertNotEmpty($users);
    }

    /**
     * 测试执行删除全部对象。
     */
    public function testDeleteAll() {
        $this->_initLink();
        ModelUser::deleteAll(array());

        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertEmpty($rows);
    }

    /**
     * 测试执行删除全部对象，指定条件字符串。
     */
    public function testDeleteAllForConditionString() {
        $this->_initLink();

        ModelUser::deleteAll("name = 'skihat'");
        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertEquals(1, count($rows));

        $this->assertEquals('skihat2', $rows[0]['name']);
    }

    /**
     * 测试执行删除全部对象，指定条件字符串参数。
     */
    public function testDeleteAllForConditionStringArgs() {
        $this->_initLink();

        ModelUser::deleteAll(array('name = ?', 'skihat'));
        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertEquals(1, count($rows));

        $this->assertEquals('skihat2', $rows[0]['name']);
    }

    /**
     * 测试执行删除全部对象，指定条件数组。
     */
    public function testDeleteAllForConditionArray() {
        $this->_initLink();

        ModelUser::deleteAll(array('name' => 'skihat'));
        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertEquals(1, count($rows));

        $this->assertEquals('skihat2', $rows[0]['name']);
    }

    /**
     * 测试执行删除全部对象，指定空格字段。
     */
    public function testDeleteAllForSpaceField() {
        $this->_initLink();
        ModelUser::deleteAll(array('name like' => '%skihat%'));

        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertEmpty($rows);
    }

    /**
     * 根据条件数值，删除内容。
     */
    public function testDeleteAllConditionNumeric() {
        $link = $this->_initLink();
        $stmt = $link->query('SELECT * FROM `user`');
        $rows = $stmt->fetchAll();

        ModelUser::deleteAll($rows[0]['id']);

        $users = ModelUser::query('SELECT * FROM `user` WHERE name=\'skihat\'');
        $this->assertEmpty($users);
    }

    /**
     * 测试保存新内容。
     */
    public function testSaveForCreate() {
        ModelUser::execute('DELETE FROM `user`');
        $user = new ModelUser(array('name' => 'skihat', 'description' => 'example', 'allow' => 1, 'group' => 4));

        $this->assertTrue($user->save());
        $this->assertNotNull($user->key());
        $this->assertTrue($user->isUpdate());

        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertNotEmpty($rows);
    }

    /**
     * 测试保存更新内容，提供更多的属性。
     */
    public function testSaveForCreateAndMoreProp() {
        ModelUser::execute('DELETE FROM `user`');
        $user = new ModelUser(array('name' => 'skihat', 'more' => 'ab', 'description' => 'example', 'allow' => 1, 'group' => 4));

        $this->assertTrue($user->save());
        $this->assertNotNull($user->key());
        $this->assertTrue($user->isUpdate());

        $rows = ModelUser::query('SELECT * FROM `user`');
        $this->assertNotEmpty($rows);

    }

    /**
     * 测试保存创建之类回调。
     */
    public function testSaveForCreateBefore() {
        $user = new ModelCallbackMock(array('name' => 'skihat',
            'description' => 'example',
            'allow' => 1,
            'group' => 4));

        ModelCallbackMock::$createBefore = FALSE;
        ModelCallbackMock::$createReturn = FALSE;

        $this->assertFalse($user->save());
        $this->assertTrue(ModelCallbackMock::$createBefore);
    }

    /**
     * 测试保存创建之类回调方法。
     */
    public function testSaveForCreateAfter() {
        $user = new ModelCallbackMock(array('name' => 'skihat',
            'description' => 'example',
            'allow' => 1,
            'group' => 3));

        ModelCallbackMock::$createReturn = TRUE;
        ModelCallbackMock::$createAfter = FALSE;

        $this->assertTrue($user->save());
        $this->assertTrue(ModelCallbackMock::$createAfter);
    }

    /**
     * 测试保存，创建失败，after回调方法。
     */
    public function testSaveForCreateFailAfter() {
        $user = new ModelCallbackMock(array('name' => 'skihat',
            'description' => 'example',
            'allow' => 1,
            'group' => 3));

        ModelCallbackMock::$createReturn = FALSE;
        ModelCallbackMock::$createAfter = FALSE;

        $this->assertFalse($user->save());
        $this->assertFalse(ModelCallbackMock::$createAfter);
    }

    /**
     * 测试构造器，强制更新。
     */
    public function testConstructForForceUpdate() {
        $user = new ModelUser(array('name' => 'skihat',
            'description' => 'example',
            'allow' => 1,
            'group' => 3,
            'id' => '1'), TRUE);

        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试构造器，强制更新缺少KEY值。
     *
     * @expectedException ModelException
     */
    public function testConstructorForForceUpdateAndNotSupportKey() {
        new ModelUser(array('name' => 'skihat', 'description' => 'example', 'allow' => 1, 'group' => 3), TRUE);
    }

    /**
     * 测试构造器，强制加载。
     */
    public function testConstructorForForceLoad() {
        $this->_initLink();
        $rows = ModelUser::query('SELECT id FROM `user`');
        $user = new ModelUser(array('id' => $rows[0]['id']), TRUE, TRUE);

        $this->assertEquals('skihat', $user->name);
    }

    /**
     * 测试构造函数，强制更新未找到。
     *
     * @expectedException ModelNotFoundException
     */
    public function testConstructorForForceLoadNotFound() {
        new ModelUser(array('id' => 1), TRUE, TRUE);
    }

    /**
     * 测试构造函数，指定非数组参数。
     */
    public function testConstructorForNotArrayProps() {
        $this->_initLink();
        $rows = ModelUser::query('SELECT id FROM `user`');
        $user = new ModelUser($rows[0]['id']);

        $this->assertEquals('skihat', $user->name);
        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试保存，更新。
     */
    public function testSaveForUpdate() {
        $this->_initLink();
        $rows = ModelUser::query('SELECT id FROM `user`');
        $user = new ModelUser($rows[1]['id']);

        $user->name = 'new skihat';
        $user->allow = 0;

        $this->assertTrue($user->save());
        $rows = ModelUser::query('SELECT * FROM `user` WHERE `name` = \'new skihat\'');

        $this->assertNotEmpty($rows);
        $this->assertEquals(1, count($rows));
        $this->assertEquals('new skihat', $rows[0]['name']);
        $this->assertEquals(0, $rows[0]['allow']);
    }

    /**
     * 测试保存更新，指定更多属性。
     */
    public function testSaveForUpdateAndMore() {
        $this->_initLink();
        $rows = ModelUser::query('SELECT id FROM `user`');
        $user = new ModelUser($rows[1]['id']);

        $user->name = 'new skihat';
        $user->allow = 0;
        $user->more = 'ab';

        $this->assertTrue($user->save());
        $rows = ModelUser::query('SELECT * FROM `user` WHERE `name` = \'new skihat\'');

        $this->assertNotEmpty($rows);
        $this->assertEquals(1, count($rows));
        $this->assertEquals('new skihat', $rows[0]['name']);
        $this->assertEquals(0, $rows[0]['allow']);
    }

    /**
     * 测试保存，更新之前。
     */
    public function testSaveForUpdateBefore() {
        $user = new ModelCallbackMock(array('id' => 1), TRUE);
        ModelCallbackMock::$updateBefore = FALSE;
        ModelCallbackMock::$updateReturn = FALSE;

        $this->assertFalse($user->save());
        $this->assertTrue(ModelCallbackMock::$updateBefore);
    }

    /**
     * 测试保存，更新之类。
     */
    public function testSaveForUpdateAfter() {
        $this->_initLink();
        $rows = ModelCallbackMock::query('SELECT `id` FROM `user`');
        $user = new ModelCallbackMock($rows[0]['id']);

        ModelCallbackMock::$updateReturn = TRUE;
        ModelCallbackMock::$updateAfter = FALSE;

        $user->name = 'new skihat';
        $this->assertTrue($user->save());
        $this->assertTrue(ModelCallbackMock::$updateAfter);
    }

    /**
     * 测试保存验证，创建内容。
     */
    public function testSaveForCreateValidate() {
        $this->_initLink();
        $user = new ModelUserValidator();

        $this->assertFalse($user->save());
        $this->assertEquals('name required', $user->error('name'));
    }

    /**
     * 测试保存验证，更新内容。
     */
    public function testSaveForUpdateValidate() {
        $this->_initLink();
        $user = ModelUserValidator::fetch(array('name = ?', 'skihat'));
        $user->name = 'ab';

        $this->assertFalse($user->save());
        $this->assertEquals('name must > 5', $user->error('name'));
    }

    /**
     * 测试执行自定义规则。
     */
    public function testCustomValidateRule() {
        $this->_initLink();
        $user = new ModelUserValidator(array('name' => 'skihat',
            'password' => 'abcd',
            'confirm_password' => 'skihat2'));

        $this->assertFalse($user->validate('login'));
        $this->assertEquals('confirm password error.', $user->error('confirm_password'));
    }

    /**
     * 测试保存回调验证。
     */
    public function testSaveCallbackValidateBefore() {
        $this->_initLink();
        $user = ModelUserValidatorCallback::fetch(array('name = ?', 'skihat'));

        ModelUserValidatorCallback::$validateReturn = false;
        ModelUserValidatorCallback::$validateBefore = false;
        ModelUserValidatorCallback::$validateAfter = false;

        $this->assertFalse($user->save());
        $this->assertTrue(ModelUserValidatorCallback::$validateBefore);
    }

    /**
     * 测试执行验证后回调方法。
     */
    public function testSaveCallbackValidateAfter() {
        $this->_initLink();
        $user = ModelUserValidatorCallback::fetch(array('name = ?', 'skihat'));

        ModelUserValidatorCallback::$validateReturn = true;
        ModelUserValidatorCallback::$validateBefore = false;
        ModelUserValidatorCallback::$validateAfter = false;

        $this->assertTrue($user->save());
        $this->assertTrue(ModelUserValidatorCallback::$validateBefore);
    }


    /**
     * 测试删除。
     */
    public function testDelete() {
        $this->_initLink();
        $rows = ModelUser::query('SELECT `id` FROM `user`');

        $user = new ModelUser($rows[0]['id']);
        $this->assertTrue($user->delete());
        $this->assertEmpty(ModelUser::query('SELECT `id` FROM `user` WHERE `id` = ?', array($rows[0]['id'])));
        $this->assertTrue($user->isDelete());

        $rows = ModelUser::query('SELECT `id` FROM `user` WHERE `id` = ?', array($rows[0]['id']));
        $this->assertEmpty($rows);
    }

    /**
     * 测试删除，删除创建实例。
     *
     * @expectedException ModelException
     */
    public function testDeleteForCreated() {
        $user = new ModelUser(array('name' => 'new skihat'));
        $user->delete();
    }

    /**
     * 测试删除之前。
     */
    public function testDeleteBefore() {
        $this->_initLink();
        $rows = ModelUser::query('select * from `user`');

        $user = new ModelCallbackMock($rows[0]['id']);
        ModelCallbackMock::$deleteBefore = FALSE;
        ModelCallbackMock::$deleteReturn = FALSE;

        $this->assertFalse($user->delete());
        $this->assertTrue(ModelCallbackMock::$deleteBefore);
    }

    /**
     * 测试删除之后。
     */
    public function testDeleteAfter() {
        $this->_initLink();
        $rows = ModelUser::query('select * from `user`');

        $user = new ModelCallbackMock($rows[0]['id']);
        ModelCallbackMock::$deleteReturn = TRUE;

        $this->assertTrue($user->delete());
        $this->assertTrue(ModelCallbackMock::$deleteAfter);
    }

    /**
     * 测试保存已删除的对象。
     *
     * @expectedException ModelException
     */
    public function testSaveForDeleted() {
        $this->_initLink();
        $rows = ModelUser::query('select * from `user`');

        $user = new ModelUser($rows[0]['id']);
        $this->assertTrue($user->delete());

        $user->save();
    }

    /**
     * 测试查找，根据单个值。
     */
    public function testFetchForKeyWhere() {
        $this->_initLink();
        $rows = ModelUser::query('select * from `user`');

        $user = ModelUser::fetch($rows[0]['id']);
        $this->assertEquals('skihat', $user->name);
        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试查找，根据字符串条件。
     */
    public function testFetchForStringWhere() {
        $this->_initLink();
        $user = ModelUser::fetch('name = \'skihat\'');

        $this->assertEquals('skihat', $user->name);
        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试查找，根据字符串参数。
     */
    public function testFetchForStringArgsWhere() {
        $this->_initLink();
        $user = ModelUser::fetch(array('name = ?', 'skihat'));

        $this->assertEquals('skihat', $user->name);
        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试查找，根据数组参数。
     */
    public function testFetchForArrayWhere() {
        $this->_initLink();
        $user = ModelUser::fetch(array('name' => 'skihat'));

        $this->assertEquals('skihat', $user->name);
        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试查找，空格Key值。
     */
    public function testFetchForArraySpaceKeyWhere() {
        $this->_initLink();
        $user = ModelUser::fetch(array('name like ' => 'skihat'));

        $this->assertEquals('skihat', $user->name);
        $this->assertTrue($user->isUpdate());
    }

    /**
     * 测试查找全部。
     */
    public function testFetchAll() {
        $link = $this->_initLink();
        $users = ModelUser::fetchAll();

        $this->assertInstanceOf('ModelFetch', $users);
        $this->assertEquals(2, count($users->fetchRows()));
    }

    /**
     * 测试查找字符串条件。
     */
    public function testFetchAllForStringWhere() {
        $this->_initLink();
        $users = ModelUser::fetchAll('name = \'skihat\'');

        $this->assertInstanceOf('ModelFetch', $users);
        $this->assertEquals(1, count($users->fetchRows()));
    }

    /**
     * 测试查找全部，字符串参数条件。
     */
    public function testFetchAllForStringArgsWhere() {
        $link = $this->_initLink();
        $users = ModelUser::fetchAll(array('name = ?', 'skihat'));

        $this->assertInstanceOf('ModelFetch', $users);
        $this->assertEquals(1, count($users->fetchRows()));
    }

    /**
     * 测试查找全部，数组条件。
     */
    public function testFetchAllForArrayWhere() {
        $this->_initLink();
        $users = ModelUser::fetchAll(array('name' => 'skihat'));

        $this->assertInstanceOf('ModelFetch', $users);
        $this->assertEquals(1, count($users->fetchRows()));
    }

    /**
     * 测试查找全部，空格条件。
     */
    public function testFetchAllForArraySpaceKeyWhere() {
        $link = $this->_initLink();
        $users = ModelUser::fetchAll(array('name like ' => 'skihat'));

        $this->assertInstanceOf('ModelFetch', $users);
        $this->assertEquals(1, count($users->fetchRows()));
    }

    /**
     * 测试查找全部，数组值条件。
     */
    public function testFetchAllForArrayValueWhere() {
        $link = $this->_initLink();
        $users = ModelUser::fetchAll(array('name' => array('skihat', 'skihat2')));

        $this->assertInstanceOf('ModelFetch', $users);
        $this->assertEquals(2, count($users->fetchRows()));
    }

    /**
     * 测试hasOne,同步加载
     */
    public function testHasOneForSyncLoad() {
        $this->_initLink();
        $user = ModelUser::fetch(array('User.name' => 'skihat'), array(Model::SYNTAX_LINK => 'GroupOne'));

        $this->assertNotEmpty($user->GroupOne);
        $this->assertEquals('system', $user->GroupOne->name);
    }

    /**
     * 测试belongsTo，同步加载。
     */
    public function testBelongsToSyncLoad() {
        $this->_initLink();
        $user = ModelUser::fetch(array('User.name' => 'skihat'), array('links' => 'Group'));

        $this->assertNotEmpty($user->Group);
        $this->assertEquals('system', $user->Group->name);
    }

    /**
     * 测试hasMany，同步加载。
     */
    public function testHasManyForSyncLoad() {
        $this->_initLink();
        $group = ModelGroup::fetch(array('Group.id' => 1), array('links' => 'Users'));

        $this->assertFalse(isset($group['Users']));
    }

    /**
     * 测试HasOne，异步加载。
     */
    public function testHasOneForAsynLoad() {
        $this->_initLink();
        $group = ModelGroup::fetch(array('Group.id' => 1));

        $user = $group->FirstUser;

        $this->assertNotEmpty($user);
        $this->assertEquals('skihat2', $user->name);
    }

    /**
     * 测试belongsTo，异步加载。
     */
    public function testBelongsToAsynLoad() {
        $this->_initLink();
        $user = ModelUser::fetch(array('name' => 'skihat2'));

        $group = $user->Group;

        $this->assertNotEmpty($group);
        $this->assertInstanceOf('ModelGroup', $group);
        $this->assertEquals('system', $group->name);
    }


    /**
     * 测试hasMany，异步加载。
     */
    public function testHasManyForAsynLoad() {
        $this->_initLink();
        $group = ModelGroup::fetch(1);

        $users = $group->Users;
        $this->assertNotEmpty($users);
        $this->assertInstanceOf('ModelFetch', $users);

        $rows = $users->fetchRows();
        $this->assertEquals(2, count($rows));
    }


    /**
     * 测试多对多关系。
     *
     * public function testHasAndBelongsMany() {
     *
     * }
     *
     * /**
     * 测试只读字段。
     */
    public function testReadonlyField() {
        $this->_initLink();
        $user = ModelReadonly::fetch(array('name' => 'skihat'));

        $user->group = 3;
        $user->name = 'new name';

        $this->assertTrue($user->save());

        $user1 = ModelReadonly::fetch(array('name' => 'skihat'));

        $this->assertNotEmpty($user1);
        $this->assertEquals($user->key(), $user1->key());

        $this->assertEquals('skihat', $user1->name);
    }

    /**
     * 测试虚拟字段读取。
     */
    public function testVirtualField() {
        $this->_initLink();

        $user = ModelVirtual::fetch(array('name' => 'skihat'));

        $this->assertNotEmpty($user);
        $this->assertEquals('skihat_all', $user->full_name);
    }

    /**
     * @see PHPUnit_Framewwork_TestCase::setUp
     */
    public function setUp() {
        # 设置数据库信息。
        Skihat::write('kernels/databases', array(
                'default' => array(
                    SKIHAT_IOC_CLASS => 'kernels.databases.engines.mysql_pdo_engine#MySqlPdoEngine',
                    'conf' => MYSQL_CONF)
            )
        );

        Skihat::register(array(SKIHAT_PATH_LIBRARY));
        Database::initialize();
    }

    /**
     * 返回初始化的数据库引擎，并已初始化数据。
     *
     * @return PDO
     */
    protected function _initLink() {
        $link = new PDO(MYSQL_DSN, MYSQL_USER, MYSQL_PASS);
        $link->exec('DELETE FROM `user`');
        $stmt = $link->prepare('INSERT INTO `user`(`name`,`description`,`allow`,`group`) VALUES(?,?,1,?);');
        $stmt->execute(array('skihat', 'skihat example', 1));
        $stmt->execute(array('skihat2', 'skihat example 2', 1));

        return $link;
    }
}


class ModelCallbackMock extends Model {
    public static $createReturn = TRUE;
    public static $createBefore = FALSE;
    public static $createAfter = FALSE;

    public static $updateBefore = FALSE;
    public static $updateReturn = TRUE;
    public static $updateAfter = FALSE;

    public static $deleteBefore = FALSE;
    public static $deleteReturn = TRUE;
    public static $deleteAfter = FALSE;

    protected function _createBefore(array &$syntax) {
        self::$createBefore = TRUE;
        return self::$createReturn;
    }

    protected function _createAfter(array &$syntax) {
        self::$createAfter = TRUE;
    }

    protected function _updateBefore(array &$syntax) {
        self::$updateBefore = TRUE;
        return self::$updateReturn;
    }

    protected function _updateAfter(array &$syntax) {
        self::$updateAfter = TRUE;
    }

    protected function _deleteBefore(array &$syntax) {
        self::$deleteBefore = TRUE;
        return self::$deleteReturn;
    }

    protected function _deleteAfter(array &$syntax) {
        self::$deleteAfter = TRUE;
    }

    public static function config() {
        return array(self::META_TABLE_ALIAS => 'User', self::META_TABLE => 'user');
    }
}

class ModelExample extends Model {
    public static function config() {
        return array(self::META_TABLE => 'user', self::META_TABLE_ALIAS => 'User');
    }
}

class ModelUser extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_DEFAULTS => array('group' => 1, 'description' => 'description'),
            self::META_LINKS => array(
                'Group' => array(
                    'type' => self::BELONGS_TO,
                    'class' => '#ModelGroup',
                    'foreign' => 'group',
                    'on' => 'User.group = Group.id'
                ),
                'GroupOne' => array(
                    'type' => self::HAS_ONE,
                    'class' => '#ModelGroup',
                    'foreign' => 'group',
                    'on' => 'User.group = Group.id')
            )
        );
    }
}

class ModelGroup extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'group',
            self::META_TABLE_ALIAS => 'Group',
            self::META_LINKS => array(
                'FirstUser' => array(
                    self::META_LINKS_CLASS => '#ModelUser',
                    self::META_LINKS_TYPE => self::HAS_ONE,
                    self::META_LINKS_FOREIGN => 'group',
                    self::META_LINKS_ON => 'User.group = Group.id',
                    self::SYNTAX_ORDER => 'id desc'
                ),
                'Users' => array(
                    'type' => self::HAS_MANY,
                    'class' => '#ModelUser',
                    'foreign' => 'group',
                    'on' => 'User.group = Group.id')
            )
        );
    }
}

class ModelUserValidator extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_RULE => array(
                self::CREATE_RULES => array('name' => 'required'),
                self::UPDATE_RULES => array(
                    'name' => array('required' => true, 'strmin' => 5)
                ),
                'login' => array(
                    'name' => 'required',
                    'password' => 'required',
                    'confirm_password' => array('compare' => 'password'))
            ),
            self::META_RULE_MESSAGE => array(
                'name' => array('required' => 'name required', 'strmin' => 'name must > 5'),
                'confirm_password' => 'confirm password error.'
            )
        );
    }
}

class ModelUserValidatorCallback extends Model {
    public static $validateBefore = false;
    public static $validateAfter = false;
    public static $validateReturn = true;

    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User'
        );
    }

    protected function _validateBefore($onRule, array &$syntax) {
        self::$validateBefore = true;
        return self::$validateReturn;
    }

    protected function _validateAfter($onRule, array &$syntax) {
        self::$validateAfter = true;
    }
}

class ModelReadonly extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_READONLY => array('name', 'description')
        );
    }
}

class ModelVirtual extends Model {
    public static function config() {
        return array(
            self::META_TABLE => 'user',
            self::META_TABLE_ALIAS => 'User',
            self::META_VIRTUAL => array('full_name')
        );
    }

    public function full_name() {
        return $this->_currentProps['name'] . '_all';
    }
}