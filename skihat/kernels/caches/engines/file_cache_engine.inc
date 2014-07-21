<?php
/**
 * file_cache_engine.inc 提供文件缓存引擎声明。
 *
 * @copyright 2012-2013 Skihat team (skihat.cn)
 * @license   http://skihat.cn/license.html
 * @author    Skihat Team
 *
 * @package   skihat.kernels.caches.engines
 * @version   1.0
 *
 * @created   2012-07-16
 * @modified  2013-08-05
 */

/**
 * 文件缓存引擎，提供缓存的本地文件解决方案。
 *
 * @package skihat.kernels.caches.engines
 */
class FileCacheEngine implements ICacheEngine {

    /**
     * @const string 文件扩展名常量，
     */
    const FILE_EXT = '.tmp';

    /**
     * @var string 文件前缀，默认为default-cache。
     */
    public $prefix = 'default-cache';

    /**
     * @var string 缓存文件路径，默认为SKIHAT_PATH_DATA_CACHES.
     */
    public $paths = SKIHAT_PATH_DATA_CACHES;

    /**
     * @see ICacheEngine::write
     */
    public function write($name, $value, $expire = Cache::DEFAULT_EXPIRE) {
        # 将缓存信息写入缓存文件中。
        $bytes = file_put_contents($this->_cacheFile($name), serialize(array($expire + time(), $value)));

        # 如果写入失败，输出到错误调试中。
        if (!$bytes && SKIHAT_DEBUG) {
            Skihat::debug(Cache::DEBUG_NAME, sprintf('FileCacheEngine: Can\'t write cache file - \'%s\'.', $this->_cacheFile($name)));
        }

        return $bytes;
    }

    /**
     * @see ICacheEngine::read
     */
    public function read($name) {
        $file = $this->_cacheFile($name);

        # 如果文件不存在。
        if (!is_file($file)) return false;
        if ($content = file_get_contents($file)) {
            list($expire, $value) = unserialize($content);
            if (time() <= $expire) {
                return $value;
            }

            unlink($file);
        }

        return FALSE;
    }

    /**
     * @see ICacheEngine::delete
     */
    public function delete($name) {
        $file = $this->_cacheFile($name);
        if (is_file($file)) {
            unlink($file);
        }

        return true;
    }

    /**
     * @see ICacheEngine::flush
     */
    public function flush() {
        if ($handle = opendir($this->paths)) {
            $pattern = '/^' . $this->prefix . '-\w+' . self::FILE_EXT . '$/';

            while ($file = readdir($handle)) {
                if ($file != '.' && $file != '..') {
                    if (preg_match($pattern, $file)) {
                        unlink($this->paths . DIRECTORY_SEPARATOR . $file);
                    }
                }
            }

            closedir($handle);
        }
    }

    /**
     * 根据$name生成完整的缓存文件名称并返回。
     *
     * @param string $name 缓存名称。
     *
     * @return string
     */
    protected function _cacheFile($name) {
        return $this->paths . DIRECTORY_SEPARATOR . $this->prefix . '-' . $name . '.tmp';
    }
}