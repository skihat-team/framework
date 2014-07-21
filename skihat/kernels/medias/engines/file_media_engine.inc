<?php
/**
 * file_media_engine.inc 提供服务器文件资源管理引擎声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.medias.engines
 * @version   1.0
 *
 * @created   2012-07-12
 * @modified  2013-08-06
 */

/**
 * 文件资源管理引擎，提供服务器资源管理引擎。
 *
 * @package skihat.kernels.medias.engines
 */
class FileMediaEngine implements IMediaEngine {

    /**
     * @const string 默认URL地址。
     */
    const DEFAULT_HOST_URL = '/uploads';

    /**
     * @const string 默认最大上传内容。
     */
    const DEFAULT_MAX_SIZE = 200000;

    /**
     * @var array 允许上传的文件类型。
     */
    public $allowUploadTypes = array(
        'png' => 'image/png',
        'gif' => 'image/gif',
        'jpg' => 'image/jpeg');

    /**
     * @var int 允许上传的文件大小。
     */
    public $allowUploadSize = self::DEFAULT_MAX_SIZE;

    /**
     * @var array 照片扩展名。
     */
    public $photoExts = array('gif', 'jpg', 'jpeg', 'png', 'bmp');

    /**
     * @var array 允许查看的文件类型。
     */
    public $allowViews = array('png', 'gif', 'jpg');

    /**
     * @var string 服务器上传地址，默认为SKIHAT_PATH_APP_PUBLICS_UPLOADS。
     */
    public $hostPath = SKIHAT_PATH_APP_PUBLICS_UPLOADS;

    /**
     * @var string 主机外部访问URL地址。
     */
    public $hostUrl = self::DEFAULT_HOST_URL;

    /**
     * @see IMediaEngine::files
     */
    public function files($path = '', $sort = 'name') {
        if ($fullPath = $this->_fullPath($path)) {
            if ($schemas = $this->_fileSchemas($fullPath)) {
                return $this->_fileSorts($schemas, $sort);
            }
        }

        return array();
    }

    /**
     * @see IMediaEngine::upload
     */
    public function upload($file, $path = '') {
        # 如果获取完整路径，没有问题。
        if ($fullPath = $this->_fullPath($path)) {
            if ($this->_uploadVerify($file)) {
                $fullName = $this->_uploadFullName($fullPath, $file);
                if ($this->_uploadStore($file, $fullName)) {
                    return $this->hostUrl . substr($fullName, strlen($this->hostPath));
                }
            }
        }

        return false;
    }

    /**
     * 根据$path遍历其中的文件，并返回结果。
     *
     * @param string $path 文件路径。
     *
     * @return array
     */
    protected function _fileSchemas($path) {
        $schemas = array();

        if ($handle = opendir($path)) {
            while (($file = readdir($handle)) !== false) {
                if ($file[0] == '.') continue;
                if ($schema = $this->_fileSchema($path, $file)) {
                    array_push($schemas, $schema);
                }
            }

            closedir($handle);
        }


        return $schemas;
    }

    /**
     * 根据$path和$file生成文件或路径的元数据信息。
     *
     * @param string $path 完整路径。
     * @param string $file 文件名。
     *
     * @return array
     */
    protected function _fileSchema($path, $file) {
        # 如果指定文件是一个文件夹.
        if (is_dir($fullFile = $path . DIRECTORY_SEPARATOR . $file)) {
            return array(
                self::SCHEMA_FILE_NAME => $file,
                self::SCHEMA_IS_DIR => true,
                self::SCHEMA_FILE_SIZE => 0,
                self::SCHEMA_IS_PHOTO => false,
                self::SCHEMA_FILE_TYPE => '',
                self::SCHEMA_HAS_FILE => (count(scandir($fullFile)) > 2),
                self::SCHEMA_DATE_TIME => date(SKIHAT_I18N_DATE_TIME, filemtime($fullFile)));
        }

        $fileExt = strtolower(pathinfo($fullFile, PATHINFO_EXTENSION));
        if (!in_array($fileExt, $this->allowViews)) return false;

        return array(
            self::SCHEMA_FILE_NAME => $file,
            self::SCHEMA_FILE_SIZE => filesize($fullFile),
            self::SCHEMA_IS_DIR => false,
            self::SCHEMA_DATE_TIME => date(SKIHAT_I18N_DATE_TIME, filemtime($fullFile)),
            self::SCHEMA_HAS_FILE => false,
            self::SCHEMA_FILE_PATH => '',
            self::SCHEMA_FILE_TYPE => $fileExt,
            self::SCHEMA_IS_PHOTO => in_array($fileExt, $this->photoExts)
        );
    }

