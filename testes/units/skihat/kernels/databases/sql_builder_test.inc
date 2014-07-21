<?php
/**
 * sql_builder_test.inc 提供SqlBuilder类单元测试服务。
 *
 * @copyright   2012-2013 SkiHat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      SkiHat Team
 *
 * @package     skihat.kernels.databases
 * @version     1.0
 *
 * @create      2012-08-30
 * @modify      2013-09-11
 */

# 引入测试测试启动文件。
require_once(dirname(dirname(dirname(dirname(__FILE__)))) . '/startup.inc');

# 引用测试准备文件。
Skihat::import('kernels.databases.database', SKIHAT_PATH_LIBRARY);
Skihat::import('kernels.databases.sql_builder', SKIHAT_PATH_LIBRARY);

/**
 * SqlBuilder 单元测试类，提供对SqlBuilder的单元测试服务。
 *
 * @package skihat.kernels.databases
 */
class SqlBuilderTest extends PHPUnit_Framework_TestCase {
    /**
     * 测试create。
     */
    public function testCreate() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->create(array('name' => 'jack',
            'allow' => 1,
            'description' => 'jack description',
            'group' => 1), array(IDataSupport::SYNTAX_TABLE => 'user'));

        $this->assertEquals('INSERT INTO [user]([name],[allow],[description],[group]) VALUES(?,?,?,?)', $cmd);
        $this->assertEquals('jack', $args[0]);
        $this->assertEquals(1, $args[1]);
        $this->assertEquals('jack description', $args[2]);
        $this->assertEquals(1, $args[3]);
    }

    /**
     * 测试Create，指定字段。
     */
    public function testCreateForSyntaxFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->create(array('name' => 'jack',
            'allow' => 1,
            'description' => 'jack description',
            'group' => 1), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_FIELDS => array('name',
                'allow',
                'group')));

        $this->assertEquals('INSERT INTO [user]([name],[allow],[group]) VALUES(?,?,?)', $cmd);
        $this->assertEquals('jack', $args[0]);
        $this->assertEquals(1, $args[1]);
        $this->assertEquals(1, $args[2]);
    }

    /**
     * 测试创建，数据字段值多于表的值。
     */
    public function testCreateForFieldsMore() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->create(array('name' => 'jack',
            'allow' => 1,
            'more' => true,
            'description' => 'jack description',
            'group' => 1), array(IDataSupport::SYNTAX_TABLE => 'user'));

        $this->assertEquals('INSERT INTO [user]([name],[allow],[more],[description],[group]) VALUES(?,?,?,?,?)', $cmd);
        $this->assertEquals('jack', $args[0]);
        $this->assertEquals(1, $args[1]);
        $this->assertEquals(true, $args[2]);
        $this->assertEquals('jack description', $args[3]);
        $this->assertEquals(1, $args[4]);
    }

    /**
     * 测试create，创建多个数据，暂时不实现。
     */
    public function testCreateMultiple() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->createMultiple(
            array('name', 'allow', 'description', 'group'),
            array(
                array('jack', 1, 'jack description', 1),
                array('jack 1', false, 'jack description2', 2)
            ),
            array(IDataSupport::SYNTAX_TABLE => 'user'));

        $this->assertEquals('INSERT INTO [user]([name],[allow],[description],[group]) VALUES(?,?,?,?),(?,?,?,?)', $cmd);

        $this->assertEquals('jack', $args[0]);
        $this->assertEquals(1, $args[1]);
        $this->assertEquals('jack description', $args[2]);
        $this->assertEquals(1, $args[3]);

        $this->assertEquals('jack 1', $args[4]);
        $this->assertEquals(false, $args[5]);
        $this->assertEquals('jack description2', $args[6]);
        $this->assertEquals(2, $args[7]);
    }

    /**
     * 测试update。
     */
    public function testUpdate() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('name' => 'new',
            'description' => 'new'), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User'));

        $this->assertEquals('UPDATE [user] AS [User] SET [name] = ? ,[description] = ?', trim($cmd));
        $this->assertEquals('new', $args[0]);
        $this->assertEquals('new', $args[1]);
    }

    /**
     * 测试update，字符串字段。
     */
    public function testUpdateForStringFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update('group = 2,allow = 0', array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User'));

        $this->assertEquals('UPDATE [user] AS [User] SET group = 2,allow = 0', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试update，使用更新参数。
     */
    public function testUpdateForStringArgsFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->update(array('group = ?,allow = ?', 1, 0), array(IDataSupport::SYNTAX_TABLE => 'user'));

        $this->assertEquals('UPDATE [user] SET group = ?,allow = ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(0, $args[1]);
    }

    /**
     * 测试更新，包含更多字段。
     */
    public function testUpdateForMoreFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('group' => 1,
            'allow' => 2,
            'more' => '3'), array(IDataSupport::SYNTAX_TABLE => 'user'));

        $this->assertEquals('UPDATE [user] SET [group] = ? ,[allow] = ? ,[more] = ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(2, $args[1]);
    }

    /**
     * 测试Update，指定字段。
     */
    public function testUpdateForSyntaxFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('group' => 1,
            'allow' => 2,
            'description' => 'example'), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_FIELDS => array('group',
                'allow')));

        $this->assertEquals('UPDATE [user] SET [group] = ? ,[allow] = ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(2, $args[1]);
    }

    /**
     * 测试Update，指定字符串条件。
     */
    public function testUpdateForSyntaxWhereString() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('group' => 1,
            'allow' => 2), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => 'group = 2'));

        $this->assertEquals('UPDATE [user] SET [group] = ? ,[allow] = ?   WHERE group = 2', $cmd);
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(2, $args[1]);
    }

    /**
     * 测试Update，指定参数条件。
     */
    public function testUpdateForSyntaxWhereStringArgs() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('group' => 1,
            'allow' => 2), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => array('group = ?',
                1)));

        $this->assertEquals('UPDATE [user] SET [group] = ? ,[allow] = ?   WHERE group = ?', $cmd);
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(2, $args[1]);
        $this->assertEquals(1, $args[2]);
    }

    /**
     * 测试Update，指定数组条件。
     */
    public function testUpdateForSyntaxWhereArray() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('group' => 1,
            'allow' => 2), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => array('group' => 1)));

        $this->assertEquals('UPDATE [user] SET [group] = ? ,[allow] = ?   WHERE [group] = ?', $cmd);
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(2, $args[1]);
        $this->assertEquals(1, $args[2]);
    }

    /**
     * 测试Update，指定Space条件。
     */
    public function testUpdateForSyntaxWhereSpaceArrayKey() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('group' => 1,
            'allow' => 2), array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => array('group like' => 1)));

        $this->assertEquals('UPDATE [user] SET [group] = ? ,[allow] = ?   WHERE group like ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
        $this->assertEquals(2, $args[1]);
        $this->assertEquals(1, $args[2]);
    }

    /**
     * 测试Delete。
     */
    public function testDelete() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->delete(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_TABLE_ALIAS => 'User'));

        $this->assertEquals('DELETE [User] FROM [user] AS [User]', trim($cmd));
        $this->assertEmpty($args);
    }


    /**
     * 测试Delete，指定字符串where条件。
     */
    public function testDeleteForSyntaxWhereString() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->delete(array(IDataSupport::SYNTAX_TABLE => 'user', 'where' => 'group = 1'));

        $this->assertEquals('DELETE [user] FROM [user]  WHERE group = 1', $cmd);
        $this->assertEmpty($args);
    }

    /**
     * 测试Delete，指定字符串参数条件。
     */
    public function testDeleteForSyntaxWhereStringArgs() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->delete(array(IDataSupport::SYNTAX_TABLE => 'user', 'where' => array('group = ?', 1)));

        $this->assertEquals('DELETE [user] FROM [user]  WHERE group = ?', $cmd);
        $this->assertEquals(1, $args[0]);
    }

    /**
     * 测试Delete，指定数组where条件。
     */
    public function testDeleteForSyntaxWhereArray() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->delete(array(IDataSupport::SYNTAX_TABLE => 'user', 'where' => array('group' => 1)));

        $this->assertEquals('DELETE [user] FROM [user]  WHERE [group] = ?', $cmd);
        $this->assertEquals(1, $args[0]);
    }

    /**
     * 测试Delete，指定字格字符参数条件。
     */
    public function testDeleteForSyntaxWhereArraySpaceKey() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->delete(array(IDataSupport::SYNTAX_TABLE => 'user', 'where' => array('group like ' => 1)));

        $this->assertEquals('DELETE [user] FROM [user]  WHERE group like  ?', $cmd);
        $this->assertEquals(1, $args[0]);
    }

    /**
     * 测试fetch。
     */
    public function testFetch() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_TABLE_ALIAS => 'User'));

        $this->assertEquals('SELECT * FROM [user] AS [User]', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，字符串条件。
     */
    public function testFetchForSyntaxWhereString() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_WHERE => 'group = 1'));

        $this->assertEquals('SELECT * FROM [user] WHERE group = 1', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，字符参数条件。
     */
    public function testFetchForSyntaxWhereStringArgs() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_WHERE => array('group = ?', 1)));

        $this->assertEquals('SELECT * FROM [user] WHERE group = ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
    }

    /**
     * 测试fetch，数组条件。
     */
    public function testFetchForSyntaxWhereArray() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_WHERE => array('group' => 1)));

        $this->assertEquals('SELECT * FROM [user] WHERE [group] = ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
    }

    /**
     * 测试fetch，数组字格关键字。
     */
    public function testFetchForSyntaxWhereArraySpaceKey() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_WHERE => array('group like' => 1)));

        $this->assertEquals('SELECT * FROM [user] WHERE group like ?', trim($cmd));
        $this->assertEquals(1, $args[0]);
    }

    /**
     * 测试fetch，指定fields。
     */
    public function testFetchForSyntaxStringFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) =
            $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_FIELDS => 'group,description'));

        $this->assertEquals('SELECT group,description FROM [user]', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，指定数组fields。
     */
    public function testFetchForSyntaxArrayFields() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_FIELDS => array('group', 'description')));

        $this->assertEquals('SELECT [group],[description] FROM [user]', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，指定数组分组。
     */
    public function testFetchForSyntaxArrayGroupBy() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_GROUP => array('id', 'name')));

        $this->assertEquals('SELECT * FROM [user] GROUP BY [id],[name]', $cmd);
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，指定数组分组。
     */
    public function testFetchForSyntaxStringGroupBy() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user', IDataSupport::SYNTAX_GROUP => 'id,name'));

        $this->assertEquals('SELECT * FROM [user] GROUP BY id,name', $cmd);
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，指定排序语法。
     */
    public function testFetchForSyntaxOrderArray() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => 'group = 1',
            IDataSupport::SYNTAX_ORDER => array('group' => 'desc')));

        $this->assertEquals('SELECT * FROM [user] WHERE group = 1 ORDER BY [group] desc', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试fetch，指定字符串排序。
     */
    public function testFetchForSyntaxOrderString() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => 'group = 1',
            IDataSupport::SYNTAX_ORDER => 'group desc'));

        $this->assertEquals('SELECT * FROM [user] WHERE group = 1 ORDER BY group desc', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 指定fetch，指定分页语法。
     */
    public function testFetchForSyntaxPage() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_WHERE => 'group = 1',
            IDataSupport::SYNTAX_PAGE => array(2, 20),
            IDataSupport::SYNTAX_ORDER => 'group desc'));

        $this->assertEquals('SELECT * FROM [user] WHERE group = 1 ORDER BY group desc LIMIT 20,20', trim($cmd));
        $this->assertEmpty($args);
    }

    /**
     * 测试Update，指定语法数组join和links。
     *
     * group = array('class' => 'models.group#Group',
     *               'type' => 'hasMany',
     *               'on'   => 'User.group = Group.id',
     *               'foreign' => 'group');
     *
     * 关系类、连接类型、连接条件、外键、中间表、关系类型
     */
    public function testUpdateForSyntaxArrayLink() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('Group.name' => 'hello',
            'User.name' => 'new name'), array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_FIELDS => array('Group.name',
                'User.name'),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => array('Group'),
            IDataSupport::SYNTAX_JOIN => array('Group' => array(IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'Group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = Group.id'))));

        $this->assertEquals('UPDATE [user] AS [User] LEFT JOIN [group] AS [Group] ON User.id = Group.id  SET [Group].[name] = ? ,[User].[name] = ?   WHERE [Group].[id] = ?', $cmd);
        $this->assertEquals('hello', $args[0]);
        $this->assertEquals('new name', $args[1]);
        $this->assertEquals(12, $args[2]);
    }

    /**
     * 测试Update，指定语法字符串join和links。
     *
     * group = array('class' => 'models.group#Group',
     *               'type' => 'hasMany',
     *               'on'   => 'User.group = Group.id',
     *               'foreign' => 'group');
     *
     * 关系类、连接类型、连接条件、外键、中间表、关系类型
     */
    public function testUpdateForSyntaxStringJoin() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->update(array('Group.name' => 'hello',
            'User.name' => 'new name'), array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_FIELDS => array('Group.name',
                'User.name'),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => 'Group',
            IDataSupport::SYNTAX_JOIN => array('Group' => array(IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'Group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = Group.id'))));

        $this->assertEquals('UPDATE [user] AS [User] LEFT JOIN [group] AS [Group] ON User.id = Group.id  SET [Group].[name] = ? ,[User].[name] = ?   WHERE [Group].[id] = ?', $cmd);
        $this->assertEquals('hello', $args[0]);
        $this->assertEquals('new name', $args[1]);
        $this->assertEquals(12, $args[2]);
    }


    /**
     * 测试Delete，指定joins语法。
     */
    public function testDeleteForSyntaxArrayJoin() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->delete(array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => array('Group'),
            IDataSupport::SYNTAX_JOIN => array('Group' => array(IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'Group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = Group.id'))));

        $this->assertEquals('DELETE [User],[Group] FROM [user] AS [User] LEFT JOIN [group] AS [Group] ON User.id = Group.id   WHERE [Group].[id] = ?', $cmd);
        $this->assertEquals(12, $args[0]);
    }

    /**
     * 测试DELETE，没有指定连接别名。
     */
    public function testDeleteForSyntaxMissingAlias() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->delete(array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => array('Group'),
            IDataSupport::SYNTAX_JOIN => array('Group' => array(IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = group.id'))));

        $this->assertEquals('DELETE [User],[group] FROM [user] AS [User] LEFT JOIN [group] ON User.id = group.id   WHERE [Group].[id] = ?', $cmd);
        $this->assertEquals(12, $args[0]);
    }

    /**
     * 测试Delete，指定字符串joins语法。
     */
    public function testDeleteForSyntaxStringJoin() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->delete(array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => 'Group',
            IDataSupport::SYNTAX_JOIN => array('Group' => array(IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'Group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = Group.id'))));

        $this->assertEquals('DELETE [User],[Group] FROM [user] AS [User] LEFT JOIN [group] AS [Group] ON User.id = Group.id   WHERE [Group].[id] = ?', $cmd);
        $this->assertEquals(12, $args[0]);
    }

    /**
     * 测试fetch，指定数组joins。
     */
    public function testFetchForJoinsArraySyntax() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_FIELDS => array('Group.name', 'User.name'),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => array('Group'),
            IDataSupport::SYNTAX_JOIN => array('Group' => array(IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'Group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = Group.id'))));

        $this->assertEquals('SELECT [Group].[name],[User].[name] FROM [user] AS [User] LEFT JOIN [group] AS [Group] ON User.id = Group.id  WHERE [Group].[id] = ?', trim($cmd));
        $this->assertEquals(12, $args[0]);
    }

    /**
     * 测试fetch，指定字符串joins。
     */
    public function testFetchForJoinsStringSyntax() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        list($cmd, $args) = $builder->fetch(array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_FIELDS => array('Group.name', 'User.name'),
            IDataSupport::SYNTAX_TABLE => 'user',
            IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
            IDataSupport::SYNTAX_LINK => 'Group',
            IDataSupport::SYNTAX_JOIN => array('Group' => array(
                IDataSupport::SYNTAX_TABLE => 'group',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'Group',
                IDataSupport::SYNTAX_JOIN_ON => 'User.id = Group.id'))));

        $this->assertEquals('SELECT [Group].[name],[User].[name] FROM [user] AS [User] LEFT JOIN [group] AS [Group] ON User.id = Group.id  WHERE [Group].[id] = ?', trim($cmd));
        $this->assertEquals(12, $args[0]);
    }

    /**
     * 测试joins，缺失JoinOn参数。
     *
     * @expectedException DBSyntaxException
     */
    public function testJoinForMissingLinks() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        $builder->fetch(array(
                IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
                IDataSupport::SYNTAX_FIELDS => array('Group.name', 'User.name'),
                IDataSupport::SYNTAX_TABLE => 'user',
                IDataSupport::SYNTAX_TABLE_ALIAS => 'User',
                IDataSupport::SYNTAX_LINK => 'Group')
        );
    }

    /**
     * 测试，没有提供table语法。
     *
     * @expectedException DBSyntaxException
     */
    public function testForNotSupportTable() {
        $builder = new SqlBuilderMock(new SqlBuilderDBEngineMock());
        $builder->fetch(array(IDataSupport::SYNTAX_WHERE => array('Group.id' => 12),
            IDataSupport::SYNTAX_FIELDS => array('Group.name', 'User.name')));
    }
}

