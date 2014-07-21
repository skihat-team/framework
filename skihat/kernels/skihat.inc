<?php
/**
 * skihat.inc 提供Skihat核心类的声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels
 * @version   1.0
 *
 * @created   2012-06-12
 * @modified  2013-08-02
 */

/**
 * SkiHat 框架核心异常类，Skihat类的所有异常都必须继承自SkihatException类。
 *
 * @package skihat.kernels
 */
class SkihatException extends Exception {
}

/**
 * SkiHat 配置异常，当动态配置发生错误时，将引发当前异常。
 *
 * @package skihat.kernels
 */
class ConfigException extends SkihatException {
}

/**
 * 文件未找到异常，当需要查找的文件不存在时，将引发当前异常。
 *
 * @package skihat.kernels
 */
class FileNotFoundException extends SkihatException {
}

/**
 * 类型未找到异常，如果指定的类型不存在时，将引发当前异常。
 *
 * @package skihat.kernels
 */
class TypeNotFoundException extends SkihatException {
}

/**
 * Skihat类，提供框架的内核处理服务，内核由动态配置，调度，Ioc和i18n核心服务。
 *
 * @package skihat.kernels
 */
abstract class Skihat {
    /**
     * 根据$paths参数注册全局文件路径，注意：路径中不包含不需要'/'结局符。
     *
     * @param array $paths 路径地址集。
     */
    public static function register(array $paths) {
        self::$_paths = $paths;
    }

    /**
     * 根据$name和$value参数写入动太配置信息，如果$name已存在则覆盖原有值。
     *
     * @param string $name  配置名字。
     * @param mixed  $value 配置值。
     */
    public static function write($name, $value) {
        self::$_conf[$name] = $value;
    }

    /**
     * 根据$name和$default参数读取动态配置信息，如果$name指定值不存在，则返回default值。
     *
     * @param string     $name    配置名称。
     * @param bool|mixed $default 默认值。
     *
     * @return mixed
     */
    public static function read($name, $default = FALSE) {
        return isset(self::$_conf[$name]) ? self::$_conf[$name] : $default;
    }

    /**
     * 根据$name和$message记录调试跟踪信息，允许相同的$name写入多个记录。
     *
     * @param string $name    调试类型名称。
     * @param string $message 调试消息。
     */
    public static function debug($name, $message) {
        # 格式化消息.
        $line = sprintf("<li><b>{$name}</b>:{$message}</li>", $name, $message);

        if (isset(self::$_debugs[$name])) {
            self::$_debugs[$name][] = $line;
        } else {
            self::$_debugs[$name] = array($line);
        }

        array_push(self::$_debugLines, $line);
    }

    /**
     * 根据$name和$file参数格式化输出调试信息，如果$file参数不为空，表示将结果保存到$file指定文件中。
     *
     * @param bool|string $name 调试类别，默认值false表示返回全部调试信息。
     * @param bool|string $file 记录文件，默认值false表示不记录到文件中。
     *
     * @return mixed
     */
    public static function debugLines($name = FALSE, $file = FALSE) {
        # 根据参数，获取需要输出的信息。
        if ($name && isset(self::$_debugs[$name])) {
            $lines = self::_debugLines(self::$_debugs[$name]);
        } else if (!$name) {
            $lines = self::_debugLines(self::$_debugLines);
        } else {
            $lines = "Skihat:debug not support '{$name}' debug lines.";
        }

        if ($file) {
            file_put_contents(SKIHAT_PATH_DATA_LOGGERS . DIRECTORY_SEPARATOR . $file, $lines, FILE_APPEND);
        }

        return $lines;
    }