    /**
     * 根据$schemas和$sort排列结果值，并返回。
     *
     * @param array  $schemas 文件元数据值。
     * @param string $sort    排列方式。
     *
     * @return array
     */
    protected function _fileSorts(&$schemas, $sort) {
        $this->sort = $sort;

        usort($schemas, array($this, '_fileSort'));
        return $schemas;
    }

    /**
     * 根据$lhs和$rhs进行文件排序。
     *
     * @param array $lhs 左值。
     * @param array $rhs 右值。
     *
     * @return int
     */
    protected function _fileSort($lhs, $rhs) {
        if ($lhs[self::SCHEMA_IS_DIR] && !$rhs[self::SCHEMA_IS_DIR]) return -1;
        if (!$lhs[self::SCHEMA_IS_DIR] && $rhs[self::SCHEMA_IS_DIR]) return 1;

        $sort = strtolower($this->sort);

        if ($sort == 'size') {
            return $lhs[self::SCHEMA_FILE_SIZE] > $rhs[self::SCHEMA_FILE_SIZE] ? -1 : 1;
        } else if ($sort == 'type') {
            return strcmp($lhs[self::SCHEMA_FILE_TYPE], $rhs[self::SCHEMA_FILE_TYPE]);
        } else if ($sort == 'datetime') {
            return $lhs[self::SCHEMA_DATE_TIME] > $rhs[self::SCHEMA_DATE_TIME] ? 1 : -1;
        }

        return strcmp($lhs[self::SCHEMA_FILE_NAME], $rhs[self::SCHEMA_FILE_NAME]);
    }

    /**
     * 根据$path获取服务器完整路径信息，并返回。
     *
     * @param string $path 路径。
     *
     * @return string
     */
    protected function _fullPath($path) {
        $fulPath = realpath($this->hostPath . DIRECTORY_SEPARATOR . $path);
        if (strpos($fulPath, realpath($this->hostPath)) === 0) {
            return $fulPath;
        }

        return false;
    }

    /**
     * 根据$file进行文件验证，验证成功返回true，否则返回false。
     *
     * @param array $file 上传文件。
     *
     * @return bool
     */
    protected function _uploadVerify($file) {
        # 判断是否包含错误和满足上传文件大小要求。
        if ($file['error']) return false;
        if ($file['size'] > $this->allowUploadSize) return false;

        # 判断文件类型和文件扩展名。
        $fileExt = strtolower(pathinfo($file['name'], PATHINFO_EXTENSION));
        return isset($this->allowUploadTypes[$fileExt]) && ($file['type'] == $this->allowUploadTypes[$fileExt]);
    }

    /**
     * 根据$fullPath和$file生成完整的完整名称。
     *
     * @param string $path 文件路径。
     * @param array  $file 上传文件。
     *
     * @return string
     */
    protected function _uploadFullName($path, $file) {
        return sprintf('%s/%d-%d.%s', $path, microtime(true) * 1000, rand(1111, 9999), pathinfo($file['name'], PATHINFO_EXTENSION));
    }

    /**
     * 根据$file和$fullName存储上传文件。
     *
     * @param array  $file     上传文件。
     * @param string $fullName 完整文件名。
     *
     * @return string
     */
    protected function _uploadStore($file, $fullName) {
        return move_uploaded_file($file['tmp_name'], $fullName);
    }

    /**
     * @var string|bool 排列暂存值。
     */
    protected $sort = false;
}