/**
 * Sql builder 测试模拟对象，提供对SqlBuilder的模拟测试服务。
 *
 * @package skihat.kernels.databases
 */
class SqlBuilderMock extends SqlBuilder {
    /**
     * @see SqlBuilder::safeName
     */
    public function safeName($name) {
        return str_replace('.', '].[', "[$name]");
    }
}

/**
 * Sql 生成器，数据引擎模拟类。
 *
 * @package skihat.kernels.databases
 */
class SqlBuilderDBEngineMock implements IDatabaseEngine {
    public function create(array $fields, array $syntax) {
    }

    public function createMultiple(array $fields, array $values, array $syntax) {

    }

    public function update($fields, array $syntax) {
    }

    public function delete(array $syntax) {
    }

    public function fetch(array $syntax, $fetchStyle = self::FETCH_STYLE_ASSOC) {
    }

    public function fetchAll(array $syntax, $fetchStyle = self::FETCH_STYLE_ASSOC) {
    }

    public function fetchLastID() {
    }

    public function fetchSchema($table) {
        switch ($table) {
            case 'user':
                return array(
                    'id' => array(
                        self::SCHEMA_FIELD => 'id',
                        self::SCHEMA_TYPE => 'int',
                        self::SCHEMA_SIZE => 11),
                    'name' => array(
                        self::SCHEMA_FIELD => 'name',
                        self::SCHEMA_TYPE => 'varchar',
                        self::SCHEMA_SIZE => 32),
                    'allow' => array(
                        self::SCHEMA_FIELD => 'allow',
                        self::SCHEMA_TYPE => 'smallint',
                        self::SCHEMA_SIZE => 6),
                    'description' => array(
                        self::SCHEMA_FIELD => 'description',
                        self::SCHEMA_TYPE => 'varchar',
                        self::SCHEMA_SIZE => 256),
                    'group' => array(
                        self::SCHEMA_FIELD => 'group',
                        self::SCHEMA_TYPE => 'int',
                        self::SCHEMA_SIZE => 11)
                );
                break;
        }

        return array();
    }

    public function execute($cmd, array $args = array(), $options = array()) {
    }

    public function query($cmd, array $args = array(), $options = array()) {
    }
}