    /**
     * 根据$file,$paths和$ext参数查找文件，如果指定文件存在则返回文件的完整路径，不存在返回false。
     *
     * @example
     *  <pre>
     *      # 从全局环境中导入controllers/filters/security_filter.inc。
     *      Skihat::locate('controllers.filters.security_filter');
     *
     *      # 从SKIHAT_PATH_LIBRARY中导入controllers/filters/security_filter.inc。
     *      Skihat::locate('controllers.filters.security_filter',SKIHAT_PATH_LIBRARY);
     *
     *      # 从SKIHAT_PATH_APP和SKIHAT_PATH_LIBRARY中导入文件，优先导入SKIHAT_PATH_APP路径下的文件。
     *      Skihat::locate('controllers.filters.security_filter',array(SKIHAT_PATH_APP,SKIHAT_PATH_LIBRARY));
     *  </pre>
     *
     * @param string $file  文件名称，子目录使用SKIHAT_PACKAGE_SEPARATOR分隔。
     * @param mixed  $paths 文件查找路径，如果参数为false则使用全局路径。
     * @param string $ext   文件扩展名，默认为SKIHAT_FILE_EXT常量。
     *
     * @return string|bool
     */
    public static function locate($file, $paths = false, $ext = SKIHAT_FILE_EXT) {
        # 转换文件名，获取真实的文件名部分。
        $realFile = self::_realFile($file, $ext);
        if ($file && is_string($paths)) {
            return self::_fullFile($paths, $realFile);
        }

        if (!$paths) $paths = self::$_paths;
        foreach ($paths as $path) {
            if ($fullFile = self::_fullFile($path, $realFile)) {
                return $fullFile;
            }
        }

        return false;
    }

    /**
     * 根据$file，$paths和$ext参数查找并引用文件并返回文件返回值，如果文件已引用将直接返回值。
     *
     * @example
     *  <pre>
     *      Skihat::import('kernels.caches.engines.file_cache_engine');
     *      Skihat::import('kernels.caches.engines.file_cache_engine',SKIHAT_PATH_LIBRARY);
     *      Skihat::import('kernels.caches.engines.file_cache_engine',array(SKIHAT_PATH_LIBRARY));
     *  </pre>
     *
     * @see Skihat::locate
     *
     * @param string $file  完整类名字，允许使用包分隔符。
     * @param mixed  $paths 文件查找路径，默认值false使用全局路径。
     * @param string $ext   文件扩展名，默认为SKIHAT_FILE_EXT。
     *
     * @return mixed 返回引用文件中的返回值。
     *
     * @throws FileNotFoundException 如果文件没有找到引发异常。
     */
    public static function import($file, $paths = FALSE, $ext = SKIHAT_FILE_EXT) {
        # 生成文件缓存key码。
        $fileKey = $file . $ext;

        if (!isset(self::$_files[$fileKey])) {
            if ($fullFile = self::locate($file, $paths, $ext)) {
                return (self::$_files[$fileKey] = include($fullFile));
            }

            throw new FileNotFoundException("Skihat:import not found file - '{$file}' in import.");
        }

        return self::$_files[$fileKey];
    }

    /**
     * 根据$class，$paths和$ext参数引用类并返回类名称，如果指定类还未引入将自动引入类。
     *
     * @example
     *  <pre>
     *      Skihat::using('kernels.caches.engines.file_cache_engine');  # FileCacheEngine
     *      Skihat::using('kernels.caches.engines.file_cache_engine#FileCacheEngine'); # FileCacheEngine
     *  </pre>
     *
     * @param string $class 完整类声明，允许使用包分隔符和类分隔符。
     * @param mixed  $paths 文件查找路径，默认值false使用全局路径。
     * @param string $ext   文件扩展名，默认为SKIHAT_FILE_EXT。
     *
     * @return string 引用类名称。
     *
     * @throws FileNotFoundException 如果文件未找到引发异常。
     * @throws TypeNotFoundException 如果类型未找到引发异常。
     */
    public static function using($class, $paths = false, $ext = SKIHAT_FILE_EXT) {
        list($file, $clsName) = self::_splitClass($class);

        # 判断类是否存在，不存在则从文件中导入。
        if (!class_exists($clsName) && !interface_exists($clsName)) {
            self::import($file, $paths, $ext);

            if (!class_exists($clsName) && !interface_exists($clsName)) {
                throw new TypeNotFoundException("Skihat:using type not found - '{$clsName}' in '{$file}");
            }
        }

        return $clsName;
    }

