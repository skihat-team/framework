<?php
/**
 * media.inc 提供网站内部资源管理接口声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.meidas
 * @version   1.0
 *
 * @created   2012-07-12
 * @modified  2013-08-06
 */

/**
 * 内部资源异常，当发生内部资源错误时将引发本异常。
 *
 * @package skihat.kernels.medias
 */
class MediaException extends SkihatException {
}

/**
 * 内部资源引擎，提供详细的内部资源管理服务。
 *
 * @package skihat.kernels.medias
 */
interface IMediaEngine {
    /**
     * @const string 文件元数据，文件名称。 FILE_SCHEMA_FILE_NAME
     */
    const SCHEMA_FILE_NAME = 'filename';

    /**
     * @const string 文件元数据，文件大小。
     */
    const SCHEMA_FILE_SIZE = 'file_size';

    /**
     * @const string 文件元数据，文件类型。
     */
    const SCHEMA_FILE_TYPE = 'filetype';

    /**
     * @const string 文件元数据，文件路径。
     */
    const SCHEMA_FILE_PATH = 'dir_path';

    /**
     * @const string 文件元数据，是否为目录。
     */
    const SCHEMA_IS_DIR = 'is_dir';

    /**
     * @const string 文件元数据，是否包含文件。
     */
    const SCHEMA_HAS_FILE = 'has_file';

    /**
     * @const string 文件元数据，是否为图片。
     */
    const SCHEMA_IS_PHOTO = 'is_photo';

    /**
     * @const string 文件元数据，最后修改时间。
     */
    const SCHEMA_DATE_TIME = 'datetime';

    /**
     * 根据$path返回当前的所有文件列表。
     *
     * @param mixed $path 当前路径。
     * @param mixed $sort 排序方式。
     *
     * @return array
     */
    function files($path = '', $sort = 'name');

    /**
     * 根据$file和$path上传文件，并返回上传后Web访问地址。
     *
     * @param mixed $file 上传文件信息。
     * @param mixed $path 上传路径。
     *
     * @return mixed
     */
    function upload($file, $path = '');
}

/**
 * 内部资源管理接口，提供对于网站内部管服务。
 *
 * @package skihat.kernels.medias
 */
abstract class Media {
    /**
     * @const 错误调试名称。
     */
    const DEBUG_NAME = 'kernels/medias';

    /**
     * @const string 默认引擎。
     */
    const DEFAULT_ENGINE = 'default';

    /**
     * @const string 配置节点。
     */
    const CONFIG_NAME = 'kernels/medias';

    /**
     * 初始化资源接口实例。
     */
    public static function initialize() {
        self::$_factory = new IocFactory(Skihat::read(self::CONFIG_NAME, array(
            self::DEFAULT_ENGINE => array(
                SKIHAT_IOC_CLASS => 'kernels.medias.engines.file_upload_engine',
                SKIHAT_IOC_PATHS => SKIHAT_PATH_LIBRARY
            ))));
    }

    /**
     * 返回所有引擎的名称集。
     *
     * @return array
     */
    public static function engines() {
        return self::$_factory->instances();
    }

    /**
     * 根据$name参数，返回资源引擎。
     *
     * @param string $name 参数名称。
     *
     * @return IMediaEngine
     */
    public static function engine($name) {
        return self::$_factory->instance($name);
    }

    /**
     * 根据$path，$sort和$engine参数获取文件列表，并返回。
     *
     * @param mixed  $path   文件路径。
     * @param mixed  $sort   排序方式。
     * @param string $engine 资源引擎。
     *
     * @return array
     */
    public static function files($path = false, $sort = false, $engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        return $engine->files($path, $sort);
    }

    /**
     * 使用$file,$path和$engine参数上传文件，并返回外部访问地址。
     *
     * @param mixed  $file   上传文件参数。
     * @param mixed  $path   上传路径。
     * @param string $engine 资源引擎。
     *
     * @return mixed
     */
    public static function upload($file, $path = false, $engine = self::DEFAULT_ENGINE) {
        $engine = self::engine($engine);
        return $engine->upload($file, $path);
    }

    /**
     * @var IocFactory Ioc工厂对象，提供对资源管理接口的管理。
     */
    protected static $_factory = null;
}

Media::initialize();