<?php

/**
 * paginate_filter.inc 提供PaginateFilter类的声明。
 *
 * @copyright   2012-2013 Skihat team (skihat.cn)
 * @license     http://skihat.cn/license.html
 * @author      Skihat Team
 *
 * @package     skihat.controllers.filters
 * @version     1.0
 *
 * @create      2013-04-08
 * @modify      2013-09-18
 */
class PaginateFilter implements IActionFilter {
    /**
     * @var int 分页大小，默认为10。
     */
    public $size = 10;

    /**
     * @var mixed 排序字段，默认为false。
     */
    public $sort = false;

    /**
     * @var string 排序方式，默认为asc。
     */
    public $dir = 'asc';

    /**
     * @var string 查找模型变量名称。
     */
    public $var = false;


    /**
     * @see IActionFilter::initialize
     */
    public function initialize(Controller $controller) {
        $this->_context = $controller;
    }

    /**
     * @see IActionFilter::invoke
     */
    public function invoke($action) {
        return true;
    }

    /**
     * @see IActionFilter::render
     */
    public function render(array &$options) {
        # 如果当前控制器中包含分页所需的变量值。
        if ($fetch = $this->_modelFetch()) {
            $size = $this->_context->query(SKIHAT_PARAM_PAGINATE_SIZE, $this->size);
            $page = $this->_context->query(SKIHAT_PARAM_PAGINATE, 1);
            $count = $fetch->fetchCount();

            # 计算分页总数，并进行除错处理。
            $pageAll = ceil($count / $size);
            if ($page > $pageAll) {
                $page = $pageAll;
            } else if ($page < 1) $page = 1;

            # 获取排序方式。
            $dir = $this->_context->query(SKIHAT_PARAM_PAGINATE_DIR, $this->dir);
            if ($sort = $this->_context->query(SKIHAT_PARAM_PAGINATE_SORT, false)) {
                $fetch->order(array($sort => $dir));
            } else {
                $fetch->order($this->sort);
            }

            # 指定分页大小。
            $fetch->page($page, $size);
            $fetch->addExtra('paginate', array(
                'size' => $size,
                'count' => $count,
                'page' => $page,
                'page-all' => $pageAll,
                'sort' => $sort,
                'dir' => $dir));
        }

        return true;

    }

    /**
     * @see IActionFilter::destory
     */
    public function complete() {
    }

    /**
     * 返回当前分布所需要模型检索器。
     *
     * @return ModelFetch|bool
     */
    protected function _modelFetch() {
        if (isset($this->_context[$this->var])) {
            return $this->_context[$this->var];
        }

        return false;
    }

    /**
     * @var Controller 上下文关系控制器。
     */
    protected $_context = null;

}