    /**
     * 根据$conf，$paths和$ext参数创建对象实例，并返回值。
     *
     * @example
     * <pre>
     *      Skihat::ioc('#FileCacheEngine');
     *      Skihat::ioc('kernels.caches.engines.file_cache_engine',SKIHAT_PATH_LIBRARY);
     *      Skihat::ioc('kernels.caches.engines.file_cache_engine#FileCacheEngine',SKIHAT_PATH_LIBRARY);
     *
     *      Skihat::ioc(array(
     *               # <b>特殊</b>name指定类名,使用$clsName格式。
     *              'class'      => 'controllers.filters.security_filter#SecurityFilter',
     *              'paths'      => SKIHAT_PATH_APP,    # <b>特殊</b>指定类路径
     *              'initial'    => true,               # <b>特殊</b>name指定初始化后执行SecurityFilter#initialize()方法。
     *              'authName'   => 'default',          # 设置SecurityFilter#$authName 字段值。
     *              'authorName' => 'default',          # 设置SecurityFilter#$authorName 字段值。
     *                       ...                        # 设置其它属性值
     *      ));
     * </pre>
     *
     * @param array|string $conf  ioc配置参数，允许使用字符串和数组。
     * @param mixed        $paths 类查找路径，默认值false表示使用全局路径。
     * @param string       $ext   文件扩展名，默认为SKIHAT_FILE_EXT。
     *
     * @return mixed
     *
     * @throws FileNotFoundException 如果类所在的文件未找到，将引发异常。
     * @throws TypeNotFoundException 如果指定类型未找到，引发异常。
     * @throws ConfigException 如果$conf没有置SKIHAT_IOC_CLASS声明。
     */
    public static function ioc($conf, $paths = false, $ext = SKIHAT_FILE_EXT) {
        if (is_string($conf)) {
            return self::_createObject($conf, $paths, $ext);
        }

        list($clsName, $paths, $ext, $initial) = self::_splitIoc($conf, $paths, $ext);
        if (!$clsName) {
            throw new ConfigException('Skihat:ioc conf not support SKIHAT_IOC_CLASS.');
        }

        $obj = self::_createObject($clsName, $paths, $ext);
        foreach ($conf as $prop => $val) {
            $obj->$prop = $val;
        }

        # 如果包含初始化方法，并且方法存在。
        if ($initial && method_exists($obj, $initial)) {
            $obj->$initial();
        } else if ($initial) {
            trigger_error("Skihat:ioc conf initialize error,class - '{$clsName}' not support method {$initial}");
        }

        return $obj;
    }

    /**
     * 根据className获取类的唯一单一实例。
     *
     * @param  string $className 类的完整名称。
     * @param mixed   $paths     类的查找路径。
     * @param string  $ext       文件扩展名。
     *
     * @return mixed
     */
    public static function singleton($className, $paths = false, $ext = SKIHAT_FILE_EXT) {
        # 如果指定的类还没有在实例中创建；
        if (!isset(self::$_singletons[$className])) {
            self::$_singletons[$className] = self::ioc($className, $paths, $ext);
        }

        return self::$_singletons[$className];
    }

    /**
     * 根据$name和$lang参数获取国际化字符串，如果指定字符串不存在则返回默认字符串。
     *
     * @example
     *  <pre>
     *      Skihat::i18n('sknuke:order.title');   # sknuke模块order语言包(app/models/sknuke/langs/zh-CN/app_order.inc)。
     *      Skihat::i18n('order.title');          # 全局语言包（app/langs/zh-CN/default.lang.inc)
     *  </pre>
     *
     * @param string $name    字符配置参数。
     * @param string $default 默认值。
     * @param string $lang    语言。
     *
     * @return string
     */
    public static function i18n($name, $default = '', $lang = SKIHAT_I18N_LANG) {
        list($file, $key) = explode(SKIHAT_PACKAGE_SEPARATOR, $name);
        $langKey = $file . '-' . $lang;

        if (!isset(self::$_langs[$langKey])) {
            list($path, $file) = self::_splitI18n($file);

            if (($fullFile = self::_fullFile($path, $file . '.' . $lang . SKIHAT_FILE_EXT)) ||
                ($fullFile = self::_fullFile($path, $file . SKIHAT_FILE_EXT))
            ) {
                self::$_langs[$langKey] = (array)include($fullFile);
            } else {
                trigger_error("Skihat:i18n not support lang file - '{$file}' in '{$path}'");
                self::$_langs[$langKey] = array();
            }
        }

        # 如果指定资源名称存在。
        return array_key_value(self::$_langs[$langKey], $key, $default);
    }

    /**
     * 返回当前框架的版本信息。
     *
     * @return string
     */
    public static function version() {
        return '1.0 beta';
    }

    /**
     * 根据$debugs和$name参数格式化调试信息，并返回格式化后的值。
     *
     * @param array $debugs 调试数组。
     *
     * @return string
     */
    protected static function _debugLines(array $debugs) {
        return "\r\n\r\n<ul>\r\n" . implode("\r\n", $debugs) . "\r\n</ul>\r\n";
    }

    /**
     * 根据$file和$ext参数，获取真实的文件信息。
     *
     * @param string $file 文件名称，允许使用SKIHAT_PACKAGE_SEPARATOR常量。
     * @param string $ext  文件扩展名。
     *
     * @return string
     */
    protected static function _realFile($file, $ext) {
        return str_replace(SKIHAT_PACKAGE_SEPARATOR, DIRECTORY_SEPARATOR, $file) . $ext;
    }

    /**
     * 根据$path和$file参数，获取文件完整路径信息，如果文件不存在则返回false。
     *
     * @param string $path 文件基本路径。
     * @param string $file 文件名称。
     *
     * @return string|bool
     */
    protected static function _fullFile($path, $file) {
        return is_file($fullFile = $path . DIRECTORY_SEPARATOR . $file) ? $fullFile : false;
    }

    /**
     * 根据参数$class分隔出文件名和类名。
     *
     * @param string $class 包含文件名的类声明。　
     *
     * @return array(file,clsName);
     */
    protected static function _splitClass($class) {
        # 判断是否包含类分隔符，如果包含则直接分隔返回。
        if (strpos($class, SKIHAT_CLASS_SEPARATOR) !== false) {
            return explode(SKIHAT_CLASS_SEPARATOR, $class);
        } else if (($pos = strrpos($class, SKIHAT_PACKAGE_SEPARATOR)) !== false) {
            return array($class, pascal(substr($class, $pos + 1)));
        }

        return array($class, pascal($class));
    }

    /**
     * 使用$class,$paths和$ext参数创建对象实例。
     *
     * @param string       $class 类的完整声明。
     * @param string|array $paths 类所在的路径。
     * @param string       $ext   文件扩展名。
     */
    protected static function _createObject($class, $paths, $ext = SKIHAT_FILE_EXT) {
        $clsName = self::using($class, $paths, $ext);
        return new $clsName();
    }

    /**
     * 根据Ioc数组配置信息，分隔出类名，路径，文件扩展名和初始化选项参数。
     *
     * @param array  $conf  ioc配置信息。
     * @param mixed  $paths 路径信息。
     * @param string $ext   文件扩展名。
     *
     * @return array(clsName,$paths,$ext,$initial);
     */
    protected static function _splitIoc(array &$conf, $paths, $ext) {
        # 处理数组配置参数。
        $initial = isset($conf[SKIHAT_IOC_INITIAL]) ? $conf[SKIHAT_IOC_INITIAL] : false;
        $clsName = isset($conf[SKIHAT_IOC_CLASS]) ? $conf[SKIHAT_IOC_CLASS] : false;
        $paths = isset($conf[SKIHAT_IOC_PATHS]) ? $conf[SKIHAT_IOC_PATHS] : $paths;
        $ext = isset($conf[SKIHAT_IOC_EXT]) ? $conf[SKIHAT_IOC_EXT] : $ext;

        unset($conf[SKIHAT_IOC_CLASS], $conf[SKIHAT_IOC_PATHS],
        $conf[SKIHAT_IOC_EXT], $conf[SKIHAT_IOC_INITIAL]);

        return array($clsName, $paths, $ext, $initial);
    }

    /**
     * 根据资源的文件名称分离出模块和文件名信息。
     *
     * @param string $file 文件名称。
     *
     * @return array(module,file)
     */
    protected static function _splitI18n($file) {
        if (count($splits = explode(':', $file)) > 1) {
            return array(SKIHAT_PATH_APP_MODULES . DIRECTORY_SEPARATOR . $splits[0] . DIRECTORY_SEPARATOR . 'langs', $splits[1]);
        }

        return array(SKIHAT_PATH_APP . DIRECTORY_SEPARATOR . 'langs', $file);
    }

    /**
     * @var array 动态配置变量，提供动态配置保存信息。
     */
    protected static $_conf = array();

    /**
     * @var array 全局路径变量，提供路径保存信息。
     */
    protected static $_paths = array();

    /**
     * @var array 文件内容变量，记录已导入的文件信息。
     */
    protected static $_files = array();

    /**
     * @var array 语言变量，提供语言信息。
     */
    protected static $_langs = array();

    /**
     * @var array 动态调试记录变量，根据分类记录调试信息。
     */
    protected static $_debugs = array();

    /**
     * @var array 动态调试记录记录变量，根据写入时间记录调试。
     */
    protected static $_debugLines = array();

    /**
     * @var array 单例实例集。
     */
    protected static $_singletons = array();
}

/**
 * Ioc 工厂类，提供根据数组进行Ioc实例配置和管理服务。
 *
 * @package skihat.kernels
 */
class IocFactory {

    /**
     * 根据$conf参数初始化IocFactory工厂。
     *
     * @param array $conf Ioc配置信息，索引值为名称，值为Ioc配置参数。
     */
    public function __construct(array $conf) {
        $this->_conf = $conf;
    }

    /**
     * 根据$name参数获取Ioc对象实例，如果实例未创建则创建实例，已存在则直接返回实例。
     *
     * @param string $name 实例名称。
     *
     * @return mixed
     *
     * @throws ConfigException 如果指定$name在配置中不存在则引发异常。
     */
    public function instance($name) {
        if (!isset($this->_instances[$name])) {
            if (isset($this->_conf[$name])) {
                return ($this->_instances[$name] = Skihat::ioc($this->_conf[$name]));
            }

            throw new ConfigException("IocFactory:instance not support '$name");
        }

        return $this->_instances[$name];
    }

    /**
     * 返回全部实例的名称集。
     *
     * @return array
     */
    public function instances() {
        return array_keys($this->_conf);
    }

    /**
     * @var array Ioc配置信息，索引值为名称，值为Ioc配置参数。
     */
    protected $_conf = array();

    /**
     * @var array 已创建的Ioc实例信息。
     */
    protected $_instances = array();
}


/**
 * 集合基础对象，提供基本的ArrayAccess接口实现。
 *
 * @package skihat.kernels
 */
abstract class CollectionBase implements ArrayAccess {
    /**
     * @see ArrayAccess::offsetExists
     */
    public function offsetExists($name) {
        return isset($this->_vars[$name]);
    }

    /**
     * @see ArrayAccess::offsetGet
     */
    public function offsetGet($name) {
        return isset($this->_vars[$name]) ? $this->_vars[$name] : null;
    }

    /**
     * @see ArrayAccess::offsetSet
     */
    public function offsetSet($name, $value) {
        $this->_vars[$name] = $value;
    }

    /**
     * @see ArrayAccess::offsetUnset
     */
    public function offsetUnset($name) {
        unset($this->_vars[$name]);
    }

    /**
     * @var array 集合信息存储字端。
     */
    protected $_vars = array();
}